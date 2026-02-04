import OpenAI from 'openai';
import config from '../config/index.js';

const client = new OpenAI({
  apiKey: config.openAiApiKey,
});

/** Only these intent values are accepted from the AI. Anything else becomes "unknown". */
const ALLOWED_INTENTS = ['greeting', 'balance', 'help', 'goodbye', 'unknown'];

const SYSTEM_PROMPT = `
You are an intent classifier for a banking chatbot.
Return ONLY one word from this list:
greeting, balance, help, goodbye, unknown

Rules:
- If user asks about money or account -> balance
- If user greets -> greeting
- If user asks for help -> help
- If user says bye or thanks -> goodbye
- Otherwise -> unknown
`;

/**
 * Sends the user message to OpenAI and returns a validated intent string.
 * If the AI returns something unexpected, we return "unknown".
 */
export const detectIntentWithAI = async (userText) => {
  const response = await client.chat.completions.create({
    model: 'gpt-4o-mini',
    messages: [
      { role: 'system', content: SYSTEM_PROMPT },
      { role: 'user', content: userText },
    ],
    max_tokens: 10,
    temperature: 0,
  });

  // Get the text from the first choice (optional chaining avoids errors if structure is missing)
  const firstChoice = response.choices?.[0];
  const message = firstChoice?.message;
  const content = message?.content ?? '';

  const normalizedIntent = content.trim().toLowerCase();
  const isValidIntent = ALLOWED_INTENTS.includes(normalizedIntent);
  return isValidIntent ? normalizedIntent : 'unknown';
};
