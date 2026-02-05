import type { SxProps, Theme } from '@mui/material';

export const rowSx = (isSelected: boolean): SxProps<Theme> => ({
  cursor: 'pointer',
  bgcolor: isSelected ? 'rgba(201, 162, 39, 0.1)' : 'inherit',
  '&:hover': {
    bgcolor: 'rgba(201, 162, 39, 0.06)',
  },
});

export const refNumberSx: SxProps<Theme> = {
  color: '#C9A227',
  fontFamily: 'monospace',
  fontWeight: 600,
};

export const amountSentSx: SxProps<Theme> = {
  color: 'error.main',
};

export const amountReceivedSx: SxProps<Theme> = {
  color: 'success.main',
};

export const viewButtonSx: SxProps<Theme> = {
  color: '#C9A227',
  minWidth: 'auto',
  textTransform: 'none',
};
