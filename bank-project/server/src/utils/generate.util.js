import crypto from 'crypto';

/**
 * Generates a cryptographically random verification token (64 hex characters).
 */
export const generateVerificationToken = () =>
  crypto.randomBytes(32).toString('hex');

/**
 * Returns the SHA-256 hash of the token as a hex string.
 * @param {string} token - Token to hash (e.g. verification token)
 */
export const hashToken = (token) => {
  if (token == null || typeof token !== 'string') {
    throw new Error('Token is required for hashing');
  }
  return crypto.createHash('sha256').update(token, 'utf8').digest('hex');
};