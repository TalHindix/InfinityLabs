import User from '../models/user.model.js';
import { USER_STATUS } from '../constants/index.js';
import { verifyToken } from '../utils/jwt.util.js';

/**
 * Extracts the "token" cookie value from the Cookie header string (e.g. "token=abc123; other=...").
 */
function getTokenFromCookie(cookieHeader) {
  if (!cookieHeader || typeof cookieHeader !== 'string') return null;
  const match = cookieHeader.match(/\btoken=([^;]+)/);
  return match ? decodeURIComponent(match[1].trim()) : null;
}

/**
 * Socket.io auth middleware: requires a valid JWT (from socket.handshake.auth.token or cookie),
 * loads the user, checks they are active, then sets socket.user and calls next(). Otherwise calls next(error).
 */
export const authenticateSocket = async (socket, next) => {
  try {
    const cookieHeader = socket.handshake.headers?.cookie;
    const token = socket.handshake.auth?.token ?? getTokenFromCookie(cookieHeader);

    if (!token) {
      return next(new Error('Authentication token is required'));
    }

    const decoded = verifyToken(token);
    const user = await User.findOne({ id: decoded.id }).select('id email status');

    if (!user) {
      return next(new Error('User not found'));
    }
    if (user.status !== USER_STATUS.ACTIVE) {
      return next(new Error('User account is not active'));
    }

    socket.user = user;
    next();
  } catch (error) {
    const isJwtError =
      error.name === 'JsonWebTokenError' || error.name === 'TokenExpiredError';
    if (isJwtError) {
      return next(new Error('Invalid or expired authentication token'));
    }
    return next(new Error('Authentication failed'));
  }
};
