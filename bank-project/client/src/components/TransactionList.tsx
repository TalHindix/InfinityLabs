import { Box, Typography, Chip } from '@mui/material';
import type { Transaction } from './useChatSocket';

interface TransactionListProps {
  transactions: Transaction[];
  summary?: string;
}

const TransactionList = ({ transactions, summary }: TransactionListProps) => (
  <Box>
    <Box sx={{ display: 'flex', flexDirection: 'column', gap: 0.75 }}>
      {transactions.map((tx) => (
        <Box
          key={tx.id}
          sx={{
            border: '1px solid',
            borderColor: 'divider',
            borderRadius: 1.5,
            p: 1,
            bgcolor: 'background.paper',
          }}
        >
          <Box sx={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', mb: 0.5 }}>
            <Chip
              label={tx.type}
              size="small"
              sx={{
                height: 20,
                fontSize: '0.7rem',
                fontWeight: 700,
                bgcolor: tx.type === 'Sent' ? 'error.light' : 'success.light',
                color: tx.type === 'Sent' ? 'error.dark' : 'success.dark',
              }}
            />
            <Typography sx={{ fontWeight: 700, fontSize: '0.85rem' }}>{tx.amount}</Typography>
          </Box>
          <Typography variant="caption" sx={{ color: 'text.secondary', display: 'block' }}>
            {tx.date}
          </Typography>
          <Typography variant="caption" sx={{ color: 'text.secondary', display: 'block' }}>
            {tx.counterpart}
          </Typography>
          {tx.description && (
            <Typography variant="caption" sx={{ display: 'block', mt: 0.25, fontStyle: 'italic' }}>
              {tx.description}
            </Typography>
          )}
        </Box>
      ))}
    </Box>
    {summary && (
      <Typography variant="caption" sx={{ display: 'block', mt: 1, color: 'text.secondary' }}>
        {summary}
      </Typography>
    )}
  </Box>
);

export default TransactionList;
