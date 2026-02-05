import { useState } from 'react';
import { transactionsService } from '../services/transactions.service';
import { type Transaction } from '../types';
import { useAsyncOperation } from './useAsyncOperation';

export const useTransactionDetail = () => {
  const [selectedTransaction, setSelectedTransaction] = useState<Transaction | null>(null);
  const { loading, error, execute } = useAsyncOperation();

  const loadTransactionDetail = async (id: string) => {
    const { result } = await execute(
      () => transactionsService.getById(id),
      (data) => setSelectedTransaction(data.transaction)
    );
    
    if (!result) {
      setSelectedTransaction(null);
    }
  };

  return {
    selectedTransaction,
    loading,
    error,
    loadTransactionDetail
  };
};
