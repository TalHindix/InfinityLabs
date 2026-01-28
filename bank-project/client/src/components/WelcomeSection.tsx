import { Box, Typography } from '../utils/ui';
import {
  rootSx,
  welcomeTitleSx,
  subtitleSx,
} from './WelcomeSection.styles';

interface WelcomeSectionProps {
  firstName: string | undefined;
}

export const WelcomeSection = ({ firstName }: WelcomeSectionProps) => {
  return (
    <Box sx={rootSx}>
      <Typography variant="h4" sx={welcomeTitleSx}>
        Welcome back, {firstName || 'User'}
      </Typography>

      <Typography sx={subtitleSx}>
        Here's your account overview
      </Typography>
    </Box>
  );
};
