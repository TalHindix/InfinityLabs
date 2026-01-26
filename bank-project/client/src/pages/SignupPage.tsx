import { useState } from 'react';
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
import { authService } from '../services/auth';
import { getErrorMessage } from '../types';
import { SignupForm } from '../components/SignupForm';
import { VerificationSuccess } from '../components/VerificationSuccess';
import { PageFooterCaption } from '../components/AuthPageFooter';
import { DubaiBankLogo } from '../components/DubaiBankLogo';
import {
  createSignupBackgroundSx,
  createSignupCardSx,
  signupThemeToggleSx,
} from '../styles/signup.styles';

const STEPS = ['Account Details', 'Verify Email'];

const SignupPage = () => {
  const { isDark, toggleTheme } = useThemeContext();

  // Step state
  const [activeStep, setActiveStep] = useState(0);

  // UI state
  const [error, setError] = useState('');
  const [loading, setLoading] = useState(false);

  // Form state - unified into single object
  const [formData, setFormData] = useState({
    firstName: '',
    lastName: '',
    email: '',
    password: '',
    phone: '',
  });

  const handleFieldChange = (field: keyof typeof formData, value: string) => {
    setFormData(prev => ({ ...prev, [field]: value }));
  };

  const handleSignup = async (e: React.FormEvent) => {
    e.preventDefault();
    setError('');
    setLoading(true);

    try {
      await authService.signup(formData);
      setActiveStep(1);
    } catch (err: unknown) {
      setError(getErrorMessage(err));
    } finally {
      setLoading(false);
    }
  };

  return (
    <Box sx={createSignupBackgroundSx(isDark)}>
      {/* Theme Toggle */}
      <IconButton onClick={toggleTheme} sx={signupThemeToggleSx}>
        {isDark ? <LightModeIcon /> : <DarkModeIcon />}
      </IconButton>

      <Container maxWidth="xs">
        <Card sx={createSignupCardSx(isDark)}>
          <CardContent sx={{ p: 4 }}>
            <Stack spacing={3}>
              {/* Logo/Branding */}
              <Box textAlign="center">
                <Box sx={{ display: 'flex', justifyContent: 'center', mb: 2 }}>
                  <DubaiBankLogo size={80} animated={true} />
                </Box>
                <Typography variant="h4" sx={{ fontWeight: 700, color: 'primary.main' }}>
                  Dubai Bank
                </Typography>
                <Typography variant="body2" color="text.secondary" sx={{ mt: 0.5 }}>
                  Open your premium account today
                </Typography>
              </Box>

              <Divider sx={{ borderColor: 'rgba(201, 162, 39, 0.3)' }} />

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
                  onSubmit={handleSignup}
                />
              ) : (
                <VerificationSuccess
                  email={formData.email}
                  onBackClick={() => setActiveStep(0)}
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
