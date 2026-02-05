import { useNavigate } from 'react-router-dom';
import { ROUTES } from '../../constants/routes';
import {Card,CardContent,Typography,Button, Table, TableBody, TableCell, TableContainer, TableHead, TableRow, Paper, Box,}
from '../../shared/ui';
import { type Transaction } from '../../types';
import { formatAmount } from '../../shared/formatters';
import { headerBarSx, sectionTitleSx, viewAllButtonSx, emptyCardContentSx, emptyIconSx, emptyIconTextSx,
          emptySubtextSx, tableContainerSx, amountCellSx, createAmountTextSx,}
from './RecentTransactions.styles';

interface RecentTransactionsProps {
  transactions: Transaction[];
  userEmail: string | undefined;
}

export const RecentTransactions = ({ transactions, userEmail }: RecentTransactionsProps) => {
  const navigate = useNavigate();

  return (
    <Box>
      <Box sx={headerBarSx}>
        <Typography variant="h5" sx={sectionTitleSx}>
          Recent Transactions
        </Typography>
        <Button variant="text" onClick={() => navigate(ROUTES.TRANSACTIONS)} sx={viewAllButtonSx}>
          View All
        </Button>
      </Box>

      {transactions.length === 0 ? (
        <Card>
          <CardContent sx={emptyCardContentSx}>
            <Box sx={emptyIconSx}>
              <Typography variant="h3" sx={emptyIconTextSx}>
                $
              </Typography>
            </Box>
            <Typography color="text.secondary">No transactions yet</Typography>
            <Typography variant="body2" color="text.secondary" sx={emptySubtextSx}>
              Make your first transfer to get started
            </Typography>
          </CardContent>
        </Card>
      ) : (
        <TableContainer component={Paper} sx={tableContainerSx}>
          <Table>
            <TableHead>
              <TableRow>
                <TableCell>Date</TableCell>
                <TableCell>Description</TableCell>
                <TableCell sx={amountCellSx}>Amount</TableCell>
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
                        {isSent ? `To: ${tx.toEmail}` : `From: ${tx.fromEmail}`}
                      </Typography>
                    </TableCell>
                    <TableCell sx={amountCellSx}>
                      <Typography fontWeight={600} sx={createAmountTextSx(isSent)}>
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
