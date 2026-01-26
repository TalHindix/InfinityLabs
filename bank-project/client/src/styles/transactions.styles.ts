import type { SxProps, Theme } from '@mui/material';

/**
 * Gold accent color used throughout the app.
 */
export const GOLD_COLOR = '#C9A227';
export const GOLD_HOVER = '#A17F1A';

/**
 * Primary button styles with gold gradient.
 */
export const primaryButtonSx: SxProps<Theme> = {
  background: `linear-gradient(135deg, ${GOLD_COLOR} 0%, #E5C158 100%)`,
  color: '#0D1B2A',
  '&:hover': {
    background: `linear-gradient(135deg, ${GOLD_HOVER} 0%, ${GOLD_COLOR} 100%)`,
  },
};

/**
 * Pagination button styles.
 */
export const paginationButtonSx: SxProps<Theme> = {
  borderColor: GOLD_COLOR,
  color: GOLD_COLOR,
  '&:hover': { borderColor: GOLD_HOVER, bgcolor: 'rgba(201, 162, 39, 0.08)' },
  '&:disabled': { borderColor: 'grey.300', color: 'grey.400' },
};

/**
 * Table row hover styles.
 */
export const createTableRowSx = (isSelected: boolean): SxProps<Theme> => ({
  cursor: 'pointer',
  bgcolor: isSelected ? 'rgba(201, 162, 39, 0.1)' : 'inherit',
  '&:hover': { bgcolor: 'rgba(201, 162, 39, 0.06)' },
});

/**
 * Transaction detail panel card styles.
 */
export const createDetailPanelSx = (hasSelection: boolean): SxProps<Theme> => ({
  position: 'sticky',
  top: 20,
  border: hasSelection ? '1px solid rgba(201, 162, 39, 0.3)' : undefined,
});

/**
 * Creates amount icon container styles based on transaction direction.
 */
export const createAmountIconSx = (isSent: boolean): SxProps<Theme> => ({
  width: 60,
  height: 60,
  mx: 'auto',
  mb: 2,
  borderRadius: '50%',
  background: isSent
    ? 'linear-gradient(135deg, rgba(239, 68, 68, 0.1) 0%, rgba(239, 68, 68, 0.2) 100%)'
    : 'linear-gradient(135deg, rgba(16, 185, 129, 0.1) 0%, rgba(16, 185, 129, 0.2) 100%)',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
});

/**
 * Status badge styles.
 */
export const statusBadgeSx: SxProps<Theme> = {
  display: 'inline-block',
  px: 2,
  py: 0.5,
  borderRadius: 2,
  bgcolor: 'rgba(16, 185, 129, 0.1)',
  color: 'success.main',
};

/**
 * Transfer form card decorative element.
 */
export const transferDecorativeSx: SxProps<Theme> = {
  position: 'absolute',
  top: -30,
  left: '50%',
  transform: 'translateX(-50%)',
  width: 60,
  height: 60,
  borderRadius: '16px',
  background: 'linear-gradient(135deg, #C9A227 0%, #E5C158 100%)',
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
  boxShadow: '0 8px 24px rgba(201, 162, 39, 0.3)',
};

/**
 * Transfer summary box styles.
 */
export const transferSummarySx: SxProps<Theme> = {
  bgcolor: 'rgba(13, 27, 42, 0.03)',
  borderRadius: 2,
  p: 2,
};
