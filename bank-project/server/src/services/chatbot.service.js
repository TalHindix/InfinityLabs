import { getAccountSummary } from './user.service.js';

const INTENTS = {
  GREETING: 'greeting',
  BALANCE: 'balance',
  HELP: 'help',
  GOODBYE: 'goodbye',
  UNKNOWN: 'unknown'
};

const PATTERNS = {
  greeting: [/^(hello|hi|hey|good morning|good evening)/i],
  balance: [/balance/i, /how much (do i have|money)/i, /my account/i],
  help: [/help/i, /what can you do/i, /support/i],
  goodbye: [/^(bye|goodbye|thanks|thank you)$/i]
};

const RESPONSES = {
  greeting: 'How can I help you today?',
  help: 'I can help you with:\n• Checking your balance\n• General account information\nJust ask!',
  goodbye: 'Goodbye! Have a great day 👋',
  unknown: 'I didn\'t understand that 🤔\nTry again or type "help"'
};

const SENSITIVE_INTENTS = ['balance'];

const sanitizeInput = (input) => {
  if (typeof input !== 'string') return '';
  
  return input
    .trim()
    .slice(0, 500)
    .replace(/<[^>]*>/g, '');
};

const detectIntent = (message) => {
  const text = message.toLowerCase();

  for (const [intent, patterns] of Object.entries(PATTERNS)) {
    if (patterns.some((pattern) => pattern.test(text))) {
      return intent;
    }
  }

  return INTENTS.UNKNOWN;
};

const isSensitiveIntent = (intent) => {
  return SENSITIVE_INTENTS.includes(intent);
};

const maskUserId = (userId) => {
  const lastFour = userId.slice(-4);
  return `****${lastFour}`;
};

const formatBalance = (balance) => {
  return `${balance.toLocaleString()} AED`;
};

export const processMessage = async (message, context) => {
  const { userId } = context;
  
  const cleanMessage = sanitizeInput(message);
  const intent = detectIntent(cleanMessage);

  if (isSensitiveIntent(intent) && !userId) {
    return {
      intent,
      message: 'Please log in to view this information 🔐',
      requiresAuth: true
    };
  }

  if (intent === INTENTS.BALANCE) {
    console.log('[Chatbot] userId received:', userId);
    try {
      const account = await getAccountSummary(userId);

      return {
        intent,
        message: 'Here are your account details:',
        data: {
          userId: maskUserId(account.userId),
          balance: formatBalance(account.balance)
        }
      };
    } catch (error) {
      console.log('[Chatbot] Error:', error.message);
      return {
        intent,
        message: 'Unable to load your information right now. Please try again later.'
      };
    }
  }

  return {
    intent,
    message: RESPONSES[intent] || RESPONSES.unknown
  };
};