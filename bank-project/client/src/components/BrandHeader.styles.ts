import type { SxProps, Theme } from '@mui/material';

export const logoContainerSx: SxProps<Theme> = {
  display: 'flex',
  justifyContent: 'center',
  mb: 2,
};

export const createTitleSx = (isDark: boolean): SxProps<Theme> => ({
  fontWeight: 800,
  letterSpacing: '-0.02em',
  color: isDark ? 'rgba(255,255,255,0.92)' : '#0D1B2A',
});

export const createSubtitleSx = (isDark: boolean): SxProps<Theme> => ({
  mt: 0.75,
  color: isDark ? 'rgba(255,255,255,0.70)' : 'rgba(13,27,42,0.70)',
});
