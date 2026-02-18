import crypto from 'crypto';
import { AppError } from './error.util.js';

export const generateVerificationToken = () =>
  crypto.randomBytes(32).toString('hex');

export const hashToken = (token) => {
  if (token == null || typeof token !== 'string') {
    throw new AppError('Token is required for hashing', 400);
  }
  return crypto.createHash('sha256').update(token, 'utf8').digest('hex');
};