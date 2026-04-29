import { processMessage } from '../services/chatbot.service.js';
import { authenticateSocket, getTokenFromCookie } from '../middleware/socketAuth.middleware.js';
import { verifyTokenOrThrow } from '../utils/jwt.util.js';

const MESSAGES = {
  ERROR: 'Sorry, something went wrong. Please try again.',
  SESSION_EXPIRED: 'Your session has expired. Please refresh the page and log in again.',
  LOGGED_OUT: 'You have been logged out. Please refresh the page.',
  RATE_LIMITED: 'Too many messages. Please wait a moment before sending another.',
};

const RATE_LIMIT_WINDOW_MS = 60_000;
const RATE_LIMIT_MAX = 10;

class SocketRegistry {
  #sockets = new Map();

  add(userId, socket) {
    if (!this.#sockets.has(userId)) this.#sockets.set(userId, new Set());
    this.#sockets.get(userId).add(socket);
  }

  remove(userId, socket) {
    const set = this.#sockets.get(userId);
    if (!set) return;
    set.delete(socket);
    if (set.size === 0) this.#sockets.delete(userId);
  }

  disconnectUser(userId, message) {
    const set = this.#sockets.get(userId);
    if (!set) return;
    for (const socket of set) {
      emitBot(socket, message, 'error');
      socket.disconnect();
    }
    this.#sockets.delete(userId);
  }
}

// Fixed-window rate limit kept in-process: fine for a single node, but if
// this ever runs behind multiple instances it should move to Redis so the
// counter is shared. Swap by passing a Redis-backed class that implements allow(key).
class RateLimiter {
  #counts = new Map();
  #windowMs;
  #max;

  constructor(windowMs, max) {
    this.#windowMs = windowMs;
    this.#max = max;
  }

  allow(key) {
    const now = Date.now();
    const entry = this.#counts.get(key);

    if (!entry || now > entry.resetAt) {
      this.#counts.set(key, { count: 1, resetAt: now + this.#windowMs });
      return true;
    }

    if (entry.count >= this.#max) return false;
    entry.count++;
    return true;
  }
}

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
    verifyTokenOrThrow(token);
    return true;
  } catch (err) {
    if (err?.name === 'TokenExpiredError' || err?.name === 'JsonWebTokenError') return false;
    throw err;
  }
};

const registry = new SocketRegistry();
const limiter = new RateLimiter(RATE_LIMIT_WINDOW_MS, RATE_LIMIT_MAX);

export const disconnectUser = (userId) =>
  registry.disconnectUser(userId, MESSAGES.LOGGED_OUT);

export const initChatbotSocket = (io) => {
  const chat = io.of('/chat');
  chat.use(authenticateSocket);

  chat.on('connection', (socket) => {
    const userId = socket.data.user.id;

    registry.add(userId, socket);
    socket.data.chatHistory = [];

    socket.on('user-message', async (message) => {
      try {
        if (!isTokenValid(socket)) {
          emitBot(socket, MESSAGES.SESSION_EXPIRED, 'error');
          socket.disconnect();
          return;
        }

        if (!limiter.allow(userId)) {
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

    socket.on('disconnect', () => registry.remove(userId, socket));
  });
};

export { SocketRegistry, RateLimiter };
