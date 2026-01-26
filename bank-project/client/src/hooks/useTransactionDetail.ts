import { useState } from 'react';
import { transactionService, type Transaction } from '../services/transaction';
import { getErrorMessage } from '../types';

export const useTransactionDetail = () => {
  const [selectedTransaction, setSelectedTransaction] = useState<Transaction | null>(null);
  const [detailLoading, setDetailLoading] = useState(false);
  const [error, setError] = useState('');

  const loadTransactionDetail = async (id: string) => {
    setDetailLoading(true);
    setError('');
    try {
      const data = await transactionService.getById(id);
      setSelectedTransaction(data.transaction);
    } catch (err: unknown) {
      setError(getErrorMessage(err));
    } finally {
      setDetailLoading(false);
    }
  };

  const clearSelection = () => {
    setSelectedTransaction(null);
  };

  return {
    selectedTransaction,
    detailLoading,
    error,
    loadTransactionDetail,
    clearSelection,
  };
};
