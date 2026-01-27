import { useEffect, useState } from 'react';
import { useSearchParams } from 'react-router-dom';
import { transactionService, type Transaction } from '../services/transaction';
import { authService } from '../services/auth';
import { getErrorMessage } from '../types';

export const useTransactions = (pageSize = 10) => {
  const [searchParams, setSearchParams] = useSearchParams();

  const [transactions, setTransactions] = useState<Transaction[]>([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');
  const [totalPages, setTotalPages] = useState(1);

  const currentPage = Number(searchParams.get('page')) || 1;
  const userEmail = authService.getUser()?.email;

  const handlePageChange = (page: number) => {
    const next = new URLSearchParams(searchParams);
    next.set('page', String(page));
    setSearchParams(next);
  };

  useEffect(() => {
  const loadTransactions = async () => {
    setLoading(true);
    setError('');

    try {
      const data = await transactionService.getAll(currentPage, pageSize);
      setTransactions(data.transactions ?? []);
      setTotalPages(data.totalPages ?? 1);
    } catch (err: unknown) {
      setError(getErrorMessage(err));
      setTransactions([]);
      setTotalPages(1);
    } finally {
      setLoading(false);
    }
  };

  loadTransactions();
  }, [currentPage, pageSize]);

  return {
    transactions,
    loading,
    error,
    totalPages,
    currentPage,
    userEmail,
    handlePageChange
  };
};
