import { useNavigate } from 'react-router-dom';
import { Container, Stack, Box, Button, IconButton } from './ui';
import DarkModeIcon from '@mui/icons-material/DarkMode';
import LightModeIcon from '@mui/icons-material/LightMode';
import { DubaiBankLogoFull } from './DubaiBankLogo';
import { useThemeContext } from '../context/ThemeContext';
import { authService } from '../services/auth';

interface AppHeaderProps {
  showThemeToggle?: boolean;
  showDashboardButton?: boolean;
  showTransactionsButton?: boolean;
  showLogout?: boolean;
}

/**
 * Reusable app header for authenticated pages.
 * Shows brand logo and navigation buttons.
 */
export const AppHeader = ({
  showThemeToggle = false,
  showDashboardButton = false,
  showTransactionsButton = false,
  showLogout = true,
}: AppHeaderProps) => {
  const navigate = useNavigate();
  const { isDark, toggleTheme } = useThemeContext();

  const handleLogout = () => {
    authService.logout();
    navigate('/login');
  };

  const outlinedButtonSx = {
    borderColor: 'rgba(255, 255, 255, 0.3)',
    color: 'white',
    '&:hover': {
      borderColor: '#C9A227',
      bgcolor: 'rgba(201, 162, 39, 0.1)',
    },
  };

  return (
    <Box
      sx={{
        background: 'linear-gradient(135deg, #0D1B2A 0%, #1B3A4B 100%)',
        color: 'white',
        py: 3,
        px: 2,
      }}
    >
      <Container maxWidth="lg">
        <Stack direction="row" justifyContent="space-between" alignItems="center">
          {/* Brand Logo */}
          <Box
            sx={{
              cursor: 'pointer',
              display: 'flex',
              alignItems: 'center',
            }}
            onClick={() => navigate('/dashboard')}
          >
            <DubaiBankLogoFull
              size={44}
              animated={false}
              showTagline={true}
            />
          </Box>

          {/* Actions */}
          <Stack direction="row" spacing={1} alignItems="center">
            {showThemeToggle && (
              <IconButton
                onClick={toggleTheme}
                sx={{
                  color: '#C9A227',
                  '&:hover': {
                    bgcolor: 'rgba(201, 162, 39, 0.1)',
                  },
                }}
                aria-label="Toggle theme"
              >
                {isDark ? <LightModeIcon /> : <DarkModeIcon />}
              </IconButton>
            )}

            {showDashboardButton && (
              <Button
                variant="outlined"
                onClick={() => navigate('/dashboard')}
                sx={outlinedButtonSx}
              >
                Dashboard
              </Button>
            )}

            {showTransactionsButton && (
              <Button
                variant="outlined"
                onClick={() => navigate('/transactions')}
                sx={outlinedButtonSx}
              >
                Transactions
              </Button>
            )}

            {showLogout && (
              <Button
                variant="outlined"
                onClick={handleLogout}
                sx={outlinedButtonSx}
              >
                Sign Out
              </Button>
            )}
          </Stack>
        </Stack>
      </Container>
    </Box>
  );
};
