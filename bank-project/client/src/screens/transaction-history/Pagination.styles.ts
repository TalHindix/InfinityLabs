import type { SxProps, Theme } from '@mui/material';

export const pageIndicatorSx: SxProps<Theme> = {
  px: 3,
  py: 1,
  borderRadius: 1,
  bgcolor: 'rgba(13, 27, 42, 0.05)',
  display: 'flex',
  alignItems: 'center',
};

export const paginationButtonSx: SxProps<Theme> = {
  borderColor: '#C9A227',
  color: '#C9A227',
  '&:hover': { borderColor: '#A17F1A', bgcolor: 'rgba(201, 162, 39, 0.08)' },
  '&:disabled': { borderColor: 'grey.300', color: 'grey.400' },
};
