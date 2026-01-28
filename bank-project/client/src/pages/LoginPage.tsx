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
} from '../utils/ui';
import { useThemeContext } from '../context/ThemeContext';
import { useLogin } from '../hooks/useLogin';
import { BrandHeader } from '../components/BrandHeader';
import { SecurityIndicator } from '../components/SecurityIndicator';
import { LoginForm } from '../components/LoginForm';
import { PageFooterCaption } from '../components/AuthPageFooter';
import { createDividerSx, createSignInTitleSx , createPageBackgroundSx , createAuthCardSx , themeToggleSx } from './LoginPage.styles';

const LoginPage = () => {
  const { isDark, toggleTheme } = useThemeContext();
  const {
    email,
    password,
    loading,
    error,
    showVerifiedMsg,
    greeting,
    handleFieldChange,
    handleSubmit,
  } = useLogin();

  return (
    <Box sx={createPageBackgroundSx(isDark)}>
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

              <Divider sx={createDividerSx(isDark)} />

              <Box textAlign="center">
                <Typography variant="h5" sx={createSignInTitleSx(isDark)}>
                  Sign in
                </Typography>
              </Box>

              <LoginForm
                email={email}
                password={password}
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
