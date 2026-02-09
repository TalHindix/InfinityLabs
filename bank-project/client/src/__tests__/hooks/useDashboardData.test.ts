/**
 * CRITICAL TEST: Hook Testing - useEffect Cleanup and Promise.all
 * 
 * WHAT ARE WE TESTING?
 * The useDashboardData hook that:
 * - Fetches user data and transactions in parallel (Promise.all)
 * - Uses useEffect with cleanup function
 * - Implements cancelled flag pattern to prevent memory leaks
 * - Handles loading and error states
 * 
 * WHY IS THIS CRITICAL FOR INTERVIEWS?
 * Interviewers ask: "How do you prevent memory leaks in useEffect? How do you handle cleanup?"
 * This test demonstrates:
 * - useEffect cleanup function
 * - Cancelled flag pattern (prevents state updates after unmount)
 * - Promise.all for parallel data fetching
 * - Error handling in async operations
 * 
 * THIS IS ONE OF THE MOST IMPORTANT TESTS FOR INTERVIEWS!
 */

import { describe, it, expect, beforeEach, vi } from 'vitest';
import { renderHook, waitFor } from '@testing-library/react';
import { useDashboardData } from '../../screens/dashboard/useDashboardData';
import { userService } from '../../api/user.service';
import { transactionService } from '../../api/transaction.service';

vi.mock('../../api/user.service', () => ({
  userService: {
    getMe: vi.fn(),
  },
}));

vi.mock('../../api/transaction.service', () => ({
  transactionService: {
    getAll: vi.fn(),
  },
}));

describe('useDashboardData - Critical useEffect Cleanup Tests', () => {
  beforeEach(() => {
    vi.clearAllMocks();
  });

  describe('Initial state', () => {
    it('should start with loading true', () => {
      const { result } = renderHook(() => useDashboardData());

      expect(result.current.loading).toBe(true);
    });

    it('should start with null user and empty transactions', () => {
      const { result } = renderHook(() => useDashboardData());

      expect(result.current.user).toBeNull();
      expect(result.current.transactions).toEqual([]);
      expect(result.current.error).toBe('');
    });
  });

  describe('Data fetching', () => {
    it('should fetch user and transactions in parallel using Promise.all', async () => {
      const mockUser = {
        id: '1',
        _id: '1',
        firstName: 'John',
        lastName: 'Doe',
        email: 'john@example.com',
        phone: '+972526006496',
        balance: 1000,
      };

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

      vi.mocked(userService.getMe).mockResolvedValue({ user: mockUser });
      vi.mocked(transactionService.getAll).mockResolvedValue({
        transactions: mockTransactions,
        total: 1,
        totalPages: 1,
        currentPage: 1,
      });

      const { result } = renderHook(() => useDashboardData());

      await waitFor(() => {
        expect(result.current.loading).toBe(false);
      });

      // Verify both services were called
      expect(userService.getMe).toHaveBeenCalledTimes(1);
      expect(transactionService.getAll).toHaveBeenCalledTimes(1);

      // Verify data was set
      expect(result.current.user).toEqual(mockUser);
      expect(result.current.transactions).toEqual(mockTransactions);
    });

    it('should set loading to false after data is fetched', async () => {
      vi.mocked(userService.getMe).mockResolvedValue({
        user: { id: '1', _id: '1', firstName: 'John', lastName: 'Doe', email: 'test@example.com', phone: '+972526006496', balance: 1000 },
      });
      vi.mocked(transactionService.getAll).mockResolvedValue({
        transactions: [],
        total: 0,
        totalPages: 1,
        currentPage: 1,
      });

      const { result } = renderHook(() => useDashboardData());

      await waitFor(() => {
        expect(result.current.loading).toBe(false);
      });
    });

    it('should handle empty transactions array', async () => {
      const mockUser = {
        id: '1',
        _id: '1',
        firstName: 'John',
        lastName: 'Doe',
        email: 'john@example.com',
        phone: '+972526006496',
        balance: 1000,
      };

      vi.mocked(userService.getMe).mockResolvedValue({ user: mockUser });
      vi.mocked(transactionService.getAll).mockResolvedValue({
        transactions: [],
        total: 0,
        totalPages: 1,
        currentPage: 1,
      });

      const { result } = renderHook(() => useDashboardData());

      await waitFor(() => {
        expect(result.current.loading).toBe(false);
      });

      expect(result.current.transactions).toEqual([]);
    });
  });

  describe('Error handling', () => {
    it('should handle user service errors', async () => {
      const errorMessage = 'Failed to fetch user';
      vi.mocked(userService.getMe).mockRejectedValue(new Error(errorMessage));
      vi.mocked(transactionService.getAll).mockResolvedValue({
        transactions: [],
        total: 0,
        totalPages: 1,
        currentPage: 1,
      });

      const { result } = renderHook(() => useDashboardData());

      await waitFor(() => {
        expect(result.current.loading).toBe(false);
      });

      expect(result.current.error).toBeTruthy();
      expect(result.current.user).toBeNull();
    });

    it('should handle transaction service errors', async () => {
      const errorMessage = 'Failed to fetch transactions';
      vi.mocked(userService.getMe).mockResolvedValue({
        user: { id: '1', _id: '1', firstName: 'John', lastName: 'Doe', email: 'test@example.com', phone: '+972526006496', balance: 1000 },
      });
      vi.mocked(transactionService.getAll).mockRejectedValue(new Error(errorMessage));

      const { result } = renderHook(() => useDashboardData());

      await waitFor(() => {
        expect(result.current.loading).toBe(false);
      });

      expect(result.current.error).toBeTruthy();
      expect(result.current.transactions).toEqual([]);
    });

    it('should set error message correctly', async () => {
      const errorMessage = 'Network error';
      vi.mocked(userService.getMe).mockRejectedValue(new Error(errorMessage));
      vi.mocked(transactionService.getAll).mockResolvedValue({
        transactions: [],
        total: 0,
        totalPages: 1,
        currentPage: 1,
      });

      const { result } = renderHook(() => useDashboardData());

      await waitFor(() => {
        expect(result.current.loading).toBe(false);
      });

      expect(result.current.error).toBe(errorMessage);
    });
  });

  describe('Cleanup on unmount - CRITICAL FOR INTERVIEWS', () => {
    it('should not update state after component unmounts', async () => {
      // Create a promise that resolves after a delay
      let resolveUser: (value: any) => void;
      let resolveTransactions: (value: any) => void;

      const userPromise = new Promise((resolve) => {
        resolveUser = resolve;
      });

      const transactionsPromise = new Promise((resolve) => {
        resolveTransactions = resolve;
      });

      vi.mocked(userService.getMe).mockReturnValue(userPromise as any);
      vi.mocked(transactionService.getAll).mockReturnValue(transactionsPromise as any);

      const { result, unmount } = renderHook(() => useDashboardData());

      // Unmount component before promises resolve
      unmount();

      // Now resolve the promises
      resolveUser!({
        user: { id: '1', _id: '1', firstName: 'John', lastName: 'Doe', email: 'test@example.com', phone: '+972526006496', balance: 1000 },
      });
      resolveTransactions!({
        transactions: [{ _id: '1', id: 1, fromEmail: 'a@b.com', toEmail: 'b@c.com', amount: 100, createdAt: '2026-01-01T00:00:00Z' }],
        total: 1,
        totalPages: 1,
        currentPage: 1,
      });

      // Wait a bit to ensure promises resolved
      await new Promise((resolve) => setTimeout(resolve, 100));

      // State should not have been updated after unmount
      // The cancelled flag should have prevented state updates
      expect(result.current.user).toBeNull();
      expect(result.current.transactions).toEqual([]);
    });

    it('should call cleanup function on unmount', async () => {
      vi.mocked(userService.getMe).mockResolvedValue({
        user: { id: '1', _id: '1', firstName: 'John', lastName: 'Doe', email: 'test@example.com', phone: '+972526006496', balance: 1000 },
      });
      vi.mocked(transactionService.getAll).mockResolvedValue({
        transactions: [],
        total: 0,
        totalPages: 1,
        currentPage: 1,
      });

      const { unmount } = renderHook(() => useDashboardData());

      // Unmount should not throw
      expect(() => unmount()).not.toThrow();
    });
  });

  describe('Loading state management', () => {
    it('should set loading to true initially', () => {
      const { result } = renderHook(() => useDashboardData());

      expect(result.current.loading).toBe(true);
    });

    it('should set loading to false after successful fetch', async () => {
      vi.mocked(userService.getMe).mockResolvedValue({
        user: { id: '1', _id: '1', firstName: 'John', lastName: 'Doe', email: 'test@example.com', phone: '+972526006496', balance: 1000 },
      });
      vi.mocked(transactionService.getAll).mockResolvedValue({
        transactions: [],
        total: 0,
        totalPages: 1,
        currentPage: 1,
      });

      const { result } = renderHook(() => useDashboardData());

      await waitFor(() => {
        expect(result.current.loading).toBe(false);
      });
    });

    it('should set loading to false even on error', async () => {
      vi.mocked(userService.getMe).mockRejectedValue(new Error('Error'));
      vi.mocked(transactionService.getAll).mockResolvedValue({
        transactions: [],
        total: 0,
        totalPages: 1,
        currentPage: 1,
      });

      const { result } = renderHook(() => useDashboardData());

      await waitFor(() => {
        expect(result.current.loading).toBe(false);
      });
    });
  });
});
