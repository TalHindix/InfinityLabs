import { useNavigate } from 'react-router-dom';
import { ROUTES } from '../constants/routes';
import { Container, Stack, Box, Button, IconButton } from '../shared/ui';
import DarkModeIcon from '@mui/icons-material/DarkMode';
import LightModeIcon from '@mui/icons-material/LightMode';
import { DubaiBankLogoFull } from '../components/DubaiBankLogo';
import { useThemeContext } from '../context/ThemeContext';
import { authStorage } from '../api/auth.storage';
import { authService } from '../api/auth.service';
import { headerContainerSx, logoContainerSx, themeToggleSx, outlinedButtonSx,}
from './AppHeader.styles';

interface AppHeaderProps {
  showThemeToggle?: boolean;
  showDashboardButton?: boolean;
  showTransactionsButton?: boolean;
  showLogout?: boolean;
}

export const AppHeader = ({
  showThemeToggle = false,
  showDashboardButton = false,
  showTransactionsButton = false,
  showLogout = true,
}: AppHeaderProps) => {
  const navigate = useNavigate();
  const { isDark, toggleTheme } = useThemeContext();

  const handleLogout = async () => {
    try {
      await authService.logout();
    } finally {
      authStorage.clearAuth();
      navigate(ROUTES.LOGIN);
    }
  };

  return (
    <Box sx={headerContainerSx}>
      <Container maxWidth="lg">
        <Stack direction="row" justifyContent="space-between" alignItems="center">
          <Box sx={logoContainerSx} onClick={() => navigate(ROUTES.DASHBOARD)}>
            <DubaiBankLogoFull size={44} animated={false} showTagline={true} />
          </Box>

          <Stack direction="row" spacing={1} alignItems="center">
            {showThemeToggle && (
              <IconButton onClick={toggleTheme} sx={themeToggleSx} aria-label="Toggle theme">
                {isDark ? <LightModeIcon /> : <DarkModeIcon />}
              </IconButton>
            )}

            {showDashboardButton && (
              <Button variant="outlined" onClick={() => navigate(ROUTES.DASHBOARD)} sx={outlinedButtonSx}>
                Dashboard
              </Button>
            )}

            {showTransactionsButton && (
              <Button variant="outlined" onClick={() => navigate(ROUTES.TRANSACTIONS)} sx={outlinedButtonSx}>
                Transactions
              </Button>
            )}

            {showLogout && (
              <Button variant="outlined" onClick={handleLogout} sx={outlinedButtonSx}>
                Sign Out
              </Button>
            )}
          </Stack>
        </Stack>
      </Container>
    </Box>
  );
};
