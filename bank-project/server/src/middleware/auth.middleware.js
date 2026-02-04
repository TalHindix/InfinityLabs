import User from '../models/user.model.js';
import { USER_STATUS } from '../constants/index.js';
import { verifyToken } from '../utils/jwt.util.js';

const UNAUTHORIZED_RESPONSE = { success: false };

/**
 * Gets the JWT from the request: first from the "token" cookie, then from the Authorization header (Bearer <token>).
 */
function getTokenFromRequest(req) {
  const cookieToken = req.cookies?.token;
  if (cookieToken) return cookieToken;

  const authHeader = req.headers.authorization;
  const parts = authHeader?.split(' ') ?? [];
  const scheme = parts[0]?.toLowerCase();
  const token = parts[1];
  return scheme === 'bearer' ? token : undefined;
}

/**
 * Protects routes: requires a valid JWT and an active user. Attaches req.user on success, returns 401 otherwise.
 */
export const protect = async (req, res, next) => {
  try {
    const token = getTokenFromRequest(req);
    if (!token) return res.status(401).json(UNAUTHORIZED_RESPONSE);

    const decoded = verifyToken(token);
    const user = await User.findOne({ id: decoded.id }).select('id email status');

    if (!user || user.status !== USER_STATUS.ACTIVE) {
      return res.status(401).json(UNAUTHORIZED_RESPONSE);
    }

    req.user = user;
    next();
  } catch {
    return res.status(401).json(UNAUTHORIZED_RESPONSE);
  }
};