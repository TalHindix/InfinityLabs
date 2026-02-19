// Fetches current user and recent transactions for the dashboard.
import { useState, useEffect, useCallback } from 'react';
import { userService } from '../../api/user.service';
import { transactionService } from '../../api/transaction.service';
import { type User, type Transaction } from '../../types';
import { getErrorMessage } from '../../types';

const DASHBOARD_REFRESH_EVENT = 'dashboard:refresh';

export const useDashboardData = () => {
  const [user, setUser] = useState<User | null>(null);
  const [transactions, setTransactions] = useState<Transaction[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState('');

  const loadData = useCallback(async (showLoading = true) => {
    setError('');
    if (showLoading) setLoading(true);

    try {
      const [userData, transactionsData] = await Promise.all([
        userService.getMe(),
        transactionService.getAll(),
      ]);
      setUser(userData.user);
      setTransactions(transactionsData.transactions ?? []);
    } catch (err) {
      setError(getErrorMessage(err));
    } finally {
      setLoading(false);
    }
  }, []);

  useEffect(() => {
    loadData();
  }, [loadData]);

  useEffect(() => {
    const handleRefresh = () => loadData(false);
    window.addEventListener(DASHBOARD_REFRESH_EVENT, handleRefresh);
    return () => window.removeEventListener(DASHBOARD_REFRESH_EVENT, handleRefresh);
  }, [loadData]);

  return {
    user,
    transactions,
    loading,
    error,
  };
};
