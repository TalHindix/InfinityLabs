import {
  TableRow,
  TableCell,
  Typography,
  Button,
} from './ui';
import { type Transaction } from '../services/transaction';
import { formatAmount, formatShortDate } from '../utils/formatters';
import { createTableRowSx } from '../styles/transactions.styles';
import { refNumberSx, viewButtonSx } from './TransactionRow.styles';

interface TransactionRowProps {
  transaction: Transaction;
  userEmail: string | undefined;
  isSelected: boolean;
  onSelect: (id: string) => void;
}

export const TransactionRow = ({
  transaction,
  userEmail,
  isSelected,
  onSelect,
}: TransactionRowProps) => {
  const isSent = transaction.fromEmail === userEmail;

  return (
    <TableRow
      sx={createTableRowSx(isSelected)}
      onClick={() => onSelect(String(transaction.id))}
    >
      <TableCell>
        <Typography variant="body2" fontWeight={600} sx={refNumberSx}>
          #{transaction.id}
        </Typography>
      </TableCell>
      <TableCell>
        <Typography variant="body2">
          {formatShortDate(transaction.createdAt)}
        </Typography>
      </TableCell>
      <TableCell>
        <Typography variant="body2">
          {isSent ? `To: ${transaction.toEmail}` : `From: ${transaction.fromEmail}`}
        </Typography>
      </TableCell>
      <TableCell align="right">
        <Typography
          fontWeight={600}
          sx={{ color: isSent ? 'error.main' : 'success.main' }}
        >
          {isSent ? '-' : '+'}{formatAmount(transaction.amount)} AED
        </Typography>
      </TableCell>
      <TableCell align="center">
        <Button size="small" variant="text" sx={viewButtonSx}>
          View
        </Button>
      </TableCell>
    </TableRow>
  );
};
