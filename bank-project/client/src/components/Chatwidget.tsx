import { useState, useEffect, useRef } from 'react';
import { io, Socket } from 'socket.io-client';
import { Box, Fab, Paper, Typography, TextField, IconButton } from '@mui/material';
import { Chat, Close, Send } from '@mui/icons-material';
import {
  fabSx,
  windowSx,
  headerSx,
  closeButtonSx,
  messagesContainerSx,
  createMessageSx,
  inputContainerSx,
  textFieldSx,
} from './Chatwidget.styles';
import { authStorage } from '../services/auth.storage';

const SOCKET_URL = import.meta.env.VITE_SOCKET_URL || 'http://localhost:3000';

interface BotData {
  userId?: string;
  balance?: string;
}

interface Message {
  type: 'user' | 'bot';
  text: string;
  data?: BotData;
}

const ChatWidget = () => {
  const [isOpen, setIsOpen] = useState(false);
  const [messages, setMessages] = useState<Message[]>([]);
  const [input, setInput] = useState('');
  const socketRef = useRef<Socket | null>(null);
  const hasGreeted = useRef(false);

  useEffect(() => {
    const user = authStorage.getUser();

    socketRef.current = io(`${SOCKET_URL}/chat`, {
      auth: { userId: user?.id }
    });

    socketRef.current.on('bot-message', (data: { response: string; data?: BotData }) => {
      setMessages((prev) => [...prev, { type: 'bot', text: data.response, data: data.data }]);
    });

    return () => {
      socketRef.current?.disconnect();
    };
  }, []);

  const handleOpen = () => {
    setIsOpen(true);
    if (!hasGreeted.current) {
      socketRef.current?.emit('request-greeting');
      hasGreeted.current = true;
    }
  };

  const handleSend = () => {
    if (!input.trim()) return;

    setMessages((prev) => [...prev, { type: 'user', text: input }]);
    socketRef.current?.emit('user-message', input);
    setInput('');
  };

  if (!isOpen) {
    return (
      <Fab color="primary" onClick={handleOpen} sx={fabSx}>  {/* ← שינוי */}
        <Chat />
      </Fab>
    );
  }

  return (
    <Paper elevation={4} sx={windowSx}>
      <Box sx={headerSx}>
        <Typography>Virtual Assistant 🏦</Typography>
        <IconButton size="small" onClick={() => setIsOpen(false)} sx={closeButtonSx}>
          <Close />
        </IconButton>
      </Box>

      <Box sx={messagesContainerSx}>
        {messages.map((msg, i) => (
          <Box key={i} sx={createMessageSx(msg.type === 'user')}>
            {msg.text}
            {msg.data && (
              <Box sx={{ mt: 1, p: 1, bgcolor: 'rgba(0,0,0,0.1)', borderRadius: 1 }}>
                {msg.data.userId && <div>Account: {msg.data.userId}</div>}
                {msg.data.balance && <div>Balance: {msg.data.balance}</div>}
              </Box>
            )}
          </Box>
        ))}
      </Box>

      <Box sx={inputContainerSx}>
        <TextField
          size="small"
          fullWidth
          value={input}
          onChange={(e) => setInput(e.target.value)}
          onKeyDown={(e) => e.key === 'Enter' && handleSend()}
          placeholder="Type a message..."
          sx={textFieldSx}
        />
        <IconButton color="primary" onClick={handleSend}>
          <Send />
        </IconButton>
      </Box>
    </Paper>
  );
};

export default ChatWidget;