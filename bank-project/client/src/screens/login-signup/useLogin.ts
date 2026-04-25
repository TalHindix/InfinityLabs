// Login form state, submit, resend-verification, and OTP verification flow.
import { useState, useEffect } from 'react';
import { useNavigate, useSearchParams } from 'react-router-dom';
import { ROUTES } from '../../constants/routePaths';
import { authStorage } from '../../api/auth.storage';
import { authService } from '../../api/auth.service';
import { getIntelligentErrorMessage } from '../../shared/authErrorMessages';
import { getErrorMessage } from '../../types';
import { getTimeBasedGreeting } from '../../shared/timeBasedGreeting';
import { useAsyncOperation } from '../../shared/useAsyncOperation';

const RESEND_OTP_COOLDOWN_SECONDS = 60;

export const useLogin = () => {
  const navigate = useNavigate();
  const [searchParams] = useSearchParams();

  const [email, setEmail] = useState('');
  const [password, setPassword] = useState('');
  const [showVerifiedMsg, setShowVerifiedMsg] = useState(false);
  const [showResendOption, setShowResendOption] = useState(false);
  const [resendSuccess, setResendSuccess] = useState(false);

  const [otpRequired, setOtpRequired] = useState(false);
  const [otp, setOtp] = useState('');
  const [resendCooldown, setResendCooldown] = useState(0);

  const { loading, error, execute, setError } = useAsyncOperation();
  const resendAsync = useAsyncOperation();
  const otpAsync = useAsyncOperation();

  const greeting = getTimeBasedGreeting();

  useEffect(() => {
    if (searchParams.get('verified') === 'true') {
      // eslint-disable-next-line react-hooks/set-state-in-effect -- sync from URL param on mount
      setShowVerifiedMsg(true);
      navigate(ROUTES.LOGIN, { replace: true });
    }
  }, [searchParams, navigate]);

  useEffect(() => {
    if (resendCooldown <= 0) return;
    const timer = setTimeout(() => setResendCooldown((c) => c - 1), 1000);
    return () => clearTimeout(timer);
  }, [resendCooldown]);

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
        if (data.otpRequired) {
          setOtpRequired(true);
        } else {
          if (data.token) authStorage.setToken(data.token);
          authStorage.setUser(data.user!);
          navigate(ROUTES.DASHBOARD);
        }
      }
    );

    if (!result && loginError) {
      const intelligentError = getIntelligentErrorMessage(loginError);
      setError(intelligentError);
      setPassword('');
      setShowResendOption(true);
    }
  };

  const handleVerifyOtp = async (e: React.FormEvent) => {
    e.preventDefault();

    await otpAsync.execute(
      () => authService.verifyOtp(email, otp),
      (data) => {
        if (data.token) authStorage.setToken(data.token);
        authStorage.setUser(data.user!);
        navigate(ROUTES.DASHBOARD);
      }
    );
  };

  const handleResendOtp = async () => {
    setResendCooldown(RESEND_OTP_COOLDOWN_SECONDS);
    try {
      await authService.resendOtp(email);
      setOtp('');
    } catch (err) {
      otpAsync.setError(getIntelligentErrorMessage(getErrorMessage(err)));
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
    otpRequired,
    otp,
    otpLoading: otpAsync.loading,
    otpError: otpAsync.error,
    resendCooldown,
    handleFieldChange,
    handleSubmit,
    handleResendVerification,
    handleVerifyOtp,
    handleResendOtp,
    setOtp,
  };
};
