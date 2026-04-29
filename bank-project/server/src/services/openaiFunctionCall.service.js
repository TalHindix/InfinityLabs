import OpenAI from 'openai';
import config from '../config/index.js';
import { getAccountSummary } from './user.service.js';
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

LANGUAGE: Always reply in the same language the user writes in. If the user writes in Hebrew, reply in Hebrew. If in English, reply in English. The JSON transaction format fields ("message", "summary") must also be in the user's language. The "type" field must always be exactly "Sent" or "Received" in English (these are used as code keys).

IMPORTANT: For transfer requests, ALWAYS ask the user "Are you sure you want to transfer X AED to Y?" before calling the transfer_money function. Only execute the transfer after the user confirms with "yes" or similar affirmation.

When a user asks for something you cannot do, always acknowledge their request specifically, explain it's not available yet, and suggest the services you CAN provide by calling get_supported_services.

Never say "I don't understand". Always show the user you understood what they wanted, even if you can't fulfill it.

FORMATTING RULES — always follow these:
- When displaying transactions, respond with ONLY a single raw JSON object (no code block, no extra text before or after).
  The "message" and "summary" values MUST be written in the user's language. The "type" field is always "Sent" or "Received" in English.
  English example: {"message":"Here are your recent transactions:","transactions":[{"id":1,"date":"Apr 09, 2026","type":"Sent","amount":"5 AED","counterpart":"user@example.com","description":"Coffee"}],"summary":"You made 1 transaction totalling 5 AED sent."}
  Hebrew example:  {"message":"הנה העסקאות האחרונות שלך:","transactions":[{"id":1,"date":"Apr 09, 2026","type":"Sent","amount":"5 AED","counterpart":"user@example.com","description":"Coffee"}],"summary":"ביצעת עסקה אחת בסך 5 AED שנשלחו."}
  - amount always includes AED and is positive (e.g. "15 AED")
  - counterpart is the other party's email address
  - date formatted as MMM DD, YYYY
- When displaying balance, use bold: e.g. **34 AED** (the surrounding text must be in the user's language).
- COMBINING MULTIPLE RESULTS: When the user asks for multiple things at once (e.g. transactions AND balance), you MUST respond with a SINGLE JSON object. Include ALL information in the "message" field. For example, if the user asks for both transactions and balance, put the balance info in the "message" field alongside the transaction intro text:
  {"message":"Your balance is **34 AED**.\n\nHere are your recent transactions:","transactions":[...],"summary":"..."}
  NEVER output multiple JSON objects. NEVER concatenate two separate JSON responses. Always merge everything into ONE response.
- For all other responses, use plain text or Markdown as appropriate.`;

const toolRegistry = {
  get_balance: {
    schema: {
      description: 'Get the current account balance for the authenticated user.',
      parameters: { type: 'object', properties: {}, required: [] },
    },
    handler: async (_args, ctx) => {
      const summary = await getAccountSummary(ctx.userId);
      return { balance: summary.balance };
    },
  },

  get_transaction_history: {
    schema: {
      description: 'Get the recent transaction history for the authenticated user.',
      parameters: {
        type: 'object',
        properties: {
          limit: { type: 'number', description: 'Number of recent transactions to return. Defaults to 5.' },
        },
        required: [],
      },
    },
    handler: async ({ limit = 5 } = {}, ctx) => {
      const transactions = await findRecentTransactions(ctx.userEmail, limit);
      return { transactions };
    },
  },

  transfer_money: {
    schema: {
      description: 'Transfer money from the authenticated user to another user by email.',
      parameters: {
        type: 'object',
        properties: {
          recipientEmail: { type: 'string', description: 'The email address of the recipient.' },
          amount: { type: 'number', description: 'The amount in AED to transfer.' },
          description: { type: 'string', description: 'The reason or description for the transfer. Ask the user if not provided.' },
        },
        required: ['recipientEmail', 'amount'],
      },
    },
    handler: async ({ recipientEmail, amount, description = '' }, ctx) => {
      const transaction = await executeTransfer(ctx.userEmail, recipientEmail, amount, description);
      return { transaction };
    },
  },

  get_supported_services: {
    schema: {
      description: 'Returns a list of all services the chatbot currently supports. Call this when the user asks what you can do, asks for help, or wants to know available options.',
      parameters: { type: 'object', properties: {}, required: [] },
    },
    handler: async () => ({
      services: [
        { name: 'Check Balance', description: 'View your current account balance' },
        { name: 'Transaction History', description: 'View your recent transactions' },
        { name: 'Transfer Money', description: 'Send money to another Dubai-Bank user' },
      ],
    }),
  },
};

const TOOLS = Object.entries(toolRegistry).map(([name, { schema }]) => ({
  type: 'function',
  function: { name, ...schema },
}));

async function executeFunctionCall(functionName, args, userId, userEmail) {
  const tool = toolRegistry[functionName];
  if (!tool) return { error: `Unknown function: ${functionName}` };
  return tool.handler(args, { userId, userEmail });
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

async function processToolCall(toolCall, userId, userEmail) {
  const { name: functionName, arguments: argsStr } = toolCall.function;
  const functionArguments = JSON.parse(argsStr || '{}');

  try {
    const result = await executeFunctionCall(functionName, functionArguments, userId, userEmail);
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

async function processAllToolCalls(toolCalls, userId, userEmail) {
  return Promise.all(toolCalls.map((toolCall) => processToolCall(toolCall, userId, userEmail)));
}

// The model may call tools, read their output, then call more tools. We loop
// until it stops asking, with a hard cap to bound cost/latency if the model
// ever gets stuck in a tool-call loop.
async function handleToolCallLoop(initialResponse, messages, tools, userId, userEmail) {
  let assistantResponse = initialResponse;
  let toolCallRounds = 0;

  while (assistantResponse.tool_calls?.length > 0 && toolCallRounds < MAX_TOOL_CALL_ROUNDS) {
    toolCallRounds++;
    messages.push(assistantResponse);
    const toolResults = await processAllToolCalls(assistantResponse.tool_calls, userId, userEmail);
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

// The model is instructed to return a raw JSON object when listing
// transactions, but in practice it sometimes wraps it in prose ("Here are your
// transactions: { ... }"). JSON.parse on the whole string fails, so we walk
// the text ourselves tracking brace depth + string escapes to pull out the
// first balanced {...} block. Duplicated (by necessity) in the client so the
// chat bubble can re-parse if the server ever ships raw content through.
function findBalancedJson(str, startIndex) {
  if (str[startIndex] !== '{') return null;
  let depth = 0;
  let inString = false;
  let escape = false;

  for (let i = startIndex; i < str.length; i++) {
    const ch = str[i];
    if (escape) { escape = false; continue; }
    if (ch === '\\' && inString) { escape = true; continue; }
    if (ch === '"') { inString = !inString; continue; }
    if (inString) continue;
    if (ch === '{') depth++;
    else if (ch === '}') {
      depth--;
      if (depth === 0) return str.slice(startIndex, i + 1);
    }
  }
  return null;
}

function parseStructuredResponse(content) {
  const str = content.trim();

  // Try each top-level JSON object in the content until we find one with transactions
  let pos = 0;

  while (pos < str.length) {
    const start = str.indexOf('{', pos);
    if (start === -1) break;

    const jsonStr = findBalancedJson(str, start);
    if (!jsonStr) break;

    try {
      const parsed = JSON.parse(jsonStr);
      if (parsed && Array.isArray(parsed.transactions)) {
        // Collect any plain text before this JSON object
        const before = str.slice(0, start).trim();
        // Collect any remaining text/JSON after this object for extra info (e.g. balance)
        const afterStart = start + jsonStr.length;
        const after = str.slice(afterStart).trim();

        // Try to extract message from remaining JSON objects (e.g. a separate balance response)
        let extraMessage = '';
        if (after) {
          const extraJsonStr = findBalancedJson(after, after.indexOf('{'));
          if (extraJsonStr) {
            try {
              const extraParsed = JSON.parse(extraJsonStr);
              if (extraParsed.message) extraMessage = extraParsed.message;
            } catch { /* ignore */ }
          } else if (!after.startsWith('{')) {
            extraMessage = after;
          }
        }

        const textParts = [before, parsed.message, extraMessage].filter(Boolean);

        return {
          message: textParts.join('\n\n'),
          transactions: parsed.transactions,
          summary: parsed.summary ?? null,
        };
      }
    } catch { /* not valid JSON, skip */ }

    pos = start + (jsonStr ? jsonStr.length : 1);
  }

  return null;
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
  const { userId, userEmail } = context;
  const tools = userId ? TOOLS : [];

  const messages = buildMessagesForOpenAI(message, chatHistory);

  try {
    const initialResponse = await callOpenAI(messages, tools);
    const finalResponse = await handleToolCallLoop(initialResponse, messages, tools, userId, userEmail);

    const replyContent = finalResponse.content || '';
    const calledFunctionNames = extractCalledFunctionNames(messages);

    return buildResponse(replyContent, chatHistory, message, calledFunctionNames);
  } catch (error) {
    logger.error('OpenAI function calling failed', { error: error.message });
    return buildErrorResponse(chatHistory, message);
  }
}
