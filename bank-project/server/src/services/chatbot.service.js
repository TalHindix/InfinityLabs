import { processWithFunctionCalling } from './openaiFunctionCall.service.js';
import logger from '../utils/logger.util.js';

function sanitizeInput(input) {
  if (typeof input !== 'string') return '';
  return input.trim().slice(0, 250).replace(/<[^>]*>/g, '');
}

export async function processMessage(message, chatHistory, context) {
  const { userId } = context;
  const cleanMessage = sanitizeInput(message);

  if (!cleanMessage) {
    return {
      intent: 'error',
      message: 'Please enter a valid message.',
      chatHistory,
    };
  }

  try {
    const result = await processWithFunctionCalling(cleanMessage, chatHistory, { userId });

    return {
      intent: 'chat',
      message: result.message,
      data: result.data ?? null,
      chatHistory: result.chatHistory,
      transferCompleted: result.transferCompleted || false,
    };
  } catch (error) {
    logger.error('Chatbot processMessage failed', { error: error.message });
    return {
      intent: 'error',
      message: 'Something went wrong. Please try again later.',
      chatHistory,
    };
  }
}
