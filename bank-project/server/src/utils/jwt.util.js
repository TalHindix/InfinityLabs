import jwt from 'jsonwebtoken';
import config from '../config/index.js';
import { AppError } from './error.util.js';

export const createToken = (user) => {
  if (!user?.id || !user?.email) {
    throw new AppError('User must have id and email to create token', 400);
  }
  return jwt.sign(
    { id: user.id, email: user.email },
    config.jwt.secret,
    { expiresIn: config.jwt.expiresIn },
  );
};

export const verifyToken = (token) => jwt.verify(token, config.jwt.secret);