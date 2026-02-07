import {
  Container,
  Card,
  CardContent,
  Typography,
  Stack,
  Box,
  Divider,
} from '../../shared/muiExports';
import { AppHeader } from '../../layout/AppHeader';
import { useTransfer } from './useTransfer';
import { TransferForm } from './TransferForm';
import {
  pageRootSx,
  containerSx,
  cardSx,
  cardContentSx,
  transferDecorativeSx,
  decorativeSymbolSx,
  pageTitleSx,
  pageSubtitleSx,
  goldDividerSx,
  footerCaptionSx,
} from './TransferPage.styles';

const TransferPage = () => {
  const transfer = useTransfer();

  return (
    <Box sx={pageRootSx}>
      <AppHeader showDashboardButton showLogout={false} />

      <Container maxWidth="sm" sx={containerSx}>
        <Card sx={cardSx}>
          <Box sx={transferDecorativeSx}>
            <Typography variant="h5" sx={decorativeSymbolSx}>$</Typography>
          </Box>

          <CardContent sx={cardContentSx}>
            <Stack spacing={3}>
              <Box textAlign="center">
                <Typography variant="h4" sx={pageTitleSx}>
                  Transfer Money
                </Typography>
                <Typography variant="body2" color="text.secondary" sx={pageSubtitleSx}>
                  Send money securely to anyone
                </Typography>
              </Box>

              <Divider sx={goldDividerSx} />

              <TransferForm
                receiverEmail={transfer.receiverEmail}
                amount={transfer.amount}
                description={transfer.description}
                loading={transfer.loading}
                error={transfer.error}
                success={transfer.success}
                onReceiverEmailChange={transfer.setReceiverEmail}
                onAmountChange={transfer.setAmount}
                onDescriptionChange={transfer.setDescription}
                onSubmit={transfer.handleSubmit}
              />
            </Stack>
          </CardContent>
        </Card>

        <Typography variant="caption" sx={footerCaptionSx}>
          Transfers are processed instantly and securely
        </Typography>
      </Container>
    </Box>
  );
};

export default TransferPage;
