import { useEffect } from 'react';

export const DASHBOARD_REFRESH_EVENT = 'dashboard:refresh';

// Subscribes to a global window event used to signal dashboard data should
// reload (e.g. after the chat assistant completes a transfer).
export const useDashboardRefresh = (handler: () => void) => {
  useEffect(() => {
    window.addEventListener(DASHBOARD_REFRESH_EVENT, handler);
    return () => window.removeEventListener(DASHBOARD_REFRESH_EVENT, handler);
  }, [handler]);
};
