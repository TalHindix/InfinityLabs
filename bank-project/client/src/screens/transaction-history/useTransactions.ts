// Paginated transaction list from URL search params and auth.
import { useEffect, useState } from 'react';
import { useSearchParams } from 'react-router-dom';
import { transactionService } from '../../api/transaction.service';
import { authStorage } from '../../api/auth.storage';
import { type Transaction } from '../../types';
import { useAsyncOperation } from '../../shared/useAsyncOperation';

export const useTransactions = (pageSize = 10) => {
  const [searchParams, setSearchParams] = useSearchParams();

  const [transactions, setTransactions] = useState<Transaction[]>([]);
  const [totalPages, setTotalPages] = useState(1);
  const { loading, error, execute } = useAsyncOperation();

  const currentPage = Number(searchParams.get('page')) || 1;
  const userEmail = authStorage.getUser()?.email;

  const handlePageChange = (page: number) => {
    const next = new URLSearchParams(searchParams);
    next.set('page', String(page));
    setSearchParams(next);
  };

  useEffect(() => {
    const loadTransactions = async () => {
      const { result } = await execute(
        () => transactionService.getAll(currentPage, pageSize),
        (data) => {
          setTransactions(data.transactions ?? []);
          setTotalPages(data.totalPages ?? 1);
        }
      );

      if (!result) {
        setTransactions([]);
        setTotalPages(1);
      }
    };

    loadTransactions();
  }, [currentPage, pageSize, execute]);

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
