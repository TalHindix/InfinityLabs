/**
 * CRITICAL TEST: API Service Testing
 * 
 * WHAT ARE WE TESTING?
 * The transactionService that handles transaction API calls:
 * - getAll: GET /transactions with pagination (page, limit)
 * - getById: GET /transactions/:id
 * - create: POST /transactions with transfer data
 * 
 * WHY IS THIS CRITICAL FOR INTERVIEWS?
 * Interviewers ask: "How do you test API calls with query parameters?"
 * This test demonstrates:
 * - Testing GET requests with query parameters
 * - Testing POST requests with payloads
 * - Verifying correct URL construction
 * - Testing pagination parameters
 */

import { describe, it, expect, beforeEach, vi } from 'vitest';
import { transactionService } from '../../api/transaction.service';
import { httpClient } from '../../api/http.client';
import type { TransferData } from '../../types';

// Mock the httpClient module
vi.mock('../../api/http.client', () => ({
  httpClient: {
    get: vi.fn(),
    post: vi.fn(),
  },
}));

describe('transactionService - Critical API Service Tests', () => {
  beforeEach(() => {
    vi.clearAllMocks();
  });

  describe('getAll', () => {
    it('should call httpClient.get with default pagination parameters', async () => {
      const mockResponse = {
        data: {
          transactions: [],
          total: 0,
          totalPages: 1,
          currentPage: 1,
        },
      };
      vi.mocked(httpClient.get).mockResolvedValue(mockResponse);

      await transactionService.getAll();

      expect(httpClient.get).toHaveBeenCalledTimes(1);
      expect(httpClient.get).toHaveBeenCalledWith('/transactions?page=1&limit=10');
    });

    it('should call httpClient.get with custom pagination parameters', async () => {
      const mockResponse = {
        data: {
          data: {
            transactions: [],
            total: 0,
            totalPages: 1,
            currentPage: 2,
          },
        },
      };
      vi.mocked(httpClient.get).mockResolvedValue(mockResponse);

      await transactionService.getAll(2, 20);

      expect(httpClient.get).toHaveBeenCalledTimes(1);
      expect(httpClient.get).toHaveBeenCalledWith('/transactions?page=2&limit=20');
    });

    it('should extract data.data from getAll response', async () => {
      const mockTransactions = [
        {
          _id: '1',
          id: 1,
          fromEmail: 'sender@example.com',
          toEmail: 'receiver@example.com',
          amount: 100,
          createdAt: '2026-01-01T00:00:00Z',
          description: 'Test transfer',
        },
      ];
      const mockResponse = {
        data: {
          transactions: mockTransactions,
          total: 1,
          totalPages: 1,
          currentPage: 1,
        },
      };
      vi.mocked(httpClient.get).mockResolvedValue(mockResponse);

      const result = await transactionService.getAll();

      expect(result).toEqual({
        transactions: mockTransactions,
        total: 1,
        totalPages: 1,
        currentPage: 1,
      });
    });

    it('should handle getAll errors', async () => {
      const errorMessage = 'Failed to fetch transactions';
      vi.mocked(httpClient.get).mockRejectedValue(new Error(errorMessage));

      await expect(transactionService.getAll()).rejects.toThrow(errorMessage);
    });
  });

  describe('getById', () => {
    it('should call httpClient.get with correct transaction id', async () => {
      const mockResponse = {
        data: {
          transaction: {
            _id: '123',
            id: 1,
            fromEmail: 'sender@example.com',
            toEmail: 'receiver@example.com',
            amount: 100,
            createdAt: '2026-01-01T00:00:00Z',
          },
        },
      };
      vi.mocked(httpClient.get).mockResolvedValue(mockResponse);

      await transactionService.getById('123');

      expect(httpClient.get).toHaveBeenCalledTimes(1);
      expect(httpClient.get).toHaveBeenCalledWith('/transactions/123');
    });

    it('should extract data.data from getById response', async () => {
      const mockTransaction = {
        _id: '123',
        id: 1,
        fromEmail: 'sender@example.com',
        toEmail: 'receiver@example.com',
        amount: 100,
        createdAt: '2026-01-01T00:00:00Z',
        description: 'Test transfer',
      };
      const mockResponse = {
        data: {
          transaction: mockTransaction,
        },
      };
      vi.mocked(httpClient.get).mockResolvedValue(mockResponse);

      const result = await transactionService.getById('123');

      expect(result).toEqual({ transaction: mockTransaction });
    });

    it('should handle getById errors', async () => {
      const errorMessage = 'Transaction not found';
      vi.mocked(httpClient.get).mockRejectedValue(new Error(errorMessage));

      await expect(transactionService.getById('nonexistent')).rejects.toThrow(errorMessage);
    });
  });

  describe('create', () => {
    it('should call httpClient.post with correct endpoint and transfer data', async () => {
      const mockResponse = {
        data: {
          transaction: {
            _id: '123',
            id: 1,
            fromEmail: 'sender@example.com',
            toEmail: 'receiver@example.com',
            amount: 100,
            createdAt: '2026-01-01T00:00:00Z',
          },
        },
      };
      vi.mocked(httpClient.post).mockResolvedValue(mockResponse);

      const transferData: TransferData = {
        receiverEmail: 'receiver@example.com',
        amount: 100,
        description: 'Test transfer',
      };

      await transactionService.create(transferData);

      expect(httpClient.post).toHaveBeenCalledTimes(1);
      expect(httpClient.post).toHaveBeenCalledWith('/transactions', transferData);
    });

    it('should extract data.data from create response', async () => {
      const mockTransaction = {
        _id: '123',
        id: 1,
        fromEmail: 'sender@example.com',
        toEmail: 'receiver@example.com',
        amount: 100,
        createdAt: '2026-01-01T00:00:00Z',
        description: 'Test transfer',
      };
      const mockResponse = {
        data: {
          transaction: mockTransaction,
        },
      };
      vi.mocked(httpClient.post).mockResolvedValue(mockResponse);

      const transferData: TransferData = {
        receiverEmail: 'receiver@example.com',
        amount: 100,
        description: 'Test transfer',
      };

      const result = await transactionService.create(transferData);

      expect(result).toEqual({ transaction: mockTransaction });
    });

    it('should handle create errors', async () => {
      const errorMessage = 'Insufficient balance';
      vi.mocked(httpClient.post).mockRejectedValue(new Error(errorMessage));

      const transferData: TransferData = {
        receiverEmail: 'receiver@example.com',
        amount: 1000,
        description: 'Large transfer',
      };

      await expect(transactionService.create(transferData)).rejects.toThrow(errorMessage);
    });
  });
});
