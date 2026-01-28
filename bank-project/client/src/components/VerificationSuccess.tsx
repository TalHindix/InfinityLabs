import { Link } from 'react-router-dom';
import { Button, Typography, Alert, Stack, Box } from '../utils/ui';
import {
  rootSx,
  verificationIconSx,
  alertSx,
  descriptionSx,
  primaryButtonSx,
} from './VerificationSuccess.styles';

interface VerificationSuccessProps {
  email: string;
  onBackClick: () => void;
}

export const VerificationSuccess = ({ email, onBackClick }: VerificationSuccessProps) => {
  return (
    <Stack spacing={3} sx={rootSx}>
      <Box sx={verificationIconSx}>
        <Typography variant="h3">✓</Typography>
      </Box>

      <Alert severity="success" sx={alertSx}>
        We sent a verification link to <strong>{email}</strong>
      </Alert>

      <Typography sx={descriptionSx}>
        Please check your email and click the verification link to complete your registration.
      </Typography>

      <Button
        variant="contained"
        component={Link}
        to="/login"
        sx={primaryButtonSx}
      >
        Go to Login
      </Button>

      <Button variant="text" onClick={onBackClick}>
        Back to registration
      </Button>
    </Stack>
  );
};
