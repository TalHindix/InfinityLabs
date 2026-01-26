import {
  Container,
  CircularProgress,
  Alert,
  Stack,
  Box,
} from '../components/ui';
import { AppHeader } from '../components/AppHeader';
import { PageFooter } from '../components/PageFooter';
import { useDashboardData } from '../hooks/useDashboardData';
import { WelcomeSection } from '../components/WelcomeSection';
import { BalanceCard } from '../components/BalanceCard';
import { RecentTransactions } from '../components/RecentTransactions';
import { loadingScreenSx } from '../styles/dashboard.styles';

const DashboardPage = () => {
  const { user, transactions, loading, error } = useDashboardData();

  if (loading) {
    return (
      <Box sx={loadingScreenSx}>
        <CircularProgress sx={{ color: '#C9A227' }} />
      </Box>
    );
  }

  return (
    <Box sx={{ minHeight: '100vh', bgcolor: 'background.default' }}>
      <AppHeader showThemeToggle />

      <Container maxWidth="lg" sx={{ py: 4 }}>
        <Stack spacing={4}>
          <WelcomeSection firstName={user?.firstName} />

          {error && <Alert severity="error">{error}</Alert>}

          <BalanceCard balance={user?.balance} />

          <RecentTransactions
            transactions={transactions}
            userEmail={user?.email}
          />
        </Stack>
      </Container>

      <PageFooter />
    </Box>
  );
};

export default DashboardPage;
