import { useState, useEffect, useCallback } from 'react';
import { useSearchParams } from 'react-router-dom';
import { transactionService, type Transaction } from '../services/transaction';
import { authService } from '../services/auth';
import { getErrorMessage } from '../types';

export const useTransactions = (pageSize = 10) => {
  const [searchParams, setSearchParams] = useSearchParams();

  const [transactions, setTransactions] = useState<Transaction[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState('');
  const [totalPages, setTotalPages] = useState(1);

  const currentPage = Number(searchParams.get('page')) || 1;
  const userEmail = authService.getUser()?.email;

  // Single function for loading - used by useEffect and as refresh
  const loadTransactions = useCallback(async () => {
    setLoading(true);
    setError('');
    try {
      const data = await transactionService.getAll(currentPage, pageSize);
      setTransactions(data.transactions || []);
      setTotalPages(data.totalPages || 1);
    } catch (err: unknown) {
      setError(getErrorMessage(err));
    } finally {
      setLoading(false);
    }
  }, [currentPage, pageSize]);

  useEffect(() => {
    loadTransactions();
  }, [loadTransactions]);

  const handlePageChange = (page: number) => {
    setSearchParams({ page: String(page) });
  };

  return {
    transactions,
    loading,
    error,
    totalPages,
    currentPage,
    userEmail,
    handlePageChange,
    refresh: loadTransactions,
  };
};
