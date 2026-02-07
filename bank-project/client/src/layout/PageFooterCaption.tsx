import { Typography } from '../shared/muiExports';
import { createFooterCaptionSx } from './pageFooterCaptionStyles';

interface PageFooterCaptionProps {
  isDark: boolean;
  variant?: 'login' | 'signup';
}

export const PageFooterCaption = ({ isDark, variant = 'login' }: PageFooterCaptionProps) => {
  const text =
    variant === 'signup'
      ? 'Secure banking powered by Dubai-Bank'
      : 'Secure banking powered by Dubai Bank';

  return (
    <Typography variant="caption" sx={createFooterCaptionSx(isDark)}>
      {text}
    </Typography>
  );
};
