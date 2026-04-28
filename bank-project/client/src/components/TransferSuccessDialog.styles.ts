import type { SxProps, Theme } from '@mui/material';
import { goldButtonSx } from '../shared/buttonStyles';

export const dialogPaperSx: SxProps<Theme> = {
  borderRadius: 3,
  minWidth: 400,
};

export const dialogTitleSx: SxProps<Theme> = {
  fontWeight: 700,
  color: 'text.primary',
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

export const videoCallButtonSx: SxProps<Theme> = {
  ...goldButtonSx,
  flex: 1,
};
