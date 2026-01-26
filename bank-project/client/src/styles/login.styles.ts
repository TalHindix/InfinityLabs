import type { SxProps, Theme } from '@mui/material';

/**
 * Creates text field styles based on theme mode.
 */
export const createFieldSx = (isDark: boolean): SxProps<Theme> => ({
  '& .MuiInputLabel-root': {
    color: isDark ? 'rgba(255,255,255,0.70)' : 'rgba(13,27,42,0.70)',
  },
  '& .MuiInputLabel-root.Mui-focused': {
    color: 'rgba(201, 162, 39, 0.95)',
  },
  '& .MuiOutlinedInput-root': {
    backgroundColor: isDark ? 'rgba(255,255,255,0.04)' : 'rgba(13,27,42,0.03)',
    borderRadius: 12,
    transition: 'box-shadow 160ms ease, border-color 160ms ease, background-color 160ms ease',
    '& fieldset': {
      borderColor: isDark ? 'rgba(255,255,255,0.12)' : 'rgba(13,27,42,0.14)',
    },
    '&:hover fieldset': {
      borderColor: 'rgba(201, 162, 39, 0.45)',
    },
    '&.Mui-focused fieldset': {
      borderColor: 'rgba(201, 162, 39, 0.85)',
    },
    '&.Mui-focused': {
      boxShadow: '0 0 0 4px rgba(201,162,39,0.16)',
    },
    '& input': {
      color: isDark ? 'rgba(255,255,255,0.92)' : '#0D1B2A',
    },
  },
  '& .MuiFormHelperText-root': {
    color: isDark ? 'rgba(255,255,255,0.65)' : 'rgba(13,27,42,0.65)',
  },
  // Autofill fix - must be at root level
  '& input:-webkit-autofill, & input:-webkit-autofill:hover, & input:-webkit-autofill:focus, & input:-webkit-autofill:active': {
    WebkitBoxShadow: isDark 
      ? '0 0 0 1000px #0a1520 inset !important' 
      : '0 0 0 1000px #f5f5f5 inset !important',
    WebkitTextFillColor: isDark ? '#fff !important' : '#0D1B2A !important',
    caretColor: isDark ? '#fff' : '#0D1B2A',
    transition: 'background-color 5000s ease-in-out 0s',
  },
});

/**
 * Creates security indicator container styles.
 */
export const createSecurityIndicatorSx = (isDark: boolean): SxProps<Theme> => ({
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
  gap: 1,
  py: 1,
  px: 2,
  borderRadius: '8px',
  backgroundColor: isDark ? 'rgba(16, 185, 129, 0.08)' : 'rgba(16, 185, 129, 0.06)',
  border: `1px solid ${isDark ? 'rgba(16, 185, 129, 0.15)' : 'rgba(16, 185, 129, 0.12)'}`,
});

/**
 * Creates page background styles.
 */
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

/**
 * Creates card styles for auth pages.
 */
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

/**
 * Theme toggle button styles for auth pages.
 */
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

/**
 * Primary button styles (gold gradient).
 */
export const primaryButtonSx: SxProps<Theme> = {
  py: 1.45,
  borderRadius: 12,
  fontSize: '1rem',
  fontWeight: 700,
  letterSpacing: '0.01em',
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
  '&.Mui-disabled': {
    color: 'rgba(13,27,42,0.75)',
    background: 'linear-gradient(135deg, rgba(201,162,39,0.65) 0%, rgba(229,193,88,0.65) 100%)',
  },
};
