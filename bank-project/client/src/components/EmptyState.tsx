import { Box, Typography } from '../shared/muiExports';
import type { SxProps, Theme } from '@mui/material';

const containerSx: SxProps<Theme> = {
  py: 6,
  textAlign: 'center',
};

const iconSx: SxProps<Theme> = {
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

const iconTextSx: SxProps<Theme> = {
  opacity: 0.3,
};

interface EmptyStateProps {
  icon?: string;
  title: string;
  subtitle?: string;
}

export const EmptyState = ({ icon = '$', title, subtitle }: EmptyStateProps) => (
  <Box sx={containerSx}>
    <Box sx={iconSx}>
      <Typography variant="h3" sx={iconTextSx}>
        {icon}
      </Typography>
    </Box>
    <Typography color="text.secondary">{title}</Typography>
    {subtitle && (
      <Typography variant="body2" color="text.secondary" sx={{ mt: 0.5 }}>
        {subtitle}
      </Typography>
    )}
  </Box>
);
