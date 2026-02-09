import { describe, it, expect, beforeEach, vi } from 'vitest';
import { renderHook, act, waitFor } from '@testing-library/react';
import { useTransfer } from '../../screens/transfer-money/useTransfer';
import { transactionService } from '../../api/transaction.service';
import type { TransactionResponse } from '../../types';

vi.mock('../../api/transaction.service', () => ({
  transactionService: {
    create: vi.fn(),
  },
}));

describe('useTransfer', () => {
  beforeEach(() => {
    vi.clearAllMocks();
  });

  it('should initialize with empty values', () => {
    const { result } = renderHook(() => useTransfer());

    expect(result.current.receiverEmail).toBe('');
    expect(result.current.amount).toBe('');
    expect(result.current.description).toBe('');
    expect(result.current.loading).toBe(false);
    expect(result.current.error).toBe('');
    expect(result.current.success).toBe(false);
  });

  it('should update receiverEmail when setReceiverEmail is called', () => {
    const { result } = renderHook(() => useTransfer());

    act(() => {
      result.current.setReceiverEmail('test@example.com');
    });

    expect(result.current.receiverEmail).toBe('test@example.com');
  });

  it('should update amount when setAmount is called', () => {
    const { result } = renderHook(() => useTransfer());

    act(() => {
      result.current.setAmount('100');
    });

    expect(result.current.amount).toBe('100');
  });

  it('should update description when setDescription is called', () => {
    const { result } = renderHook(() => useTransfer());

    act(() => {
      result.current.setDescription('Test transfer');
    });

    expect(result.current.description).toBe('Test transfer');
  });

  it('should set loading to true during transfer', async () => {
    const mockTransactionResponse: TransactionResponse = {
      transaction: {
        _id: '1',
        id: 1,
        fromEmail: 'sender@example.com',
        toEmail: 'receiver@example.com',
        amount: 100,
        createdAt: new Date().toISOString(),
        description: 'Test',
      },
    };
    vi.mocked(transactionService.create).mockImplementation(
      () => new Promise((resolve) => setTimeout(() => resolve(mockTransactionResponse), 100))
    );

    const { result } = renderHook(() => useTransfer());

    act(() => {
      result.current.setReceiverEmail('receiver@example.com');
      result.current.setAmount('100');
      result.current.setDescription('Test');
    });

    act(() => {
      result.current.handleSubmit({
        preventDefault: vi.fn(),
      } as any);
    });

    expect(result.current.loading).toBe(true);

    await waitFor(() => {
      expect(result.current.loading).toBe(false);
    });
  });

  it('should reset form fields on successful transfer', async () => {
    const mockTransactionResponse: TransactionResponse = {
      transaction: {
        _id: '1',
        id: 1,
        fromEmail: 'sender@example.com',
        toEmail: 'receiver@example.com',
        amount: 100,
        createdAt: new Date().toISOString(),
        description: 'Test transfer',
      },
    };
    vi.mocked(transactionService.create).mockResolvedValue(mockTransactionResponse);

    const { result } = renderHook(() => useTransfer());

    act(() => {
      result.current.setReceiverEmail('receiver@example.com');
      result.current.setAmount('100');
      result.current.setDescription('Test transfer');
    });

    await act(async () => {
      await result.current.handleSubmit({
        preventDefault: vi.fn(),
      } as any);
    });

    await waitFor(() => {
      expect(result.current.success).toBe(true);
      expect(result.current.receiverEmail).toBe('');
      expect(result.current.amount).toBe('');
      expect(result.current.description).toBe('');
    });
  });

  it('should not submit if description is empty', async () => {
    const { result } = renderHook(() => useTransfer());

    act(() => {
      result.current.setReceiverEmail('receiver@example.com');
      result.current.setAmount('100');
      result.current.setDescription('');
    });

    await act(async () => {
      await result.current.handleSubmit({
        preventDefault: vi.fn(),
      } as any);
    });

    expect(transactionService.create).not.toHaveBeenCalled();
  });

  it('should set error on failed transfer', async () => {
    const errorMessage = 'Transfer failed';
    vi.mocked(transactionService.create).mockRejectedValue(new Error(errorMessage));

    const { result } = renderHook(() => useTransfer());

    act(() => {
      result.current.setReceiverEmail('receiver@example.com');
      result.current.setAmount('100');
      result.current.setDescription('Test');
    });

    await act(async () => {
      await result.current.handleSubmit({
        preventDefault: vi.fn(),
      } as any);
    });

    await waitFor(() => {
      expect(result.current.error).toBeTruthy();
      expect(result.current.success).toBe(false);
    });
  });

  it('should call transactionService.create with correct parameters', async () => {
    const mockTransactionResponse: TransactionResponse = {
      transaction: {
        _id: '1',
        id: 1,
        fromEmail: 'sender@example.com',
        toEmail: 'receiver@example.com',
        amount: 100.5,
        createdAt: new Date().toISOString(),
        description: 'Test transfer',
      },
    };
    vi.mocked(transactionService.create).mockResolvedValue(mockTransactionResponse);

    const { result } = renderHook(() => useTransfer());

    act(() => {
      result.current.setReceiverEmail('receiver@example.com');
      result.current.setAmount('100.50');
      result.current.setDescription('Test transfer');
    });

    await act(async () => {
      await result.current.handleSubmit({
        preventDefault: vi.fn(),
      } as any);
    });

    expect(transactionService.create).toHaveBeenCalledWith({
      receiverEmail: 'receiver@example.com',
      amount: 100.5,
      description: 'Test transfer',
    });
  });

  it('should reset success flag when submitting again', async () => {
    const mockTransactionResponse: TransactionResponse = {
      transaction: {
        _id: '1',
        id: 1,
        fromEmail: 'sender@example.com',
        toEmail: 'receiver@example.com',
        amount: 100,
        createdAt: new Date().toISOString(),
        description: 'First transfer',
      },
    };
    vi.mocked(transactionService.create).mockResolvedValue(mockTransactionResponse);

    const { result } = renderHook(() => useTransfer());

    act(() => {
      result.current.setReceiverEmail('receiver@example.com');
      result.current.setAmount('100');
      result.current.setDescription('First transfer');
    });

    await act(async () => {
      await result.current.handleSubmit({
        preventDefault: vi.fn(),
      } as any);
    });

    await waitFor(() => {
      expect(result.current.success).toBe(true);
    });

    act(() => {
      result.current.setReceiverEmail('receiver2@example.com');
      result.current.setAmount('200');
      result.current.setDescription('Second transfer');
    });

    act(() => {
      result.current.handleSubmit({
        preventDefault: vi.fn(),
      } as any);
    });

    expect(result.current.success).toBe(false);
  });
});
