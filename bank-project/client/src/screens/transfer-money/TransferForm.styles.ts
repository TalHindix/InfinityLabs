import type { SxProps, Theme } from '@mui/material';
import { goldButtonSx } from '../../shared/buttonStyles';

export const amountAdornmentSx: SxProps<Theme> = {
  fontWeight: 700,
  letterSpacing: '0.02em',
  color: '#C9A227',
};

export const transferSummarySx: SxProps<Theme> = {
  bgcolor: 'rgba(13, 27, 42, 0.03)',
  borderRadius: 2,
  p: 2,
};

export const feeFreeTextSx: SxProps<Theme> = {
  color: 'success.main',
};

export const summaryDividerSx: SxProps<Theme> = {
  my: 1.5,
};

export const totalAmountSx: SxProps<Theme> = {
  color: 'primary.main',
};

export const primaryButtonSx = goldButtonSx;

export const cancelButtonSx: SxProps<Theme> = {
  color: 'text.secondary',
};

export const dialogPaperSx: SxProps<Theme> = {
  borderRadius: 3,
  minWidth: 360,
};

export const dialogTitleSx: SxProps<Theme> = {
  fontWeight: 700,
  color: 'text.primary',
  pb: 1,
};

export const dialogContentSx: SxProps<Theme> = {
  pt: 1,
};

export const dialogRecipientSx: SxProps<Theme> = {
  color: '#C9A227',
  fontWeight: 600,
};

export const dialogWarningTextSx: SxProps<Theme> = {
  color: 'text.secondary',
};

export const dialogCancelButtonSx: SxProps<Theme> = {
  color: 'text.secondary',
};

export const dialogConfirmButtonSx = goldButtonSx;

export const dialogActionsSx: SxProps<Theme> = {
  justifyContent: 'center',
};
