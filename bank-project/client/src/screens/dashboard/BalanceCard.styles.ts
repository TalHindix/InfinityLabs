import type { SxProps, Theme } from '@mui/material';

export const balanceCardSx: SxProps<Theme> = {
  background: 'linear-gradient(135deg, #0D1B2A 0%, #1B3A4B 100%)',
  color: 'white',
  overflow: 'visible',
  position: 'relative',
};

export const balanceBadgeSx: SxProps<Theme> = {
  position: 'absolute',
  top: { xs: -14, sm: -20 },
  right: { xs: 16, sm: 30 },
  width: { xs: 56, sm: 80 },
  height: { xs: 56, sm: 80 },
  borderRadius: { xs: '14px', sm: '20px' },
  background: 'linear-gradient(135deg, #C9A227 0%, #E5C158 100%)',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
  boxShadow: '0 8px 32px rgba(201, 162, 39, 0.4)',
};

export const badgeTextSx: SxProps<Theme> = {
  color: '#0D1B2A',
};

export const cardContentSx: SxProps<Theme> = {
  p: { xs: 2.5, sm: 4 },
};

export const availableLabelSx: SxProps<Theme> = {
  opacity: 0.8,
  mb: 1,
};

export const amountValueSx: SxProps<Theme> = {
  fontWeight: 700,
  mb: 3,
};

export const amountCurrencySx: SxProps<Theme> = {
  ml: 1,
  opacity: 0.7,
  fontWeight: 400,
};

export const primaryButtonSx: SxProps<Theme> = {
  background: 'linear-gradient(135deg, #C9A227 0%, #E5C158 100%)',
  color: '#0D1B2A',
  px: { xs: 3, sm: 4 },
  py: { xs: 1, sm: 1.5 },
  fontSize: { xs: '0.85rem', sm: '0.875rem' },
  '&:hover': {
    background: 'linear-gradient(135deg, #A17F1A 0%, #C9A227 100%)',
  },
};
