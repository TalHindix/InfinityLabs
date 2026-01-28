import { Table, TableBody, TableCell, TableContainer, TableHead, TableRow, Paper, Typography, Box } from '../utils/ui';
import { type Transaction } from '../types';
import { TransactionRow } from './TransactionRow';
import { tableContainerSx, emptyStateSx } from './TransactionTable.styles';

interface TransactionTableProps {
  transactions: Transaction[];
  userEmail?: string;
  selectedTransactionId?: string;
  onSelect: (id: string) => void;
}

const HEADERS = [
  ['Ref #', 'left'],
  ['Date', 'left'],
  ['Description', 'left'],
  ['Amount', 'right'],
  ['Action', 'center'],
] as const;

export const TransactionTable = ({
  transactions,
  userEmail,
  selectedTransactionId,
  onSelect,
}: TransactionTableProps) => {
  if (transactions.length === 0) {
    return (
      <Box sx={emptyStateSx}>
        <Typography color="text.secondary">No transactions yet.</Typography>
      </Box>
    );
  }

  return (
    <TableContainer component={Paper} sx={tableContainerSx}>
      <Table>
        <TableHead>
          <TableRow>
            {HEADERS.map(([label, align]) => (
              <TableCell key={label} align={align}>
                {label}
              </TableCell>
            ))}
          </TableRow>
        </TableHead>

        <TableBody>
          {transactions.map((tx) => (
            <TransactionRow
              key={tx._id}
              transaction={tx}
              userEmail={userEmail}
              isSelected={selectedTransactionId === tx._id}
              onSelect={onSelect}
            />
          ))}
        </TableBody>
      </Table>
    </TableContainer>
  );
};
