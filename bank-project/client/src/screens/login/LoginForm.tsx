import { Link } from 'react-router-dom';
import { ROUTES } from '../../constants/routePaths';
import { TextField, Button, Typography, Alert, Stack, Box } from '../../shared/muiExports';
import { useThemeContext } from '../../context/ThemeContext';
import { OtpForm } from './OtpForm';
import {
  createFieldSx,
  primaryButtonSx,
  createSecurityNoteSx,
  footerContainerSx,
  errorAlertSx,
  errorCaptionSx,
  resendLinkButtonSx,
  signUpLinkStyle,
} from './LoginForm.styles';

interface LoginFormProps {
  email: string;
  password: string;
  error: string;
  loading: boolean;
  showVerifiedMsg: boolean;
  showResendOption: boolean;
  resendLoading: boolean;
  resendSuccess: boolean;
  onFieldChange: (field: 'email' | 'password', value: string) => void;
  onSubmit: (e: React.FormEvent) => void;
  onResendVerification: () => void;
  otpRequired: boolean;
  otp: string;
  otpLoading: boolean;
  otpError: string;
  resendCooldown: number;
  onOtpChange: (value: string) => void;
  onVerifyOtp: (e: React.FormEvent) => void;
  onResendOtp: () => void;
}

export const LoginForm = ({
  email,
  password,
  error,
  loading,
  showVerifiedMsg,
  showResendOption,
  resendLoading,
  resendSuccess,
  onFieldChange,
  onSubmit,
  onResendVerification,
  otpRequired,
  otp,
  otpLoading,
  otpError,
  resendCooldown,
  onOtpChange,
  onVerifyOtp,
  onResendOtp,
}: LoginFormProps) => {
  const { isDark } = useThemeContext();
  const fieldSx = createFieldSx(isDark);

  if (otpRequired) {
    return (
      <OtpForm
        email={email}
        otp={otp}
        otpLoading={otpLoading}
        otpError={otpError}
        resendCooldown={resendCooldown}
        onOtpChange={onOtpChange}
        onVerifyOtp={onVerifyOtp}
        onResendOtp={onResendOtp}
      />
    );
  }

  const buttonText = loading ? 'Signing in…' : 'Sign In';

  return (
    <>
      {showVerifiedMsg && (
        <Alert severity="success">Email verified successfully! You can now sign in.</Alert>
      )}

      {resendSuccess && (
        <Alert severity="info">
          If this email exists and is not verified, a new verification link has been sent.
        </Alert>
      )}

      {error && (
        <Alert severity="error" sx={errorAlertSx}>
          <Typography variant="body2" sx={{ fontWeight: 500 }}>
            {error}
          </Typography>
          <Typography variant="caption" sx={errorCaptionSx}>
            If this issue persists, contact support.
          </Typography>
        </Alert>
      )}

      <form onSubmit={onSubmit}>
        <Stack spacing={2.5}>
          <TextField
            fullWidth
            label="Email Address"
            type="email"
            value={email}
            onChange={(e) => onFieldChange('email', e.target.value)}
            required
            placeholder="you@example.com"
            autoComplete="email"
            disabled={loading}
            sx={fieldSx}
          />
          <TextField
            fullWidth
            label="Password"
            type="password"
            value={password}
            onChange={(e) => onFieldChange('password', e.target.value)}
            required
            autoComplete="current-password"
            disabled={loading}
            sx={fieldSx}
          />

          <Button
            fullWidth
            type="submit"
            variant="contained"
            size="large"
            disabled={loading}
            sx={primaryButtonSx}
          >
            {buttonText}
          </Button>

          <Typography variant="caption" sx={createSecurityNoteSx(isDark)}>
            Behavioral patterns and device signals are analyzed to protect your account
          </Typography>

          <Box sx={footerContainerSx}>
            <Typography variant="body2">
              <Link to={ROUTES.SIGNUP} style={signUpLinkStyle}>
                Create account
              </Link>
            </Typography>
          </Box>

          {showResendOption && !resendSuccess && (
            <Box sx={{ textAlign: 'center', mt: 1 }}>
              <Typography variant="body2" sx={{ color: isDark ? '#aaa' : '#666', mb: 1 }}>
                Didn't receive a verification email?
              </Typography>
              <Button
                variant="text"
                size="small"
                onClick={onResendVerification}
                disabled={resendLoading || !email}
                sx={resendLinkButtonSx(isDark)}
              >
                {resendLoading ? 'Sending...' : 'Resend verification email'}
              </Button>
            </Box>
          )}
        </Stack>
      </form>
    </>
  );
};
