import { Container, CircularProgress, Alert, Stack, Box, Typography } from '../../shared/muiExports';
import { AppHeader } from '../../layout/AppHeader';
import { PageFooter } from '../../layout/PageFooter';
import { useDashboardData } from './useDashboardData';
import { BalanceCard } from './BalanceCard';
import { RecentTransactions } from './RecentTransactions';
import { SpendingChart } from './SpendingChart';
import {
  pageRootSx,
  containerSx,
  loadingScreenSx,
  loadingSpinnerSx,
  welcomeHeadingSx,
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
            <Typography sx={welcomeHeadingSx}>
              Welcome back, {user?.firstName || 'User'}
            </Typography>
            <Typography color="text.secondary" variant="body2">
              Here's your account overview
            </Typography>
          </Box>

          {error && <Alert severity="error">{error}</Alert>}

          <BalanceCard balance={user?.balance} />

          <SpendingChart />

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
