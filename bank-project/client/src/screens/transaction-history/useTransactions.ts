// Paginated transaction list from URL search params and auth.
import { useEffect, useState, useRef } from 'react';
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

  const loadingRef = useRef(false);

  const handlePageChange = (page: number) => {
    const next = new URLSearchParams(searchParams);
    next.set('page', String(page));
    setSearchParams(next);
  };

  // Normalize URL: ensure ?page= is set so currentPage is stable and effect doesn't re-run unnecessarily.
  useEffect(() => {
    if (!searchParams.has('page')) {
      setSearchParams((prev) => {
        const next = new URLSearchParams(prev);
        next.set('page', '1');
        return next;
      });
    }
  }, [searchParams, setSearchParams]);

  useEffect(() => {
    let cancelled = false;

    const loadTransactions = async () => {
      setError('');
      if (!loadingRef.current) {
        loadingRef.current = true;
        setLoading(true);
      }

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
        loadingRef.current = false;
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
