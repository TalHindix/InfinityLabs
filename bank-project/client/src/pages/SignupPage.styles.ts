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
