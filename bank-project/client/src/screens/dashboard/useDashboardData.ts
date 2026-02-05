// Fetches current user and recent transactions for the dashboard.
import { useState, useEffect } from 'react';
import { userService } from '../../api/user.service';
import { transactionService } from '../../api/transaction.service';
import { type User, type Transaction } from '../../types';
import { getErrorMessage } from '../../types';

export const useDashboardData = () => {
  const [user, setUser] = useState<User | null>(null);
  const [transactions, setTransactions] = useState<Transaction[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState('');

  useEffect(() => {
    let cancelled = false;

    const loadData = async () => {
      setError('');
      setLoading(true);

      try {
        const [userData, transactionsData] = await Promise.all([
          userService.getMe(),
          transactionService.getAll(),
        ]);
        if (!cancelled) {
          setUser(userData.user);
          setTransactions(transactionsData.transactions ?? []);
        }
      } catch (err) {
        if (!cancelled) {
          setError(getErrorMessage(err));
        }
      } finally {
        if (!cancelled) setLoading(false);
      }
    };

    loadData();
    return () => {
      cancelled = true;
    };
  }, []);

  return {
    user,
    transactions,
    loading,
    error,
  };
};
