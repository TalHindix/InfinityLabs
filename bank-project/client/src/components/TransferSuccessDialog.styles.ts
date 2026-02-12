import type { SxProps, Theme } from '@mui/material';

export const dialogPaperSx: SxProps<Theme> = {
  borderRadius: 3,
  minWidth: 400,
};

export const dialogTitleSx: SxProps<Theme> = {
  fontWeight: 700,
  color: '#0D1B2A',
  pb: 1,
  textAlign: 'center',
};

export const dialogContentSx: SxProps<Theme> = {
  pt: 2,
};

export const dialogActionsSx: SxProps<Theme> = {
  justifyContent: 'center',
  px: 3,
  pb: 3,
};
