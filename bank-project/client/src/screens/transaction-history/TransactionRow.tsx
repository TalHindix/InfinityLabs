import { TableRow, TableCell, Typography, Button } from '../../shared/muiExports';
import { type Transaction } from '../../types';
import { formatAmount, formatShortDate } from '../../shared/displayFormatters';
import {
  rowSx,
  refNumberSx,
  amountSentSx,
  amountReceivedSx,
  viewButtonSx,
} from './TransactionRow.styles';

interface TransactionRowProps {
  transaction: Transaction;
  userEmail?: string;
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
  const amountPrefix = isSent ? '-' : '+';
  const amountSx = isSent ? amountSentSx : amountReceivedSx;

  return (
    <TableRow
      sx={rowSx(isSelected)}
      onClick={() => onSelect(String(transaction.id))}
      hover
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
          {isSent
            ? `To: ${transaction.toEmail}`
            : `From: ${transaction.fromEmail}`}
        </Typography>
      </TableCell>

      <TableCell align="right">
        <Typography fontWeight={600} sx={amountSx}>
          {amountPrefix}
          {formatAmount(transaction.amount)} AED
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
