export class AppError extends Error {
  constructor(message, statusCode) {
    super(message);
    this.statusCode = statusCode;
  }
}

export class ValidationFailedError extends AppError {
  constructor(mongooseError) {
    const messages = Object.values(mongooseError.errors).map((e) => e.message);
    super(messages.join(', ') || 'Validation failed', 400);
  }
}

export class ConflictError extends AppError {
  constructor(field) {
    super(`This ${field} is already registered`, 409);
  }
}

export function toAppError(err) {
  if (err instanceof AppError) return err;
  if (err.name === 'ValidationError') return new ValidationFailedError(err);
  if (err.code === 11000) return new ConflictError(Object.keys(err.keyPattern || {})[0] ?? 'field');
  return new AppError('Internal server error', 500);
}
