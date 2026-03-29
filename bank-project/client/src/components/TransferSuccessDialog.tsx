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
  Alert,
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
  onVideoCall: (transactionId: string) => Promise<void>;
}

export const TransferSuccessDialog = ({
  open,
  transaction,
  videoRoomName,
  onClose,
  onVideoCall,
}: TransferSuccessDialogProps) => {
  const [showVideoCall, setShowVideoCall] = useState(false);
  const [videoCallLoading, setVideoCallLoading] = useState(false);
  const [videoCallError, setVideoCallError] = useState('');

  if (!transaction) return null;

  const currentUser = authStorage.getUser();
  const amountText = `${formatAmount(transaction.amount)} AED`;

  const handleStartVideoCall = async () => {
    setVideoCallLoading(true);
    setVideoCallError('');
    try {
      await onVideoCall(transaction.id.toString());
      setShowVideoCall(true);
    } catch {
      setVideoCallError('Could not start the video call. Please try again.');
    } finally {
      setVideoCallLoading(false);
    }
  };
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
      {videoCallError && (
        <Box sx={{ px: 3, pb: 1 }}>
          <Alert severity="error">{videoCallError}</Alert>
        </Box>
      )}

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
            startIcon={<VideocamIcon />}
            disabled={videoCallLoading}
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
            {videoCallLoading ? 'Starting...' : 'Video Call'}
          </Button>
        </Stack>
      </DialogActions>
    </Dialog>
  );
};
