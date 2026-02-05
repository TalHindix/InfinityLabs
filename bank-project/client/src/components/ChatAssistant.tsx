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
} from './ChatAssistant.styles';
import { authStorage } from '../api/auth.storage';

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

const ChatAssistant = () => {
  const [isOpen, setIsOpen] = useState(false);
  const [messages, setMessages] = useState<Message[]>([]);
  const [input, setInput] = useState('');
  const socketRef = useRef<Socket | null>(null);
  const messagesEndRef = useRef<HTMLDivElement | null>(null);

  useEffect(() => {
    if (!authStorage.isAuthenticated()) {
      return;
    }

    socketRef.current = io(`${SOCKET_URL}/chat`, {
      withCredentials: true,
    });

    socketRef.current.on('connect_error', () => {
      setMessages((prev) => [
        ...prev,
        {
          type: 'bot',
          text: 'Authentication failed. Please log in again to use the chat assistant.',
        },
      ]);
      socketRef.current?.disconnect();
    });

    socketRef.current.on('bot-message', (data: { response: string; data?: BotData }) => {
      setMessages((prev) => [...prev, { type: 'bot', text: data.response, data: data.data }]);
    });

    return () => {
      if (socketRef.current) {
        socketRef.current.disconnect();
        socketRef.current = null;
      }
    };
  }, []);

  useEffect(() => {
    if (isOpen && messages.length > 0 && messagesEndRef.current) {
      messagesEndRef.current.scrollIntoView({ behavior: 'smooth' });
    }
  }, [messages, isOpen]);

  const handleOpen = () => {
    setIsOpen(true);
  };

  const handleClose = () => {
    setIsOpen(false);
  };

  const handleSend = () => {
    if (!input.trim()) return;

    setMessages((prev) => [...prev, { type: 'user', text: input }]);
    socketRef.current?.emit('user-message', input);
    setInput('');
  };

  if (!isOpen) {
    return (
      <Fab color="primary" onClick={handleOpen} sx={fabSx}>
        <Chat />
      </Fab>
    );
  }

  return (
    <Paper elevation={4} sx={windowSx}>
      <Box sx={headerSx}>
        <Typography>Virtual Assistant 🏦</Typography>
        <IconButton size="small" onClick={handleClose} sx={closeButtonSx}>
          <Close />
        </IconButton>
      </Box>

      <Box sx={messagesContainerSx}>
        {messages.map((msg, index) => (
          <Box key={`${msg.type}-${index}`} sx={createMessageSx(msg.type === 'user')}>
            {msg.text}
            {msg.data && (
              <Box sx={{ mt: 1, p: 1, bgcolor: 'rgba(0,0,0,0.1)', borderRadius: 1 }}>
                {msg.data.userId && <div>Account: {msg.data.userId}</div>}
                {msg.data.balance && <div>Balance: {msg.data.balance}</div>}
              </Box>
            )}
          </Box>
        ))}
        <div ref={messagesEndRef} />
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

export default ChatAssistant;
