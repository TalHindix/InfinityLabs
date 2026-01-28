import { Stack, Button, Box, Typography } from '../utils/ui';
import { paginationButtonSx, pageIndicatorSx } from './Pagination.styles';

interface PaginationProps {
  currentPage: number;
  totalPages: number;
  onPageChange: (page: number) => void;
}

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

      <Box sx={pageIndicatorSx}>
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
