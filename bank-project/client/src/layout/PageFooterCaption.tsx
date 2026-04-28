import { Typography } from '../shared/muiExports';
import { createFooterCaptionSx } from './PageFooterCaption.styles';

interface PageFooterCaptionProps {
  isDark: boolean;
}

export const PageFooterCaption = ({ isDark }: PageFooterCaptionProps) => (
  <Typography variant="caption" sx={createFooterCaptionSx(isDark)}>
    Secure banking powered by Dubai Bank
  </Typography>
);
