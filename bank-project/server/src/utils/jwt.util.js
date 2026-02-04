import jwt from 'jsonwebtoken';
import config from '../config/index.js';

/**
 * Creates a JWT for the given user. The user must have id and email.
 */
export const createToken = (user) => {
  if (!user?.id || !user?.email) {
    throw new Error('User must have id and email to create token');
  }
  return jwt.sign(
    { id: user.id, email: user.email },
    config.jwt.secret,
    { expiresIn: config.jwt.expiresIn },
  );
};

/**
 * Verifies a JWT and returns the decoded payload. Throws if invalid or expired.
 */
export const verifyToken = (token) => jwt.verify(token, config.jwt.secret);