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
} from '../components/ui';
import { useThemeContext } from '../context/ThemeContext';
import { useSignup } from '../hooks/useSignup';
import { SignupForm } from '../components/SignupForm';
import { VerificationSuccess } from '../components/VerificationSuccess';
import { PageFooterCaption } from '../components/AuthPageFooter';
import { DubaiBankLogo } from '../components/DubaiBankLogo';
import {
  createSignupBackgroundSx,
  createSignupCardSx,
  signupThemeToggleSx,
} from '../styles/signup.styles';
import {
  logoContainerSx,
  pageTitleSx,
  pageSubtitleSx,
  goldDividerSx,
} from './SignupPage.styles';

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
                <VerificationSuccess
                  email={formData.email}
                  onBackClick={goBack}
                />
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
