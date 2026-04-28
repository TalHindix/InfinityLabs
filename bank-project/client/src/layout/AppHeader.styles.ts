import type { SxProps, Theme } from '@mui/material';

export const headerContainerSx: SxProps<Theme> = {
  background: 'linear-gradient(135deg, #0D1B2A 0%, #1B3A4B 100%)',
  color: 'white',
  py: { xs: 1.5, sm: 3 },
  px: { xs: 1, sm: 2 },
};

export const logoContainerSx: SxProps<Theme> = {
  cursor: 'pointer',
  display: 'flex',
  alignItems: 'center',
};

export const themeToggleSx: SxProps<Theme> = {
  color: '#C9A227',
  '&:hover': {
    bgcolor: 'rgba(201, 162, 39, 0.1)',
  },
};

export const outlinedButtonSx: SxProps<Theme> = {
  borderColor: 'rgba(255, 255, 255, 0.3)',
  color: 'white',
  display: { xs: 'none', sm: 'inline-flex' },
  '&:hover': {
    borderColor: '#C9A227',
    bgcolor: 'rgba(201, 162, 39, 0.1)',
  },
};

export const mobileIconButtonSx: SxProps<Theme> = {
  color: 'rgba(255, 255, 255, 0.8)',
  display: { xs: 'inline-flex', sm: 'none' },
  '&:hover': {
    bgcolor: 'rgba(201, 162, 39, 0.1)',
    color: '#C9A227',
  },
};
