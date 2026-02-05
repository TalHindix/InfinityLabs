import type { SxProps, Theme } from '@mui/material';

export const contentSx: SxProps<Theme> = {
  py: 8,
  display: 'flex',
  flexDirection: 'column',
  alignItems: 'center',
  gap: 2,
};

export const spinnerSx: SxProps<Theme> = {
  color: 'primary.main',
};

export const emptyIconContainerSx: SxProps<Theme> = {
  width: 80,
  height: 80,
  borderRadius: '50%',
  bgcolor: 'action.hover',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
};

export const emptyIconTextSx: SxProps<Theme> = {
  opacity: 0.3,
};

export const emptyTextSx: SxProps<Theme> = {
  color: 'text.secondary',
};
