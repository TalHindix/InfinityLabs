import type { SxProps, Theme } from '@mui/material';
import type { CSSProperties } from 'react';
import { createFieldSx } from '../../shared/authFormStyles';
import { goldButtonSx } from '../../shared/buttonStyles';

export { createFieldSx };
export const primaryButtonSx = goldButtonSx;

export const createSecurityNoteSx = (isDark: boolean): SxProps<Theme> => ({
  textAlign: 'center',
  color: isDark ? 'rgba(255,255,255,0.50)' : 'rgba(13,27,42,0.50)',
  lineHeight: 1.5,
  px: 1,
});

export const footerContainerSx: SxProps<Theme> = {
  display: 'flex',
  justifyContent: 'center',
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

export const resendLinkButtonSx = (isDark: boolean): SxProps<Theme> => ({
  textTransform: 'none',
  color: isDark ? '#90caf9' : '#1976d2',
  '&:hover': {
    backgroundColor: isDark
      ? 'rgba(144, 202, 249, 0.08)'
      : 'rgba(25, 118, 210, 0.08)',
  },
});

export const signUpLinkStyle: CSSProperties = {
  fontWeight: 700,
  color: '#C9A227',
  textDecoration: 'none',
};
