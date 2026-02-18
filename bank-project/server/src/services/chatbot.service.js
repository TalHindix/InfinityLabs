import { getAccountSummary } from './user.service.js';
import { detectIntentWithAI } from './openaiIntent.service.js';
import logger from '../utils/logger.util.js';

const INTENTS = {
  GREETING: 'greeting',
  BALANCE: 'balance',
  HELP: 'help',
  GOODBYE: 'goodbye',
  UNKNOWN: 'unknown',
};

const RESPONSES = {
  greeting: 'Hello! How can I help you today?',
  help: 'I can help you with:\n• Checking your balance\n• General account information\nJust ask!',
  goodbye: 'Goodbye! Have a great day.',
  unknown: 'I didn\'t understand that. Try again or type "help".',
};

const SENSITIVE_INTENTS = ['balance'];

const KEYWORDS = {
  greeting: ['hello', 'hi'],
  balance: ['balance', 'money'],
  help: ['help'],
  goodbye: ['bye', 'thank'],
};

function sanitizeInput(input) {
  if (typeof input !== 'string') return '';
  return input.trim().slice(0, 250).replace(/<[^>]*>/g, '');
}

function detectIntentByKeywords(message) {
  const text = message.toLowerCase();
  for (const [intent, words] of Object.entries(KEYWORDS)) {
    if (words.some((word) => text.includes(word))) return intent;
  }
  return INTENTS.UNKNOWN;
}

async function detectIntentWithFallback(message) {
  const intent = detectIntentByKeywords(message);
  if (intent !== INTENTS.UNKNOWN) return intent;
  try {
    return await detectIntentWithAI(message);
  } catch (error) {
    logger.warn('Chatbot AI intent detection failed', { error: error.message });
    return INTENTS.UNKNOWN;
  }
}

function isSensitiveIntent(intent) {
  return SENSITIVE_INTENTS.includes(intent);
}

function canUserAccessIntent(intent, userId) {
  if (!isSensitiveIntent(intent)) return true;
  return !!userId;
}

function maskUserId(userId) {
  return `****${String(userId).slice(-4)}`;
}

function formatBalance(balance) {
  return `${Number(balance).toLocaleString()} AED`;
}

function buildAuthRequiredResponse(intent) {
  return {
    intent,
    message: 'Please log in to view this information.',
    requiresAuth: true,
  };
}

async function buildBalanceResponse(userId) {
  try {
    const account = await getAccountSummary(userId);
    return {
      intent: INTENTS.BALANCE,
      message: 'Here are your account details:',
      data: {
        userId: maskUserId(account.userId),
        balance: formatBalance(account.balance),
      },
    };
  } catch (error) {
    logger.warn('Chatbot balance fetch failed', { userId, error: error.message });
    return {
      intent: INTENTS.BALANCE,
      message: 'Unable to load your information right now. Please try again later.',
    };
  }
}

function buildStandardResponse(intent) {
  return {
    intent,
    message: RESPONSES[intent] || RESPONSES.unknown,
  };
}

export async function processMessage(message, context) {
  const { userId } = context;
  const cleanMessage = sanitizeInput(message);
  const intent = await detectIntentWithFallback(cleanMessage);

  if (!canUserAccessIntent(intent, userId)) {
    return buildAuthRequiredResponse(intent);
  }

  if (intent === INTENTS.BALANCE) {
    return buildBalanceResponse(userId);
  }

  return buildStandardResponse(intent);
}
