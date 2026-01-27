import { keyframes } from '@emotion/react';
import { Box, Typography } from './ui';
import { createSecurityIndicatorSx } from './SecurityIndicator.styles';

const aiPulse = keyframes`
  0%, 100% { opacity: 0.6; transform: scale(1); }
  50% { opacity: 1; transform: scale(1.15); }
`;

const aiGlow = keyframes`
  0%, 100% { box-shadow: 0 0 4px rgba(16, 185, 129, 0.4); }
  50% { box-shadow: 0 0 8px rgba(16, 185, 129, 0.7); }
`;

interface SecurityIndicatorProps {
  isDark: boolean;
}

export const SecurityIndicator = ({ isDark }: SecurityIndicatorProps) => {
  const prefersReducedMotion =
    typeof window !== 'undefined' &&
    window.matchMedia('(prefers-reduced-motion: reduce)').matches;

  return (
    <Box sx={createSecurityIndicatorSx(isDark)}>
      <Box
        sx={{
          width: 8,
          height: 8,
          borderRadius: '50%',
          backgroundColor: '#10B981',
          flexShrink: 0,
          ...(!prefersReducedMotion && {
            animation: `${aiPulse} 2.5s ease-in-out infinite, ${aiGlow} 2.5s ease-in-out infinite`,
          }),
        }}
      />
      <Typography
        variant="caption"
        sx={{
          color: isDark ? 'rgba(16, 185, 129, 0.9)' : 'rgba(16, 185, 129, 0.85)',
          fontWeight: 500,
          letterSpacing: '0.01em',
        }}
      >
        Adaptive risk analysis enabled
      </Typography>
    </Box>
  );
};
