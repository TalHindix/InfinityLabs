import type { ReactNode } from 'react';
import { BrowserRouter, Routes, Route, Navigate } from 'react-router-dom';

import { ROUTES } from './constants/routePaths';
import ChatAssistant from './components/ChatAssistant';
import LoginPage from './screens/login-signup/LoginPage';
import SignupPage from './screens/login-signup/SignupPage';
import DashboardPage from './screens/dashboard/DashboardPage';
import TransferPage from './screens/transfer-money/TransferPage';
import TransactionsPage from './screens/transaction-history/TransactionsPage';
import VideoCallPage from './screens/video-call/VideoCallPage';
import { authStorage } from './api/auth.storage';

function RequireAuth({ children }: { children: ReactNode }) {
  if (!authStorage.isAuthenticated()) {
    return <Navigate to={ROUTES.LOGIN} replace />;
  }
  return <>{children}</>;
}

export default function App() {
  return (
    <BrowserRouter>
      <ChatAssistant />
      <Routes>
        {/* Public routes */}
        <Route index element={<Navigate to={ROUTES.LOGIN} replace />} />
        <Route path={ROUTES.LOGIN} element={<LoginPage />} />
        <Route path={ROUTES.SIGNUP} element={<SignupPage />} />

        {/* Protected routes: require login; each route wrapped explicitly */}
        <Route path={ROUTES.DASHBOARD} element={<RequireAuth><DashboardPage /></RequireAuth>} />
        <Route path={ROUTES.TRANSFER} element={<RequireAuth><TransferPage /></RequireAuth>} />
        <Route path={ROUTES.TRANSACTIONS} element={<RequireAuth><TransactionsPage /></RequireAuth>} />
        <Route path={`${ROUTES.VIDEO_CALL}/:roomName`} element={<RequireAuth><VideoCallPage /></RequireAuth>} />

        {/* Fallback */}
        <Route path="*" element={<Navigate to={ROUTES.LOGIN} replace />} />
      </Routes>
    </BrowserRouter>
  );
}
