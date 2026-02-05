// Paginated transaction list from URL search params and auth.
import { useEffect, useState } from 'react';
import { useSearchParams } from 'react-router-dom';
import { transactionService } from '../../api/transaction.service';
import { authStorage } from '../../api/auth.storage';
import { type Transaction } from '../../types';
import { getErrorMessage } from '../../types';

export const useTransactions = (pageSize = 10) => {
  const [searchParams, setSearchParams] = useSearchParams();

  const [transactions, setTransactions] = useState<Transaction[]>([]);
  const [totalPages, setTotalPages] = useState(1);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState('');

  const currentPage = Number(searchParams.get('page')) || 1;
  const userEmail = authStorage.getUser()?.email;

  const handlePageChange = (page: number) => {
    const next = new URLSearchParams(searchParams);
    next.set('page', String(page));
    setSearchParams(next);
  };

  useEffect(() => {
    let cancelled = false;

    const loadTransactions = async () => {
      setError('');
      setLoading(true);

      try {
        const data = await transactionService.getAll(currentPage, pageSize);
        if (!cancelled) {
          setTransactions(data.transactions ?? []);
          setTotalPages(data.totalPages ?? 1);
        }
      } catch (err) {
        if (!cancelled) {
          setError(getErrorMessage(err));
          setTransactions([]);
          setTotalPages(1);
        }
      } finally {
        if (!cancelled) setLoading(false);
      }
    };

    loadTransactions();
    return () => {
      cancelled = true;
    };
  }, [currentPage, pageSize]);

  return {
    transactions,
    loading,
    error,
    totalPages,
    currentPage,
    userEmail,
    handlePageChange,
  };
};
