import { Box, Typography } from './ui';
import { DubaiBankLogo } from './DubaiBankLogo';

interface BrandHeaderProps {
  isDark: boolean;
  greeting: string;
}

/**
 * Brand header with animated AI logo, title, and context-aware greeting.
 * Used in login page for visual branding and personalization.
 */
export const BrandHeader = ({ isDark, greeting }: BrandHeaderProps) => {
  return (
    <Box textAlign="center">
      <Box sx={{ display: 'flex', justifyContent: 'center', mb: 2 }}>
        <DubaiBankLogo size={80} animated={true} />
      </Box>

      <Typography
        variant="h4"
        sx={{
          fontWeight: 800,
          letterSpacing: '-0.02em',
          color: isDark ? 'rgba(255,255,255,0.92)' : '#0D1B2A',
        }}
      >
        Dubai Bank
      </Typography>

      <Typography
        variant="body2"
        sx={{
          mt: 0.75,
          color: isDark ? 'rgba(255,255,255,0.70)' : 'rgba(13,27,42,0.70)',
        }}
      >
        {greeting} — secure access to your account
      </Typography>
    </Box>
  );
};
