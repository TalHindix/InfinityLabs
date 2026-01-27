import type { SxProps, Theme } from '@mui/material';

export const loadingContentSx: SxProps<Theme> = {
  py: 8,
  textAlign: 'center',
};

export const loadingSpinnerSx: SxProps<Theme> = {
  color: '#C9A227',
};

export const emptyIconContainerSx: SxProps<Theme> = {
  width: 80,
  height: 80,
  mx: 'auto',
  mb: 2,
  borderRadius: '50%',
  bgcolor: 'rgba(13, 27, 42, 0.05)',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
};

export const emptyIconTextSx: SxProps<Theme> = {
  opacity: 0.3,
};
