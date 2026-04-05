import { useState, useEffect, useCallback } from 'react';
import { transactionService } from '../../api/transaction.service';
import { type MonthlySpending, type TopRecipient, getErrorMessage } from '../../types';
import { DASHBOARD_REFRESH_EVENT } from './useDashboardData';

export const useSpendingAnalytics = () => {
  const [monthlySpending, setMonthlySpending] = useState<MonthlySpending[]>([]);
  const [topRecipients, setTopRecipients] = useState<TopRecipient[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState('');

  const loadAnalytics = useCallback(async (showLoading = true) => {
    setError('');
    if (showLoading) setLoading(true);

    try {
      const data = await transactionService.getSpendingAnalytics();
      setMonthlySpending(data.monthlySpending);
      setTopRecipients(data.topRecipients);
    } catch (err) {
      setError(getErrorMessage(err));
    } finally {
      setLoading(false);
    }
  }, []);

  useEffect(() => {
    loadAnalytics();
  }, [loadAnalytics]);

  useEffect(() => {
    const handleRefresh = () => loadAnalytics(false);
    window.addEventListener(DASHBOARD_REFRESH_EVENT, handleRefresh);
    return () => window.removeEventListener(DASHBOARD_REFRESH_EVENT, handleRefresh);
  }, [loadAnalytics]);

  return { monthlySpending, topRecipients, loading, error };
};
