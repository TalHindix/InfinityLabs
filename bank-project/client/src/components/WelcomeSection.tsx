import { Box, Typography } from './ui';

interface WelcomeSectionProps {
  firstName: string | undefined;
}

/**
 * Welcome section with personalized greeting.
 */
export const WelcomeSection = ({ firstName }: WelcomeSectionProps) => {
  return (
    <Box>
      <Typography variant="h4" sx={{ fontWeight: 600, color: 'primary.main' }}>
        Welcome back, {firstName || 'User'}
      </Typography>
      <Typography color="text.secondary">
        Here's your account overview
      </Typography>
    </Box>
  );
};
