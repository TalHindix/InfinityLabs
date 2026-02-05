import type { SxProps, Theme } from '@mui/material';

export const pageRootSx: SxProps<Theme> = {
  minHeight: '100vh',
  bgcolor: 'background.default',
};

export const containerSx: SxProps<Theme> = {
  py: 4,
};

export const titleSx: SxProps<Theme> = {
  fontWeight: 600,
  color: 'primary.main',
};

export const listColSx: SxProps<Theme> = {
  flex: 2,
};

export const detailColSx: SxProps<Theme> = {
  flex: 1,
  minWidth: 300,
};

export const primaryButtonSx: SxProps<Theme> = {
  background: 'linear-gradient(135deg, #C9A227 0%, #E5C158 100%)',
  color: '#0D1B2A',
  '&:hover': {
    background: 'linear-gradient(135deg, #A17F1A 0%, #C9A227 100%)',
  },
};
