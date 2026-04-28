import type { SxProps, Theme } from '@mui/material';
import type { CSSProperties } from 'react';
import { createFieldSx } from '../../shared/authFormStyles';
import { goldButtonSx } from '../../shared/buttonStyles';

export { createFieldSx };

export const submitButtonSx: SxProps<Theme> = {
  ...goldButtonSx,
  py: 1.5,
  mt: 1,
};

export const signInLinkStyle: CSSProperties = {
  fontWeight: 600,
};
