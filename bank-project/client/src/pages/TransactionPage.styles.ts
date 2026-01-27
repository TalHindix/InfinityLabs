import type { SxProps, Theme } from '@mui/material';

export const decorativeSymbolSx: SxProps<Theme> = {
  color: '#0D1B2A',
};

export const pageTitleSx: SxProps<Theme> = {
  fontWeight: 600,
  color: 'primary.main',
};

export const pageSubtitleSx: SxProps<Theme> = {
  mt: 0.5,
};

export const goldDividerSx: SxProps<Theme> = {
  borderColor: 'rgba(201, 162, 39, 0.2)',
};

export const footerCaptionSx: SxProps<Theme> = {
  display: 'block',
  textAlign: 'center',
  mt: 3,
  color: 'text.secondary',
};
