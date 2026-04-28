/**
 * CRITICAL TEST: API Service Testing
 * 
 * WHAT ARE WE TESTING?
 * The userService that handles user API calls:
 * - getMe: GET /me to fetch current authenticated user
 * 
 * WHY IS THIS CRITICAL FOR INTERVIEWS?
 * Interviewers ask: "How do you test simple API calls?"
 * This test demonstrates:
 * - Testing GET requests without parameters
 * - Verifying correct endpoint usage
 * - Testing response data extraction
 */

import { describe, it, expect, beforeEach, vi } from 'vitest';
import { userService } from '../../api/user.service';
import { httpClient } from '../../api/http.client';

// Mock the httpClient module
vi.mock('../../api/http.client', () => ({
  httpClient: {
    get: vi.fn(),
  },
}));

describe('userService - Critical API Service Tests', () => {
  beforeEach(() => {
    vi.clearAllMocks();
  });

  describe('getMe', () => {
    it('should call httpClient.get with correct endpoint', async () => {
      const mockResponse = {
        data: {
          user: {
            id: '1',
            _id: '1',
            firstName: 'John',
            lastName: 'Doe',
            email: 'john@example.com',
            phone: '+972526006496',
            balance: 1000,
          },
        },
      };
      vi.mocked(httpClient.get).mockResolvedValue(mockResponse);

      await userService.getMe();

      expect(httpClient.get).toHaveBeenCalledTimes(1);
      expect(httpClient.get).toHaveBeenCalledWith('/me');
    });

    it('should extract data.data from getMe response', async () => {
      const mockUser = {
        id: '1',
        _id: '1',
        firstName: 'John',
        lastName: 'Doe',
        email: 'john@example.com',
        phone: '+972526006496',
        balance: 1000,
      };
      const mockResponse = {
        data: {
          user: mockUser,
        },
      };
      vi.mocked(httpClient.get).mockResolvedValue(mockResponse);

      const result = await userService.getMe();

      expect(result).toEqual({ user: mockUser });
    });

    it('should handle getMe errors', async () => {
      const errorMessage = 'Unauthorized';
      vi.mocked(httpClient.get).mockRejectedValue(new Error(errorMessage));

      await expect(userService.getMe()).rejects.toThrow(errorMessage);
    });

    it('should handle network errors', async () => {
      const networkError = new Error('Network request failed');
      vi.mocked(httpClient.get).mockRejectedValue(networkError);

      await expect(userService.getMe()).rejects.toThrow('Network request failed');
    });
  });
});
