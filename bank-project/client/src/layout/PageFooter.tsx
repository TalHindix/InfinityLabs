import { Container, Box, Typography } from '../shared/muiExports';
import { footerContainerSx } from './pageFooterStyles';

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
