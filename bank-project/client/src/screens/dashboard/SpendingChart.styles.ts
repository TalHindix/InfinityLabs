import type { SxProps, Theme } from '@mui/material';

const NAVY = '#0d1526';
const NAVY_LIGHT = '#131d33';
const GOLD = '#c9a227';
const RED = '#ef4444';
const GREEN = '#22c55e';

export const dashboardRootSx: SxProps<Theme> = {
  backgroundColor: NAVY,
  borderRadius: 3,
  p: { xs: 2, sm: 3 },
};

export const headerRowSx: SxProps<Theme> = {
  display: 'flex',
  justifyContent: 'space-between',
  alignItems: { xs: 'flex-start', sm: 'center' },
  flexDirection: { xs: 'column', sm: 'row' },
  gap: 2,
  mb: 3,
};

export const titleSx: SxProps<Theme> = {
  fontWeight: 700,
  fontSize: { xs: '1.1rem', sm: '1.4rem' },
  color: '#fff',
};

export const filterGroupSx: SxProps<Theme> = {
  display: 'flex',
  gap: 1,
};

export const filterBtnSx = (active: boolean): SxProps<Theme> => ({
  minWidth: 44,
  px: 2,
  py: 0.5,
  borderRadius: 2,
  fontSize: '0.8rem',
  fontWeight: 600,
  textTransform: 'none',
  color: active ? NAVY : 'rgba(255,255,255,0.6)',
  backgroundColor: active ? GOLD : 'rgba(255,255,255,0.08)',
  '&:hover': {
    backgroundColor: active ? GOLD : 'rgba(255,255,255,0.14)',
  },
});

export const summaryGridSx: SxProps<Theme> = {
  display: 'grid',
  gridTemplateColumns: { xs: '1fr', sm: 'repeat(3, 1fr)' },
  gap: 2,
  mb: 3,
};

export const summaryCardSx = (accentColor: string): SxProps<Theme> => ({
  backgroundColor: NAVY_LIGHT,
  borderRadius: 2,
  p: 2,
  borderLeft: `3px solid ${accentColor}`,
});

export const summaryLabelSx: SxProps<Theme> = {
  color: 'rgba(255,255,255,0.5)',
  fontSize: '0.75rem',
  fontWeight: 500,
  textTransform: 'uppercase',
  letterSpacing: 0.5,
  mb: 0.5,
};

export const summaryValueSx = (color: string): SxProps<Theme> => ({
  color,
  fontWeight: 700,
  fontSize: { xs: '1.2rem', sm: '1.5rem' },
});

export const chartSectionSx: SxProps<Theme> = {
  backgroundColor: NAVY_LIGHT,
  borderRadius: 2,
  p: { xs: 1.5, sm: 2.5 },
  mb: 3,
};

export const chartTitleSx: SxProps<Theme> = {
  color: '#fff',
  fontWeight: 600,
  fontSize: '1rem',
  mb: 2,
};

export const legendRowSx: SxProps<Theme> = {
  display: 'flex',
  gap: 3,
  mb: 2,
};

export const legendItemSx: SxProps<Theme> = {
  display: 'flex',
  alignItems: 'center',
  gap: 0.75,
};

export const legendDotSx = (color: string): SxProps<Theme> => ({
  width: 10,
  height: 10,
  borderRadius: '50%',
  backgroundColor: color,
  flexShrink: 0,
});

export const legendTextSx: SxProps<Theme> = {
  color: 'rgba(255,255,255,0.6)',
  fontSize: '0.8rem',
};

export const recipientsSectionSx: SxProps<Theme> = {
  backgroundColor: NAVY_LIGHT,
  borderRadius: 2,
  p: { xs: 1.5, sm: 2.5 },
};

export const recipientsTitleSx: SxProps<Theme> = {
  color: '#fff',
  fontWeight: 600,
  fontSize: '1rem',
  mb: 2,
};

export const recipientRowSx: SxProps<Theme> = {
  display: 'flex',
  alignItems: 'center',
  gap: { xs: 1, sm: 2 },
  py: 1.5,
  '&:not(:last-child)': {
    borderBottom: '1px solid rgba(255,255,255,0.06)',
  },
};

export const rankSx: SxProps<Theme> = {
  color: 'rgba(255,255,255,0.3)',
  fontWeight: 700,
  fontSize: '0.85rem',
  width: 20,
  textAlign: 'center',
  flexShrink: 0,
};

export const avatarSx: SxProps<Theme> = {
  width: 36,
  height: 36,
  borderRadius: '50%',
  backgroundColor: 'rgba(201,162,39,0.15)',
  color: GOLD,
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
  fontWeight: 700,
  fontSize: '0.8rem',
  flexShrink: 0,
};

export const recipientInfoSx: SxProps<Theme> = {
  flex: 1,
  minWidth: 0,
};

export const recipientEmailSx: SxProps<Theme> = {
  color: '#fff',
  fontSize: '0.85rem',
  fontWeight: 500,
  overflow: 'hidden',
  textOverflow: 'ellipsis',
  whiteSpace: 'nowrap',
};

export const recipientCountSx: SxProps<Theme> = {
  color: 'rgba(255,255,255,0.4)',
  fontSize: '0.75rem',
};

export const progressBarContainerSx: SxProps<Theme> = {
  width: { xs: 60, sm: 100 },
  height: 6,
  borderRadius: 3,
  backgroundColor: 'rgba(255,255,255,0.08)',
  overflow: 'hidden',
  flexShrink: 0,
};

export const progressBarFillSx = (widthPercent: number): SxProps<Theme> => ({
  width: `${widthPercent}%`,
  height: '100%',
  borderRadius: 3,
  backgroundColor: RED,
});

export const recipientAmountSx: SxProps<Theme> = {
  color: RED,
  fontWeight: 700,
  fontSize: '0.85rem',
  flexShrink: 0,
  textAlign: 'right',
  minWidth: { xs: 70, sm: 90 },
};

export const loadingContainerSx: SxProps<Theme> = {
  backgroundColor: NAVY,
  borderRadius: 3,
  display: 'flex',
  justifyContent: 'center',
  py: 8,
};

export const emptyTextSx: SxProps<Theme> = {
  color: 'rgba(255,255,255,0.4)',
  textAlign: 'center',
  py: 4,
};

export const COLORS = { NAVY, NAVY_LIGHT, GOLD, RED, GREEN };
