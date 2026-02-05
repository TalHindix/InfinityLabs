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
} from '../../shared/ui';
import { useThemeContext } from '../../context/ThemeContext';
import { useLogin } from './useLogin';
import { DubaiBankLogo } from '../../components/DubaiBankLogo';
import { LoginForm } from './LoginForm';
import { PageFooterCaption } from '../../layout/PageFooterCaption';
import {
  createDividerSx,
  createSignInTitleSx,
  createPageBackgroundSx,
  createAuthCardSx,
  themeToggleSx,
} from './LoginPage.styles';
import { logoContainerSx, createTitleSx, createSubtitleSx } from './BrandHeader.styles';
import { createContainerSx, createDotSx, createTextSx } from './SecurityIndicator.styles';

const LoginPage = () => {
  const { isDark, toggleTheme } = useThemeContext();
  const {
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
              <Box textAlign="center">
                <Box sx={logoContainerSx}>
                  <DubaiBankLogo size={80} animated={true} />
                </Box>

                <Typography variant="h4" sx={createTitleSx(isDark)}>
                  Dubai Bank
                </Typography>

                <Typography variant="body2" sx={createSubtitleSx(isDark)}>
                  {greeting} — secure access to your account
                </Typography>
              </Box>

              <Box sx={createContainerSx(isDark)}>
                <Box sx={createDotSx(false)} />
                <Typography variant="caption" sx={createTextSx(isDark)}>
                  Adaptive risk analysis enabled
                </Typography>
              </Box>

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
                showResendOption={showResendOption}
                resendLoading={resendLoading}
                resendSuccess={resendSuccess}
                onFieldChange={handleFieldChange}
                onSubmit={handleSubmit}
                onResendVerification={handleResendVerification}
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
