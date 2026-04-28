import {
  Button,
  Typography,
  Stack,
  Dialog,
  DialogTitle,
  DialogContent,
  DialogActions,
} from '../../shared/muiExports';
import {
  dialogPaperSx,
  dialogTitleSx,
  dialogContentSx,
  dialogRecipientSx,
  dialogWarningTextSx,
  dialogCancelButtonSx,
  dialogConfirmButtonSx,
  dialogActionsSx,
} from './TransferForm.styles';

interface ConfirmTransferDialogProps {
  open: boolean;
  amountText: string;
  receiverEmail: string;
  description: string;
  onCancel: () => void;
  onConfirm: () => void;
}

export const ConfirmTransferDialog = ({
  open,
  amountText,
  receiverEmail,
  description,
  onCancel,
  onConfirm,
}: ConfirmTransferDialogProps) => (
  <Dialog
    open={open}
    onClose={onCancel}
    slotProps={{ paper: { sx: dialogPaperSx } }}
  >
    <DialogTitle sx={dialogTitleSx}>Confirm Transfer</DialogTitle>
    <DialogContent sx={dialogContentSx}>
      <Stack spacing={2} sx={{ pt: 1 }}>
        <Typography>
          You are about to send <strong>{amountText}</strong> to:
        </Typography>
        <Typography sx={dialogRecipientSx}>{receiverEmail}</Typography>
        <Typography variant="body2">
          <strong>Reason:</strong> {description.trim() || '—'}
        </Typography>
        <Typography variant="body2" sx={dialogWarningTextSx}>
          This action cannot be undone.
        </Typography>
      </Stack>
    </DialogContent>
    <DialogActions sx={dialogActionsSx}>
      <Button onClick={onCancel} sx={dialogCancelButtonSx}>
        Cancel
      </Button>
      <Button variant="contained" onClick={onConfirm} sx={dialogConfirmButtonSx}>
        Confirm Transfer
      </Button>
    </DialogActions>
  </Dialog>
);
