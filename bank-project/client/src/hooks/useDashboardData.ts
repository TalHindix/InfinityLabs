import { useState, useEffect } from 'react';
import { userService } from '../services/user.service';
import { transactionsService } from '../services/transactions.service';
import { type User, type Transaction, getErrorMessage } from '../types';

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
        transactionsService.getAll(),
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
    error
  };
};
