import type { SxProps, Theme } from '@mui/material';

export const goldDividerSx: SxProps<Theme> = {
  borderColor: 'rgba(201, 162, 39, 0.3)',
};

export const createSignupCardSx = (isDark: boolean): SxProps<Theme> => ({
  position: 'relative',
  overflow: 'hidden',
  borderRadius: '20px',
  backdropFilter: 'blur(14px)',
  background: isDark ? 'rgba(18, 26, 34, 0.70)' : 'rgba(255, 255, 255, 0.78)',
  border: isDark
    ? '1px solid rgba(255,255,255,0.10)'
    : '1px solid rgba(13,27,42,0.10)',
  boxShadow: isDark
    ? '0 30px 80px rgba(0,0,0,0.55)'
    : '0 30px 80px rgba(13,27,42,0.25)',
  '&::before': {
    content: '""',
    position: 'absolute',
    inset: 0,
    pointerEvents: 'none',
    background:
      'linear-gradient(180deg, rgba(255,255,255,0.10) 0%, rgba(255,255,255,0.00) 38%)',
    opacity: isDark ? 1 : 0.75,
  },
});
