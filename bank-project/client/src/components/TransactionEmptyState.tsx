import { Card, CardContent, Typography, CircularProgress, Box } from './ui';
import {
  loadingContentSx,
  loadingSpinnerSx,
  emptyIconContainerSx,
  emptyIconTextSx,
} from './TransactionEmptyState.styles';

interface TransactionEmptyStateProps {
  loading: boolean;
}

export const TransactionEmptyState = ({ loading }: TransactionEmptyStateProps) => {
  if (loading) {
    return (
      <Card>
        <CardContent sx={loadingContentSx}>
          <CircularProgress sx={loadingSpinnerSx} />
        </CardContent>
      </Card>
    );
  }

  return (
    <Card>
      <CardContent sx={loadingContentSx}>
        <Box sx={emptyIconContainerSx}>
          <Typography variant="h3" sx={emptyIconTextSx}>$</Typography>
        </Box>
        <Typography color="text.secondary">No transactions found</Typography>
      </CardContent>
    </Card>
  );
};
