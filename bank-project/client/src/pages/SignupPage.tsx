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
} from '../utils/ui';
import { Link } from 'react-router-dom';
import { useThemeContext } from '../context/ThemeContext';
import { useSignup } from '../hooks/useSignup';
import { SignupForm } from '../components/SignupForm';
import { PageFooterCaption } from '../components/PageFooterCaption';
import { DubaiBankLogo } from '../components/DubaiBankLogo';
import {
  createSignupBackgroundSx,
  createSignupCardSx,
  signupThemeToggleSx,
  logoContainerSx,
  pageTitleSx,
  pageSubtitleSx,
  goldDividerSx,
} from './SignupPage.styles';
import {
  rootSx as verificationRootSx,
  verificationIconSx,
  alertSx as verificationAlertSx,
  descriptionSx as verificationDescriptionSx,
  primaryButtonSx as verificationPrimaryButtonSx,
} from '../components/VerificationSuccess.styles';

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
    <Box sx={createSignupBackgroundSx(isDark)}>
      <IconButton onClick={toggleTheme} sx={signupThemeToggleSx}>
        {isDark ? <LightModeIcon /> : <DarkModeIcon />}
      </IconButton>

      <Container maxWidth="xs">
        <Card sx={createSignupCardSx(isDark)}>
          <CardContent sx={{ p: 4 }}>
            <Stack spacing={3}>
              <Box textAlign="center">
                <Box sx={logoContainerSx}>
                  <DubaiBankLogo size={80} animated={true} />
                </Box>
                <Typography variant="h4" sx={pageTitleSx}>
                  Dubai Bank
                </Typography>
                <Typography variant="body2" color="text.secondary" sx={pageSubtitleSx}>
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
                    to="/login"
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

        <PageFooterCaption isDark={isDark} variant="signup" />
      </Container>
    </Box>
  );
};

export default SignupPage;
