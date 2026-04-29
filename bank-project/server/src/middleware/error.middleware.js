import { AppError, toAppError } from '../utils/error.util.js';
import logger from '../utils/logger.util.js';

export function notFoundHandler(req, res, next) {
  next(new AppError('Resource not found', 404));
}

export function errorHandler(err, req, res, next) {
  logger.warn(`${req.method} ${req.path} - ${err.name}: ${err.message}`);
  if (process.env.NODE_ENV === 'development') {
    logger.debug(err);
  }

  const { statusCode, message } = toAppError(err);
  res.status(statusCode).json({ success: false, error: message });
}
