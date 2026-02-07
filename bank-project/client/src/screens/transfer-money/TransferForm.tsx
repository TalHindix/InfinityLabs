import { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import { ROUTES } from '../../constants/routes';
import {
  TextField,
  Button,
  Typography,
  Alert,
  Stack,
  Box,
  Divider,
  InputAdornment,
  Dialog,
  DialogTitle,
  DialogContent,
  DialogActions,
} from '../../shared/ui';
import {
  amountAdornmentSx,
  cancelButtonSx,
  transferSummarySx,
  feeFreeTextSx,
  summaryDividerSx,
  totalAmountSx,
  primaryButtonSx,
  dialogPaperSx,
  dialogTitleSx,
  dialogContentSx,
  dialogRecipientSx,
  dialogWarningTextSx,
  dialogCancelButtonSx,
  dialogConfirmButtonSx,
  dialogActionsSx
} from './TransferForm.styles';
import { formatAmount } from '../../shared/formatters';

interface TransferFormProps {
  receiverEmail: string;
  amount: string;
  description: string;
  loading: boolean;
  error: string;
  success: boolean;
  onReceiverEmailChange: (value: string) => void;
  onAmountChange: (value: string) => void;
  onDescriptionChange: (value: string) => void;
  onSubmit: (e: React.FormEvent) => void;
}

export const TransferForm = ({
  receiverEmail,
  amount,
  description,
  loading,
  error,
  success,
  onReceiverEmailChange,
  onAmountChange,
  onDescriptionChange,
  onSubmit,
}: TransferFormProps) => {
  const navigate = useNavigate();
  const [confirmOpen, setConfirmOpen] = useState(false);

  const numAmount = Number(amount);
  const formattedAmount = Number.isFinite(numAmount) ? formatAmount(numAmount) : '0.00';
  const amountText = `${formattedAmount} AED`;

  const handleSubmitClick = (e: React.FormEvent) => {
    e.preventDefault();
    setConfirmOpen(true);
  };

  const handleConfirm = () => {
    setConfirmOpen(false);
    onSubmit({ preventDefault: () => {} } as React.FormEvent);
  };

  return (
    <>
      {error && <Alert severity="error">{error}</Alert>}
      {success && (
        <Alert severity="success">
          Transfer completed successfully! The funds have been sent.
        </Alert>
      )}

      <form onSubmit={handleSubmitClick}>
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
            onBlur={(e) => {
              const val = parseFloat(e.target.value);
              if (Number.isFinite(val) && val > 0) {
                onAmountChange(val.toFixed(2));
              }
            }}
            required
            slotProps={{
              htmlInput: { min: 1, step: '1' },
              input: {
                endAdornment: (
                  <InputAdornment position="end">
                    <Typography sx={amountAdornmentSx}>AED</Typography>
                  </InputAdornment>
                ),
              },
            }}
            helperText="Minimum transfer amount: 1 AED"
          />

          <TextField
            fullWidth
            label="Transfer reason"
            value={description}
            onChange={(e) => onDescriptionChange(e.target.value)}
            required
            multiline
            minRows={2}
            maxRows={4}
            placeholder="e.g. Payment for services"
            slotProps={{
              htmlInput: { maxLength: 500 },
            }}
            helperText="Required. Max 500 characters."
          />

          <Box sx={transferSummarySx}>
            <Stack spacing={1}>
              <Stack direction="row" justifyContent="space-between">
                <Typography variant="body2" color="text.secondary">
                  Transfer Amount
                </Typography>
                <Typography variant="body2" fontWeight={500}>
                  {amountText}
                </Typography>
              </Stack>

              <Stack direction="row" justifyContent="space-between">
                <Typography variant="body2" color="text.secondary">
                  Fee
                </Typography>
                <Typography variant="body2" fontWeight={500} sx={feeFreeTextSx}>
                  Free
                </Typography>
              </Stack>
            </Stack>

            <Divider sx={summaryDividerSx} />

            <Stack direction="row" justifyContent="space-between">
              <Typography variant="body1" fontWeight={600}>
                Total
              </Typography>
              <Typography variant="body1" fontWeight={600} sx={totalAmountSx}>
                {amountText}
              </Typography>
            </Stack>
          </Box>

          <Button
            fullWidth
            type="submit"
            variant="contained"
            size="large"
            disabled={loading}
            sx={primaryButtonSx}
          >
            {loading ? 'Processing...' : 'Send Money'}
          </Button>

          <Button
            fullWidth
            variant="text"
            onClick={() => navigate(ROUTES.DASHBOARD)}
            sx={cancelButtonSx}
          >
            Cancel
          </Button>
        </Stack>
      </form>

      <Dialog
        open={confirmOpen}
        onClose={() => setConfirmOpen(false)}
        slotProps={{ paper: { sx: dialogPaperSx } }}
      >
        <DialogTitle sx={dialogTitleSx}>Confirm Transfer</DialogTitle>
        <DialogContent sx={dialogContentSx}>
          <Stack spacing={2} sx={{ pt: 1 }}>
            <Typography>
              You are about to send <strong>{amountText}</strong> to:
            </Typography>
            <Typography sx={dialogRecipientSx}>
              {receiverEmail}
            </Typography>
            <Typography variant="body2">
              <strong>Reason:</strong> {description.trim() || '—'}
            </Typography>
            <Typography variant="body2" sx={dialogWarningTextSx}>
              This action cannot be undone.
            </Typography>
          </Stack>
        </DialogContent>
        <DialogActions sx={dialogActionsSx}>
          <Button onClick={() => setConfirmOpen(false)} sx={dialogCancelButtonSx}>
            Cancel
          </Button>
          <Button variant="contained" onClick={handleConfirm} sx={dialogConfirmButtonSx}>
            Confirm Transfer
          </Button>
        </DialogActions>
      </Dialog>
    </>
  );
};
