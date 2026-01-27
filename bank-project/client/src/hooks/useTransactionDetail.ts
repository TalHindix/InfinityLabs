import { useState } from 'react';
import { transactionService, type Transaction } from '../services/transaction';
import { getErrorMessage } from '../types';

export const useTransactionDetail = () => {
  const [selectedTransaction, setSelectedTransaction] = useState<Transaction | null>(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');

  const loadTransactionDetail = async (id: string) => {
    setLoading(true);
    setError('');
    try {
      const data = await transactionService.getById(id);
      setSelectedTransaction(data.transaction);
    } catch (err: unknown) {
      setError(getErrorMessage(err));
      setSelectedTransaction(null);
    } finally {
      setLoading(false);
    }
  };

  return {
    selectedTransaction,
    detailLoading: loading,
    error,
    loadTransactionDetail
  };
};
