import type { SxProps, Theme } from '@mui/material';

export const createDividerSx = (isDark: boolean): SxProps<Theme> => ({
  borderColor: isDark ? 'rgba(255,255,255,0.10)' : 'rgba(13,27,42,0.10)',
});

export const createSignInTitleSx = (isDark: boolean): SxProps<Theme> => ({
  fontWeight: 700,
  color: isDark ? 'rgba(255,255,255,0.92)' : '#0D1B2A',
});

export const createAuthCardSx = (isDark: boolean): SxProps<Theme> => ({
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

export const createPageBackgroundSx = (isDark: boolean): SxProps<Theme> => ({
  minHeight: '100vh',
  position: 'relative',
  overflow: 'hidden',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
  py: { xs: 3, sm: 4 },
  px: { xs: 2, sm: 3 },
  background: isDark
    ? [
        'radial-gradient(1200px 700px at 50% 35%, rgba(201,162,39,0.11) 0%, rgba(5,10,15,0) 55%)',
        'linear-gradient(135deg, #050A0F 0%, #0A1520 50%, #050A0F 100%)',
      ].join(', ')
    : [
        'radial-gradient(1200px 700px at 50% 35%, rgba(201,162,39,0.12) 0%, rgba(13,27,42,0) 55%)',
        'linear-gradient(135deg, #0D1B2A 0%, #1B3A4B 50%, #0D1B2A 100%)',
      ].join(', '),
  '&::after': {
    content: '""',
    position: 'absolute',
    inset: 0,
    pointerEvents: 'none',
    backgroundImage: [
      'radial-gradient(circle at 1px 1px, rgba(255,255,255,0.06) 1px, rgba(0,0,0,0) 0)',
      'radial-gradient(1200px 900px at 50% 30%, rgba(0,0,0,0) 40%, rgba(0,0,0,0.45) 100%)',
    ].join(', '),
    backgroundSize: '3px 3px, 100% 100%',
    opacity: isDark ? 0.1 : 0.07,
    mixBlendMode: 'overlay',
  },
});

export const themeToggleSx: SxProps<Theme> = {
  position: 'absolute',
  top: 16,
  right: 16,
  zIndex: 2,
  color: 'rgba(201,162,39,0.95)',
  bgcolor: 'rgba(201, 162, 39, 0.10)',
  border: '1px solid rgba(201,162,39,0.22)',
  backdropFilter: 'blur(10px)',
  '&:hover': { bgcolor: 'rgba(201, 162, 39, 0.18)' },
};