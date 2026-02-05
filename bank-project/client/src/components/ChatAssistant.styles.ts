import type { SxProps, Theme } from '@mui/material';

export const fabSx: SxProps<Theme> = {
  position: 'fixed',
  bottom: 20,
  right: 20,
};

export const windowSx: SxProps<Theme> = {
  position: 'fixed',
  bottom: 20,
  right: 20,
  width: 320,
  height: 400,
  display: 'flex',
  flexDirection: 'column',
  borderRadius: 3,
  overflow: 'hidden',
  direction: 'ltr',
  bgcolor: 'background.paper',
};

export const headerSx: SxProps<Theme> = {
  bgcolor: 'primary.main',
  color: 'white',
  p: 1.5,
  display: 'flex',
  justifyContent: 'space-between',
  alignItems: 'center',
};

export const closeButtonSx: SxProps<Theme> = {
  color: 'white',
};

export const messagesContainerSx: SxProps<Theme> = {
  flex: 1,
  p: 2,
  overflowY: 'auto',
  display: 'flex',
  flexDirection: 'column',
  gap: 1,
  bgcolor: 'background.paper',
};

export const createMessageSx = (isUser: boolean): SxProps<Theme> => ({
  maxWidth: '80%',
  p: 1.5,
  borderRadius: 2,
  alignSelf: isUser ? 'flex-end' : 'flex-start',
  bgcolor: isUser ? 'primary.main' : 'action.hover',
  color: isUser ? 'white' : 'text.primary',
  whiteSpace: 'pre-line',
  textAlign: 'left',
});

export const inputContainerSx: SxProps<Theme> = {
  p: 1.5,
  borderTop: 1,
  borderColor: 'divider',
  display: 'flex',
  gap: 1,
  bgcolor: 'background.paper',
};

export const textFieldSx: SxProps<Theme> = {
  '& .MuiInputBase-input': {
    textAlign: 'left',
  },
};
