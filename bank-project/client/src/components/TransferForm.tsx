import { useNavigate } from 'react-router-dom';
import {
  TextField,
  Button,
  Typography,
  Alert,
  Stack,
  Box,
  Divider,
  InputAdornment,
} from './ui';
import { primaryButtonSx, transferSummarySx, GOLD_COLOR } from '../styles/transactions.styles';

interface TransferFormProps {
  receiverEmail: string;
  amount: string;
  loading: boolean;
  error: string;
  success: boolean;
  onReceiverEmailChange: (value: string) => void;
  onAmountChange: (value: string) => void;
  onSubmit: (e: React.FormEvent) => void;
}

/**
 * Money transfer form with amount summary.
 */
export const TransferForm = ({
  receiverEmail,
  amount,
  loading,
  error,
  success,
  onReceiverEmailChange,
  onAmountChange,
  onSubmit,
}: TransferFormProps) => {
  const navigate = useNavigate();

  const formattedAmount = amount
    ? Number(amount).toLocaleString('en-AE', { minimumFractionDigits: 2 })
    : '0.00';

  return (
    <>
      {error && <Alert severity="error">{error}</Alert>}
      {success && (
        <Alert severity="success">
          Transfer completed successfully! The funds have been sent.
        </Alert>
      )}

      <form onSubmit={onSubmit}>
        <Stack spacing={3}>
          <TextField
            fullWidth
            label="Recipient Email"
            type="email"
            value={receiverEmail}
            onChange={(e) => onReceiverEmailChange(e.target.value)}
            required
            placeholder="recipient@example.com"
            helperText="Enter the email address of the recipient"
          />
          <TextField
            fullWidth
            label="Amount"
            type="number"
            value={amount}
            onChange={(e) => onAmountChange(e.target.value)}
            required
            slotProps={{
              htmlInput: { min: 1, step: '0.01' },
              input: {
                endAdornment: (
                  <InputAdornment position="end">
                    <Typography sx={{ color: GOLD_COLOR, fontWeight: 600 }}>
                      AED
                    </Typography>
                  </InputAdornment>
                ),
              },
            }}
            helperText="Minimum transfer amount: 1 AED"
          />

          <Box sx={transferSummarySx}>
            <Stack direction="row" justifyContent="space-between" mb={1}>
              <Typography variant="body2" color="text.secondary">
                Transfer Amount
              </Typography>
              <Typography variant="body2" fontWeight={500}>
                {formattedAmount} AED
              </Typography>
            </Stack>
            <Stack direction="row" justifyContent="space-between">
              <Typography variant="body2" color="text.secondary">
                Fee
              </Typography>
              <Typography variant="body2" fontWeight={500} sx={{ color: 'success.main' }}>
                Free
              </Typography>
            </Stack>
            <Divider sx={{ my: 1.5 }} />
            <Stack direction="row" justifyContent="space-between">
              <Typography variant="body1" fontWeight={600}>
                Total
              </Typography>
              <Typography variant="body1" fontWeight={600} sx={{ color: 'primary.main' }}>
                {formattedAmount} AED
              </Typography>
            </Stack>
          </Box>

          <Button
            fullWidth
            type="submit"
            variant="contained"
            size="large"
            disabled={loading}
            sx={{
              ...primaryButtonSx,
              py: 1.5,
              fontSize: '1rem',
            }}
          >
            {loading ? 'Processing...' : 'Send Money'}
          </Button>

          <Button
            fullWidth
            variant="text"
            onClick={() => navigate('/dashboard')}
            sx={{ color: 'text.secondary' }}
          >
            Cancel
          </Button>
        </Stack>
      </form>
    </>
  );
};
