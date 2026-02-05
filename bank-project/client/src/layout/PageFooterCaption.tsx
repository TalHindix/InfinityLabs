import { Typography } from '../shared/ui';
import { createFooterCaptionSx } from './PageFooterCaption.styles';

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
