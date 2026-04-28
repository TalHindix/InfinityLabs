import type { SxProps, Theme } from '@mui/material';
import { goldButtonSx } from '../../shared/buttonStyles';

export const rootSx: SxProps<Theme> = {
  textAlign: 'center',
};

export const verificationIconSx: SxProps<Theme> = {
  width: 56,
  height: 56,
  mx: 'auto',
  borderRadius: '50%',
  bgcolor: 'rgba(16, 185, 129, 0.12)',
  color: 'success.main',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
};

export const alertSx: SxProps<Theme> = {
  textAlign: 'left',
};

export const descriptionSx: SxProps<Theme> = {
  color: 'text.secondary',
};

export const primaryButtonSx = goldButtonSx;
