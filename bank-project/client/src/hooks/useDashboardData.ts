import { useState, useEffect } from 'react';
import { userService } from '../services/user';
import { transactionService, type Transaction } from '../services/transaction';
import { type User, getErrorMessage } from '../types';

/**
 * Hook for loading dashboard data (user profile and recent transactions).
 */
export const useDashboardData = () => {
  const [user, setUser] = useState<User | null>(null);
  const [transactions, setTransactions] = useState<Transaction[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState('');

  const loadData = async () => {
    setLoading(true);
    setError('');
    try {
      const [userData, transactionsData] = await Promise.all([
        userService.getMe(),
        transactionService.getAll(),
      ]);
      setUser(userData.user);
      setTransactions(transactionsData.transactions || []);
    } catch (err: unknown) {
      setError(getErrorMessage(err));
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    loadData();
  }, []);

  return {
    user,
    transactions,
    loading,
    error,
    refresh: loadData,
  };
};
