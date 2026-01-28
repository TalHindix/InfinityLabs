import { Box, Typography, Stack } from '../utils/ui';
import { DubaiBankLogo } from './DubaiBankLogo';

interface BrandLogoProps {
  size?: 'small' | 'medium' | 'large';
  showText?: boolean;
  showSubtitle?: boolean;
  animated?: boolean;
}

const SIZES = {
  small: { logoSize: 36, variant: 'h6' as const },
  medium: { logoSize: 44, variant: 'h5' as const },
  large: { logoSize: 56, variant: 'h4' as const },
};

/**
 * Reusable brand logo component with animated AI logo.
 * Shows the logo with optional text and subtitle.
 */
export const BrandLogo = ({ 
  size = 'medium', 
  showText = false, 
  showSubtitle = false,
  animated = true 
}: BrandLogoProps) => {
  const { logoSize, variant } = SIZES[size];

  const logo = <DubaiBankLogo size={logoSize} animated={animated} />;

  if (!showText) return logo;

  return (
    <Stack direction="row" alignItems="center" spacing={1.5}>
      {logo}
      <Box>
        <Typography variant={variant} sx={{ fontWeight: 700, color: '#C9A227' }}>
          Dubai Bank
        </Typography>
        {showSubtitle && (
          <Typography 
            variant="caption" 
            sx={{ 
              opacity: 0.6, 
              letterSpacing: '1px',
              textTransform: 'uppercase',
              fontSize: '0.65rem'
            }}
          >
            Think Forward
          </Typography>
        )}
      </Box>
    </Stack>
  );
};
