import User from '../models/user.model.js';
import { USER_STATUS } from '../constants/index.js';
import { verifyToken } from '../utils/jwt.util.js';

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

export const protect = async (req, res, next) => {
  try {
    const token = getTokenFromRequest(req);
    if (!token) return res.status(401).json(UNAUTHORIZED_RESPONSE);

    const decoded = verifyToken(token);
    const user = await User.findOne({ id: decoded.id }).select('id email status');

    if (!user || user.status !== USER_STATUS.ACTIVE) {
      return res.status(401).json(UNAUTHORIZED_RESPONSE);
    }

    // next route can use user object
    req.user = user;
    next();
  } catch {
    return res.status(401).json(UNAUTHORIZED_RESPONSE);
  }
};