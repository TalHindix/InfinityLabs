import bcrypt from 'bcrypt';
import { verifyTokenOrThrow } from './jwt.util.js';
import { findActiveUserById } from '../services/user.service.js';

function getTokenFromRequest(req) {
  const cookieToken = req.cookies?.token;
  if (cookieToken) return cookieToken;

  const authHeader = req.headers.authorization;
  const parts = authHeader?.split(' ') ?? [];
  const scheme = parts[0]?.toLowerCase();
  const token = parts[1];
  return scheme === 'bearer' ? token : null;
}

export const validatePassword = async (inputPassword, hashedPassword) => {
  return bcrypt.compare(inputPassword, hashedPassword);
};

export async function getAuthenticatedUser(req) {
  const token = getTokenFromRequest(req);
  if (!token) return null;
  try {
    const decoded = verifyTokenOrThrow(token);
    return await findActiveUserById(decoded.id);
  } catch {
    return null;
  }
}
