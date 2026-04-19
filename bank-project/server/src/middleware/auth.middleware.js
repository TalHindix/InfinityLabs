import { getAuthenticatedUser } from '../utils/auth.util.js';

const UNAUTHORIZED_RESPONSE = { success: false, error: 'Authentication failed' };

export const protect = async (req, res, next) => {
  const user = await getAuthenticatedUser(req);
  if (!user) return res.status(401).json(UNAUTHORIZED_RESPONSE);
  req.user = user;
  next();
};
