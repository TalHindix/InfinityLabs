import User from '../models/user.model.js';
import { HTTP_STATUS, USER_STATUS } from '../constants/index.js';
import { verifyToken } from '../utils/jwt.util.js';


const UNAUTHORIZED_RESPONSE = {
  success: false
};

export const protect = async (req, res, next) => {
  try {
    const authHeader = req.headers.authorization;
    const [scheme, token] = authHeader?.split(' ') ?? [];

    if (!token || scheme?.toLowerCase() !== 'bearer') {
      return res.status(HTTP_STATUS.UNAUTHORIZED).json(UNAUTHORIZED_RESPONSE);
    }

    const decoded = verifyToken(token);
    const user = await User.findOne({ id: decoded.id }).select('id email status');

    if (!user || user.status !== USER_STATUS.ACTIVE) {
      return res.status(HTTP_STATUS.UNAUTHORIZED).json(UNAUTHORIZED_RESPONSE);
    }

    req.user = user;
    next();
  } catch (error) {
    return res.status(HTTP_STATUS.UNAUTHORIZED).json(UNAUTHORIZED_RESPONSE);
  }
};