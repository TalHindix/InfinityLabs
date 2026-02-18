import { processMessage } from '../services/chatbot.service.js';
import { authenticateSocket, getTokenFromCookie } from '../middleware/socketAuth.middleware.js';
import { verifyToken } from '../utils/jwt.util.js';

const MESSAGES = {
  GREETING: "Hello! I'm your virtual banking assistant.",
  ERROR: 'Sorry, something went wrong. Please try again.',
  SESSION_EXPIRED: 'Your session has expired. Please refresh the page and log in again.',
  LOGGED_OUT: 'You have been logged out. Please refresh the page.',
};

const activeSocketsByUserId = new Map();

const emitBot = (socket, response, intent, data = null, requiresAuth = false) => {
  socket.emit('bot-message', {
    response,
    intent,
    data,
    requiresAuth,
    timestamp: new Date().toISOString(),
  });
};

const isTokenValid = (socket) => {
  const cookieHeader = socket.handshake.headers?.cookie;
  const token = getTokenFromCookie(cookieHeader);
  if (!token) return false;

  try {
    verifyToken(token);
    return true;
  } catch (err) {
    if (err?.name === 'TokenExpiredError' || err?.name === 'JsonWebTokenError') return false;
    throw err;
  }
};

const trackSocket = (userId, socket) => {
  if (!activeSocketsByUserId.has(userId)) activeSocketsByUserId.set(userId, new Set());
  activeSocketsByUserId.get(userId).add(socket);
};

const untrackSocket = (userId, socket) => {
  const set = activeSocketsByUserId.get(userId);
  if (!set) return;
  set.delete(socket);
  if (set.size === 0) activeSocketsByUserId.delete(userId);
};

export const disconnectUser = (userId) => {
  const set = activeSocketsByUserId.get(userId);
  if (!set) return;

  for (const socket of set) {
    emitBot(socket, MESSAGES.LOGGED_OUT, 'error');
    socket.disconnect();
  }
  activeSocketsByUserId.delete(userId);
};

export const initChatbotSocket = (io) => {
  const chat = io.of('/chat');
  chat.use(authenticateSocket);

  chat.on('connection', (socket) => {
    const userId = socket.data.user.id;

    trackSocket(userId, socket);
    emitBot(socket, MESSAGES.GREETING, 'greeting');

    socket.on('user-message', async (message) => {
      try {
        if (!isTokenValid(socket)) {
          emitBot(socket, MESSAGES.SESSION_EXPIRED, 'error');
          socket.disconnect();
          return;
        }

        const result = await processMessage(message, { userId });
        emitBot(
          socket,
          result.message,
          result.intent,
          result.data ?? null,
          result.requiresAuth ?? false
        );
      } catch (err) {
        emitBot(socket, MESSAGES.ERROR, 'error');
      }
    });

    socket.on('disconnect', () => untrackSocket(userId, socket));
  });
};
