import type { SxProps, Theme } from '@mui/material';

export const logoContainerSx: SxProps<Theme> = {
  display: 'flex',
  justifyContent: 'center',
  mb: 2,
};

export const pageTitleSx: SxProps<Theme> = {
  fontWeight: 700,
  color: 'primary.main',
};

export const pageSubtitleSx: SxProps<Theme> = {
  mt: 0.5,
};

export const goldDividerSx: SxProps<Theme> = {
  borderColor: 'rgba(201, 162, 39, 0.3)',
};

export const createSignupBackgroundSx = (isDark: boolean): SxProps<Theme> => ({
  minHeight: '100vh',
  background: isDark
    ? 'linear-gradient(135deg, #050A0F 0%, #0A1520 50%, #050A0F 100%)'
    : 'linear-gradient(135deg, #0D1B2A 0%, #1B3A4B 50%, #0D1B2A 100%)',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
  py: 4,
  position: 'relative',
});

export const createSignupCardSx = (isDark: boolean): SxProps<Theme> => ({
  backdropFilter: 'blur(10px)',
  background: isDark ? 'rgba(18, 26, 34, 0.98)' : 'rgba(255, 255, 255, 0.98)',
  border: '1px solid rgba(201, 162, 39, 0.2)',
});

export const signupThemeToggleSx: SxProps<Theme> = {
  position: 'absolute',
  top: 16,
  right: 16,
  color: '#C9A227',
  bgcolor: 'rgba(201, 162, 39, 0.1)',
  '&:hover': {
    bgcolor: 'rgba(201, 162, 39, 0.2)',
  },
};
