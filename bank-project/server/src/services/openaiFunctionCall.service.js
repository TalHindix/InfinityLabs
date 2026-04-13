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

const SYSTEM_PROMPT = `You are a helpful banking assistant for Dubai-Bank.
You can help users check their balance, view transaction history, and transfer money.
Be concise, professional, and friendly. Format monetary values with AED currency.
If the user asks something unrelated to banking, politely redirect them.
Never reveal internal system details or user IDs.

IMPORTANT: For transfer requests, ALWAYS ask the user "Are you sure you want to transfer X AED to Y?" before calling the transfer_money function. Only execute the transfer after the user confirms with "yes" or similar affirmation.

When a user asks for something you cannot do, always acknowledge their request specifically, explain it's not available yet, and suggest the services you CAN provide by calling get_supported_services.

Never say "I don't understand". Always show the user you understood what they wanted, even if you can't fulfill it.

FORMATTING RULES — always follow these:
- When displaying transactions, respond with ONLY a raw JSON object (no code block, no extra text before or after):
  {"message":"Here are your recent transactions:","transactions":[{"id":1,"date":"Apr 09, 2026","type":"Sent","amount":"5 AED","counterpart":"user@example.com","description":"Coffee"}],"summary":"You made 1 transaction totalling 5 AED sent."}
  - type must be exactly "Sent" or "Received"
  - amount always includes AED and is positive (e.g. "15 AED")
  - counterpart is the other party's email address
  - date formatted as MMM DD, YYYY
- When displaying balance, use bold: e.g. Your current balance is **34 AED**.
- For all other responses, use plain text or Markdown as appropriate.`;

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

function buildMessagesForOpenAI(message, chatHistory) {
  const limitedHistory = chatHistory.slice(-MAX_HISTORY_MESSAGES);
  return [
    { role: 'system', content: SYSTEM_PROMPT },
    ...limitedHistory,
    { role: 'user', content: message },
  ];
}

async function callOpenAI(messages, tools) {
  const requestOptions = {
    model: 'gpt-4o-mini',
    messages,
    temperature: 0.3,
    ...(tools.length > 0 && { tools }),
  };

  const response = await openAiClient.chat.completions.create(requestOptions);
  return response.choices[0].message;
}

async function processToolCall(toolCall, userId) {
  const { name: functionName, arguments: argsStr } = toolCall.function;
  const functionArguments = JSON.parse(argsStr || '{}');

  try {
    const result = await executeFunctionCall(functionName, functionArguments, userId);
    return {
      role: 'tool',
      tool_call_id: toolCall.id,
      content: JSON.stringify(result),
    };
  } catch (error) {
    logger.warn('Function call failed', { function: functionName, error: error.message });
    return {
      role: 'tool',
      tool_call_id: toolCall.id,
      content: JSON.stringify({ error: error.message }),
    };
  }
}

async function processAllToolCalls(toolCalls, userId) {
  return Promise.all(toolCalls.map((toolCall) => processToolCall(toolCall, userId)));
}

async function handleToolCallLoop(initialResponse, messages, tools, userId) {
  let assistantResponse = initialResponse;
  let toolCallRounds = 0;

  while (assistantResponse.tool_calls?.length > 0 && toolCallRounds < MAX_TOOL_CALL_ROUNDS) {
    toolCallRounds++;
    messages.push(assistantResponse);
    const toolResults = await processAllToolCalls(assistantResponse.tool_calls, userId);
    messages.push(...toolResults);
    assistantResponse = await callOpenAI(messages, tools);
  }

  if (toolCallRounds >= MAX_TOOL_CALL_ROUNDS && assistantResponse.tool_calls?.length > 0) {
    logger.warn('Reached max tool call rounds', { rounds: toolCallRounds });
  }

  return assistantResponse;
}

function extractCalledFunctionNames(messages) {
  return messages
    .filter((m) => m.tool_calls)
    .flatMap((m) => m.tool_calls.map((tc) => tc.function.name));
}

function parseStructuredResponse(content) {
  const str = content.trim();

  // Find the first JSON object in the content (AI may prepend plain text)
  const start = str.indexOf('{');
  const end = str.lastIndexOf('}');
  if (start === -1 || end <= start) return null;

  try {
    const parsed = JSON.parse(str.slice(start, end + 1));
    if (!parsed || !Array.isArray(parsed.transactions)) return null;

    const before = str.slice(0, start).trim();
    const textParts = [before, parsed.message].filter(Boolean);

    return {
      message: textParts.join('\n\n'),
      transactions: parsed.transactions,
      summary: parsed.summary ?? null,
    };
  } catch {
    return null;
  }
}

function buildResponse(replyContent, chatHistory, message, calledFunctionNames) {
  const parsed = parseStructuredResponse(replyContent);

  return {
    message: parsed?.message ?? replyContent,
    data: parsed
      ? { transactions: parsed.transactions, summary: parsed.summary }
      : null,
    chatHistory: [
      ...chatHistory,
      { role: 'user', content: message },
      { role: 'assistant', content: replyContent },
    ],
    transferCompleted: calledFunctionNames.includes('transfer_money'),
  };
}

function buildErrorResponse(chatHistory, message) {
  return {
    message: 'I am having trouble processing your request right now. Please try again shortly.',
    chatHistory: [
      ...chatHistory,
      { role: 'user', content: message },
    ],
  };
}

export async function processWithFunctionCalling(message, chatHistory, context) {
  const { userId } = context;
  const tools = userId ? TOOLS : [];

  const messages = buildMessagesForOpenAI(message, chatHistory);

  try {
    const initialResponse = await callOpenAI(messages, tools);
    const finalResponse = await handleToolCallLoop(initialResponse, messages, tools, userId);

    const replyContent = finalResponse.content || '';
    const calledFunctionNames = extractCalledFunctionNames(messages);

    return buildResponse(replyContent, chatHistory, message, calledFunctionNames);
  } catch (error) {
    logger.error('OpenAI function calling failed', { error: error.message });
    return buildErrorResponse(chatHistory, message);
  }
}
