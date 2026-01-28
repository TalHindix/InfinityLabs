import type { SxProps, Theme } from '@mui/material';
import type { CSSProperties } from 'react';

export const createSecurityNoteSx = (isDark: boolean): SxProps<Theme> => ({
  textAlign: 'center',
  color: isDark ? 'rgba(255,255,255,0.50)' : 'rgba(13,27,42,0.50)',
  lineHeight: 1.5,
  px: 1,
});

export const createForgotPasswordSx = (isDark: boolean): SxProps<Theme> => ({
  color: isDark ? 'rgba(255,255,255,0.70)' : 'rgba(13,27,42,0.70)',
  cursor: 'pointer',
  '&:hover': { color: '#C9A227' },
});

export const footerContainerSx: SxProps<Theme> = {
  display: 'flex',
  justifyContent: 'space-between',
  alignItems: 'center',
  pt: 0.5,
};

export const errorAlertSx: SxProps<Theme> = {
  '& .MuiAlert-message': { width: '100%' },
};

export const errorCaptionSx: SxProps<Theme> = {
  display: 'block',
  mt: 0.5,
  opacity: 0.85,
};

export const createFieldSx = (isDark: boolean): SxProps<Theme> => ({
  '& .MuiInputLabel-root': {
    color: isDark ? 'rgba(255,255,255,0.70)' : 'rgba(13,27,42,0.70)',
  },
  '& .MuiInputLabel-root.Mui-focused': {
    color: 'rgba(201, 162, 39, 0.95)',
  },
  '& .MuiOutlinedInput-root': {
    backgroundColor: isDark ? 'rgba(255,255,255,0.04)' : 'rgba(13,27,42,0.03)',
    borderRadius: 12,
    transition:
      'box-shadow 160ms ease, border-color 160ms ease, background-color 160ms ease',
    '& fieldset': {
      borderColor: isDark ? 'rgba(255,255,255,0.12)' : 'rgba(13,27,42,0.14)',
    },
    '&:hover fieldset': {
      borderColor: 'rgba(201, 162, 39, 0.45)',
    },
    '&.Mui-focused fieldset': {
      borderColor: 'rgba(201, 162, 39, 0.85)',
    },
    '&.Mui-focused': {
      boxShadow: '0 0 0 4px rgba(201,162,39,0.16)',
    },
    '& input': {
      color: isDark ? 'rgba(255,255,255,0.92)' : '#0D1B2A',
    },
  },
  '& .MuiFormHelperText-root': {
    color: isDark ? 'rgba(255,255,255,0.65)' : 'rgba(13,27,42,0.65)',
  },
  '& input:-webkit-autofill, & input:-webkit-autofill:hover, & input:-webkit-autofill:focus, & input:-webkit-autofill:active': {
    WebkitBoxShadow: isDark
      ? '0 0 0 1000px #0a1520 inset !important'
      : '0 0 0 1000px #f5f5f5 inset !important',
    WebkitTextFillColor: isDark ? '#fff !important' : '#0D1B2A !important',
    caretColor: isDark ? '#fff' : '#0D1B2A',
    transition: 'background-color 5000s ease-in-out 0s',
  },
});

export const primaryButtonSx: SxProps<Theme> = {
  py: 1.45,
  borderRadius: 12,
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

export const signUpLinkStyle: CSSProperties = {
  fontWeight: 700,
  color: '#C9A227',
  textDecoration: 'none',
};
