import { useState, useEffect, useCallback } from 'react';
import { transactionService } from '../../api/transaction.service';
import {
  type MonthlySpending,
  type MonthlyReceived,
  type TopRecipient,
  getErrorMessage,
} from '../../types';
import { DASHBOARD_REFRESH_EVENT } from './useDashboardData';

export const useSpendingAnalytics = () => {
  const [months, setMonths] = useState(6);
  const [monthlySpending, setMonthlySpending] = useState<MonthlySpending[]>([]);
  const [monthlyReceived, setMonthlyReceived] = useState<MonthlyReceived[]>([]);
  const [topRecipients, setTopRecipients] = useState<TopRecipient[]>([]);
  const [totalSpent, setTotalSpent] = useState(0);
  const [totalReceived, setTotalReceived] = useState(0);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState('');

  const loadAnalytics = useCallback(async (showLoading = true) => {
    setError('');
    if (showLoading) setLoading(true);

    try {
      const data = await transactionService.getSpendingAnalytics(months);
      setMonthlySpending(data.monthlySpending);
      setMonthlyReceived(data.monthlyReceived);
      setTopRecipients(data.topRecipients);
      setTotalSpent(data.totalSpent);
      setTotalReceived(data.totalReceived);
    } catch (err) {
      setError(getErrorMessage(err));
    } finally {
      setLoading(false);
    }
  }, [months]);

  useEffect(() => {
    loadAnalytics();
  }, [loadAnalytics]);

  useEffect(() => {
    const handleRefresh = () => loadAnalytics(false);
    window.addEventListener(DASHBOARD_REFRESH_EVENT, handleRefresh);
    return () => window.removeEventListener(DASHBOARD_REFRESH_EVENT, handleRefresh);
  }, [loadAnalytics]);

  return {
    months,
    setMonths,
    monthlySpending,
    monthlyReceived,
    topRecipients,
    totalSpent,
    totalReceived,
    loading,
    error,
  };
};
