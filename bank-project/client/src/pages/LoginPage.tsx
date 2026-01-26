import { useState, useEffect, useMemo } from 'react';
import { useNavigate, useSearchParams } from 'react-router-dom';
import DarkModeIcon from '@mui/icons-material/DarkMode';
import LightModeIcon from '@mui/icons-material/LightMode';
import {
  Container,
  Card,
  CardContent,
  IconButton,
  Typography,
  Stack,
  Box,
  Divider,
} from '../components/ui';
import { useThemeContext } from '../context/ThemeContext';
import { authService } from '../services/auth';
import { getErrorMessage } from '../types';
import { getTimeBasedGreeting } from '../utils/greetings';
import { getIntelligentErrorMessage } from '../utils/messages';
import { BrandHeader } from '../components/BrandHeader';
import { SecurityIndicator } from '../components/SecurityIndicator';
import { LoginForm } from '../components/LoginForm';
import { PageFooterCaption } from '../components/AuthPageFooter';
import {
  createPageBackgroundSx,
  createAuthCardSx,
  themeToggleSx,
} from '../styles/login.styles';

const LoginPage = () => {
  const navigate = useNavigate();
  const [searchParams] = useSearchParams();
  const { isDark, toggleTheme } = useThemeContext();

  // Form state
  const [formData, setFormData] = useState({ email: '', password: '' });

  // UI state
  const [error, setError] = useState('');
  const [loading, setLoading] = useState(false);
  const [showVerifiedMsg, setShowVerifiedMsg] = useState(false);

  // Memoized greeting - calculated once per page load
  const greeting = useMemo(() => getTimeBasedGreeting(), []);

  // Handle verified query param
  useEffect(() => {
    if (searchParams.get('verified') === 'true') {
      setShowVerifiedMsg(true);
      navigate('/login', { replace: true });
    }
  }, [searchParams, navigate]);

  const handleFieldChange = (field: 'email' | 'password', value: string) => {
    setFormData(prev => ({ ...prev, [field]: value }));
  };

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setError('');
    setLoading(true);

    try {
      const data = await authService.login(formData.email, formData.password);
      authService.setToken(data.token);
      localStorage.setItem('user', JSON.stringify(data.user));
      navigate('/dashboard');
    } catch (err: unknown) {
      const originalError = getErrorMessage(err);
      setError(getIntelligentErrorMessage(originalError));
      setFormData(prev => ({ ...prev, password: '' }));
    } finally {
      setLoading(false);
    }
  };

  return (
    <Box sx={createPageBackgroundSx(isDark)}>
      {/* Theme Toggle */}
      <IconButton
        onClick={toggleTheme}
        aria-label={isDark ? 'Switch to light mode' : 'Switch to dark mode'}
        sx={themeToggleSx}
      >
        {isDark ? <LightModeIcon /> : <DarkModeIcon />}
      </IconButton>

      <Container maxWidth="xs" sx={{ position: 'relative', zIndex: 1 }}>
        <Card sx={createAuthCardSx(isDark)}>
          <CardContent sx={{ p: { xs: 3, sm: 4 } }}>
            <Stack spacing={3}>
              <BrandHeader isDark={isDark} greeting={greeting} />
              <SecurityIndicator isDark={isDark} />

              <Divider sx={{ borderColor: isDark ? 'rgba(255,255,255,0.10)' : 'rgba(13,27,42,0.10)' }} />

              <Box textAlign="center">
                <Typography
                  variant="h5"
                  sx={{ fontWeight: 700, color: isDark ? 'rgba(255,255,255,0.92)' : '#0D1B2A' }}
                >
                  Sign in
                </Typography>
              </Box>

              <LoginForm
                email={formData.email}
                password={formData.password}
                error={error}
                loading={loading}
                showVerifiedMsg={showVerifiedMsg}
                onFieldChange={handleFieldChange}
                onSubmit={handleSubmit}
              />
            </Stack>
          </CardContent>
        </Card>

        <PageFooterCaption isDark={isDark} variant="login" />
      </Container>
    </Box>
  );
};

export default LoginPage;
