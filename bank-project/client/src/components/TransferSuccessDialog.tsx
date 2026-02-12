import { useState } from 'react';
import {
  Dialog,
  DialogTitle,
  DialogContent,
  DialogActions,
  Button,
  Typography,
  Stack,
  Box,
  Divider,
} from '../shared/muiExports';
import CheckCircleIcon from '@mui/icons-material/CheckCircle';
import VideocamIcon from '@mui/icons-material/Videocam';
import type { Transaction } from '../types';
import { formatAmount } from '../shared/displayFormatters';
import { authStorage } from '../api/auth.storage';
import { VideoCall } from './VideoCall';
import {
  dialogPaperSx,
  dialogTitleSx,
  dialogContentSx,
  dialogActionsSx,
} from './TransferSuccessDialog.styles';

interface TransferSuccessDialogProps {
  open: boolean;
  transaction: Transaction | null;
  videoRoomName: string;
  onClose: () => void;
}

export const TransferSuccessDialog = ({
  open,
  transaction,
  videoRoomName,
  onClose,
}: TransferSuccessDialogProps) => {
  const [showVideoCall, setShowVideoCall] = useState(false);

  if (!transaction) return null;

  const currentUser = authStorage.getUser();
  const amountText = `${formatAmount(transaction.amount)} AED`;

  const handleStartVideoCall = () => setShowVideoCall(true);
  const handleCloseVideoCall = () => setShowVideoCall(false);

  if (showVideoCall && currentUser) {
    return (
      <VideoCall
        roomName={videoRoomName}
        userName={`${currentUser.firstName} ${currentUser.lastName}`}
        userEmail={currentUser.email}
        onClose={handleCloseVideoCall}
      />
    );
  }

  return (
    <Dialog
      open={open}
      onClose={onClose}
      slotProps={{ paper: { sx: dialogPaperSx } }}
      maxWidth="sm"
      fullWidth
    >
      <DialogTitle sx={dialogTitleSx}>
        Transfer Completed!
      </DialogTitle>
      <DialogContent sx={dialogContentSx}>
        <Stack spacing={2}>
          <Stack direction="row" spacing={1} alignItems="center">
            <CheckCircleIcon sx={{ color: '#4CAF50' }} />
            <Typography variant="body1" color="text.secondary">
              Transfer successful — notification email sent to recipient
            </Typography>
          </Stack>
          <Divider />
          <Box>
            <Typography variant="body2" color="text.secondary">
              Amount
            </Typography>
            <Typography variant="h6" sx={{ color: '#C9A227', fontWeight: 700 }}>
              {amountText}
            </Typography>
          </Box>
          <Box>
            <Typography variant="body2" color="text.secondary">
              Recipient
            </Typography>
            <Typography variant="body1" sx={{ fontWeight: 500 }}>
              {transaction.toEmail}
            </Typography>
          </Box>
          <Box>
            <Typography variant="body2" color="text.secondary">
              Transaction ID
            </Typography>
            <Typography variant="body1" sx={{ fontWeight: 500 }}>
              #{transaction.id}
            </Typography>
          </Box>
          {transaction.description && (
            <Box>
              <Typography variant="body2" color="text.secondary">
                Description
              </Typography>
              <Typography variant="body1">
                {transaction.description}
              </Typography>
            </Box>
          )}
        </Stack>
      </DialogContent>
      <DialogActions sx={dialogActionsSx}>
        <Stack direction="row" spacing={2} sx={{ width: '100%' }}>
          <Button
            onClick={onClose}
            variant="outlined"
            sx={{ flex: 1 }}
          >
            Close
          </Button>
          <Button
            onClick={handleStartVideoCall}
            variant="contained"
            disabled={!videoRoomName}
            startIcon={<VideocamIcon />}
            sx={{
              flex: 1,
              fontWeight: 600,
              background: 'linear-gradient(135deg, #C9A227 0%, #E5C158 100%)',
              color: '#0D1B2A',
              '&:hover': {
                background: 'linear-gradient(135deg, #A17F1A 0%, #C9A227 100%)',
              },
            }}
          >
            Video Call
          </Button>
        </Stack>
      </DialogActions>
    </Dialog>
  );
};
