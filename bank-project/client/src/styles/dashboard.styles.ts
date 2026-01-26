import type { SxProps, Theme } from '@mui/material';

/**
 * Balance card container styles.
 */
export const balanceCardSx: SxProps<Theme> = {
  background: 'linear-gradient(135deg, #0D1B2A 0%, #1B3A4B 100%)',
  color: 'white',
  overflow: 'visible',
  position: 'relative',
};

/**
 * Balance card decorative "AED" badge.
 */
export const balanceBadgeSx: SxProps<Theme> = {
  position: 'absolute',
  top: -20,
  right: 30,
  width: 80,
  height: 80,
  borderRadius: '20px',
  background: 'linear-gradient(135deg, #C9A227 0%, #E5C158 100%)',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
  boxShadow: '0 8px 32px rgba(201, 162, 39, 0.4)',
};

/**
 * Primary button styles with gold gradient.
 */
export const primaryButtonSx: SxProps<Theme> = {
  background: 'linear-gradient(135deg, #C9A227 0%, #E5C158 100%)',
  color: '#0D1B2A',
  px: 4,
  py: 1.5,
  '&:hover': {
    background: 'linear-gradient(135deg, #A17F1A 0%, #C9A227 100%)',
  },
};

/**
 * View all link button styles.
 */
export const viewAllButtonSx: SxProps<Theme> = {
  color: '#C9A227',
  '&:hover': { bgcolor: 'rgba(201, 162, 39, 0.08)' },
};

/**
 * Empty state icon container.
 */
export const emptyIconSx: SxProps<Theme> = {
  width: 80,
  height: 80,
  mx: 'auto',
  mb: 2,
  borderRadius: '50%',
  bgcolor: 'rgba(13, 27, 42, 0.05)',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
};

/**
 * Loading screen styles.
 */
export const loadingScreenSx: SxProps<Theme> = {
  minHeight: '100vh',
  background: 'linear-gradient(180deg, #0D1B2A 0%, #1B3A4B 100%)',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
};
