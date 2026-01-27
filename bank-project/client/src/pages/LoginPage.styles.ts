import type { SxProps, Theme } from '@mui/material';

export const createDividerSx = (isDark: boolean): SxProps<Theme> => ({
  borderColor: isDark ? 'rgba(255,255,255,0.10)' : 'rgba(13,27,42,0.10)',
});

export const createSignInTitleSx = (isDark: boolean): SxProps<Theme> => ({
  fontWeight: 700,
  color: isDark ? 'rgba(255,255,255,0.92)' : '#0D1B2A',
});
