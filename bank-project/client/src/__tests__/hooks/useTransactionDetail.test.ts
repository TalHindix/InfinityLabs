/**
 * CRITICAL TEST: Hook Testing - Async Operation with State Management
 * 
 * WHAT ARE WE TESTING?
 * The useTransactionDetail hook that:
 * - Loads a single transaction by ID
 * - Uses useAsyncOperation for async handling
 * - Manages selectedTransaction state
 * - Clears transaction on error
 * 
 * WHY IS THIS CRITICAL FOR INTERVIEWS?
 * Interviewers ask: "How do you test async operations in hooks? How do you manage state?"
 * This test demonstrates:
 * - Async operation pattern
 * - State management for selected item
 * - Error handling
 * - Integration with useAsyncOperation
 */

import { describe, it, expect, beforeEach, vi } from 'vitest';
import { renderHook, act, waitFor } from '@testing-library/react';
import { useTransactionDetail } from '../../screens/transaction-history/useTransactionDetail';
import { transactionService } from '../../api/transaction.service';

vi.mock('../../api/transaction.service', () => ({
  transactionService: {
    getById: vi.fn(),
  },
}));

describe('useTransactionDetail - Critical Async Operation Tests', () => {
  beforeEach(() => {
    vi.clearAllMocks();
  });

  describe('Initial state', () => {
    it('should start with null selectedTransaction', () => {
      const { result } = renderHook(() => useTransactionDetail());

      expect(result.current.selectedTransaction).toBeNull();
    });

    it('should start with loading false and no error', () => {
      const { result } = renderHook(() => useTransactionDetail());

      expect(result.current.loading).toBe(false);
      expect(result.current.error).toBe('');
    });
  });

  describe('Loading transaction detail', () => {
    it('should call transactionService.getById with correct id', async () => {
      const mockTransaction = {
        _id: '123',
        id: 1,
        fromEmail: 'sender@example.com',
        toEmail: 'receiver@example.com',
        amount: 100,
        createdAt: '2026-01-01T00:00:00Z',
        description: 'Test transfer',
      };

      vi.mocked(transactionService.getById).mockResolvedValue({
        transaction: mockTransaction,
      });

      const { result } = renderHook(() => useTransactionDetail());

      await act(async () => {
        await result.current.loadTransactionDetail('123');
      });

      expect(transactionService.getById).toHaveBeenCalledWith('123');
    });

    it('should set selectedTransaction on successful load', async () => {
      const mockTransaction = {
        _id: '123',
        id: 1,
        fromEmail: 'sender@example.com',
        toEmail: 'receiver@example.com',
        amount: 100,
        createdAt: '2026-01-01T00:00:00Z',
        description: 'Test transfer',
      };

      vi.mocked(transactionService.getById).mockResolvedValue({
        transaction: mockTransaction,
      });

      const { result } = renderHook(() => useTransactionDetail());

      await act(async () => {
        await result.current.loadTransactionDetail('123');
      });

      await waitFor(() => {
        expect(result.current.selectedTransaction).toEqual(mockTransaction);
      });
    });

    it('should set loading to true during load', async () => {
      vi.mocked(transactionService.getById).mockImplementation(
        () => new Promise((resolve) => setTimeout(() => resolve({ transaction: {} as any }), 100))
      );

      const { result } = renderHook(() => useTransactionDetail());

      act(() => {
        result.current.loadTransactionDetail('123');
      });

      expect(result.current.loading).toBe(true);

      await waitFor(() => {
        expect(result.current.loading).toBe(false);
      });
    });

    it('should set loading to false after load completes', async () => {
      const mockTransaction = {
        _id: '123',
        id: 1,
        fromEmail: 'sender@example.com',
        toEmail: 'receiver@example.com',
        amount: 100,
        createdAt: '2026-01-01T00:00:00Z',
      };

      vi.mocked(transactionService.getById).mockResolvedValue({
        transaction: mockTransaction,
      });

      const { result } = renderHook(() => useTransactionDetail());

      await act(async () => {
        await result.current.loadTransactionDetail('123');
      });

      await waitFor(() => {
        expect(result.current.loading).toBe(false);
      });
    });
  });

  describe('Error handling', () => {
    it('should handle errors when loading transaction', async () => {
      const errorMessage = 'Transaction not found';
      vi.mocked(transactionService.getById).mockRejectedValue(new Error(errorMessage));

      const { result } = renderHook(() => useTransactionDetail());

      await act(async () => {
        await result.current.loadTransactionDetail('nonexistent');
      });

      await waitFor(() => {
        expect(result.current.error).toBeTruthy();
      });
    });

    it('should clear selectedTransaction on error', async () => {
      // First, set a transaction
      const mockTransaction = {
        _id: '123',
        id: 1,
        fromEmail: 'sender@example.com',
        toEmail: 'receiver@example.com',
        amount: 100,
        createdAt: '2026-01-01T00:00:00Z',
      };

      vi.mocked(transactionService.getById).mockResolvedValueOnce({
        transaction: mockTransaction,
      });

      const { result } = renderHook(() => useTransactionDetail());

      await act(async () => {
        await result.current.loadTransactionDetail('123');
      });

      await waitFor(() => {
        expect(result.current.selectedTransaction).toEqual(mockTransaction);
      });

      // Now try to load a non-existent transaction
      vi.mocked(transactionService.getById).mockRejectedValue(new Error('Not found'));

      await act(async () => {
        await result.current.loadTransactionDetail('nonexistent');
      });

      await waitFor(() => {
        expect(result.current.selectedTransaction).toBeNull();
      });
    });

    it('should set error message on failure', async () => {
      const errorMessage = 'Failed to load transaction';
      vi.mocked(transactionService.getById).mockRejectedValue(new Error(errorMessage));

      const { result } = renderHook(() => useTransactionDetail());

      await act(async () => {
        await result.current.loadTransactionDetail('123');
      });

      await waitFor(() => {
        expect(result.current.error).toBe(errorMessage);
      });
    });
  });

  describe('Loading multiple transactions', () => {
    it('should replace previous transaction when loading new one', async () => {
      const firstTransaction = {
        _id: '1',
        id: 1,
        fromEmail: 'sender1@example.com',
        toEmail: 'receiver1@example.com',
        amount: 100,
        createdAt: '2026-01-01T00:00:00Z',
      };

      const secondTransaction = {
        _id: '2',
        id: 2,
        fromEmail: 'sender2@example.com',
        toEmail: 'receiver2@example.com',
        amount: 200,
        createdAt: '2026-01-02T00:00:00Z',
      };

      vi.mocked(transactionService.getById)
        .mockResolvedValueOnce({ transaction: firstTransaction })
        .mockResolvedValueOnce({ transaction: secondTransaction });

      const { result } = renderHook(() => useTransactionDetail());

      await act(async () => {
        await result.current.loadTransactionDetail('1');
      });

      await waitFor(() => {
        expect(result.current.selectedTransaction).toEqual(firstTransaction);
      });

      await act(async () => {
        await result.current.loadTransactionDetail('2');
      });

      await waitFor(() => {
        expect(result.current.selectedTransaction).toEqual(secondTransaction);
      });
    });
  });
});
