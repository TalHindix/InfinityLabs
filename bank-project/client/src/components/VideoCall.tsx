import { JitsiMeeting } from '@jitsi/react-sdk';
import { Box, IconButton } from '../shared/muiExports';
import CloseIcon from '@mui/icons-material/Close';

interface VideoCallProps {
  roomName: string;
  userName: string;
  onClose: () => void;
}

export const VideoCall = ({ roomName, userName, onClose }: VideoCallProps) => (
  <Box
    sx={{
      position: 'fixed',
      inset: 0,
      zIndex: 9999,
      background: '#000',
      display: 'flex',
      flexDirection: 'column',
    }}
  >
    <IconButton
      onClick={onClose}
      sx={{
        position: 'absolute',
        top: 8,
        right: 8,
        zIndex: 10000,
        color: '#fff',
        bgcolor: 'rgba(0,0,0,0.5)',
        '&:hover': { bgcolor: 'rgba(0,0,0,0.7)' },
      }}
    >
      <CloseIcon />
    </IconButton>
    <JitsiMeeting
      domain="meet.jit.si"
      roomName={`dubai-bank-${roomName}`}
      configOverwrite={{
        startWithAudioMuted: true,
        startWithVideoMuted: false,
        disableModeratorIndicator: true,
        enableEmailInStats: false,
      }}
      interfaceConfigOverwrite={{
        DISABLE_JOIN_LEAVE_NOTIFICATIONS: true,
        SHOW_JITSI_WATERMARK: false,
        SHOW_BRAND_WATERMARK: false,
        MOBILE_APP_PROMO: false,
      }}
      userInfo={{ displayName: userName, email: '' }}
      onReadyToClose={onClose}
      getIFrameRef={(iframeRef) => {
        iframeRef.style.height = '100%';
      }}
    />
  </Box>
);
