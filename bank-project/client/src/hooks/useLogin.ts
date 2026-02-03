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

  // Resend verification state
  const [showResendOption, setShowResendOption] = useState(false);
  const [resendLoading, setResendLoading] = useState(false);
  const [resendSuccess, setResendSuccess] = useState(false);

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
      // Reset resend state when email changes
      setShowResendOption(false);
      setResendSuccess(false);
    } else {
      setPassword(value);
    }
  };

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setError('');
    setLoading(true);
    setShowResendOption(false);
    setResendSuccess(false);

    try {
      const data = await authService.login(email, password);
      authStorage.setToken(data.token);
      localStorage.setItem('user', JSON.stringify(data.user));
      navigate('/dashboard');
    } catch (err: unknown) {
      const originalError = getErrorMessage(err);
      setError(getIntelligentErrorMessage(originalError));
      setPassword('');
      // Show resend option after failed login (could be unverified email)
      setShowResendOption(true);
    } finally {
      setLoading(false);
    }
  };

  const handleResendVerification = async () => {
    if (!email) {
      setError('Please enter your email address first.');
      return;
    }

    setResendLoading(true);
    setError('');

    try {
      await authService.resendVerification(email);
      setResendSuccess(true);
      setShowResendOption(false);
    } catch (err: unknown) {
      // Security: Show generic success message even on error
      // to prevent email enumeration
      setResendSuccess(true);
      setShowResendOption(false);
    } finally {
      setResendLoading(false);
    }
  };

  return {
    email,
    password,
    loading,
    error,
    showVerifiedMsg,
    greeting,
    showResendOption,
    resendLoading,
    resendSuccess,
    handleFieldChange,
    handleSubmit,
    handleResendVerification,
  };
};