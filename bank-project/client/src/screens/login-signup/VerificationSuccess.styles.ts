import type { SxProps, Theme } from '@mui/material';

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
};
