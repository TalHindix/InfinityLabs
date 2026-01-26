import { useNavigate } from 'react-router-dom';
import {
  Container,
  Typography,
  Button,
  Alert,
  Stack,
  Box,
} from '../components/ui';
import { AppHeader } from '../components/AppHeader';
import { PageFooter } from '../components/PageFooter';
import { useTransactions } from '../hooks/useTransactions';
import { useTransactionDetail } from '../hooks/useTransactionDetail';
import { TransactionTable } from '../components/TransactionTable';
import { TransactionDetailPanel } from '../components/TransactionDetailPanel';
import { TransactionEmptyState } from '../components/TransactionEmptyState';
import { Pagination } from '../components/Pagination';
import { primaryButtonSx } from '../styles/transactions.styles';

const TransactionsPage = () => {
  const navigate = useNavigate();

  const {
    transactions,
    loading,
    error: listError,
    totalPages,
    currentPage,
    userEmail,
    handlePageChange,
  } = useTransactions();

  const {
    selectedTransaction,
    detailLoading,
    error: detailError,
    loadTransactionDetail,
  } = useTransactionDetail();

  const error = listError || detailError;

  return (
    <Box sx={{ minHeight: '100vh', bgcolor: 'background.default' }}>
      <AppHeader showDashboardButton />

      <Container maxWidth="lg" sx={{ py: 4 }}>
        <Stack spacing={4}>
          {/* Page Header */}
          <Stack direction="row" justifyContent="space-between" alignItems="center">
            <Box>
              <Typography variant="h4" sx={{ fontWeight: 600, color: 'primary.main' }}>
                Transaction History
              </Typography>
              <Typography color="text.secondary">
                View all your transactions
              </Typography>
            </Box>
            <Button
              variant="contained"
              onClick={() => navigate('/transfer')}
              sx={primaryButtonSx}
            >
              New Transfer
            </Button>
          </Stack>

          {error && <Alert severity="error">{error}</Alert>}

          <Stack direction={{ xs: 'column', md: 'row' }} spacing={3}>
            {/* Transactions List */}
            <Box sx={{ flex: 2 }}>
              {loading || transactions.length === 0 ? (
                <TransactionEmptyState loading={loading} />
              ) : (
                <>
                  <TransactionTable
                    transactions={transactions}
                    userEmail={userEmail}
                    selectedTransactionId={selectedTransaction?._id}
                    onSelectTransaction={loadTransactionDetail}
                  />
                  <Pagination
                    currentPage={currentPage}
                    totalPages={totalPages}
                    onPageChange={handlePageChange}
                  />
                </>
              )}
            </Box>

            {/* Transaction Detail Panel */}
            <Box sx={{ flex: 1, minWidth: 300 }}>
              <TransactionDetailPanel
                transaction={selectedTransaction}
                loading={detailLoading}
                userEmail={userEmail}
              />
            </Box>
          </Stack>
        </Stack>
      </Container>

      <PageFooter />
    </Box>
  );
};

export default TransactionsPage;
