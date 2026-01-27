import type { SxProps, Theme } from '@mui/material';

export const createSecurityNoteSx = (isDark: boolean): SxProps<Theme> => ({
  textAlign: 'center',
  color: isDark ? 'rgba(255,255,255,0.50)' : 'rgba(13,27,42,0.50)',
  lineHeight: 1.5,
  px: 1,
});

export const createForgotPasswordSx = (isDark: boolean): SxProps<Theme> => ({
  color: isDark ? 'rgba(255,255,255,0.70)' : 'rgba(13,27,42,0.70)',
  cursor: 'pointer',
  '&:hover': { color: '#C9A227' },
});

export const footerContainerSx: SxProps<Theme> = {
  display: 'flex',
  justifyContent: 'space-between',
  alignItems: 'center',
  pt: 0.5,
};
