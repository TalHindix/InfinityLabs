import { useNavigate } from 'react-router-dom';
import { ROUTES } from '../../constants/routePaths';
import {
  Container,
  Typography,
  Button,
  Alert,
  Stack,
  Box,
  Card,
  CardContent,
  CircularProgress,
} from '../../shared/muiExports';
import { AppHeader } from '../../layout/AppHeader';
import { PageFooter } from '../../layout/PageFooter';
import { EmptyState } from '../../components/EmptyState';
import { useTransactions } from './useTransactions';
import { useTransactionDetail } from './useTransactionDetail';
import { TransactionTable } from './TransactionTable';
import { TransactionDetailPanel } from './TransactionDetailPanel';
import {
  pageRootSx,
  containerSx,
  titleSx,
  listColSx,
  detailColSx,
  primaryButtonSx,
} from './TransactionsPage.styles';
import {
  paginationButtonSx,
  pageIndicatorSx,
} from './Pagination.styles';

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
    loading: detailLoading,
    error: detailError,
    loadTransactionDetail,
  } = useTransactionDetail();

  const error = listError || detailError;

  return (
    <Box sx={pageRootSx}>
      <AppHeader showDashboardButton />

      <Container maxWidth="lg" sx={containerSx}>
        <Stack spacing={4}>
          <Stack direction="row" justifyContent="space-between" alignItems="center">
            <Box>
              <Typography variant="h4" sx={titleSx}>
                Transaction History
              </Typography>
              <Typography color="text.secondary">View all your transactions</Typography>
            </Box>

            <Button
              variant="contained"
              onClick={() => navigate(ROUTES.TRANSFER)}
              sx={primaryButtonSx}
            >
              New Transfer
            </Button>
          </Stack>

          {error && <Alert severity="error">{error}</Alert>}

          <Stack direction={{ xs: 'column', md: 'row' }} spacing={3}>
            <Box sx={listColSx}>
              {loading || transactions.length === 0 ? (
                <Card>
                  {loading ? (
                    <CardContent sx={{ py: 8, display: 'flex', justifyContent: 'center' }}>
                      <CircularProgress sx={{ color: 'secondary.main' }} />
                    </CardContent>
                  ) : (
                    <EmptyState title="No transactions found" />
                  )}
                </Card>
              ) : (
                <>
                  <TransactionTable
                    transactions={transactions}
                    userEmail={userEmail}
                    selectedTransactionId={selectedTransaction?._id}
                    onSelect={loadTransactionDetail}
                  />
                  <Stack direction="row" justifyContent="center" spacing={1}>
                    <Button
                      variant="outlined"
                      size="small"
                      disabled={currentPage <= 1}
                      onClick={() => handlePageChange(currentPage - 1)}
                      sx={paginationButtonSx}
                    >
                      Previous
                    </Button>

                    <Box sx={pageIndicatorSx}>
                      <Typography variant="body2">
                        Page {currentPage} of {totalPages}
                      </Typography>
                    </Box>

                    <Button
                      variant="outlined"
                      size="small"
                      disabled={currentPage >= totalPages}
                      onClick={() => handlePageChange(currentPage + 1)}
                      sx={paginationButtonSx}
                    >
                      Next
                    </Button>
                  </Stack>
                </>
              )}
            </Box>

            <Box sx={detailColSx}>
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
