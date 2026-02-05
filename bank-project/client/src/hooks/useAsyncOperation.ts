// Wraps async calls with loading and error state; optional onSuccess callback.
import { useState } from 'react';
import { getErrorMessage } from '../types';

export const useAsyncOperation = (initialLoading = false) => {
  const [loading, setLoading] = useState(initialLoading);
  const [error, setError] = useState('');

  const execute = async (
    operation: () => Promise<any>,
    onSuccess?: (result: any) => void
  ) => {
    setError('');
    setLoading(true);

    try {
      const result = await operation();
      
      if (onSuccess !== undefined) {
        onSuccess(result);
      }
      
      return { result: result, error: '' };
    } catch (err: unknown) {
      const errorMessage = getErrorMessage(err);
      setError(errorMessage);
      return { result: undefined, error: errorMessage };
    } finally {
      setLoading(false);
    }
  };

  const setErrorManually = (message: string) => {
    setError(message);
  };

  return {
    loading,
    error,
    execute,
    setError: setErrorManually,
  };
};
