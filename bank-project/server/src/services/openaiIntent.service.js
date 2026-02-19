// DEPRECATED: This service is no longer used after migrating to OpenAI Function Calling.
// Kept for reference/rollback purposes. See openaiFunctionCall.service.js for new implementation.

// import OpenAI from 'openai';
// import config from '../config/index.js';
//
// const openAiClient = new OpenAI({
//   apiKey: config.openAiApiKey,
// });
//
// const ALLOWED_INTENTS = ['greeting', 'balance', 'help', 'goodbye', 'unknown'];
//
// const SYSTEM_PROMPT = `You are an intent classifier for a banking chatbot.
// Return ONLY one word from this list: greeting, balance, help, goodbye, unknown
//
// Rules:
// - If user asks about money or account -> balance
// - If user greets -> greeting
// - If user asks for help -> help
// - If user says bye or thanks -> goodbye
// - Otherwise -> unknown`;
//
// export async function detectIntentWithAI(userText) {
//   const response = await openAiClient.chat.completions.create({
//     model: 'gpt-4o-mini',
//     messages: [
//       { role: 'system', content: SYSTEM_PROMPT },
//       { role: 'user', content: userText },
//     ],
//     max_tokens: 10,
//     temperature: 0,
//   });
//
//   const firstChoice = response.choices?.[0];
//   const aiMessage = firstChoice?.message;
//   const content = aiMessage?.content ?? '';
//   const normalizedIntent = content.trim().toLowerCase();
//
//   if (ALLOWED_INTENTS.includes(normalizedIntent)) {
//     return normalizedIntent;
//   }
//   return 'unknown';
// }
