import { describe, it, expect, beforeEach, jest } from '@jest/globals';
import { SocketRegistry, RateLimiter } from '../../socket/socket.handler.js';

// --- SocketRegistry ---

const makeSocket = (id = 'socket-1') => ({
  id,
  disconnect: jest.fn(),
  emit: jest.fn(),
  handshake: { headers: {}, auth: {} },
});

describe('SocketRegistry', () => {
  let registry;

  beforeEach(() => {
    registry = new SocketRegistry();
  });

  it('tracks a socket for a user', () => {
    const socket = makeSocket();
    registry.add('user-1', socket);
    // disconnectUser exercises the internal set — if socket.disconnect is called, it was tracked
    registry.disconnectUser('user-1', 'bye');
    expect(socket.disconnect).toHaveBeenCalledTimes(1);
  });

  it('tracks multiple sockets for the same user', () => {
    const s1 = makeSocket('s1');
    const s2 = makeSocket('s2');
    registry.add('user-1', s1);
    registry.add('user-1', s2);
    registry.disconnectUser('user-1', 'bye');
    expect(s1.disconnect).toHaveBeenCalledTimes(1);
    expect(s2.disconnect).toHaveBeenCalledTimes(1);
  });

  it('does nothing when disconnecting a user with no sockets', () => {
    expect(() => registry.disconnectUser('ghost', 'bye')).not.toThrow();
  });

  it('removes a specific socket on remove()', () => {
    const s1 = makeSocket('s1');
    const s2 = makeSocket('s2');
    registry.add('user-1', s1);
    registry.add('user-1', s2);
    registry.remove('user-1', s1);
    registry.disconnectUser('user-1', 'bye');
    expect(s1.disconnect).not.toHaveBeenCalled();
    expect(s2.disconnect).toHaveBeenCalledTimes(1);
  });

  it('cleans up the user entry when last socket is removed', () => {
    const socket = makeSocket();
    registry.add('user-1', socket);
    registry.remove('user-1', socket);
    // second disconnectUser should be a no-op (entry was cleaned up)
    expect(() => registry.disconnectUser('user-1', 'bye')).not.toThrow();
    expect(socket.disconnect).not.toHaveBeenCalled();
  });

  it('remove() is a no-op when user has no tracked sockets', () => {
    const socket = makeSocket();
    expect(() => registry.remove('ghost', socket)).not.toThrow();
  });

  it('emits the message to each socket before disconnecting', () => {
    const socket = makeSocket();
    registry.add('user-1', socket);
    registry.disconnectUser('user-1', 'You have been logged out.');
    expect(socket.emit).toHaveBeenCalledWith('bot-message', expect.objectContaining({
      response: 'You have been logged out.',
      intent: 'error',
    }));
  });

  it('isolates sockets between different users', () => {
    const s1 = makeSocket('s1');
    const s2 = makeSocket('s2');
    registry.add('user-1', s1);
    registry.add('user-2', s2);
    registry.disconnectUser('user-1', 'bye');
    expect(s1.disconnect).toHaveBeenCalledTimes(1);
    expect(s2.disconnect).not.toHaveBeenCalled();
  });
});

// --- RateLimiter ---

describe('RateLimiter', () => {
  let limiter;

  beforeEach(() => {
    limiter = new RateLimiter(60_000, 3);
  });

  it('allows the first request', () => {
    expect(limiter.allow('user-1')).toBe(true);
  });

  it('allows up to the max requests in a window', () => {
    expect(limiter.allow('user-1')).toBe(true);
    expect(limiter.allow('user-1')).toBe(true);
    expect(limiter.allow('user-1')).toBe(true);
  });

  it('blocks requests beyond the max', () => {
    limiter.allow('user-1');
    limiter.allow('user-1');
    limiter.allow('user-1');
    expect(limiter.allow('user-1')).toBe(false);
    expect(limiter.allow('user-1')).toBe(false);
  });

  it('resets the count after the window expires', () => {
    const fastLimiter = new RateLimiter(1, 2); // 1ms window
    fastLimiter.allow('user-1');
    fastLimiter.allow('user-1');
    expect(fastLimiter.allow('user-1')).toBe(false);

    return new Promise((resolve) => setTimeout(() => {
      expect(fastLimiter.allow('user-1')).toBe(true);
      resolve();
    }, 5));
  });

  it('tracks limits independently per user', () => {
    limiter.allow('user-1');
    limiter.allow('user-1');
    limiter.allow('user-1');
    expect(limiter.allow('user-1')).toBe(false);
    expect(limiter.allow('user-2')).toBe(true);
  });

  it('a new instance has a clean slate', () => {
    limiter.allow('user-1');
    limiter.allow('user-1');
    limiter.allow('user-1');
    expect(limiter.allow('user-1')).toBe(false);

    const fresh = new RateLimiter(60_000, 3);
    expect(fresh.allow('user-1')).toBe(true);
  });
});
