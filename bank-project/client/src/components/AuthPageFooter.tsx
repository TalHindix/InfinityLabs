import { Box, Typography } from './ui';

interface AuthPageFooterProps {
  isDark: boolean;
}

/**
 * Session protection notice shown at bottom of auth card.
 * Displays lock icon and auto-logout message.
 */
export const SessionProtectionNotice = ({ isDark }: AuthPageFooterProps) => {
  return (
    <Box
      sx={{
        display: 'flex',
        alignItems: 'center',
        justifyContent: 'center',
        gap: 0.75,
        pt: 1,
      }}
    >
      <Box
        component="span"
        sx={{
          fontSize: '0.75rem',
        }}
      >
        🔒
      </Box>
      <Typography
        variant="caption"
        sx={{
          color: isDark ? 'rgba(255,255,255,0.55)' : 'rgba(13,27,42,0.55)',
        }}
      >
        Protected session • Auto logout on inactivity
      </Typography>
    </Box>
  );
};

interface PageFooterCaptionProps {
  isDark: boolean;
  variant?: 'login' | 'signup';
}

/**
 * Footer caption shown below auth card.
 */
export const PageFooterCaption = ({ isDark, variant = 'login' }: PageFooterCaptionProps) => {
  const text = variant === 'signup'
    ? 'Secure banking powered by Dubai-Bank'
    : 'Secure banking powered by Dubai Bank';

  return (
    <Typography
      variant="caption"
      sx={{
        display: 'block',
        textAlign: 'center',
        mt: 3,
        color: isDark ? 'rgba(255,255,255,0.55)' : 'rgba(255,255,255,0.60)',
      }}
    >
      {text}
    </Typography>
  );
};
