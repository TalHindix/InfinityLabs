import { verifyToken } from '../utils/jwt.util.js';
import { findActiveUserById } from '../services/user.service.js';

export function getTokenFromCookie(cookieHeader) {
  if (!cookieHeader || typeof cookieHeader !== 'string') return null;
  const match = cookieHeader.match(/\btoken=([^;]+)/);
  return match ? decodeURIComponent(match[1].trim()) : null;
}

export const authenticateSocket = async (socket, next) => {
  try {
    const cookieHeader = socket.handshake.headers?.cookie;
    const token = socket.handshake.auth?.token ?? getTokenFromCookie(cookieHeader);

    if (!token) {
      return next(new Error('Authentication token is required'));
    }

    const decoded = verifyToken(token);
    const user = await findActiveUserById(decoded.id);

    if (!user) {
      return next(new Error('User not found or account is not active'));
    }

    socket.data.user = user;
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
