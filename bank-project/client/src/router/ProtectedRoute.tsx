import { Navigate, Outlet } from 'react-router-dom';
import { authStorage } from '../services/auth.storage';

const ProtectedRoute = () => {
  const isLoggedIn = authStorage.isAuthenticated();

  if (!isLoggedIn) {
    return <Navigate to="/login" replace />;
  }

  return <Outlet />;
};

export default ProtectedRoute;