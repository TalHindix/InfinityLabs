import { useState } from 'react';
import { transactionsService } from '../services/transactions.service';
import { type Transaction } from '../types';
import { getErrorMessage } from '../types';

export const useTransactionDetail = () => {
  const [selectedTransaction, setSelectedTransaction] = useState<Transaction | null>(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');

  const loadTransactionDetail = async (id: string) => {
    setLoading(true);
    setError('');
    try {
      const data = await transactionsService.getById(id);
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
