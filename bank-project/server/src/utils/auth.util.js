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

// Core: verify a raw token string and return the active user, or null on any failure.
// Used by both HTTP and socket auth so the verify+lookup logic lives in one place.
export async function authenticate(token) {
  if (!token) return null;
  try {
    const { id } = verifyTokenOrThrow(token);
    return await findActiveUserById(id);
  } catch {
    return null;
  }
}

export async function getAuthenticatedUser(req) {
  return authenticate(getTokenFromRequest(req));
}
