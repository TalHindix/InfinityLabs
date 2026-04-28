import type { SxProps, Theme } from '@mui/material';
import { goldButtonSx } from '../../shared/buttonStyles';

export const pageRootSx: SxProps<Theme> = {
  minHeight: '100vh',
  bgcolor: 'background.default',
};

export const containerSx: SxProps<Theme> = {
  py: { xs: 2, sm: 4 },
  px: { xs: 2, sm: 3 },
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

export const primaryButtonSx = goldButtonSx;
