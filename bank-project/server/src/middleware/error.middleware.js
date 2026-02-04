import logger from '../utils/logger.util.js';

/**
 * Custom error for expected business failures (e.g. validation, not found).
 * statusCode is sent to the client; isOperational means the message is safe to show to users.
 */
export class AppError extends Error {
  constructor(message, statusCode, isOperational = true) {
    super(message);
    this.statusCode = statusCode;
    this.isOperational = isOperational;
    Error.captureStackTrace(this, this.constructor);
  }
}

/**
 * Turns an error into a safe status code and message for the client.
 * Known error types (Mongoose, JWT, AppError) get a clear response; everything else becomes 500.
 */
function getUserSafeMessage(err) {
  // Mongoose validation errors (e.g. required field missing)
  if (err.name === 'ValidationError') {
    const messages = Object.values(err.errors).map((e) => e.message);
    return { statusCode: 400, message: messages.join('. ') };
  }

  // Mongoose duplicate key (e.g. email already registered)
  if (err.code === 11000) {
    const field = Object.keys(err.keyValue)[0];
    return { statusCode: 409, message: `This ${field} is already registered` };
  }

  // Mongoose CastError (e.g. invalid ID format)
  if (err.name === 'CastError') {
    return { statusCode: 400, message: 'Invalid request parameter' };
  }

  // JWT invalid or expired
  if (err.name === 'JsonWebTokenError' || err.name === 'TokenExpiredError') {
    return { statusCode: 401, message: 'Authentication failed' };
  }

  // Our own AppError (thrown in controllers/services)
  if (err.isOperational) {
    return { statusCode: err.statusCode, message: err.message };
  }

  // Unknown errors: never expose details to the client
  return { statusCode: 500, message: 'Something went wrong' };
}

/**
 * Central error handler: log the error, then send a safe JSON response (statusCode + message).
 */
export const errorHandler = (err, req, res, next) => {
  logger.error(`[${req.method}] ${req.path} - ${err.name}: ${err.message}`, {
    stack: err.stack,
    code: err.code,
    statusCode: err.statusCode,
  });

  const { statusCode, message } = getUserSafeMessage(err);
  res.status(statusCode).json({ success: false, error: message });
};

/**
 * Used when no route matched the request. Passes a 404 AppError to the error handler.
 */
export const notFoundHandler = (req, res, next) => {
  next(new AppError('Resource not found', 404));
};
