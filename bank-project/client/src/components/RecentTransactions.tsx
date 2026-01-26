import { useNavigate } from 'react-router-dom';
import {
  Card,
  CardContent,
  Typography,
  Button,
  Table,
  TableBody,
  TableCell,
  TableContainer,
  TableHead,
  TableRow,
  Paper,
  Stack,
  Box,
} from './ui';
import { type Transaction } from '../services/transaction';
import { formatAmount } from '../utils/formatters';
import { viewAllButtonSx, emptyIconSx } from '../styles/dashboard.styles';

interface RecentTransactionsProps {
  transactions: Transaction[];
  userEmail: string | undefined;
}

/**
 * Recent transactions section with table or empty state.
 */
export const RecentTransactions = ({ transactions, userEmail }: RecentTransactionsProps) => {
  const navigate = useNavigate();

  return (
    <Box>
      <Stack direction="row" justifyContent="space-between" alignItems="center" mb={2}>
        <Typography variant="h5" sx={{ fontWeight: 600 }}>
          Recent Transactions
        </Typography>
        <Button
          variant="text"
          onClick={() => navigate('/transactions')}
          sx={viewAllButtonSx}
        >
          View All
        </Button>
      </Stack>

      {transactions.length === 0 ? (
        <Card>
          <CardContent sx={{ py: 6, textAlign: 'center' }}>
            <Box sx={emptyIconSx}>
              <Typography variant="h3" sx={{ opacity: 0.3 }}>$</Typography>
            </Box>
            <Typography color="text.secondary">
              No transactions yet
            </Typography>
            <Typography variant="body2" color="text.secondary" sx={{ mt: 0.5 }}>
              Make your first transfer to get started
            </Typography>
          </CardContent>
        </Card>
      ) : (
        <TableContainer component={Paper} sx={{ borderRadius: 3 }}>
          <Table>
            <TableHead>
              <TableRow>
                <TableCell>Date</TableCell>
                <TableCell>Description</TableCell>
                <TableCell align="right">Amount</TableCell>
              </TableRow>
            </TableHead>
            <TableBody>
              {transactions.map((tx) => {
                const isSent = tx.fromEmail === userEmail;
                return (
                  <TableRow key={tx._id}>
                    <TableCell>
                      <Typography variant="body2" fontWeight={500}>
                        {new Date(tx.createdAt).toLocaleDateString('en-AE', {
                          day: 'numeric',
                          month: 'short',
                          year: 'numeric',
                        })}
                      </Typography>
                    </TableCell>
                    <TableCell>
                      <Typography variant="body2">
                        {isSent ? `Sent to ${tx.toEmail}` : `Received from ${tx.fromEmail}`}
                      </Typography>
                    </TableCell>
                    <TableCell align="right">
                      <Typography
                        fontWeight={600}
                        sx={{ color: isSent ? 'error.main' : 'success.main' }}
                      >
                        {isSent ? '-' : '+'}
                        {formatAmount(tx.amount)} AED
                      </Typography>
                    </TableCell>
                  </TableRow>
                );
              })}
            </TableBody>
          </Table>
        </TableContainer>
      )}
    </Box>
  );
};
