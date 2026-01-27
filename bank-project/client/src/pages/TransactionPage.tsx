import {
  Container,
  Card,
  CardContent,
  Typography,
  Stack,
  Box,
  Divider,
} from '../components/ui';
import { AppHeader } from '../components/AppHeader';
import { useTransfer } from '../hooks/useTransfer';
import { TransferForm } from '../components/TransferForm';
import { transferDecorativeSx } from '../styles/transactions.styles';
import {
  decorativeSymbolSx,
  pageTitleSx,
  pageSubtitleSx,
  goldDividerSx,
  footerCaptionSx,
} from './TransactionPage.styles';

const TransferPage = () => {
  const transfer = useTransfer();

  return (
    <Box sx={{ minHeight: '100vh', bgcolor: 'background.default' }}>
      <AppHeader showDashboardButton showLogout={false} />

      <Container maxWidth="sm" sx={{ py: 6 }}>
        <Card sx={{ overflow: 'visible', position: 'relative' }}>
          <Box sx={transferDecorativeSx}>
            <Typography variant="h5" sx={decorativeSymbolSx}>$</Typography>
          </Box>

          <CardContent sx={{ p: 4, pt: 6 }}>
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
