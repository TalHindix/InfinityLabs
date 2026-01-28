import { Box, Typography } from '../utils/ui';
import { DubaiBankLogo } from './DubaiBankLogo';
import { logoContainerSx, createTitleSx, createSubtitleSx } from './BrandHeader.styles';

interface BrandHeaderProps {
  isDark: boolean;
  greeting: string;
}

export const BrandHeader = ({ isDark, greeting }: BrandHeaderProps) => {
  return (
    <Box textAlign="center">
      <Box sx={logoContainerSx}>
        <DubaiBankLogo size={80} animated={true} />
      </Box>

      <Typography variant="h4" sx={createTitleSx(isDark)}>
        Dubai Bank
      </Typography>

      <Typography variant="body2" sx={createSubtitleSx(isDark)}>
        {greeting} — secure access to your account
      </Typography>
    </Box>
  );
};
