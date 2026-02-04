import {
  Container,
  Card,
  CardContent,
  Typography,
  Stack,
  Box,
  Divider,
} from '../utils/ui';
import { AppHeader } from '../components/AppHeader';
import { useTransfer } from '../hooks/useTransfer';
import { TransferForm } from '../components/TransferForm';
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
                loading={transfer.loading}
                error={transfer.error}
                success={transfer.success}
                onReceiverEmailChange={transfer.setReceiverEmail}
                onAmountChange={transfer.setAmount}
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
