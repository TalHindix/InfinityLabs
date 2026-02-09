/**
 * CRITICAL TEST: API Service Testing
 * 
 * WHAT ARE WE TESTING?
 * The authService that handles authentication API calls:
 * - login: POST /auth/login with email/password
 * - signup: POST /auth/signup with user data
 * - resendVerification: POST /auth/resend-verification with email
 * - logout: POST /auth/logout
 * 
 * WHY IS THIS CRITICAL FOR INTERVIEWS?
 * Interviewers ask: "How do you test API calls? How do you mock dependencies?"
 * This test demonstrates:
 * - Mocking httpClient to isolate service logic
 * - Verifying correct endpoints and parameters
 * - Testing response data extraction (res.data.data pattern)
 * - Testing error handling
 */

import { describe, it, expect, beforeEach, vi } from 'vitest';
import { authService } from '../../api/auth.service';
import { httpClient } from '../../api/http-client';
import type { SignupData } from '../../types';

// Mock the httpClient module
vi.mock('../../api/http-client', () => ({
  httpClient: {
    post: vi.fn(),
  },
}));

describe('authService - Critical API Service Tests', () => {
  beforeEach(() => {
    vi.clearAllMocks();
  });

  describe('login', () => {
    it('should call httpClient.post with correct endpoint and data', async () => {
      const mockResponse = {
        data: {
          data: {
            user: { id: '1', email: 'test@example.com', firstName: 'Test', lastName: 'User' },
          },
        },
      };
      vi.mocked(httpClient.post).mockResolvedValue(mockResponse);

      await authService.login('test@example.com', 'password123');

      expect(httpClient.post).toHaveBeenCalledTimes(1);
      expect(httpClient.post).toHaveBeenCalledWith('/auth/login', {
        email: 'test@example.com',
        password: 'password123',
      });
    });

    it('should extract data.data from response', async () => {
      const mockUser = { id: '1', email: 'test@example.com', firstName: 'Test', lastName: 'User' };
      const mockResponse = {
        data: {
          data: {
            user: mockUser,
          },
        },
      };
      vi.mocked(httpClient.post).mockResolvedValue(mockResponse);

      const result = await authService.login('test@example.com', 'password123');

      expect(result).toEqual({ user: mockUser });
    });

    it('should handle errors and propagate them', async () => {
      const errorMessage = 'Invalid credentials';
      vi.mocked(httpClient.post).mockRejectedValue(new Error(errorMessage));

      await expect(authService.login('test@example.com', 'wrongpassword')).rejects.toThrow(
        errorMessage
      );
    });
  });

  describe('signup', () => {
    it('should call httpClient.post with correct endpoint and signup data', async () => {
      const mockResponse = {
        data: {
          data: {
            message: 'User created successfully',
          },
        },
      };
      vi.mocked(httpClient.post).mockResolvedValue(mockResponse);

      const signupData: SignupData = {
        firstName: 'John',
        lastName: 'Doe',
        email: 'john@example.com',
        password: 'password123',
        phone: '+972526006496',
      };

      await authService.signup(signupData);

      expect(httpClient.post).toHaveBeenCalledTimes(1);
      expect(httpClient.post).toHaveBeenCalledWith('/auth/signup', signupData);
    });

    it('should extract data.data from signup response', async () => {
      const mockResponse = {
        data: {
          data: {
            message: 'User created successfully',
          },
        },
      };
      vi.mocked(httpClient.post).mockResolvedValue(mockResponse);

      const signupData: SignupData = {
        firstName: 'John',
        lastName: 'Doe',
        email: 'john@example.com',
        password: 'password123',
        phone: '+972526006496',
      };

      const result = await authService.signup(signupData);

      expect(result).toEqual({ message: 'User created successfully' });
    });

    it('should handle signup errors', async () => {
      const errorMessage = 'Email already exists';
      vi.mocked(httpClient.post).mockRejectedValue(new Error(errorMessage));

      const signupData: SignupData = {
        firstName: 'John',
        lastName: 'Doe',
        email: 'existing@example.com',
        password: 'password123',
        phone: '+972526006496',
      };

      await expect(authService.signup(signupData)).rejects.toThrow(errorMessage);
    });
  });

  describe('resendVerification', () => {
    it('should call httpClient.post with correct endpoint and email', async () => {
      const mockResponse = {
        data: {
          data: {
            message: 'Verification email sent',
          },
        },
      };
      vi.mocked(httpClient.post).mockResolvedValue(mockResponse);

      await authService.resendVerification('test@example.com');

      expect(httpClient.post).toHaveBeenCalledTimes(1);
      expect(httpClient.post).toHaveBeenCalledWith('/auth/resend-verification', {
        email: 'test@example.com',
      });
    });

    it('should extract data.data from resendVerification response', async () => {
      const mockResponse = {
        data: {
          data: {
            message: 'Verification email sent',
          },
        },
      };
      vi.mocked(httpClient.post).mockResolvedValue(mockResponse);

      const result = await authService.resendVerification('test@example.com');

      expect(result).toEqual({ message: 'Verification email sent' });
    });

    it('should handle resendVerification errors', async () => {
      const errorMessage = 'User not found';
      vi.mocked(httpClient.post).mockRejectedValue(new Error(errorMessage));

      await expect(authService.resendVerification('nonexistent@example.com')).rejects.toThrow(
        errorMessage
      );
    });
  });

  describe('logout', () => {
    it('should call httpClient.post with correct endpoint', async () => {
      vi.mocked(httpClient.post).mockResolvedValue({ data: {} });

      await authService.logout();

      expect(httpClient.post).toHaveBeenCalledTimes(1);
      expect(httpClient.post).toHaveBeenCalledWith('/auth/logout');
    });

    it('should not throw on successful logout', async () => {
      vi.mocked(httpClient.post).mockResolvedValue({ data: {} });

      await expect(authService.logout()).resolves.not.toThrow();
    });

    it('should handle logout errors', async () => {
      const errorMessage = 'Network error';
      vi.mocked(httpClient.post).mockRejectedValue(new Error(errorMessage));

      await expect(authService.logout()).rejects.toThrow(errorMessage);
    });
  });
});
