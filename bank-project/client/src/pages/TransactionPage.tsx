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

const TransferPage = () => {
  const transfer  = useTransfer();

  return (
    <Box sx={{ minHeight: '100vh', bgcolor: 'background.default' }}>
      <AppHeader showDashboardButton showLogout={false} />

      <Container maxWidth="sm" sx={{ py: 6 }}>
        <Card sx={{ overflow: 'visible', position: 'relative' }}>
          {/* Decorative element */}
          <Box sx={transferDecorativeSx}>
            <Typography variant="h5" sx={{ color: '#0D1B2A' }}>$</Typography>
          </Box>

          <CardContent sx={{ p: 4, pt: 6 }}>
            <Stack spacing={3}>
              <Box textAlign="center">
                <Typography variant="h4" sx={{ fontWeight: 600, color: 'primary.main' }}>
                  Transfer Money
                </Typography>
                <Typography variant="body2" color="text.secondary" sx={{ mt: 0.5 }}>
                  Send money securely to anyone
                </Typography>
              </Box>

              <Divider sx={{ borderColor: 'rgba(201, 162, 39, 0.2)' }} />

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

        <Typography
          variant="caption"
          sx={{
            display: 'block',
            textAlign: 'center',
            mt: 3,
            color: 'text.secondary',
          }}
        >
          Transfers are processed instantly and securely
        </Typography>
      </Container>
    </Box>
  );
};

export default TransferPage;
