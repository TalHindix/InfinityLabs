import { getAccountSummary } from './user.service.js';
import { detectIntentWithAI } from './openai-intent.service.js';

const INTENTS = {
  GREETING: 'greeting',
  BALANCE: 'balance',
  HELP: 'help',
  GOODBYE: 'goodbye',
  UNKNOWN: 'unknown'
};

const RESPONSES = {
  greeting: 'Hello! How can I help you today?',
  help: 'I can help you with:\n• Checking your balance\n• General account information\nJust ask!',
  goodbye: 'Goodbye! Have a great day 👋',
  unknown: 'I didn\'t understand that 🤔\nTry again or type "help"'
};

const SENSITIVE_INTENTS = ['balance'];

const sanitizeInput = (input) => {
  if (typeof input !== "string") {
    return "";
  }

  let cleanText = input.trim();

  if (cleanText.length > 250) {
    cleanText = cleanText.slice(0, 250);
  }

  // Remove HTML tags
  cleanText = cleanText.replace(/<[^>]*>/g, "");

  return cleanText;
};


const detectIntent = (message) => {
  const text = message.toLowerCase();

  if (text.includes("hello") || text.includes("hi")) {
    return "greeting";
  }

  if (text.includes("balance") || text.includes("money")) {
    return "balance";
  }

  if (text.includes("help")) {
    return "help";
  }

  if (text.includes("bye") || text.includes("thank")) {
    return "goodbye";
  }

  return "unknown";
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
  
  let intent = detectIntent(cleanMessage);

  if (intent === INTENTS.UNKNOWN) {
    try {
      intent = await detectIntentWithAI(cleanMessage);
      console.log('[Chatbot] 🤖 AI detected intent:', intent);
    } catch (error) {
      intent = INTENTS.UNKNOWN;
    }
  }

  if (isSensitiveIntent(intent) && !userId) {
    return {
      intent,
      message: 'Please log in to view this information 🔐',
      requiresAuth: true
    };
  }

  if (intent === INTENTS.BALANCE) {
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