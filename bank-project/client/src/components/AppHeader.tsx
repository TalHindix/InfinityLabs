import { useNavigate } from 'react-router-dom';
import { Container, Stack, Box, Button, IconButton } from '../utils/ui';
import DarkModeIcon from '@mui/icons-material/DarkMode';
import LightModeIcon from '@mui/icons-material/LightMode';
import { DubaiBankLogoFull } from './DubaiBankLogo';
import { useThemeContext } from '../context/ThemeContext';
import { authService } from '../services/auth';
import {
  headerContainerSx,
  logoContainerSx,
  themeToggleSx,
  outlinedButtonSx,
} from './AppHeader.styles';

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

  const handleLogout = () => {
    authService.logout();
    navigate('/login');
  };

  return (
    <Box sx={headerContainerSx}>
      <Container maxWidth="lg">
        <Stack direction="row" justifyContent="space-between" alignItems="center">
          <Box sx={logoContainerSx} onClick={() => navigate('/dashboard')}>
            <DubaiBankLogoFull size={44} animated={false} showTagline={true} />
          </Box>

          <Stack direction="row" spacing={1} alignItems="center">
            {showThemeToggle && (
              <IconButton onClick={toggleTheme} sx={themeToggleSx} aria-label="Toggle theme">
                {isDark ? <LightModeIcon /> : <DarkModeIcon />}
              </IconButton>
            )}

            {showDashboardButton && (
              <Button variant="outlined" onClick={() => navigate('/dashboard')} sx={outlinedButtonSx}>
                Dashboard
              </Button>
            )}

            {showTransactionsButton && (
              <Button variant="outlined" onClick={() => navigate('/transactions')} sx={outlinedButtonSx}>
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
