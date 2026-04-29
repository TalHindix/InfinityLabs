import { jest, describe, it, expect, beforeEach } from '@jest/globals';

// --- mocks ---
const mockAuthenticate = jest.fn();

jest.unstable_mockModule('../../utils/auth.util.js', () => ({
  authenticate: mockAuthenticate,
}));

const { getTokenFromCookie, authenticateSocket } = await import('../../middleware/socketAuth.middleware.js');

const ACTIVE_USER = { id: 'user-1', email: 'test@example.com' };

// helpers
const makeSocket = ({ authToken, cookieHeader } = {}) => ({
  handshake: {
    auth: authToken !== undefined ? { token: authToken } : {},
    headers: cookieHeader !== undefined ? { cookie: cookieHeader } : {},
  },
  data: {},
});

describe('getTokenFromCookie', () => {
  it('extracts token from a cookie header', () => {
    expect(getTokenFromCookie('session=abc; token=my.jwt.token; path=/')).toBe('my.jwt.token');
  });

  it('returns null when token cookie is absent', () => {
    expect(getTokenFromCookie('session=abc; other=xyz')).toBeNull();
  });

  it('returns null for empty string', () => {
    expect(getTokenFromCookie('')).toBeNull();
  });

  it('returns null for null', () => {
    expect(getTokenFromCookie(null)).toBeNull();
  });

  it('returns null for non-string input', () => {
    expect(getTokenFromCookie(123)).toBeNull();
  });

  it('decodes URI-encoded token values', () => {
    const encoded = 'token=' + encodeURIComponent('my.jwt.token');
    expect(getTokenFromCookie(encoded)).toBe('my.jwt.token');
  });
});

describe('authenticateSocket', () => {
  beforeEach(() => jest.clearAllMocks());

  it('authenticates via handshake auth token', async () => {
    mockAuthenticate.mockResolvedValue(ACTIVE_USER);
    const socket = makeSocket({ authToken: 'auth.token' });
    const next = jest.fn();

    await authenticateSocket(socket, next);

    expect(mockAuthenticate).toHaveBeenCalledWith('auth.token');
    expect(socket.data.user).toEqual(ACTIVE_USER);
    expect(next).toHaveBeenCalledWith();
  });

  it('falls back to cookie token when auth token is absent', async () => {
    mockAuthenticate.mockResolvedValue(ACTIVE_USER);
    const socket = makeSocket({ cookieHeader: 'token=cookie.token' });
    const next = jest.fn();

    await authenticateSocket(socket, next);

    expect(mockAuthenticate).toHaveBeenCalledWith('cookie.token');
    expect(socket.data.user).toEqual(ACTIVE_USER);
    expect(next).toHaveBeenCalledWith();
  });

  it('prefers auth token over cookie token', async () => {
    mockAuthenticate.mockResolvedValue(ACTIVE_USER);
    const socket = makeSocket({ authToken: 'auth.token', cookieHeader: 'token=cookie.token' });
    const next = jest.fn();

    await authenticateSocket(socket, next);

    expect(mockAuthenticate).toHaveBeenCalledWith('auth.token');
  });

  it('calls next with error when no token is provided', async () => {
    mockAuthenticate.mockResolvedValue(null);
    const socket = makeSocket();
    const next = jest.fn();

    await authenticateSocket(socket, next);

    expect(next).toHaveBeenCalledWith(expect.any(Error));
    expect(next.mock.calls[0][0].message).toBe('Authentication failed');
  });

  it('calls next with error when token is invalid', async () => {
    mockAuthenticate.mockResolvedValue(null);
    const socket = makeSocket({ authToken: 'bad.token' });
    const next = jest.fn();

    await authenticateSocket(socket, next);

    expect(next).toHaveBeenCalledWith(expect.any(Error));
    expect(socket.data.user).toBeUndefined();
  });

  it('calls next with error when user is not found', async () => {
    mockAuthenticate.mockResolvedValue(null);
    const socket = makeSocket({ authToken: 'valid.token' });
    const next = jest.fn();

    await authenticateSocket(socket, next);

    expect(next).toHaveBeenCalledWith(expect.any(Error));
  });
});
