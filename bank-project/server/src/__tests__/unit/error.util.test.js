import { AppError, ValidationFailedError, ConflictError, toAppError } from '../../utils/error.util.js';

describe('AppError', () => {
  it('sets message and statusCode', () => {
    const err = new AppError('Not found', 404);
    expect(err.message).toBe('Not found');
    expect(err.statusCode).toBe(404);
    expect(err).toBeInstanceOf(Error);
  });
});

describe('ValidationFailedError', () => {
  it('joins mongoose field messages and uses 400', () => {
    const mongooseError = {
      name: 'ValidationError',
      errors: {
        email: { message: 'Email is required' },
        phone: { message: 'Phone is invalid' },
      },
    };
    const err = new ValidationFailedError(mongooseError);
    expect(err.statusCode).toBe(400);
    expect(err.message).toBe('Email is required, Phone is invalid');
    expect(err).toBeInstanceOf(AppError);
  });

  it('falls back to "Validation failed" when errors is empty', () => {
    const err = new ValidationFailedError({ errors: {} });
    expect(err.message).toBe('Validation failed');
  });
});

describe('ConflictError', () => {
  it('builds a 409 message from the field name', () => {
    const err = new ConflictError('email');
    expect(err.statusCode).toBe(409);
    expect(err.message).toBe('This email is already registered');
    expect(err).toBeInstanceOf(AppError);
  });
});

describe('toAppError', () => {
  it('returns AppError instances unchanged', () => {
    const original = new AppError('Forbidden', 403);
    expect(toAppError(original)).toBe(original);
  });

  it('returns subclass instances unchanged', () => {
    const original = new ConflictError('phone');
    expect(toAppError(original)).toBe(original);
  });

  it('converts a Mongoose ValidationError', () => {
    const mongooseErr = {
      name: 'ValidationError',
      errors: { firstName: { message: 'First name is required' } },
    };
    const result = toAppError(mongooseErr);
    expect(result).toBeInstanceOf(ValidationFailedError);
    expect(result.statusCode).toBe(400);
    expect(result.message).toBe('First name is required');
  });

  it('converts a Mongo duplicate-key error (code 11000)', () => {
    const mongoErr = { code: 11000, keyPattern: { email: 1 } };
    const result = toAppError(mongoErr);
    expect(result).toBeInstanceOf(ConflictError);
    expect(result.statusCode).toBe(409);
    expect(result.message).toBe('This email is already registered');
  });

  it('falls back to "field" when keyPattern is missing', () => {
    const result = toAppError({ code: 11000, keyPattern: {} });
    expect(result.statusCode).toBe(409);
    expect(result.message).toBe('This field is already registered');
  });

  it('converts unknown errors to 500 AppError', () => {
    const result = toAppError(new Error('something unexpected'));
    expect(result).toBeInstanceOf(AppError);
    expect(result.statusCode).toBe(500);
    expect(result.message).toBe('Internal server error');
  });

  it('handles null/undefined gracefully', () => {
    const result = toAppError({});
    expect(result.statusCode).toBe(500);
  });
});
