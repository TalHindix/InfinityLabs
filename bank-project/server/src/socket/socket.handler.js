import { processMessage } from '../services/chatbot.service.js';
import { authenticateSocket, getTokenFromCookie } from '../middleware/socketAuth.middleware.js';
import { verifyToken } from '../utils/jwt.util.js';

const getTimestamp = () => new Date().toISOString();

/** Message sent when a client first connects to the chat. */
const INITIAL_GREETING = 'Hello! I\'m your virtual banking assistant 🏦';

/** Message sent when processing a user message fails. */
const ERROR_FALLBACK_MESSAGE = 'Sorry, something went wrong. Please try again.';

// Track active sockets per user: userId → Set<socket>
const activeSockets = new Map();

/**
 * Disconnects all sockets for a given user ID.
 * @param {string} userId - User ID to disconnect
 */
export const disconnectUser = (userId) => {
  const sockets = activeSockets.get(userId);
  if (sockets) {
    sockets.forEach((socket) => {
      socket.emit('bot-message', {
        response: 'You have been logged out. Please refresh the page.',
        intent: 'error',
        timestamp: getTimestamp(),
      });
      socket.disconnect();
    });
    activeSockets.delete(userId);
  }
};

/**
 * Sets up the /chat Socket.IO namespace: auth on connect, then greeting; user-message -> processMessage -> bot-message.
 */
export const initChatbotSocket = (io) => {
  const chatNamespace = io.of('/chat');
  chatNamespace.use(authenticateSocket);

  chatNamespace.on('connection', (socket) => {
    const userId = socket.data.user.id;

    // Track socket
    if (!activeSockets.has(userId)) {
      activeSockets.set(userId, new Set());
    }
    activeSockets.get(userId).add(socket);

    socket.emit('bot-message', {
      response: INITIAL_GREETING,
      intent: 'greeting',
      timestamp: getTimestamp(),
    });

    socket.on('user-message', async (message) => {
      try {
        // Re-verify token on each message
        const cookieHeader = socket.handshake.headers?.cookie;
        const token = getTokenFromCookie(cookieHeader);

        if (!token) {
          socket.emit('bot-message', {
            response: 'Your session has expired. Please refresh the page and log in again.',
            intent: 'error',
            timestamp: getTimestamp(),
          });
          socket.disconnect();
          return;
        }

        try {
          verifyToken(token); // Throws if expired or invalid
        } catch (error) {
          if (error.name === 'TokenExpiredError' || error.name === 'JsonWebTokenError') {
            socket.emit('bot-message', {
              response: 'Your session has expired. Please refresh the page and log in again.',
              intent: 'error',
              timestamp: getTimestamp(),
            });
            socket.disconnect();
            return;
          }
          throw error; // Re-throw non-JWT errors
        }

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
      // Remove socket from tracking
      const sockets = activeSockets.get(userId);
      if (sockets) {
        sockets.delete(socket);
        if (sockets.size === 0) {
          activeSockets.delete(userId);
        }
      }
    });
  });
};