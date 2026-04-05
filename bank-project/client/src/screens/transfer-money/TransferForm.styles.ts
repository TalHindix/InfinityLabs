import type { SxProps, Theme } from '@mui/material';

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

export const primaryButtonSx: SxProps<Theme> = {
  py: 1.45,
  fontSize: '1rem',
  fontWeight: 700,
  letterSpacing: '0.01em',
  background: 'linear-gradient(135deg, #C9A227 0%, #E5C158 100%)',
  color: '#0D1B2A',
  boxShadow: '0 12px 30px rgba(201,162,39,0.22)',
  transition: 'all 200ms ease',
  '&:hover': {
    background: 'linear-gradient(135deg, #A17F1A 0%, #C9A227 100%)',
    boxShadow: '0 14px 34px rgba(201,162,39,0.28)',
  },
  '&:active': {
    transform: 'translateY(1px)',
  },
  '&.Mui-disabled': {
    color: 'rgba(13,27,42,0.75)',
    background:
      'linear-gradient(135deg, rgba(201,162,39,0.65) 0%, rgba(229,193,88,0.65) 100%)',
  },
};

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

export const dialogConfirmButtonSx: SxProps<Theme> = {
  fontWeight: 700,
  background: 'linear-gradient(135deg, #C9A227 0%, #E5C158 100%)',
  color: '#0D1B2A',
  boxShadow: '0 12px 30px rgba(201,162,39,0.22)',
  transition: 'all 200ms ease',
  '&:hover': {
    background: 'linear-gradient(135deg, #A17F1A 0%, #C9A227 100%)',
    boxShadow: '0 14px 34px rgba(201,162,39,0.28)',
  },
};

export const dialogActionsSx: SxProps<Theme> = {
  justifyContent: 'center',
};
