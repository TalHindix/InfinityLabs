/**
 * Single source of truth for all error handling.
 */

/**
 * Custom error for expected business failures (e.g. validation, not found).
 */
export class AppError extends Error {
  constructor(message, statusCode) {
    super(message);
    this.statusCode = statusCode;
  }
}

/**
 * Maps an error to appropriate HTTP status code and message
 * @param {Error} error - The caught error
 * @returns {{ statusCode: number, message: string }}
 */
export const mapErrorToResponse = (error) => {
  // AppError - use as-is
  if (error.statusCode) {
    return {
      statusCode: error.statusCode,
      message: error.message,
    };
  }

  // Mongoose ValidationError
  if (error.name === 'ValidationError') {
    const messages = Object.values(error.errors).map((e) => e.message);
    return {
      statusCode: 400,
      message: messages.join(', ') || 'Validation failed',
    };
  }

  // Mongoose Duplicate Key
  if (error.code === 11000) {
    const field = Object.keys(error.keyPattern || {})[0] || 'field';
    return {
      statusCode: 409,
      message: `This ${field} is already registered`,
    };
  }

  // Unknown error - hide details
  return {
    statusCode: 500,
    message: 'Internal server error',
  };
};

/**
 * Used when no route matched the request. Passes a 404 AppError to the error handler.
 */
export const notFoundHandler = (req, res, next) => {
  next(new AppError('Resource not found', 404));
};

/**
 * Central error handler: log the error, then send a safe JSON response.
 */
export const errorHandler = (err, req, res, next) => {
  console.error(`[${req.method}] ${req.path} - ${err.name}: ${err.message}`);

  const { statusCode, message } = mapErrorToResponse(err);
  res.status(statusCode).json({ success: false, error: message });
};
