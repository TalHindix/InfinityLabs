import rateLimit from 'express-rate-limit';

export const authLimiter = rateLimit({
  windowMs: 15 * 60 * 1000, // 15 minutes
  max: process.env.NODE_ENV === "production" ? 5 : 999999, // 5 attempts per window
  message: {
    error: 'Too many authentication attempts. Please try again after 15 minutes.'
  },
  standardHeaders: true,
  legacyHeaders: false,
  skipSuccessfulRequests: false
});

// Rate limiter for email verification
export const verifyLimiter = rateLimit({
  windowMs: 60 * 60 * 1000, // 1 hour
  max: 10, // 10 attempts per hour
  message: {
    error: 'Too many verification attempts. Please try again later.'
  },
  standardHeaders: true,
  legacyHeaders: false
});

// General API rate limiter
export const apiLimiter = rateLimit({
  windowMs: 60 * 1000, // 1 minute
  max: 100, // 100 requests per minute
  message: {
    error: 'Too many requests. Please slow down.'
  },
  standardHeaders: true,
  legacyHeaders: false
});

// Transaction rate limiter
export const transactionLimiter = rateLimit({
  windowMs: 60 * 1000, // 1 minute
  max: 10, // 10 transactions per minute
  message: {
    error: 'Too many transactions. Please wait before making another transfer.'
  },
  standardHeaders: true,
  legacyHeaders: false
});
