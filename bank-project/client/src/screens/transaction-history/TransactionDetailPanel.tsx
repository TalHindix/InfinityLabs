import { Card, CardContent, Typography, Stack, Box, Divider, CircularProgress } from '../../shared/muiExports';
import { type Transaction } from '../../types';
import { formatAmount, formatDate } from '../../shared/displayFormatters';
import { createDetailPanelSx, createAmountIconSx, loadingContainerSx, loadingSpinnerSx, goldDividerSx, refTextSx, statusBadgeSx, emptyStateSx, emptyIconSx } from './TransactionDetailPanel.styles';

interface TransactionDetailPanelProps {
  transaction: Transaction | null;
  loading: boolean;
  userEmail: string | undefined;
}

export const TransactionDetailPanel = ({ transaction, loading, userEmail }: TransactionDetailPanelProps) => {
  const isSent = transaction?.fromEmail === userEmail;

  return (
    <Card sx={createDetailPanelSx(!!transaction)}>
      <CardContent>
        {loading ? (
          <Box sx={loadingContainerSx}>
            <CircularProgress size={32} sx={loadingSpinnerSx} />
          </Box>
        ) : transaction ? (
          <Stack spacing={3}>
            <Box textAlign="center">
              <Box sx={createAmountIconSx(isSent)}>
                <Typography variant="h5" sx={{ color: isSent ? 'error.main' : 'success.main' }}>
                  {isSent ? '↑' : '↓'}
                </Typography>
              </Box>

              <Typography variant="h4" fontWeight={700} sx={{ color: 'primary.main' }}>
                {isSent ? '-' : '+'}
                {formatAmount(transaction.amount)} AED
              </Typography>

              <Typography variant="body2" sx={{ color: isSent ? 'error.main' : 'success.main', fontWeight: 500 }}>
                {isSent ? 'Money Sent' : 'Money Received'}
              </Typography>
            </Box>

            <Divider sx={goldDividerSx} />

            <Stack spacing={2}>
              <Box>
                <Typography variant="caption" color="text.secondary">Reference Number</Typography>
                <Typography sx={refTextSx}>#{transaction.id}</Typography>
              </Box>

              <Box>
                <Typography variant="caption" color="text.secondary">Date & Time</Typography>
                <Typography fontWeight={500}>{formatDate(transaction.createdAt)}</Typography>
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
                <Typography variant="caption" color="text.secondary">Transfer reason</Typography>
                <Typography fontWeight={500}>
                  {transaction.description?.trim() || '—'}
                </Typography>
              </Box>

              <Box>
                <Typography variant="caption" color="text.secondary">Status</Typography>
                <Box sx={statusBadgeSx}>
                  <Typography variant="body2" fontWeight={500}>Completed</Typography>
                </Box>
              </Box>
            </Stack>
          </Stack>
        ) : (
          <Box sx={emptyStateSx}>
            <Typography sx={emptyIconSx}>📋</Typography>
            <Typography color="text.secondary">Select a transaction to view details</Typography>
          </Box>
        )}
      </CardContent>
    </Card>
  );
};
