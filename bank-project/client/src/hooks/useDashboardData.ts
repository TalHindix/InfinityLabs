// Fetches current user and recent transactions for the dashboard.
import { useState, useEffect } from 'react';
import { userService } from '../services/user.service';
import { transactionsService } from '../services/transactions.service';
import { type User, type Transaction } from '../types';
import { useAsyncOperation } from './useAsyncOperation';

export const useDashboardData = () => {
  const [user, setUser] = useState<User | null>(null);
  const [transactions, setTransactions] = useState<Transaction[]>([]);
  const { loading, error, execute } = useAsyncOperation(true);

  const loadData = async () => {
    await execute(
      async () => {
        const [userData, transactionsData] = await Promise.all([
          userService.getMe(),
          transactionsService.getAll(),
        ]);
        return { userData, transactionsData };
      },
      ({ userData, transactionsData }) => {
        setUser(userData.user);
        setTransactions(transactionsData.transactions || []);
      }
    );
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
