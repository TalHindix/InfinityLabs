import { IconButton } from '@mui/material';
import type { SxProps, Theme } from '@mui/material';
import DarkModeIcon from '@mui/icons-material/DarkMode';
import LightModeIcon from '@mui/icons-material/LightMode';
import { useThemeContext } from '../context/ThemeContext';

interface ThemeToggleProps {
  sx?: SxProps<Theme>;
}

/**
 * Reusable theme toggle button.
 * Accepts custom sx props to allow positioning override per page.
 */
export const ThemeToggle = ({ sx }: ThemeToggleProps) => {
  const { isDark, toggleTheme } = useThemeContext();

  return (
    <IconButton
      onClick={toggleTheme}
      aria-label={isDark ? 'Switch to light mode' : 'Switch to dark mode'}
      sx={sx}
    >
      {isDark ? <LightModeIcon /> : <DarkModeIcon />}
    </IconButton>
  );
};
