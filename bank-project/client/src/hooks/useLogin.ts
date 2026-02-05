import { useState, useEffect, useMemo } from 'react';
import { useNavigate, useSearchParams } from 'react-router-dom';
import { authStorage } from '../services/auth.storage';
import { authService } from '../services/auth.service';
import { getIntelligentErrorMessage } from '../utils/messages';
import { getTimeBasedGreeting } from '../utils/greetings';
import { useAsyncOperation } from './useAsyncOperation';

export const useLogin = () => {

  // for navigate between pages after login.
  const navigate = useNavigate();

  // for reading parameters ?verified=true
  const [searchParams] = useSearchParams();

  const [email, setEmail] = useState('');
  const [password, setPassword] = useState('');

  const { loading, error, execute, setError } = useAsyncOperation();
  
  const resendAsync = useAsyncOperation();
  
  const [showVerifiedMsg, setShowVerifiedMsg] = useState(false);
  const [showResendOption, setShowResendOption] = useState(false);
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
        navigate('/dashboard');
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

    setResendSuccess(false);
    
    const { result, error: resendError } = await resendAsync.execute(
      () => authService.resendVerification(email)
    );

    if (result && !resendError) {
      setResendSuccess(true);
      setShowResendOption(false);
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