// Loads and holds a single transaction by id for the detail panel.
import { useState } from 'react';
import { transactionService } from '../../api/transaction.service';
import { type Transaction } from '../../types';
import { useAsyncOperation } from '../../shared/useAsyncOperation';

export const useTransactionDetail = () => {
  const [selectedTransaction, setSelectedTransaction] = useState<Transaction | null>(null);
  const { loading, error, execute } = useAsyncOperation();

  const loadTransactionDetail = async (id: string) => {
    const { result } = await execute(
      () => transactionService.getById(id),
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
