import { Card, CardContent, Typography, CircularProgress, Box } from '../utils/ui';
import {
  contentSx,
  spinnerSx,
  emptyIconContainerSx,
  emptyIconTextSx,
  emptyTextSx,
} from './TransactionEmptyState.styles';

interface TransactionEmptyStateProps {
  loading: boolean;
}

export const TransactionEmptyState = ({ loading }: TransactionEmptyStateProps) => {
  return (
    <Card>
      <CardContent sx={contentSx}>
        {loading ? (
          <CircularProgress sx={spinnerSx} />
        ) : (
          <>
            <Box sx={emptyIconContainerSx}>
              <Typography variant="h3" sx={emptyIconTextSx}>
                $
              </Typography>
            </Box>

            <Typography sx={emptyTextSx}>
              No transactions found
            </Typography>
          </>
        )}
      </CardContent>
    </Card>
  );
};
