import type { SxProps, Theme } from '@mui/material';

export const loadingContainerSx: SxProps<Theme> = {
  py: 4,
  textAlign: 'center',
};

export const loadingSpinnerSx: SxProps<Theme> = {
  color: '#C9A227',
};

export const goldDividerSx: SxProps<Theme> = {
  borderColor: 'rgba(201, 162, 39, 0.2)',
};

export const refTextSx: SxProps<Theme> = {
  color: '#C9A227',
};

export const statusBadgeSx: SxProps<Theme> = {
  display: 'inline-block',
  px: 2,
  py: 0.5,
  borderRadius: 2,
  bgcolor: 'rgba(16, 185, 129, 0.1)',
  color: 'success.main',
};

export const emptyStateSx: SxProps<Theme> = {
  py: 8,
  textAlign: 'center',
};

export const emptyIconSx: SxProps<Theme> = {
  fontSize: 48,
  color: 'text.disabled',
  mb: 2,
};
