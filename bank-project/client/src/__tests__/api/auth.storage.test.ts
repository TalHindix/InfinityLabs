/**
 * CRITICAL TEST: Storage Edge Cases Testing
 * 
 * WHAT ARE WE TESTING?
 * The authStorage that handles localStorage operations:
 * - getUser: retrieves user from localStorage, handles null/invalid JSON
 * - setUser: saves user to localStorage
 * - clearAuth: removes user from localStorage
 * - isAuthenticated: checks if user exists
 * - logout: clears authentication
 * 
 * WHY IS THIS CRITICAL FOR INTERVIEWS?
 * Interviewers ask: "How do you handle edge cases? Invalid JSON? Null values?"
 * This test demonstrates:
 * - Defensive programming for localStorage
 * - Handling invalid JSON gracefully
 * - Testing edge cases (null, empty, corrupted data)
 * - Testing localStorage error scenarios
 * 
 * NOTE: Using real localStorage instead of mocks for more realistic testing
 */

import { describe, it, expect, beforeEach, afterEach, vi } from 'vitest';
import { authStorage } from '../../api/auth.storage';
import type { User, StoredUser } from '../../types';

const toStored = (u: User): StoredUser => ({
  id: u.id,
  firstName: u.firstName,
  lastName: u.lastName,
  email: u.email,
});

describe('authStorage - Critical Edge Case Tests', () => {
  beforeEach(() => {
    // Clear real localStorage before each test
    localStorage.clear();
  });

  afterEach(() => {
    // Clean up after each test
    localStorage.clear();
  });

  describe('getUser', () => {
    it('should return null when localStorage is empty', () => {
      const result = authStorage.getUser();

      expect(result).toBeNull();
    });

    it('should return parsed user when valid JSON exists', () => {
      const stored: StoredUser = {
        id: '1',
        firstName: 'John',
        lastName: 'Doe',
        email: 'john@example.com',
      };

      localStorage.setItem('user', JSON.stringify(stored));

      const result = authStorage.getUser();

      expect(result).toEqual(stored);
    });

    it('should return null when JSON is invalid', () => {
      localStorage.setItem('user', 'invalid json{');

      expect(authStorage.getUser()).toBeNull();
    });

    it('should handle null value from localStorage', () => {
      // localStorage.getItem returns null when key doesn't exist
      const result = authStorage.getUser();

      expect(result).toBeNull();
    });

    it('should handle empty string in localStorage', () => {
      localStorage.setItem('user', '');

      // Empty string is falsy, so getUser returns null (doesn't reach JSON.parse)
      // The code checks `if (user)` before parsing, so empty string returns null
      const result = authStorage.getUser();
      expect(result).toBeNull();
    });
  });

  describe('setUser', () => {
    it('should save user to localStorage as JSON string, stripping sensitive fields', () => {
      const mockUser: User = {
        id: '1',
        _id: '1',
        firstName: 'John',
        lastName: 'Doe',
        email: 'john@example.com',
        phone: '+972526006496',
        balance: 1000,
      };

      authStorage.setUser(mockUser);

      const stored = localStorage.getItem('user');
      expect(stored).toBe(JSON.stringify(toStored(mockUser)));

      const result = authStorage.getUser();
      expect(result).toEqual(toStored(mockUser));
      // phone and balance must NOT be persisted
      expect(stored).not.toContain('phone');
      expect(stored).not.toContain('balance');
    });

    it('should overwrite existing user in localStorage', () => {
      const firstUser: User = {
        id: '1',
        _id: '1',
        firstName: 'John',
        lastName: 'Doe',
        email: 'john@example.com',
        phone: '+972526006496',
        balance: 1000,
      };

      const secondUser: User = {
        id: '2',
        _id: '2',
        firstName: 'Jane',
        lastName: 'Smith',
        email: 'jane@example.com',
        phone: '+972526006497',
        balance: 500,
      };

      authStorage.setUser(firstUser);
      expect(localStorage.getItem('user')).toBe(JSON.stringify(toStored(firstUser)));

      authStorage.setUser(secondUser);
      expect(localStorage.getItem('user')).toBe(JSON.stringify(toStored(secondUser)));

      const result = authStorage.getUser();
      expect(result).toEqual(toStored(secondUser));
    });
  });

  describe('clearAuth', () => {
    it('should remove user from localStorage', () => {
      const mockUser: User = {
        id: '1',
        _id: '1',
        firstName: 'John',
        lastName: 'Doe',
        email: 'john@example.com',
        phone: '+972526006496',
        balance: 1000,
      };

      localStorage.setItem('user', JSON.stringify(mockUser));

      authStorage.clearAuth();

      expect(localStorage.getItem('user')).toBeNull();
      expect(authStorage.getUser()).toBeNull();
    });

    it('should not throw when clearing non-existent user', () => {
      expect(() => authStorage.clearAuth()).not.toThrow();
      expect(localStorage.getItem('user')).toBeNull();
    });
  });

  describe('clearAuth', () => {
    it('should remove user from localStorage', () => {
      const mockUser: User = {
        id: '1',
        _id: '1',
        firstName: 'John',
        lastName: 'Doe',
        email: 'john@example.com',
        phone: '+972526006496',
        balance: 1000,
      };

      localStorage.setItem('user', JSON.stringify(mockUser));

      authStorage.clearAuth();

      expect(authStorage.getUser()).toBeNull();
    });
  });

  describe('isAuthenticated', () => {
    it('should return false when no user exists', () => {
      const result = authStorage.isAuthenticated();

      expect(result).toBe(false);
    });

    it('should return true when user exists', () => {
      const mockUser: User = {
        id: '1',
        _id: '1',
        firstName: 'John',
        lastName: 'Doe',
        email: 'john@example.com',
        phone: '+972526006496',
        balance: 1000,
      };

      authStorage.setUser(mockUser);

      const result = authStorage.isAuthenticated();

      expect(result).toBe(true);
    });

    it('should return false after clearAuth', () => {
      const mockUser: User = {
        id: '1',
        _id: '1',
        firstName: 'John',
        lastName: 'Doe',
        email: 'john@example.com',
        phone: '+972526006496',
        balance: 1000,
      };

      authStorage.setUser(mockUser);
      expect(authStorage.isAuthenticated()).toBe(true);

      authStorage.clearAuth();
      expect(authStorage.isAuthenticated()).toBe(false);
    });

    it('should return false when getUser returns null', () => {
      // localStorage is empty, so getUser returns null
      const result = authStorage.isAuthenticated();

      expect(result).toBe(false);
    });
  });

  describe('Edge Cases - localStorage failures', () => {
    it('should handle localStorage.getItem throwing error', () => {
      // Mock localStorage.getItem to throw error
      const originalGetItem = Storage.prototype.getItem;
      Storage.prototype.getItem = vi.fn(() => {
        throw new Error('localStorage access denied');
      });

      expect(() => authStorage.getUser()).toThrow('localStorage access denied');

      // Restore original
      Storage.prototype.getItem = originalGetItem;
    });

    it('should handle localStorage.setItem throwing error', () => {
      // Mock localStorage.setItem to throw error
      const originalSetItem = Storage.prototype.setItem;
      Storage.prototype.setItem = vi.fn(() => {
        throw new Error('localStorage quota exceeded');
      });

      const mockUser: User = {
        id: '1',
        _id: '1',
        firstName: 'John',
        lastName: 'Doe',
        email: 'john@example.com',
        phone: '+972526006496',
        balance: 1000,
      };

      expect(() => authStorage.setUser(mockUser)).toThrow('localStorage quota exceeded');

      // Restore original
      Storage.prototype.setItem = originalSetItem;
    });
  });
});
