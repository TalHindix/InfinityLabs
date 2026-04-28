import type { SxProps, Theme } from '@mui/material';

export const createDetailPanelSx = (hasSelection: boolean): SxProps<Theme> => ({
  position: 'sticky',
  top: 20,
  border: hasSelection ? '1px solid rgba(201, 162, 39, 0.3)' : undefined,
});

export const createAmountIconSx = (isSent: boolean): SxProps<Theme> => ({
  width: 60,
  height: 60,
  mx: 'auto',
  mb: 2,
  borderRadius: '50%',
  background: isSent
    ? 'linear-gradient(135deg, rgba(239, 68, 68, 0.1) 0%, rgba(239, 68, 68, 0.2) 100%)'
    : 'linear-gradient(135deg, rgba(16, 185, 129, 0.1) 0%, rgba(16, 185, 129, 0.2) 100%)',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
});

export const loadingContainerSx: SxProps<Theme> = {
  py: 6,
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
};

export const loadingSpinnerSx: SxProps<Theme> = {
  color: 'secondary.main',
};

export const goldDividerSx: SxProps<Theme> = {
  my: 1,
};

export const refTextSx: SxProps<Theme> = {
  fontWeight: 600,
};

export const statusBadgeSx: SxProps<Theme> = {
  display: 'inline-block',
  px: 2,
  py: 0.5,
  borderRadius: 2,
  bgcolor: 'rgba(16, 185, 129, 0.1)',
  color: 'success.main',
};
