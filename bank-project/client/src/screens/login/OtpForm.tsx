import { TextField, Button, Typography, Alert, Stack, Box } from '../../shared/muiExports';
import { useThemeContext } from '../../context/ThemeContext';
import {
  createFieldSx,
  primaryButtonSx,
  errorAlertSx,
  resendLinkButtonSx,
} from './LoginForm.styles';

interface OtpFormProps {
  email: string;
  otp: string;
  otpLoading: boolean;
  otpError: string;
  resendCooldown: number;
  onOtpChange: (value: string) => void;
  onVerifyOtp: (e: React.FormEvent) => void;
  onResendOtp: () => void;
}

export const OtpForm = ({
  email,
  otp,
  otpLoading,
  otpError,
  resendCooldown,
  onOtpChange,
  onVerifyOtp,
  onResendOtp,
}: OtpFormProps) => {
  const { isDark } = useThemeContext();
  const fieldSx = createFieldSx(isDark);

  return (
    <>
      {otpError && (
        <Alert severity="error" sx={errorAlertSx}>
          <Typography variant="body2" sx={{ fontWeight: 500 }}>{otpError}</Typography>
        </Alert>
      )}

      <form onSubmit={onVerifyOtp}>
        <Stack spacing={2.5}>
          <Typography variant="body2" color="text.secondary" textAlign="center">
            A 6-digit code was sent to <strong>{email}</strong>
          </Typography>

          <TextField
            fullWidth
            label="Verification Code"
            value={otp}
            onChange={(e) => onOtpChange(e.target.value)}
            inputProps={{ maxLength: 6 }}
            placeholder="000000"
            autoComplete="one-time-code"
            disabled={otpLoading}
            sx={fieldSx}
          />

          <Button
            fullWidth
            type="submit"
            variant="contained"
            size="large"
            disabled={otpLoading || otp.length !== 6}
            sx={primaryButtonSx}
          >
            {otpLoading ? 'Verifying…' : 'Verify Code'}
          </Button>

          <Box sx={{ textAlign: 'center' }}>
            <Button
              variant="text"
              size="small"
              onClick={onResendOtp}
              disabled={resendCooldown > 0}
              sx={resendLinkButtonSx(isDark)}
            >
              {resendCooldown > 0 ? `Resend code in ${resendCooldown}s` : 'Resend code'}
            </Button>
          </Box>
        </Stack>
      </form>
    </>
  );
};
