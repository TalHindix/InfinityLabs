import { useEffect } from 'react';
import { useParams, useNavigate } from 'react-router-dom';
import { ROUTES } from '../../constants/routePaths';
import { authStorage } from '../../api/auth.storage';
import { VideoCall } from '../../components/VideoCall';

const VideoCallPage = () => {
  const { roomName } = useParams<{ roomName: string }>();
  const navigate = useNavigate();
  const currentUser = authStorage.getUser();

  useEffect(() => {
    if (!roomName || !currentUser) {
      navigate(ROUTES.DASHBOARD, { replace: true });
    }
  }, [roomName, currentUser, navigate]);

  if (!roomName || !currentUser) return null;

  return (
    <VideoCall
      roomName={roomName}
      userName={`${currentUser.firstName} ${currentUser.lastName}`}
      onClose={() => navigate(ROUTES.DASHBOARD)}
    />
  );
};

export default VideoCallPage;
