import { getAccountSummary } from './user.service.js';
import { detectIntentWithAI } from './openaiIntent.service.js';

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
  goodbye: 'Goodbye! Have a great day 👋',
  unknown: 'I didn\'t understand that 🤔\nTry again or type "help"',
};

const SENSITIVE_INTENTS = ['balance'];

const KEYWORDS = {
  greeting: ['hello', 'hi'],
  balance: ['balance', 'money'],
  help: ['help'],
  goodbye: ['bye', 'thank'],
};

const sanitizeInput = (input) =>
  typeof input === 'string'
    ? input.trim().slice(0, 250).replace(/<[^>]*>/g, '')
    : '';

const detectIntent = (message) => {
  const text = message.toLowerCase();
  for (const [intent, words] of Object.entries(KEYWORDS)) {
    if (words.some((w) => text.includes(w))) return intent;
  }
  return INTENTS.UNKNOWN;
};

const isSensitiveIntent = (intent) => SENSITIVE_INTENTS.includes(intent);
const maskUserId = (userId) => `****${userId.slice(-4)}`;
const formatBalance = (balance) => `${balance.toLocaleString()} AED`;

/**
 * Detects user intent from message, with AI fallback.
 * @param {string} message - User's message
 * @returns {Promise<string>} Detected intent
 */
const detectUserIntent = async (message) => {
  const intent = detectIntent(message);
  if (intent !== INTENTS.UNKNOWN) return intent;
  try {
    return await detectIntentWithAI(message);
  } catch {
    return INTENTS.UNKNOWN;
  }
};

const isAuthorizedForIntent = (intent, userId) =>
  !isSensitiveIntent(intent) || !!userId;

/**
 * Handles balance inquiry intent.
 * @param {string} userId - Authenticated user ID
 * @returns {Promise<Object>} Response object with account data
 */
const handleBalanceIntent = async (userId) => {
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
  } catch {
    return {
      intent: INTENTS.BALANCE,
      message: 'Unable to load your information right now. Please try again later.',
    };
  }
};

const handleStandardIntent = (intent) => ({
  intent,
  message: RESPONSES[intent] || RESPONSES.unknown,
});

/**
 * Processes user message and returns appropriate bot response.
 * @param {string} message - Raw user message
 * @param {Object} context - Context object containing userId
 * @param {string|null} context.userId - Authenticated user ID (null if not logged in)
 * @returns {Promise<Object>} Response object with intent, message, and optional data
 */
export const processMessage = async (message, context) => {
  const { userId } = context;
  const cleanMessage = sanitizeInput(message);
  const intent = await detectUserIntent(cleanMessage);

  if (!isAuthorizedForIntent(intent, userId)) {
    return {
      intent,
      message: 'Please log in to view this information 🔐',
      requiresAuth: true,
    };
  }
  if (intent === INTENTS.BALANCE) return await handleBalanceIntent(userId);
  return handleStandardIntent(intent);
};
