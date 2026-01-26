import { Stack, Button, Box, Typography } from './ui';
import { paginationButtonSx } from '../styles/transactions.styles';

interface PaginationProps {
  currentPage: number;
  totalPages: number;
  onPageChange: (page: number) => void;
}

/**
 * Pagination controls for transactions list.
 */
export const Pagination = ({ currentPage, totalPages, onPageChange }: PaginationProps) => {
  return (
    <Stack direction="row" justifyContent="center" spacing={1}>
      <Button
        variant="outlined"
        size="small"
        disabled={currentPage <= 1}
        onClick={() => onPageChange(currentPage - 1)}
        sx={paginationButtonSx}
      >
        Previous
      </Button>
      <Box
        sx={{
          px: 3,
          py: 1,
          borderRadius: 1,
          bgcolor: 'rgba(13, 27, 42, 0.05)',
          display: 'flex',
          alignItems: 'center',
        }}
      >
        <Typography variant="body2">
          Page {currentPage} of {totalPages}
        </Typography>
      </Box>
      <Button
        variant="outlined"
        size="small"
        disabled={currentPage >= totalPages}
        onClick={() => onPageChange(currentPage + 1)}
        sx={paginationButtonSx}
      >
        Next
      </Button>
    </Stack>
  );
};
