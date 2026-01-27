import type { SxProps, Theme } from '@mui/material';
import { primaryButtonSx } from '../styles/login.styles';

export const submitButtonSx: SxProps<Theme> = {
  ...primaryButtonSx,
  py: 1.5,
  mt: 1,
};
