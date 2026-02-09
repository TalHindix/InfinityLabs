import { describe, it, expect, beforeEach, vi } from 'vitest';
import { renderHook, waitFor, act } from '@testing-library/react';
import { useAsyncOperation } from '../../shared/useAsyncOperation';

describe('useAsyncOperation', () => {
  beforeEach(() => {
    vi.clearAllMocks();
  });

  it('should start with loading false by default', () => {
    const { result } = renderHook(() => useAsyncOperation());

    expect(result.current.loading).toBe(false);
    expect(result.current.error).toBe('');
  });

  it('should start with loading true if initialLoading is true', () => {
    const { result } = renderHook(() => useAsyncOperation(true));

    expect(result.current.loading).toBe(true);
  });

  it('should set loading to true when execute starts', async () => {
    const { result } = renderHook(() => useAsyncOperation());

    const promise = new Promise((resolve) => setTimeout(() => resolve('success'), 100));
    
    act(() => {
      result.current.execute(() => promise);
    });

    await waitFor(() => {
      expect(result.current.loading).toBe(true);
    });
  });

  it('should set loading to false when execute finishes', async () => {
    const { result } = renderHook(() => useAsyncOperation());

    const promise = Promise.resolve('success');
    await result.current.execute(() => promise);

    expect(result.current.loading).toBe(false);
  });

  it('should return result when operation succeeds', async () => {
    const { result } = renderHook(() => useAsyncOperation());

    const operationResult = { data: 'test' };
    const executeResult = await result.current.execute(() => Promise.resolve(operationResult));

    expect(executeResult.result).toEqual(operationResult);
    expect(executeResult.error).toBe('');
    expect(result.current.error).toBe('');
  });

  it('should set error when operation fails', async () => {
    const { result } = renderHook(() => useAsyncOperation());

    const errorMessage = 'Something went wrong';
    
    await act(async () => {
      await result.current.execute(() => Promise.reject(new Error(errorMessage)));
    });

    await waitFor(() => {
      expect(result.current.error).toBe(errorMessage);
    });
  });

  it('should return error when operation fails', async () => {
    const { result } = renderHook(() => useAsyncOperation());

    const errorMessage = 'Operation failed';
    const executeResult = await result.current.execute(() => Promise.reject(new Error(errorMessage)));

    expect(executeResult.result).toBeUndefined();
    expect(executeResult.error).toBe(errorMessage);
  });

  it('should clear error when new operation starts', async () => {
    const { result } = renderHook(() => useAsyncOperation());

    await act(async () => {
      await result.current.execute(() => Promise.reject(new Error('First error')));
    });

    await waitFor(() => {
      expect(result.current.error).toBe('First error');
    });

    await act(async () => {
      await result.current.execute(() => Promise.resolve('success'));
    });

    await waitFor(() => {
      expect(result.current.error).toBe('');
    });
  });

  it('should call onSuccess callback when operation succeeds', async () => {
    const { result } = renderHook(() => useAsyncOperation());
    const onSuccessCallback = vi.fn();

    const operationResult = { data: 'test' };
    await result.current.execute(() => Promise.resolve(operationResult), onSuccessCallback);

    expect(onSuccessCallback).toHaveBeenCalledWith(operationResult);
  });

  it('should not call onSuccess callback when operation fails', async () => {
    const { result } = renderHook(() => useAsyncOperation());
    const onSuccessCallback = vi.fn();

    await result.current.execute(() => Promise.reject(new Error('Error')), onSuccessCallback);

    expect(onSuccessCallback).not.toHaveBeenCalled();
  });

  it('should allow manual error setting', () => {
    const { result } = renderHook(() => useAsyncOperation());

    act(() => {
      result.current.setError('Manual error');
    });

    expect(result.current.error).toBe('Manual error');
  });

  it('should handle async operations that take time', async () => {
    const { result } = renderHook(() => useAsyncOperation());

    const startTime = Date.now();
    
    await act(async () => {
      await result.current.execute(() => new Promise((resolve) => setTimeout(() => resolve('done'), 50)));
    });
    
    const endTime = Date.now();

    expect(endTime - startTime).toBeGreaterThanOrEqual(50);
    
    await waitFor(() => {
      expect(result.current.loading).toBe(false);
    });
  });
});
