import { verifyToken } from '../utils/jwt.util.js';
import { findActiveUserById } from '../services/user.service.js';

const UNAUTHORIZED_RESPONSE = { success: false, error: 'Authentication failed' };

function getTokenFromRequest(req) {
  const cookieToken = req.cookies?.token;
  if (cookieToken) return cookieToken;

  const authHeader = req.headers.authorization;
  const parts = authHeader?.split(' ') ?? [];
  const scheme = parts[0]?.toLowerCase();
  const token = parts[1];
  return scheme === 'bearer' ? token : undefined;
}

export async function getAuthenticatedUser(req) {
  const token = getTokenFromRequest(req);
  if (!token) return null;
  try {
    const decoded = verifyToken(token);
    return await findActiveUserById(decoded.id);
  } catch {
    return null;
  }
}

export const protect = async (req, res, next) => {
  const user = await getAuthenticatedUser(req);
  if (!user) return res.status(401).json(UNAUTHORIZED_RESPONSE);
  req.user = user;
  next();
};