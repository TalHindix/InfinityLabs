import type { SxProps, Theme } from '@mui/material';

export const sessionNoticeContainerSx: SxProps<Theme> = {
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
  gap: 0.75,
  pt: 1,
};

export const lockEmojiSx: SxProps<Theme> = {
  fontSize: '0.75rem',
};

export const createSessionTextSx = (isDark: boolean): SxProps<Theme> => ({
  color: isDark ? 'rgba(255,255,255,0.55)' : 'rgba(13,27,42,0.55)',
});

export const createFooterCaptionSx = (isDark: boolean): SxProps<Theme> => ({
  display: 'block',
  textAlign: 'center',
  mt: 3,
  color: isDark ? 'rgba(255,255,255,0.55)' : 'rgba(255,255,255,0.60)',
});
