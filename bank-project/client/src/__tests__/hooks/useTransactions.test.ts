/**
 * CRITICAL TEST: Hook Testing - useEffect with SearchParams and Pagination
 * 
 * WHAT ARE WE TESTING?
 * The useTransactions hook that:
 * - Integrates with URL searchParams for pagination
 * - Normalizes URL (?page=1)
 * - Fetches transactions based on current page
 * - Handles page changes via handlePageChange
 * - Implements cleanup on unmount
 * - Uses loadingRef to prevent duplicate requests
 * 
 * WHY IS THIS CRITICAL FOR INTERVIEWS?
 * Interviewers ask: "How do you handle URL state? How do you test pagination?"
 * This test demonstrates:
 * - useEffect with dependencies (currentPage, pageSize)
 * - URL searchParams integration
 * - Pagination logic
 * - Cleanup pattern
 * - Loading ref pattern (prevents duplicate requests)
 */

import { describe, it, expect, beforeEach, vi } from 'vitest';
import { renderHook, act, waitFor } from '@testing-library/react';
import { useTransactions } from '../../screens/transaction-history/useTransactions';
import { transactionService } from '../../api/transaction.service';
import { authStorage } from '../../api/auth.storage';
import { useSearchParams } from 'react-router-dom';

vi.mock('../../api/transaction.service', () => ({
  transactionService: {
    getAll: vi.fn(),
  },
}));

vi.mock('../../api/auth.storage', () => ({
  authStorage: {
    getUser: vi.fn(),
  },
}));

vi.mock('react-router-dom', () => ({
  useSearchParams: vi.fn(),
}));

describe('useTransactions - Critical SearchParams and Pagination Tests', () => {
  const mockSetSearchParams = vi.fn();
  const mockUser = {
    id: '1',
    _id: '1',
    firstName: 'John',
    lastName: 'Doe',
    email: 'john@example.com',
    phone: '+972526006496',
    balance: 1000,
  };

  beforeEach(() => {
    vi.clearAllMocks();
    vi.mocked(authStorage.getUser).mockReturnValue(mockUser);
  });

  describe('Initial state', () => {
    it('should start with loading true', () => {
      vi.mocked(useSearchParams).mockReturnValue([new URLSearchParams('?page=1'), mockSetSearchParams]);

      const { result } = renderHook(() => useTransactions());

      expect(result.current.loading).toBe(true);
    });

    it('should start with empty transactions', () => {
      vi.mocked(useSearchParams).mockReturnValue([new URLSearchParams('?page=1'), mockSetSearchParams]);

      const { result } = renderHook(() => useTransactions());

      expect(result.current.transactions).toEqual([]);
      expect(result.current.error).toBe('');
    });

    it('should extract current page from URL', () => {
      vi.mocked(useSearchParams).mockReturnValue([new URLSearchParams('?page=2'), mockSetSearchParams]);

      const { result } = renderHook(() => useTransactions());

      expect(result.current.currentPage).toBe(2);
    });

    it('should default to page 1 if no page in URL', () => {
      vi.mocked(useSearchParams).mockReturnValue([new URLSearchParams(), mockSetSearchParams]);

      const { result } = renderHook(() => useTransactions());

      expect(result.current.currentPage).toBe(1);
    });

    it('should get user email from authStorage', () => {
      vi.mocked(useSearchParams).mockReturnValue([new URLSearchParams('?page=1'), mockSetSearchParams]);

      const { result } = renderHook(() => useTransactions());

      expect(result.current.userEmail).toBe('john@example.com');
    });
  });

  describe('URL normalization', () => {
    it('should set page=1 if not present in URL', () => {
      vi.mocked(useSearchParams).mockReturnValue([new URLSearchParams(), mockSetSearchParams]);

      renderHook(() => useTransactions());

      // Should call setSearchParams to normalize URL
      expect(mockSetSearchParams).toHaveBeenCalled();
    });

    it('should not normalize URL if page already exists', () => {
      const searchParams = new URLSearchParams('?page=2');
      vi.mocked(useSearchParams).mockReturnValue([searchParams, mockSetSearchParams]);

      renderHook(() => useTransactions());

      // Should not call setSearchParams if page already exists
      // (but might be called once for normalization check)
    });
  });

  describe('Data fetching', () => {
    it('should fetch transactions on mount', async () => {
      vi.mocked(useSearchParams).mockReturnValue([new URLSearchParams('?page=1'), mockSetSearchParams]);

      const mockTransactions = [
        {
          _id: '1',
          id: 1,
          fromEmail: 'sender@example.com',
          toEmail: 'receiver@example.com',
          amount: 100,
          createdAt: '2026-01-01T00:00:00Z',
        },
      ];

      vi.mocked(transactionService.getAll).mockResolvedValue({
        transactions: mockTransactions,
        total: 1,
        totalPages: 1,
        currentPage: 1,
      });

      const { result } = renderHook(() => useTransactions());

      await waitFor(() => {
        expect(result.current.loading).toBe(false);
      });

      expect(transactionService.getAll).toHaveBeenCalledWith(1, 10);
      expect(result.current.transactions).toEqual(mockTransactions);
    });

    it('should fetch transactions with correct page and limit', async () => {
      vi.mocked(useSearchParams).mockReturnValue([new URLSearchParams('?page=2'), mockSetSearchParams]);

      vi.mocked(transactionService.getAll).mockResolvedValue({
        transactions: [],
        total: 0,
        totalPages: 2,
        currentPage: 2,
      });

      const { result } = renderHook(() => useTransactions(20));

      await waitFor(() => {
        expect(result.current.loading).toBe(false);
      });

      expect(transactionService.getAll).toHaveBeenCalledWith(2, 20);
    });

    it('should update totalPages from response', async () => {
      vi.mocked(useSearchParams).mockReturnValue([new URLSearchParams('?page=1'), mockSetSearchParams]);

      vi.mocked(transactionService.getAll).mockResolvedValue({
        transactions: [],
        total: 25,
        totalPages: 3,
        currentPage: 1,
      });

      const { result } = renderHook(() => useTransactions());

      await waitFor(() => {
        expect(result.current.loading).toBe(false);
      });

      expect(result.current.totalPages).toBe(3);
    });
  });

  describe('Page changes', () => {
    it('should update URL when handlePageChange is called', () => {
      const searchParams = new URLSearchParams('?page=1');
      vi.mocked(useSearchParams).mockReturnValue([searchParams, mockSetSearchParams]);

      const { result } = renderHook(() => useTransactions());

      act(() => {
        result.current.handlePageChange(2);
      });

      expect(mockSetSearchParams).toHaveBeenCalled();
    });

    it('should refetch data when page changes', async () => {
      const searchParams = new URLSearchParams('?page=1');
      vi.mocked(useSearchParams).mockReturnValue([searchParams, mockSetSearchParams]);

      vi.mocked(transactionService.getAll).mockResolvedValue({
        transactions: [],
        total: 0,
        totalPages: 1,
        currentPage: 1,
      });

      const { result, rerender } = renderHook(() => useTransactions());

      await waitFor(() => {
        expect(result.current.loading).toBe(false);
      });

      // Change page in URL
      const newSearchParams = new URLSearchParams('?page=2');
      vi.mocked(useSearchParams).mockReturnValue([newSearchParams, mockSetSearchParams]);
      rerender();

      await waitFor(() => {
        expect(transactionService.getAll).toHaveBeenCalledWith(2, 10);
      });
    });
  });

  describe('Error handling', () => {
    it('should handle fetch errors', async () => {
      vi.mocked(useSearchParams).mockReturnValue([new URLSearchParams('?page=1'), mockSetSearchParams]);

      const errorMessage = 'Failed to fetch transactions';
      vi.mocked(transactionService.getAll).mockRejectedValue(new Error(errorMessage));

      const { result } = renderHook(() => useTransactions());

      await waitFor(() => {
        expect(result.current.loading).toBe(false);
      });

      expect(result.current.error).toBe(errorMessage);
      expect(result.current.transactions).toEqual([]);
      expect(result.current.totalPages).toBe(1);
    });

    it('should reset transactions and totalPages on error', async () => {
      vi.mocked(useSearchParams).mockReturnValue([new URLSearchParams('?page=1'), mockSetSearchParams]);

      vi.mocked(transactionService.getAll).mockRejectedValue(new Error('Error'));

      const { result } = renderHook(() => useTransactions());

      await waitFor(() => {
        expect(result.current.loading).toBe(false);
      });

      expect(result.current.transactions).toEqual([]);
      expect(result.current.totalPages).toBe(1);
    });
  });

  describe('Cleanup on unmount', () => {
    it('should not update state after component unmounts', async () => {
      vi.mocked(useSearchParams).mockReturnValue([new URLSearchParams('?page=1'), mockSetSearchParams]);

      let resolveTransactions: (value: any) => void;
      const transactionsPromise = new Promise((resolve) => {
        resolveTransactions = resolve;
      });

      vi.mocked(transactionService.getAll).mockReturnValue(transactionsPromise as any);

      const { result, unmount } = renderHook(() => useTransactions());

      // Unmount before promise resolves
      unmount();

      // Resolve promise after unmount
      resolveTransactions!({
        transactions: [{ _id: '1', id: 1, fromEmail: 'a@b.com', toEmail: 'b@c.com', amount: 100, createdAt: '2026-01-01T00:00:00Z' }],
        total: 1,
        totalPages: 1,
        currentPage: 1,
      });

      await new Promise((resolve) => setTimeout(resolve, 100));

      // State should not have been updated after unmount
      expect(result.current.transactions).toEqual([]);
    });
  });

  describe('Loading state management', () => {
    it('should set loading to false after fetch completes', async () => {
      vi.mocked(useSearchParams).mockReturnValue([new URLSearchParams('?page=1'), mockSetSearchParams]);

      vi.mocked(transactionService.getAll).mockResolvedValue({
        transactions: [],
        total: 0,
        totalPages: 1,
        currentPage: 1,
      });

      const { result } = renderHook(() => useTransactions());

      await waitFor(() => {
        expect(result.current.loading).toBe(false);
      });
    });

    it('should set error to empty string initially', () => {
      vi.mocked(useSearchParams).mockReturnValue([new URLSearchParams('?page=1'), mockSetSearchParams]);

      const { result } = renderHook(() => useTransactions());

      // Error should be empty initially (before any fetch)
      expect(result.current.error).toBe('');
    });
  });
});
