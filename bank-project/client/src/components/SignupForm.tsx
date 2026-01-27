import { Link } from 'react-router-dom';
import { TextField, Button, Typography, Stack } from './ui';
import { useThemeContext } from '../context/ThemeContext';
import { createFieldSx } from '../styles/login.styles';
import { submitButtonSx } from './SignupForm.styles';

interface SignupFormData {
  firstName: string;
  lastName: string;
  email: string;
  password: string;
  phone: string;
}

interface SignupFormProps {
  formData: SignupFormData;
  loading: boolean;
  onFieldChange: (field: keyof SignupFormData, value: string) => void;
  onSubmit: (e: React.FormEvent) => void;
}

export const SignupForm = ({
  formData,
  loading,
  onFieldChange,
  onSubmit,
}: SignupFormProps) => {
  const { isDark } = useThemeContext();
  const fieldSx = createFieldSx(isDark);

  return (
    <form onSubmit={onSubmit}>
      <Stack spacing={2}>
        <Stack direction="row" spacing={2}>
          <TextField
            fullWidth
            label="First Name"
            value={formData.firstName}
            onChange={(e) => onFieldChange('firstName', e.target.value)}
            required
            sx={fieldSx}
          />
          <TextField
            fullWidth
            label="Last Name"
            value={formData.lastName}
            onChange={(e) => onFieldChange('lastName', e.target.value)}
            required
            sx={fieldSx}
          />
        </Stack>
        <TextField
          fullWidth
          label="Email Address"
          type="email"
          value={formData.email}
          onChange={(e) => onFieldChange('email', e.target.value)}
          required
          placeholder="you@example.com"
          sx={fieldSx}
        />
        <TextField
          fullWidth
          label="Phone Number"
          placeholder="+971501234567"
          value={formData.phone}
          onChange={(e) => onFieldChange('phone', e.target.value)}
          required
          helperText="International format (e.g., +971...)"
          sx={fieldSx}
        />
        <TextField
          fullWidth
          label="Password"
          type="password"
          value={formData.password}
          onChange={(e) => onFieldChange('password', e.target.value)}
          required
          helperText="At least 8 characters with uppercase, lowercase, and number"
          sx={fieldSx}
        />
        <Button
          fullWidth
          type="submit"
          variant="contained"
          size="large"
          disabled={loading}
          sx={submitButtonSx}
        >
          {loading ? 'Creating Account...' : 'Create Account'}
        </Button>
        <Typography textAlign="center" variant="body2" color="text.secondary">
          Already have an account?{' '}
          <Link to="/login" style={{ fontWeight: 600 }}>
            Sign In
          </Link>
        </Typography>
      </Stack>
    </form>
  );
};
