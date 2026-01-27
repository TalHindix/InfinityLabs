import { Box, Typography } from './ui';
import { welcomeTitleSx } from './WelcomeSection.styles';

interface WelcomeSectionProps {
  firstName: string | undefined;
}

export const WelcomeSection = ({ firstName }: WelcomeSectionProps) => {
  return (
    <Box>
      <Typography variant="h4" sx={welcomeTitleSx}>
        Welcome back, {firstName || 'User'}
      </Typography>
      <Typography color="text.secondary">
        Here's your account overview
      </Typography>
    </Box>
  );
};
