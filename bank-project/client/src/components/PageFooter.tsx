import { Container, Box, Typography } from './ui';

/**
 * Footer for authenticated pages.
 */
export const PageFooter = () => {
  return (
    <Box sx={{ bgcolor: 'rgba(13, 27, 42, 0.03)', py: 3, mt: 4 }}>
      <Container maxWidth="lg">
        <Typography variant="body2" color="text.secondary" textAlign="center">
          Dubai-Bank - Your trusted partner in premium banking
        </Typography>
      </Container>
    </Box>
  );
};
