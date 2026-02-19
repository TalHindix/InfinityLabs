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
  width: { xs: 340, sm: 400 },
  height: { xs: 460, sm: 520 },
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
  maxWidth: '85%',
  px: 1.5,
  py: 1,
  borderRadius: 2,
  alignSelf: isUser ? 'flex-end' : 'flex-start',
  bgcolor: isUser ? 'primary.main' : 'action.hover',
  color: isUser ? 'white' : 'text.primary',
  whiteSpace: 'pre-line',
  textAlign: 'left',
  fontSize: '0.875rem',
  lineHeight: 1.5,
  wordBreak: 'break-word',
});

export const botMarkdownSx: SxProps<Theme> = {
  '& p': { m: 0, lineHeight: 1.6 },
  '& p + p': { mt: 1 },
  '& ul, & ol': { m: 0, mt: 0.5, mb: 0.5, pl: 2.5 },
  '& ol': { listStyleType: 'decimal' },
  '& li': { mb: 0.5, lineHeight: 1.5 },
  '& li::marker': { fontWeight: 600 },
  '& strong': { fontWeight: 700 },
  '& em': { fontStyle: 'italic' },
  '& code': {
    px: 0.5,
    py: 0.25,
    borderRadius: 0.5,
    bgcolor: 'rgba(0,0,0,0.08)',
    fontFamily: 'monospace',
    fontSize: '0.8em',
  },
  '& pre': {
    m: 0,
    mt: 0.5,
    p: 1,
    borderRadius: 1,
    bgcolor: 'rgba(0,0,0,0.06)',
    overflowX: 'auto',
  },
  '& a': { color: 'primary.main', textDecoration: 'underline' },
  '& h1, & h2, & h3': { m: 0, mt: 0.5, mb: 0.5, fontSize: '0.95rem', fontWeight: 700 },
  '& hr': { my: 1, border: 'none', borderTop: '1px solid', borderColor: 'divider' },
};

export const inputContainerSx: SxProps<Theme> = {
  p: 1.5,
  borderTop: 1,
  borderColor: 'divider',
  display: 'flex',
  gap: 1,
  bgcolor: 'background.paper',
};

export const textFieldSx: SxProps<Theme> = {
  '& .MuiInputBase-root': {
    fontSize: '0.875rem',
  },
  '& textarea': {
    textAlign: 'left',
    overflowWrap: 'break-word',
    wordBreak: 'break-word',
  },
};
