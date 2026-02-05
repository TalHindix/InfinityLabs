import type { ReactNode } from 'react';
import { BrowserRouter, Routes, Route, Navigate } from 'react-router-dom';

import { ROUTES } from './constants/routes';
import ChatWidget from './components/ChatWidget';
import LoginPage from './pages/LoginPage';
import SignupPage from './pages/SignupPage';
import DashboardPage from './pages/DashboardPage';
import TransferPage from './pages/TransferPage';
import TransactionsPage from './pages/TransactionsPage';
import { authStorage } from './services/auth.storage';

/** If not logged in, redirects to login; otherwise renders ChatWidget and the page (children). */
function RequireAuth({ children }: { children: ReactNode }) {
  if (!authStorage.isAuthenticated()) {
    return <Navigate to={ROUTES.LOGIN} replace />;
  }
  return (
    <>
      <ChatWidget />
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
