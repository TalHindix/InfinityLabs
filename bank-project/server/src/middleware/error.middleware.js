import logger from '../utils/logger.util.js';

// Custom error class for application errors
export class AppError extends Error {
  constructor(message, statusCode, isOperational = true) {
    super(message);
    this.statusCode = statusCode;
    this.isOperational = isOperational; // Operational errors are safe to show to users
    Error.captureStackTrace(this, this.constructor);
  }
}

const getUserSafeMessage = (err) => {
  if (err.name === 'ValidationError') {
    const errors = Object.values(err.errors).map(el => el.message);
    return { statusCode: 400, message: errors.join('. ') };
  }

  if (err.code === 11000) {
    const field = Object.keys(err.keyValue)[0];
    return { statusCode: 409, message: `This ${field} is already registered` };
  }

  if (err.name === 'CastError') {
    return { statusCode: 400, message: 'Invalid request parameter' };
  }

  if (err.name === 'JsonWebTokenError' || err.name === 'TokenExpiredError') {
    return { statusCode: 401, message: 'Authentication failed' };
  }

  if (err.isOperational) {
    return { statusCode: err.statusCode, message: err.message };
  }

  return { statusCode: 500, message: 'Something went wrong' };
};

export const errorHandler = (err, req, res, next) => {
  logger.error(`[${req.method}] ${req.path} - ${err.name}: ${err.message}`, {
    stack: err.stack,
    code: err.code,
    statusCode: err.statusCode
  });

  const { statusCode, message } = getUserSafeMessage(err);

  res.status(statusCode).json({
    success: false,
    error: message
  });
};

export const notFoundHandler = (req, res, next) => {
  const error = new AppError('Resource not found', 404);
  next(error);
};
