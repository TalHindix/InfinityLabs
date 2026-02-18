import rateLimit from 'express-rate-limit';

const FIFTEEN_MINUTES_MS = 15 * 60 * 1000;
const ONE_HOUR_MS = 60 * 60 * 1000;
const ONE_MINUTE_MS = 60 * 1000;

const RATE_LIMIT_HEADERS = { standardHeaders: true, legacyHeaders: false };

export const authLimiter = rateLimit({
  windowMs: FIFTEEN_MINUTES_MS,
  max: process.env.NODE_ENV === 'production' ? 5 : 999999,
  message: {
    success: false,
    error: 'Too many authentication attempts. Please try again after 15 minutes.',
  },
  ...RATE_LIMIT_HEADERS,
  skipSuccessfulRequests: false,
});

export const verifyLimiter = rateLimit({
  windowMs: ONE_HOUR_MS,
  max: 10,
  message: {
    success: false,
    error: 'Too many verification attempts. Please try again later.',
  },
  ...RATE_LIMIT_HEADERS,
});

export const transactionLimiter = rateLimit({
  windowMs: ONE_MINUTE_MS,
  max: 10,
  message: {
    success: false,
    error: 'Too many transactions. Please wait before making another transfer.',
  },
  ...RATE_LIMIT_HEADERS,
});
