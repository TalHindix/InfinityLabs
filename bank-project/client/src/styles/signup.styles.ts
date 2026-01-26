import type { SxProps, Theme } from '@mui/material';

/**
 * Creates simple page background styles for signup.
 */
export const createSignupBackgroundSx = (isDark: boolean): SxProps<Theme> => ({
  minHeight: '100vh',
  background: isDark
    ? 'linear-gradient(135deg, #050A0F 0%, #0A1520 50%, #050A0F 100%)'
    : 'linear-gradient(135deg, #0D1B2A 0%, #1B3A4B 50%, #0D1B2A 100%)',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
  py: 4,
  position: 'relative',
});

/**
 * Creates card styles for signup page.
 */
export const createSignupCardSx = (isDark: boolean): SxProps<Theme> => ({
  backdropFilter: 'blur(10px)',
  background: isDark ? 'rgba(18, 26, 34, 0.98)' : 'rgba(255, 255, 255, 0.98)',
  border: '1px solid rgba(201, 162, 39, 0.2)',
});

/**
 * Simple theme toggle styles for signup.
 */
export const signupThemeToggleSx: SxProps<Theme> = {
  position: 'absolute',
  top: 16,
  right: 16,
  color: '#C9A227',
  bgcolor: 'rgba(201, 162, 39, 0.1)',
  '&:hover': {
    bgcolor: 'rgba(201, 162, 39, 0.2)',
  },
};

/**
 * Verification success icon container styles.
 */
export const verificationIconSx: SxProps<Theme> = {
  width: 80,
  height: 80,
  mx: 'auto',
  borderRadius: '50%',
  background: 'linear-gradient(135deg, rgba(16, 185, 129, 0.1) 0%, rgba(16, 185, 129, 0.2) 100%)',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
};
