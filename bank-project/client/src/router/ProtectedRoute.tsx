import { Navigate, Outlet } from 'react-router-dom';
import { authStorage } from '../services/auth.storage';
import ChatWidget from '../components/Chatwidget';

const ProtectedRoute = () => {
  const isLoggedIn = authStorage.isAuthenticated();

  if (!isLoggedIn) {
    return <Navigate to="/login" replace />;
  }

  return (
    <>
    <ChatWidget/>
    <Outlet />
    </>
  );
};

export default ProtectedRoute;