import {
  Card,
  CardContent,
  Typography,
  Stack,
  Box,
  Divider,
  CircularProgress,
} from './ui';
import { type Transaction } from '../services/transaction';
import { formatAmount, formatDate } from '../utils/formatters';
import {
  createDetailPanelSx,
  createAmountIconSx,
  statusBadgeSx,
  GOLD_COLOR,
} from '../styles/transactions.styles';

interface TransactionDetailPanelProps {
  transaction: Transaction | null;
  loading: boolean;
  userEmail: string | undefined;
}

/**
 * Detailed view panel for a selected transaction.
 */
export const TransactionDetailPanel = ({
  transaction,
  loading,
  userEmail,
}: TransactionDetailPanelProps) => {
  const isSent = transaction?.fromEmail === userEmail;

  return (
    <Card sx={createDetailPanelSx(!!transaction)}>
      <CardContent>
        {loading ? (
          <Box sx={{ py: 4, textAlign: 'center' }}>
            <CircularProgress size={32} sx={{ color: GOLD_COLOR }} />
          </Box>
        ) : transaction ? (
          <Stack spacing={3}>
            <Box textAlign="center">
              <Box sx={createAmountIconSx(isSent)}>
                <Typography
                  variant="h5"
                  sx={{ color: isSent ? 'error.main' : 'success.main' }}
                >
                  {isSent ? '↑' : '↓'}
                </Typography>
              </Box>
              <Typography variant="h4" fontWeight={700} sx={{ color: 'primary.main' }}>
                {isSent ? '-' : '+'}
                {formatAmount(transaction.amount)} AED
              </Typography>
              <Typography
                variant="body2"
                sx={{
                  color: isSent ? 'error.main' : 'success.main',
                  fontWeight: 500,
                }}
              >
                {isSent ? 'Money Sent' : 'Money Received'}
              </Typography>
            </Box>

            <Divider sx={{ borderColor: 'rgba(201, 162, 39, 0.2)' }} />

            <Stack spacing={2}>
              <Box>
                <Typography variant="caption" color="text.secondary">
                  Reference Number
                </Typography>
                <Typography fontWeight={600} sx={{ color: GOLD_COLOR }}>
                  #{transaction.id}
                </Typography>
              </Box>

              <Box>
                <Typography variant="caption" color="text.secondary">
                  Date & Time
                </Typography>
                <Typography fontWeight={500}>
                  {formatDate(transaction.createdAt)}
                </Typography>
              </Box>

              <Box>
                <Typography variant="caption" color="text.secondary">
                  {isSent ? 'Sent To' : 'Received From'}
                </Typography>
                <Typography fontWeight={500}>
                  {isSent ? transaction.toEmail : transaction.fromEmail}
                </Typography>
              </Box>

              <Box>
                <Typography variant="caption" color="text.secondary">
                  Status
                </Typography>
                <Box sx={statusBadgeSx}>
                  <Typography variant="body2" fontWeight={600}>
                    Completed
                  </Typography>
                </Box>
              </Box>
            </Stack>
          </Stack>
        ) : (
          <Box sx={{ py: 6, textAlign: 'center' }}>
            <Typography variant="h6" color="text.secondary" sx={{ mb: 1 }}>
              Transaction Details
            </Typography>
            <Typography variant="body2" color="text.secondary">
              Select a transaction to view details
            </Typography>
          </Box>
        )}
      </CardContent>
    </Card>
  );
};
