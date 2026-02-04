import type { SxProps, Theme } from '@mui/material';

export const createFooterCaptionSx = (isDark: boolean): SxProps<Theme> => ({
  display: 'block',
  textAlign: 'center',
  mt: 3,
  color: isDark ? 'rgba(255,255,255,0.55)' : 'rgba(255,255,255,0.60)',
});
