import rateLimit from 'express-rate-limit';

/** Durations in milliseconds (used for rate limit windows). */
const FIFTEEN_MINUTES_MS = 15 * 60 * 1000;
const ONE_HOUR_MS = 60 * 60 * 1000;
const ONE_MINUTE_MS = 60 * 1000;

/**
 * Limits login/signup attempts: 5 per 15 minutes in production, effectively off in development.
 */
export const authLimiter = rateLimit({
  windowMs: FIFTEEN_MINUTES_MS,
  max: process.env.NODE_ENV === 'production' ? 5 : 999999,
  message: {
    error: 'Too many authentication attempts. Please try again after 15 minutes.',
  },
  standardHeaders: true,
  legacyHeaders: false,
  skipSuccessfulRequests: false,
});

/**
 * Limits email verification requests: 10 per hour.
 */
export const verifyLimiter = rateLimit({
  windowMs: ONE_HOUR_MS,
  max: 10,
  message: {
    error: 'Too many verification attempts. Please try again later.',
  },
  standardHeaders: true,
  legacyHeaders: false,
});

/**
 * Limits transfer requests: 10 per minute.
 */
export const transactionLimiter = rateLimit({
  windowMs: ONE_MINUTE_MS,
  max: 10,
  message: {
    error: 'Too many transactions. Please wait before making another transfer.',
  },
  standardHeaders: true,
  legacyHeaders: false,
});
