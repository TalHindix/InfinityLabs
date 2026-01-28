import { Box, Typography } from '../utils/ui';
import { createContainerSx, createDotSx, createTextSx } from './SecurityIndicator.styles';

interface SecurityIndicatorProps {
  isDark: boolean;
}

export const SecurityIndicator = ({ isDark }: SecurityIndicatorProps) => {
  const prefersReducedMotion =
    typeof window !== 'undefined' &&
    window.matchMedia('(prefers-reduced-motion: reduce)').matches;

  return (
    <Box sx={createContainerSx(isDark)}>
      <Box sx={createDotSx(prefersReducedMotion)} />
      <Typography variant="caption" sx={createTextSx(isDark)}>
        Adaptive risk analysis enabled
      </Typography>
    </Box>
  );
};
