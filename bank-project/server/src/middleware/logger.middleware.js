import logger from '../utils/logger.util.js';

/**
 * Logs each request when the response finishes (so we have the real status code and duration).
 * Format: "METHOD /path STATUS - 123ms". Uses warn for 4xx/5xx, info for success.
 */
export const requestLogger = (req, res, next) => {
  const start = Date.now();

  res.on('finish', () => {
    const duration = Date.now() - start;
    const message = `${req.method} ${req.originalUrl} ${res.statusCode} - ${duration}ms`;

    if (res.statusCode >= 400) {
      logger.warn(message);
    } else {
      logger.info(message);
    }
  });

  next();
};