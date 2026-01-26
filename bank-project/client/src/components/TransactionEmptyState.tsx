import { Card, CardContent, Typography, CircularProgress, Box } from './ui';

interface TransactionEmptyStateProps {
  loading: boolean;
}

/**
 * Empty state display for transactions list.
 * Shows loading spinner or "no transactions" message.
 */
export const TransactionEmptyState = ({ loading }: TransactionEmptyStateProps) => {
  if (loading) {
    return (
      <Card>
        <CardContent sx={{ py: 8, textAlign: 'center' }}>
          <CircularProgress sx={{ color: '#C9A227' }} />
        </CardContent>
      </Card>
    );
  }

  return (
    <Card>
      <CardContent sx={{ py: 8, textAlign: 'center' }}>
        <Box
          sx={{
            width: 80,
            height: 80,
            mx: 'auto',
            mb: 2,
            borderRadius: '50%',
            bgcolor: 'rgba(13, 27, 42, 0.05)',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'center',
          }}
        >
          <Typography variant="h3" sx={{ opacity: 0.3 }}>$</Typography>
        </Box>
        <Typography color="text.secondary">No transactions found</Typography>
      </CardContent>
    </Card>
  );
};
