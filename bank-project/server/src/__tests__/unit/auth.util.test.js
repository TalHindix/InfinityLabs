import { jest, describe, it, expect, beforeEach } from '@jest/globals';

// --- mocks ---
const mockVerifyTokenOrThrow = jest.fn();
const mockFindActiveUserById = jest.fn();

jest.unstable_mockModule('../../utils/jwt.util.js', () => ({
  verifyTokenOrThrow: mockVerifyTokenOrThrow,
}));

jest.unstable_mockModule('../../services/user.service.js', () => ({
  findActiveUserById: mockFindActiveUserById,
}));

const { authenticate } = await import('../../utils/auth.util.js');

const ACTIVE_USER = { id: 'user-1', email: 'test@example.com', status: 'active' };

describe('authenticate(token)', () => {
  beforeEach(() => jest.clearAllMocks());

  it('returns null when token is null', async () => {
    expect(await authenticate(null)).toBeNull();
    expect(mockVerifyTokenOrThrow).not.toHaveBeenCalled();
  });

  it('returns null when token is undefined', async () => {
    expect(await authenticate(undefined)).toBeNull();
    expect(mockVerifyTokenOrThrow).not.toHaveBeenCalled();
  });

  it('returns null when token is empty string', async () => {
    expect(await authenticate('')).toBeNull();
    expect(mockVerifyTokenOrThrow).not.toHaveBeenCalled();
  });

  it('returns the active user for a valid token', async () => {
    mockVerifyTokenOrThrow.mockReturnValue({ id: 'user-1' });
    mockFindActiveUserById.mockResolvedValue(ACTIVE_USER);

    const result = await authenticate('valid.jwt.token');

    expect(mockVerifyTokenOrThrow).toHaveBeenCalledWith('valid.jwt.token');
    expect(mockFindActiveUserById).toHaveBeenCalledWith('user-1');
    expect(result).toEqual(ACTIVE_USER);
  });

  it('returns null when verifyTokenOrThrow throws (invalid token)', async () => {
    mockVerifyTokenOrThrow.mockImplementation(() => {
      const err = new Error('invalid signature');
      err.name = 'JsonWebTokenError';
      throw err;
    });

    expect(await authenticate('bad.token')).toBeNull();
    expect(mockFindActiveUserById).not.toHaveBeenCalled();
  });

  it('returns null when verifyTokenOrThrow throws (expired token)', async () => {
    mockVerifyTokenOrThrow.mockImplementation(() => {
      const err = new Error('jwt expired');
      err.name = 'TokenExpiredError';
      throw err;
    });

    expect(await authenticate('expired.token')).toBeNull();
  });

  it('returns null when user does not exist', async () => {
    mockVerifyTokenOrThrow.mockReturnValue({ id: 'ghost-id' });
    mockFindActiveUserById.mockResolvedValue(null);

    expect(await authenticate('valid.jwt.token')).toBeNull();
  });

  it('returns null when user is inactive', async () => {
    mockVerifyTokenOrThrow.mockReturnValue({ id: 'user-1' });
    mockFindActiveUserById.mockResolvedValue(null); // findActiveUserById already filters inactive

    expect(await authenticate('valid.jwt.token')).toBeNull();
  });
});
