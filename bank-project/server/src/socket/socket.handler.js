import { processMessage } from '../services/chatbot.service.js';
import { authenticateSocket, getTokenFromCookie } from '../middleware/socketAuth.middleware.js';
import { verifyToken } from '../utils/jwt.util.js';

const MESSAGES = {
  ERROR: 'Sorry, something went wrong. Please try again.',
  SESSION_EXPIRED: 'Your session has expired. Please refresh the page and log in again.',
  LOGGED_OUT: 'You have been logged out. Please refresh the page.',
  RATE_LIMITED: 'Too many messages. Please wait a moment before sending another.',
};

const RATE_LIMIT_WINDOW_MS = 60_000;
const RATE_LIMIT_MAX = 10;

const activeSocketsByUserId = new Map();
const userMessageCounts = new Map();

const emitBot = (socket, response, intent, data = null, requiresAuth = false) => {
  socket.emit('bot-message', {
    response,
    intent,
    data,
    requiresAuth,
    timestamp: new Date().toISOString(),
  });
};

// Re-verify the JWT on every incoming message (not just at handshake) so a
// socket that was authenticated hours ago can't keep talking after its token
// expires or gets revoked.
const isTokenValid = (socket) => {
  const cookieHeader = socket.handshake.headers?.cookie;
  const token = socket.handshake.auth?.token ?? getTokenFromCookie(cookieHeader);
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

// Fixed-window rate limit kept in-process: fine for a single node, but if
// this ever runs behind multiple instances it should move to Redis so the
// counter is shared.
const checkRateLimit = (userId) => {
  const now = Date.now();
  const entry = userMessageCounts.get(userId);

  if (!entry || now > entry.resetAt) {
    userMessageCounts.set(userId, { count: 1, resetAt: now + RATE_LIMIT_WINDOW_MS });
    return true;
  }

  if (entry.count >= RATE_LIMIT_MAX) return false;

  entry.count++;
  return true;
};

export const initChatbotSocket = (io) => {
  const chat = io.of('/chat');
  chat.use(authenticateSocket);

  chat.on('connection', (socket) => {
    const userId = socket.data.user.id;

    trackSocket(userId, socket);
    socket.data.chatHistory = [];

    socket.on('user-message', async (message) => {
      try {
        if (!isTokenValid(socket)) {
          emitBot(socket, MESSAGES.SESSION_EXPIRED, 'error');
          socket.disconnect();
          return;
        }

        if (!checkRateLimit(userId)) {
          emitBot(socket, MESSAGES.RATE_LIMITED, 'error');
          return;
        }

        socket.emit('typing');
        const result = await processMessage(message, socket.data.chatHistory, { userId, userEmail: socket.data.user.email });
        socket.emit('stop_typing');
        socket.data.chatHistory = result.chatHistory;
        emitBot(
          socket,
          result.message,
          result.intent,
          result.data ?? null,
          result.requiresAuth ?? false
        );

        if (result.transferCompleted) {
          socket.emit('transfer-completed');
        }
      } catch (err) {
        socket.emit('stop_typing');
        emitBot(socket, MESSAGES.ERROR, 'error');
      }
    });

    socket.on('typing', () => socket.broadcast.emit('typing'));
    socket.on('stop_typing', () => socket.broadcast.emit('stop_typing'));

    socket.on('disconnect', () => untrackSocket(userId, socket));
  });
};
