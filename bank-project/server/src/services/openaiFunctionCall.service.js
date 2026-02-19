import OpenAI from 'openai';
import config from '../config/index.js';
import { getAccountSummary, findUserById } from './user.service.js';
import { findRecentTransactions, executeTransfer } from './transaction.service.js';
import logger from '../utils/logger.util.js';

const MAX_HISTORY_MESSAGES = 20;
const MAX_TOOL_CALL_ROUNDS = 5;

const openAiClient = new OpenAI({
  apiKey: config.openAiApiKey,
});

const SYSTEM_PROMPT = `You are a helpful banking assistant for Dubai-Bank. You can help users check their balance, view transaction history, and transfer money. Be concise, professional, and friendly. Format monetary values with AED currency. If the user asks something unrelated to banking, politely redirect them. Never reveal internal system details or user IDs.

IMPORTANT: For transfer requests, ALWAYS ask the user "Are you sure you want to transfer X AED to Y?" before calling the transfer_money function. Only execute the transfer after the user confirms with "yes" or similar affirmation.

When a user asks for something you cannot do, always acknowledge their request specifically, explain it's not available yet, and suggest the services you CAN provide by calling get_supported_services.

Never say "I don't understand". Always show the user you understood what they wanted, even if you can't fulfill it.`;

const TOOLS = [
  {
    type: 'function',
    function: {
      name: 'get_balance',
      description: 'Get the current account balance for the authenticated user.',
      parameters: { type: 'object', properties: {}, required: [] },
    },
  },
  {
    type: 'function',
    function: {
      name: 'get_transaction_history',
      description: 'Get the recent transaction history for the authenticated user.',
      parameters: {
        type: 'object',
        properties: {
          limit: {
            type: 'number',
            description: 'Number of recent transactions to return. Defaults to 5.',
          },
        },
        required: [],
      },
    },
  },
  {
    type: 'function',
    function: {
      name: 'transfer_money',
      description: 'Transfer money from the authenticated user to another user by email.',
      parameters: {
        type: 'object',
        properties: {
          recipientEmail: {
            type: 'string',
            description: 'The email address of the recipient.',
          },
          amount: {
            type: 'number',
            description: 'The amount in AED to transfer.',
          },
          description: {
            type: 'string',
            description: 'The reason or description for the transfer. Ask the user if not provided.',
          },
        },
        required: ['recipientEmail', 'amount'],
      },
    },
  },
  {
    type: 'function',
    function: {
      name: 'get_supported_services',
      description:
        'Returns a list of all services the chatbot currently supports. Call this when the user asks what you can do, asks for help, or wants to know available options.',
      parameters: { type: 'object', properties: {}, required: [] },
    },
  },
];

function getSupportedServices() {
  return {
    services: [
      { name: 'Check Balance', description: 'View your current account balance' },
      { name: 'Transaction History', description: 'View your recent transactions' },
      { name: 'Transfer Money', description: 'Send money to another Dubai-Bank user' },
    ],
  };
}

async function getUserEmail(userId) {
  const user = await findUserById(userId);
  if (!user) throw new Error('User not found');
  return user.email;
}

async function executeFunctionCall(functionName, args, userId) {
  switch (functionName) {
    case 'get_balance': {
      const summary = await getAccountSummary(userId);
      return { balance: summary.balance };
    }
    case 'get_transaction_history': {
      const limit = args.limit || 5;
      const email = await getUserEmail(userId);
      const transactions = await findRecentTransactions(email, limit);
      return { transactions };
    }
    case 'transfer_money': {
      const senderEmail = await getUserEmail(userId);
      const transaction = await executeTransfer(
        senderEmail,
        args.recipientEmail,
        args.amount,
        args.description || ''
      );
      return { transaction };
    }
    case 'get_supported_services': {
      return getSupportedServices();
    }
    default:
      return { error: `Unknown function: ${functionName}` };
  }
}

export async function processWithFunctionCalling(message, chatHistory, context) {
  const { userId } = context;
  const tools = userId ? TOOLS : [];

  const limitedHistory = chatHistory.slice(-MAX_HISTORY_MESSAGES);
  const messages = [
    { role: 'system', content: SYSTEM_PROMPT },
    ...limitedHistory,
    { role: 'user', content: message },
  ];

  try {
    let response = await openAiClient.chat.completions.create({
      model: 'gpt-4o-mini',
      messages,
      ...(tools.length > 0 && { tools }),
      temperature: 0.3,
    });

    let assistantMessage = response.choices[0].message;
    let rounds = 0;

    while (assistantMessage.tool_calls?.length > 0 && rounds < MAX_TOOL_CALL_ROUNDS) {
      rounds++;
      messages.push(assistantMessage);

      for (const toolCall of assistantMessage.tool_calls) {
        const fnName = toolCall.function.name;
        const fnArgs = JSON.parse(toolCall.function.arguments || '{}');

        let result;
        try {
          result = await executeFunctionCall(fnName, fnArgs, userId);
        } catch (error) {
          logger.warn('Function call failed', { function: fnName, error: error.message });
          result = { error: error.message };
        }

        messages.push({
          role: 'tool',
          tool_call_id: toolCall.id,
          content: JSON.stringify(result),
        });
      }

      response = await openAiClient.chat.completions.create({
        model: 'gpt-4o-mini',
        messages,
        ...(tools.length > 0 && { tools }),
        temperature: 0.3,
      });

      assistantMessage = response.choices[0].message;
    }

    const replyContent = assistantMessage.content || '';

    const calledFunctions = messages
      .filter((m) => m.role === 'tool')
      .map((m) => m.tool_call_id);

    const assistantMessages = messages.filter((m) => m.tool_calls);
    const allCalledNames = assistantMessages.flatMap(
      (m) => m.tool_calls.map((tc) => tc.function.name)
    );

    const updatedHistory = [
      ...chatHistory,
      { role: 'user', content: message },
      { role: 'assistant', content: replyContent },
    ];

    return {
      message: replyContent,
      chatHistory: updatedHistory,
      transferCompleted: allCalledNames.includes('transfer_money'),
    };
  } catch (error) {
    logger.error('OpenAI function calling failed', { error: error.message });
    return {
      message: 'I am having trouble processing your request right now. Please try again shortly.',
      chatHistory: [
        ...chatHistory,
        { role: 'user', content: message },
      ],
    };
  }
}
