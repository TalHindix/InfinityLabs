import type { ReactNode } from 'react';
import { BrowserRouter, Routes, Route, Navigate } from 'react-router-dom';

import { ROUTES } from './constants/routes';
import ChatAssistant from './components/ChatAssistant';
import LoginPage from './screens/login-signup/LoginPage';
import SignupPage from './screens/login-signup/SignupPage';
import DashboardPage from './screens/dashboard/DashboardPage';
import TransferPage from './screens/transfer-money/TransferPage';
import TransactionsPage from './screens/transaction-history/TransactionsPage';
import { authStorage } from './api/auth.storage';

/** If not logged in, redirects to login; otherwise renders ChatAssistant and the page (children). */
function RequireAuth({ children }: { children: ReactNode }) {
  if (!authStorage.isAuthenticated()) {
    return <Navigate to={ROUTES.LOGIN} replace />;
  }
  return (
    <>
      <ChatAssistant />
      {children}
    </>
  );
}

export default function App() {
  return (
    <BrowserRouter>
      <Routes>
        {/* Public routes */}
        <Route index element={<Navigate to={ROUTES.LOGIN} replace />} />
        <Route path={ROUTES.LOGIN} element={<LoginPage />} />
        <Route path={ROUTES.SIGNUP} element={<SignupPage />} />

        {/* Protected routes: require login; each route wrapped explicitly */}
        <Route path={ROUTES.DASHBOARD} element={<RequireAuth><DashboardPage /></RequireAuth>} />
        <Route path={ROUTES.TRANSFER} element={<RequireAuth><TransferPage /></RequireAuth>} />
        <Route path={ROUTES.TRANSACTIONS} element={<RequireAuth><TransactionsPage /></RequireAuth>} />

        {/* Fallback */}
        <Route path="*" element={<Navigate to={ROUTES.LOGIN} replace />} />
      </Routes>
    </BrowserRouter>
  );
}
