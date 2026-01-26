import { Link } from 'react-router-dom';
import {
  Button,
  Typography,
  Alert,
  Stack,
  Box,
} from './ui';
import { verificationIconSx } from '../styles/signup.styles';
import { primaryButtonSx } from '../styles/login.styles';


interface VerificationSuccessProps {
  email: string;
  onBackClick: () => void;
}

/**
 * Verification success step shown after signup.
 * Displays success message and prompts user to check email.
 */
export const VerificationSuccess = ({ email, onBackClick }: VerificationSuccessProps) => {
  return (
    <Stack spacing={3} textAlign="center">
      <Box sx={verificationIconSx}>
        <Typography variant="h3">✓</Typography>
      </Box>
      <Alert severity="success" sx={{ textAlign: 'left' }}>
        We sent a verification link to <strong>{email}</strong>
      </Alert>
      <Typography color="text.secondary">
        Please check your email and click the verification link to complete your registration.
      </Typography>
      <Button
        variant="contained"
        component={Link}
        to="/login"
        sx={{
          ...primaryButtonSx,
          py: 1.5,
        }}
      >
        Go to Login
      </Button>
      <Button variant="text" onClick={onBackClick}>
        Back to registration
      </Button>
    </Stack>
  );
};
