import {
  Table,
  TableBody,
  TableCell,
  TableContainer,
  TableHead,
  TableRow,
  Paper,
} from './ui';
import { type Transaction } from '../services/transaction';
import { TransactionRow } from './TransactionRow';

interface TransactionTableProps {
  transactions: Transaction[];
  userEmail: string | undefined;
  selectedTransactionId: string | undefined;
  onSelectTransaction: (id: string) => void;
}

/**
 * Transactions table component with header and rows.
 */
export const TransactionTable = ({
  transactions,
  userEmail,
  selectedTransactionId,
  onSelectTransaction,
}: TransactionTableProps) => {
  return (
    <TableContainer component={Paper} sx={{ borderRadius: 3, mb: 2 }}>
      <Table>
        <TableHead>
          <TableRow>
            <TableCell>Ref #</TableCell>
            <TableCell>Date</TableCell>
            <TableCell>Description</TableCell>
            <TableCell align="right">Amount</TableCell>
            <TableCell align="center">Action</TableCell>
          </TableRow>
        </TableHead>
        <TableBody>
          {transactions.map((tx) => (
            <TransactionRow
              key={tx._id}
              transaction={tx}
              userEmail={userEmail}
              isSelected={selectedTransactionId === tx._id}
              onSelect={onSelectTransaction}
            />
          ))}
        </TableBody>
      </Table>
    </TableContainer>
  );
};
