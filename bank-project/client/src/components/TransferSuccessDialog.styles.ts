import type { SxProps, Theme } from '@mui/material';

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
  flex: 1,
  fontWeight: 700,
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
