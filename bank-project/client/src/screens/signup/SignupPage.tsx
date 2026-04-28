import DarkModeIcon from '@mui/icons-material/DarkMode';
import LightModeIcon from '@mui/icons-material/LightMode';
import {
  Container,
  Card,
  CardContent,
  IconButton,
  Typography,
  Alert,
  Stack,
  Box,
  Divider,
  Stepper,
  Step,
  StepLabel,
  Button,
} from '../../shared/muiExports';
import { Link } from 'react-router-dom';
import { ROUTES } from '../../constants/routePaths';
import { useThemeContext } from '../../context/ThemeContext';
import { useSignup } from './useSignup';
import { SignupForm } from './SignupForm';
import { PageFooterCaption } from '../../layout/PageFooterCaption';
import { DubaiBankLogo } from '../../components/DubaiBankLogo';
import {
  logoContainerSx,
  createTitleSx,
  createSubtitleSx,
} from '../../shared/brandHeader.styles';
import { createPageBackgroundSx, themeToggleSx } from '../login/LoginPage.styles';
import { createSignupCardSx, goldDividerSx } from './SignupPage.styles';
import {
  rootSx as verificationRootSx,
  verificationIconSx,
  alertSx as verificationAlertSx,
  descriptionSx as verificationDescriptionSx,
  primaryButtonSx as verificationPrimaryButtonSx,
} from './VerificationSuccess.styles';

const STEPS = ['Account Details', 'Verify Email'];

const SignupPage = () => {
  const { isDark, toggleTheme } = useThemeContext();

  const {
    formData,
    activeStep,
    loading,
    error,
    handleFieldChange,
    handleSubmit,
    goBack,
  } = useSignup();

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
        <Card sx={createSignupCardSx(isDark)}>
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
                  Open your premium account today
                </Typography>
              </Box>

              <Divider sx={goldDividerSx} />

              <Stepper activeStep={activeStep} alternativeLabel>
                {STEPS.map((label) => (
                  <Step key={label}>
                    <StepLabel>{label}</StepLabel>
                  </Step>
                ))}
              </Stepper>

              {error && <Alert severity="error">{error}</Alert>}

              {activeStep === 0 ? (
                <SignupForm
                  formData={formData}
                  loading={loading}
                  onFieldChange={handleFieldChange}
                  onSubmit={handleSubmit}
                />
              ) : (
                <Stack spacing={3} sx={verificationRootSx}>
                  <Box sx={verificationIconSx}>
                    <Typography variant="h3">✓</Typography>
                  </Box>

                  <Alert severity="success" sx={verificationAlertSx}>
                    We sent a verification link to <strong>{formData.email}</strong>
                  </Alert>

                  <Typography sx={verificationDescriptionSx}>
                    Please check your email and click the verification link to complete your registration.
                  </Typography>

                  <Button
                    variant="contained"
                    component={Link}
                    to={ROUTES.LOGIN}
                    sx={verificationPrimaryButtonSx}
                  >
                    Go to Login
                  </Button>

                  <Button variant="text" onClick={goBack}>
                    Back to registration
                  </Button>
                </Stack>
              )}
            </Stack>
          </CardContent>
        </Card>

        <PageFooterCaption isDark={isDark} />
      </Container>
    </Box>
  );
};

export default SignupPage;
