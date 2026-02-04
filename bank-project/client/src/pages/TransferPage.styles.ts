import type { SxProps, Theme } from '@mui/material';

export const decorativeSymbolSx: SxProps<Theme> = {
  color: '#0D1B2A',
};

export const pageTitleSx: SxProps<Theme> = {
  fontWeight: 600,
  color: 'primary.main',
};

export const pageSubtitleSx: SxProps<Theme> = {
  mt: 0.5,
};

export const goldDividerSx: SxProps<Theme> = {
  borderColor: 'rgba(201, 162, 39, 0.2)',
};

export const footerCaptionSx: SxProps<Theme> = {
  display: 'block',
  textAlign: 'center',
  mt: 3,
  color: 'text.secondary',
};

export const transferDecorativeSx: SxProps<Theme> = {
  position: 'absolute',
  top: -30,
  left: '50%',
  transform: 'translateX(-50%)',
  width: 60,
  height: 60,
  borderRadius: '16px',
  background: 'linear-gradient(135deg, #C9A227 0%, #E5C158 100%)',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
  boxShadow: '0 8px 24px rgba(201, 162, 39, 0.3)',
};

export const pageRootSx: SxProps<Theme> = {
  minHeight: '100vh',
  bgcolor: 'background.default',
};

export const containerSx: SxProps<Theme> = {
  py: 6,
};

export const cardSx: SxProps<Theme> = {
  overflow: 'visible',
  position: 'relative',
};

export const cardContentSx: SxProps<Theme> = {
  p: 4,
  pt: 6,
};
