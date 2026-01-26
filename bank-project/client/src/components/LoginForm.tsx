import { Link } from 'react-router-dom';
import {
  TextField,
  Button,
  Typography,
  Alert,
  Stack,
  Box,
} from './ui';
import { useThemeContext } from '../context/ThemeContext';
import { createFieldSx, primaryButtonSx } from '../styles/login.styles';

interface LoginFormProps {
  email: string;
  password: string;
  error: string;
  loading: boolean;
  showVerifiedMsg: boolean;
  onFieldChange: (field: 'email' | 'password', value: string) => void;
  onSubmit: (e: React.FormEvent) => void;
}

export const LoginForm = ({
  email,
  password,
  error,
  loading,
  showVerifiedMsg,
  onFieldChange,
  onSubmit,
}: LoginFormProps) => {
  const { isDark } = useThemeContext();
  const fieldSx = createFieldSx(isDark);
  const buttonText = loading ? 'Signing in…' : 'Sign In';

  return (
    <>
      {showVerifiedMsg && (
        <Alert severity="success">Email verified successfully! You can now sign in.</Alert>
      )}

      {error && (
        <Alert
          severity="error"
          sx={{ '& .MuiAlert-message': { width: '100%' } }}
        >
          <Typography variant="body2" sx={{ fontWeight: 500 }}>
            {error}
          </Typography>
          <Typography variant="caption" sx={{ display: 'block', mt: 0.5, opacity: 0.85 }}>
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

          <Typography
            variant="caption"
            sx={{
              textAlign: 'center',
              color: isDark ? 'rgba(255,255,255,0.50)' : 'rgba(13,27,42,0.50)',
              lineHeight: 1.5,
              px: 1,
            }}
          >
            Behavioral patterns and device signals are analyzed to protect your account
          </Typography>

          <Box sx={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', pt: 0.5 }}>
            <Typography
              variant="body2"
              sx={{
                color: isDark ? 'rgba(255,255,255,0.70)' : 'rgba(13,27,42,0.70)',
                cursor: 'pointer',
                '&:hover': { color: '#C9A227' },
              }}
            >
              Forgot password? (TODO)
            </Typography>
            <Typography variant="body2">
              <Link to="/signup" style={{ fontWeight: 700, color: '#C9A227', textDecoration: 'none' }}>
                Create account
              </Link>
            </Typography>
          </Box>
        </Stack>
      </form>
    </>
  );
};
