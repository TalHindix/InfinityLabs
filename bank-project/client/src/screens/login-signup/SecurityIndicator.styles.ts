import { keyframes } from '@emotion/react';
import type { SxProps, Theme } from '@mui/material';

const aiPulse = keyframes`
  0%, 100% { opacity: 0.6; transform: scale(1); }
  50% { opacity: 1; transform: scale(1.15); }
`;

const aiGlow = keyframes`
  0%, 100% { box-shadow: 0 0 4px rgba(16, 185, 129, 0.4); }
  50% { box-shadow: 0 0 8px rgba(16, 185, 129, 0.7); }
`;

export const createContainerSx = (isDark: boolean): SxProps<Theme> => ({
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
  gap: 1,
  py: 1,
  px: 2,
  borderRadius: '8px',
  backgroundColor: isDark ? 'rgba(16, 185, 129, 0.08)' : 'rgba(16, 185, 129, 0.06)',
  border: `1px solid ${isDark ? 'rgba(16, 185, 129, 0.15)' : 'rgba(16, 185, 129, 0.12)'}`,
});

export const createDotSx = (prefersReducedMotion: boolean): SxProps<Theme> => ({
  width: 8,
  height: 8,
  borderRadius: '50%',
  backgroundColor: '#10B981',
  flexShrink: 0,
  ...(!prefersReducedMotion && {
    animation: `${aiPulse} 2.5s ease-in-out infinite, ${aiGlow} 2.5s ease-in-out infinite`,
  }),
});

export const createTextSx = (isDark: boolean): SxProps<Theme> => ({
  color: isDark ? 'rgba(16, 185, 129, 0.9)' : 'rgba(16, 185, 129, 0.85)',
  fontWeight: 500,
  letterSpacing: '0.01em',
});
