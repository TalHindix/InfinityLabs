// Login form state, submit, and resend-verification flow.
import { useState, useEffect } from 'react';
import { useNavigate, useSearchParams } from 'react-router-dom';
import { ROUTES } from '../constants/routes';
import { authStorage } from '../services/auth.storage';
import { authService } from '../services/auth.service';
import { getIntelligentErrorMessage } from '../utils/messages';
import { getTimeBasedGreeting } from '../utils/greetings';
import { useAsyncOperation } from './useAsyncOperation';

export const useLogin = () => {
  const navigate = useNavigate();
  const [searchParams] = useSearchParams();

  const [email, setEmail] = useState('');
  const [password, setPassword] = useState('');
  const [showVerifiedMsg, setShowVerifiedMsg] = useState(false);
  const [showResendOption, setShowResendOption] = useState(false);
  const [resendSuccess, setResendSuccess] = useState(false);

  const { loading, error, execute, setError } = useAsyncOperation();
  const resendAsync = useAsyncOperation();

  const greeting = getTimeBasedGreeting();

  useEffect(() => {
    if (searchParams.get('verified') === 'true') {
      setShowVerifiedMsg(true);
      navigate(ROUTES.LOGIN, { replace: true });
    }
  }, [searchParams, navigate]);

  const handleFieldChange = (field: 'email' | 'password', value: string) => {
    if (field === 'email') {
      setEmail(value);
      setShowResendOption(false);
      setResendSuccess(false);
    } else {
      setPassword(value);
    }
  };

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setShowResendOption(false);
    setResendSuccess(false);

    const { result, error: loginError } = await execute(
      () => authService.login(email, password),
      (data) => {
        authStorage.setUser(data.user);
        navigate(ROUTES.DASHBOARD);
      }
    );

    if (!result && loginError) {
      const intelligentError = getIntelligentErrorMessage(loginError);
      setError(intelligentError);
      setPassword('');
      setShowResendOption(true);
    }
  };

  const handleResendVerification = async () => {
    if (!email) {
      setError('Please enter your email address first.');
      return;
    }

    setShowResendOption(false);
    setResendSuccess(false);

    const { result, error: resendError } = await resendAsync.execute(
      () => authService.resendVerification(email)
    );

    if (result && !resendError) {
      setResendSuccess(true);
    } else if (resendError) {
      setError(getIntelligentErrorMessage(resendError));
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
    resendLoading: resendAsync.loading,
    resendSuccess,
    handleFieldChange,
    handleSubmit,
    handleResendVerification,
  };
};