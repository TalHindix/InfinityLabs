import type { SxProps, Theme } from '@mui/material';

export const chartHeaderSx: SxProps<Theme> = {
  display: 'flex',
  justifyContent: 'space-between',
  alignItems: 'center',
  mb: 2,
};

export const chartTitleSx: SxProps<Theme> = {
  fontWeight: 600,
  fontSize: { xs: '1.1rem', sm: '1.5rem' },
};

export const chartCardSx: SxProps<Theme> = {
  borderRadius: 3,
};

export const chartContainerSx: SxProps<Theme> = {
  width: '100%',
  overflowX: 'auto',
};

export const recipientsSectionSx: SxProps<Theme> = {
  mt: 2,
};

export const recipientRowSx: SxProps<Theme> = {
  display: 'flex',
  justifyContent: 'space-between',
  alignItems: 'center',
  py: 1,
  borderBottom: '1px solid',
  borderColor: 'divider',
};

export const recipientEmailSx: SxProps<Theme> = {
  overflow: 'hidden',
  textOverflow: 'ellipsis',
  whiteSpace: 'nowrap',
  maxWidth: { xs: 180, sm: 'none' },
};

export const recipientAmountSx: SxProps<Theme> = {
  color: 'error.main',
  flexShrink: 0,
  ml: 2,
};
