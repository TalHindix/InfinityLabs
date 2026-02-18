export class AppError extends Error {
  constructor(message, statusCode) {
    super(message);
    this.statusCode = statusCode;
  }
}

export function mapErrorToResponse(error) {
  if (error.statusCode) {
    return { statusCode: error.statusCode, message: error.message };
  }

  if (error.name === 'ValidationError') {
    const messages = Object.values(error.errors).map((e) => e.message);
    return {
      statusCode: 400,
      message: messages.join(', ') || 'Validation failed',
    };
  }

  if (error.code === 11000) {
    const field = Object.keys(error.keyPattern || {})[0] || 'field';
    return {
      statusCode: 409,
      message: `This ${field} is already registered`,
    };
  }

  return {
    statusCode: 500,
    message: 'Internal server error',
  };
}
