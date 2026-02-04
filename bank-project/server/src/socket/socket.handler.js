import { processMessage } from '../services/chatbot.service.js';
import { authenticateSocket } from '../middleware/socketAuth.middleware.js';

const getTimestamp = () => new Date().toISOString();

/** Message sent when a client first connects to the chat. */
const INITIAL_GREETING = 'Hello! I\'m your virtual banking assistant 🏦';

/** Message sent when processing a user message fails. */
const ERROR_FALLBACK_MESSAGE = 'Sorry, something went wrong. Please try again.';

/**
 * Sets up the /chat Socket.IO namespace: auth on connect, then greeting; user-message -> processMessage -> bot-message.
 */
export const initChatbotSocket = (io) => {
  const chatNamespace = io.of('/chat');
  chatNamespace.use(authenticateSocket);

  chatNamespace.on('connection', (socket) => {
    const userId = socket.user.id;

    socket.emit('bot-message', {
      response: INITIAL_GREETING,
      intent: 'greeting',
      timestamp: getTimestamp(),
    });

    socket.on('user-message', async (message) => {
      try {
        const context = { userId };
        const result = await processMessage(message, context);
        socket.emit('bot-message', {
          response: result.message,
          intent: result.intent,
          data: result.data ?? null,
          requiresAuth: result.requiresAuth ?? false,
          timestamp: getTimestamp(),
        });
      } catch {
        socket.emit('bot-message', {
          response: ERROR_FALLBACK_MESSAGE,
          intent: 'error',
          timestamp: getTimestamp(),
        });
      }
    });

    socket.on('disconnect', () => {
      // No cleanup needed; user just left the chat
    });
  });
};