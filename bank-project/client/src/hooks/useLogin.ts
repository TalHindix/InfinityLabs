import { useState, useEffect, useMemo } from 'react';
import { useNavigate, useSearchParams } from 'react-router-dom';
import { authStorage } from '../services/auth.storage';
import { authService } from '../services/auth.service';
import { getErrorMessage } from '../types';
import { getTimeBasedGreeting } from '../utils/greetings';
import { getIntelligentErrorMessage } from '../utils/messages';

export const useLogin = () => {
  const navigate = useNavigate();
  const [searchParams] = useSearchParams();

  const [email, setEmail] = useState('');
  const [password, setPassword] = useState('');

  const [error, setError] = useState('');
  const [loading, setLoading] = useState(false);
  const [showVerifiedMsg, setShowVerifiedMsg] = useState(false);

  const greeting = useMemo(() => getTimeBasedGreeting(), []);

  useEffect(() => {
    if (searchParams.get('verified') === 'true') {
      setShowVerifiedMsg(true);
      navigate('/login', { replace: true });
    }
  }, [searchParams, navigate]);

  const handleFieldChange = (field: 'email' | 'password', value: string) => {
    if (field === 'email') {
      setEmail(value);
    } else {
      setPassword(value);
    }
  };

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setError('');
    setLoading(true);

    try {
      const data = await authService.login(email, password);
      authStorage.setToken(data.token);
      localStorage.setItem('user', JSON.stringify(data.user));
      navigate('/dashboard');
    } catch (err: unknown) {
      const originalError = getErrorMessage(err);
      setError(getIntelligentErrorMessage(originalError));
      setPassword('');
    } finally {
      setLoading(false);
    }
  };

  return {
    email,
    password,
    loading,
    error,
    showVerifiedMsg,
    greeting,
    handleFieldChange,
    handleSubmit
  };
};
