// Tracks authentication state by listening to auth storage events.
import { useState, useEffect } from 'react';
import { authStorage, AUTH_CHANGE_EVENT } from '../api/auth.storage';

export const useAuth = () => {
  const [isAuthenticated, setIsAuthenticated] = useState(authStorage.isAuthenticated());

  useEffect(() => {
    const handleAuthChange = () => {
      setIsAuthenticated(authStorage.isAuthenticated());
    };

    handleAuthChange();

    window.addEventListener(AUTH_CHANGE_EVENT, handleAuthChange);
    return () => window.removeEventListener(AUTH_CHANGE_EVENT, handleAuthChange);
  }, []);

  return { isAuthenticated };
};
