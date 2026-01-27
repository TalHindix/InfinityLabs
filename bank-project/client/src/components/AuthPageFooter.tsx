import { Box, Typography } from './ui';
import {
  sessionNoticeContainerSx,
  lockEmojiSx,
  createSessionTextSx,
  createFooterCaptionSx,
} from './AuthPageFooter.styles';

interface AuthPageFooterProps {
  isDark: boolean;
}

export const SessionProtectionNotice = ({ isDark }: AuthPageFooterProps) => {
  return (
    <Box sx={sessionNoticeContainerSx}>
      <Box component="span" sx={lockEmojiSx}>
        🔒
      </Box>
      <Typography variant="caption" sx={createSessionTextSx(isDark)}>
        Protected session • Auto logout on inactivity
      </Typography>
    </Box>
  );
};

interface PageFooterCaptionProps {
  isDark: boolean;
  variant?: 'login' | 'signup';
}

export const PageFooterCaption = ({ isDark, variant = 'login' }: PageFooterCaptionProps) => {
  const text = variant === 'signup'
    ? 'Secure banking powered by Dubai-Bank'
    : 'Secure banking powered by Dubai Bank';

  return (
    <Typography variant="caption" sx={createFooterCaptionSx(isDark)}>
      {text}
    </Typography>
  );
};
