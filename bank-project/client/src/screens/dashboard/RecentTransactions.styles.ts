import type { SxProps, Theme } from '@mui/material';

export const headerBarSx: SxProps<Theme> = {
  display: 'flex',
  flexDirection: 'row',
  justifyContent: 'space-between',
  alignItems: 'center',
  mb: 2,
};

export const sectionTitleSx: SxProps<Theme> = {
  fontWeight: 600,
  fontSize: { xs: '1.1rem', sm: '1.5rem' },
};

export const viewAllButtonSx: SxProps<Theme> = {
  color: '#C9A227',
  '&:hover': { bgcolor: 'rgba(201, 162, 39, 0.08)' },
};

export const tableContainerSx: SxProps<Theme> = {
  borderRadius: 3,
  overflowX: 'auto',
};

export const amountCellSx: SxProps<Theme> = {
  textAlign: 'right',
};

export const createAmountTextSx = (isSent: boolean): SxProps<Theme> => ({
  color: isSent ? 'error.main' : 'success.main',
});
