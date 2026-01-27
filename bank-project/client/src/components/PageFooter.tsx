import { Container, Box, Typography } from './ui';
import { footerContainerSx } from './PageFooter.styles';

export const PageFooter = () => {
  return (
    <Box sx={footerContainerSx}>
      <Container maxWidth="lg">
        <Typography variant="body2" color="text.secondary" textAlign="center">
          Dubai-Bank - Your trusted partner in premium banking
        </Typography>
      </Container>
    </Box>
  );
};
