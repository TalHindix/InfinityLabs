import { authenticate } from '../utils/auth.util.js';

export function getTokenFromCookie(cookieHeader) {
  if (!cookieHeader || typeof cookieHeader !== 'string') return null;
  const match = cookieHeader.match(/\btoken=([^;]+)/);
  return match ? decodeURIComponent(match[1].trim()) : null;
}

export const authenticateSocket = async (socket, next) => {
  const cookieHeader = socket.handshake.headers?.cookie;
  const token = socket.handshake.auth?.token ?? getTokenFromCookie(cookieHeader);

  const user = await authenticate(token);
  if (!user) return next(new Error('Authentication failed'));

  socket.data.user = user;
  next();
};
