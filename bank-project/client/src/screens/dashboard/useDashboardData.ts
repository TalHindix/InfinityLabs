// Fetches current user and recent transactions for the dashboard.
import { useState, useEffect } from 'react';
import { userService } from '../../api/user.service';
import { transactionService } from '../../api/transaction.service';
import { type User, type Transaction } from '../../types';
import { useAsyncOperation } from '../../shared/useAsyncOperation';

export const useDashboardData = () => {
  const [user, setUser] = useState<User | null>(null);
  const [transactions, setTransactions] = useState<Transaction[]>([]);
  const { loading, error, execute } = useAsyncOperation(true);

  const loadData = async () => {
    await execute(
      async () => {
        const [userData, transactionsData] = await Promise.all([
          userService.getMe(),
          transactionService.getAll(),
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
