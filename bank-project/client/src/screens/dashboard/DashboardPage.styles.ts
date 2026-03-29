import type { SxProps, Theme } from '@mui/material';

export const pageRootSx: SxProps<Theme> = {
  minHeight: '100vh',
  bgcolor: 'background.default',
};

export const containerSx: SxProps<Theme> = {
  py: { xs: 2, sm: 4 },
  px: { xs: 2, sm: 3 },
};

export const loadingScreenSx: SxProps<Theme> = {
  minHeight: '100vh',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
};

export const loadingSpinnerSx: SxProps<Theme> = {
  color: 'primary.main',
};
