import OpenAI from 'openai';
import config from '../config/index.js';

const client = new OpenAI({
  apiKey: config.openAiApiKey
});

const ALLOWED_INTENTS = [
  'greeting',
  'balance',
  'help',
  'goodbye',
  'unknown'
];

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

// Main function: send text to AI and get intent
export const detectIntentWithAI = async (userText) => {

  // Send request to OpenAI
  const response = await client.chat.completions.create({
    model: 'gpt-4o-mini',
    messages: [
      { role: 'system', content: SYSTEM_PROMPT },
      { role: 'user', content: userText }
    ],
    max_tokens: 10,
    temperature: 0
  });

  let rawIntent = '';

  if (
    response.choices[0] &&
    response.choices[0].message &&
    response.choices[0].message.content
  ) {
    rawIntent = response.choices[0].message.content;
  }

  rawIntent = rawIntent.trim();
  rawIntent = rawIntent.toLowerCase();

  if (ALLOWED_INTENTS.includes(rawIntent)) {
    return rawIntent;
  }

  return 'unknown';
};
