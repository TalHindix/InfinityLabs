import { Container, CircularProgress, Alert, Stack, Box, Typography } from '../utils/ui';
import { AppHeader } from '../components/AppHeader';
import { PageFooter } from '../components/PageFooter';
import { useDashboardData } from '../hooks/useDashboardData';
import { BalanceCard } from '../components/BalanceCard';
import { RecentTransactions } from '../components/RecentTransactions';
import {
  pageRootSx,
  containerSx,
  loadingScreenSx,
  loadingSpinnerSx,
} from './DashboardPage.styles';

const DashboardPage = () => {
  const { user, transactions, loading, error } = useDashboardData();

  if (loading) {
    return (
      <Box sx={loadingScreenSx}>
        <CircularProgress sx={loadingSpinnerSx} />
      </Box>
    );
  }

  return (
    <Box sx={pageRootSx}>
      <AppHeader showThemeToggle showTransactionsButton />

      <Container maxWidth="lg" sx={containerSx}>
        <Stack spacing={4}>
          <Box>
            <Typography variant="h4">
              Welcome back, {user?.firstName || 'User'}
            </Typography>
            <Typography color="text.secondary">
              Here's your account overview
            </Typography>
          </Box>

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
