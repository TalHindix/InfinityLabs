import { useNavigate } from 'react-router-dom';
import { ROUTES } from '../constants/routePaths';
import { Container, Stack, Box, Button, IconButton } from '../shared/muiExports';
import DarkModeIcon from '@mui/icons-material/DarkMode';
import LightModeIcon from '@mui/icons-material/LightMode';
import HomeIcon from '@mui/icons-material/Home';
import ReceiptLongIcon from '@mui/icons-material/ReceiptLong';
import LogoutIcon from '@mui/icons-material/Logout';
import { DubaiBankLogoFull } from '../components/DubaiBankLogo';
import { useThemeContext } from '../context/ThemeContext';
import { authStorage } from '../api/auth.storage';
import { authService } from '../api/auth.service';
import { headerContainerSx, logoContainerSx, themeToggleSx, outlinedButtonSx, mobileIconButtonSx } from './AppHeader.styles';

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
          {/* Desktop: full logo with tagline; Mobile: compact logo */}
          <Box sx={{ ...logoContainerSx, display: { xs: 'none', sm: 'flex' } }} onClick={() => navigate(ROUTES.DASHBOARD)}>
            <DubaiBankLogoFull size={44} animated={false} showTagline={true} />
          </Box>
          <Box sx={{ ...logoContainerSx, display: { xs: 'flex', sm: 'none' } }} onClick={() => navigate(ROUTES.DASHBOARD)}>
            <DubaiBankLogoFull size={32} animated={false} showTagline={false} />
          </Box>

          <Stack direction="row" spacing={{ xs: 0.5, sm: 1 }} alignItems="center">
            {showThemeToggle && (
              <IconButton onClick={toggleTheme} sx={themeToggleSx} aria-label="Toggle theme">
                {isDark ? <LightModeIcon /> : <DarkModeIcon />}
              </IconButton>
            )}

            {showDashboardButton && (
              <>
                <Button variant="outlined" onClick={() => navigate(ROUTES.DASHBOARD)} sx={outlinedButtonSx}>
                  Dashboard
                </Button>
                <IconButton onClick={() => navigate(ROUTES.DASHBOARD)} sx={mobileIconButtonSx} aria-label="Dashboard">
                  <HomeIcon />
                </IconButton>
              </>
            )}

            {showTransactionsButton && (
              <>
                <Button variant="outlined" onClick={() => navigate(ROUTES.TRANSACTIONS)} sx={outlinedButtonSx}>
                  Transactions
                </Button>
                <IconButton onClick={() => navigate(ROUTES.TRANSACTIONS)} sx={mobileIconButtonSx} aria-label="Transactions">
                  <ReceiptLongIcon />
                </IconButton>
              </>
            )}

            {showLogout && (
              <>
                <Button variant="outlined" onClick={handleLogout} sx={outlinedButtonSx}>
                  Sign Out
                </Button>
                <IconButton onClick={handleLogout} sx={mobileIconButtonSx} aria-label="Sign out">
                  <LogoutIcon />
                </IconButton>
              </>
            )}
          </Stack>
        </Stack>
      </Container>
    </Box>
  );
};
