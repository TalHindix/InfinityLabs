import type { SxProps, Theme } from '@mui/material';

export const rootSx: SxProps<Theme> = {
  display: 'flex',
  flexDirection: 'column',
  gap: 0.5,
};

export const welcomeTitleSx: SxProps<Theme> = {
  fontWeight: 700,
};

export const subtitleSx: SxProps<Theme> = {
  color: 'text.secondary',
};
