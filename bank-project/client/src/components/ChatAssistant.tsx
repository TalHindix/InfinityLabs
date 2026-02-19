import { useState, useEffect, useRef } from 'react';
import { useNavigate } from 'react-router-dom';
import { io, Socket } from 'socket.io-client';
import { Box, Fab, Paper, Typography, TextField, IconButton } from '@mui/material';
import { Chat, Close, Send } from '@mui/icons-material';
import Markdown from 'react-markdown';
import {
  fabSx,
  windowSx,
  headerSx,
  closeButtonSx,
  messagesContainerSx,
  createMessageSx,
  botMarkdownSx,
  inputContainerSx,
  textFieldSx,
} from './ChatAssistant.styles';
import { authStorage, AUTH_CHANGE_EVENT } from '../api/auth.storage';
import { ROUTES } from '../constants/routePaths';

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
  const navigate = useNavigate();
  const [isOpen, setIsOpen] = useState(false);
  const [messages, setMessages] = useState<Message[]>([]);
  const [input, setInput] = useState('');
  const socketRef = useRef<Socket | null>(null);
  const messagesEndRef = useRef<HTMLDivElement | null>(null);
  const [isAuthenticated, setIsAuthenticated] = useState(authStorage.isAuthenticated());

  // Monitor auth state changes via events
  useEffect(() => {
    const handleAuthChange = () => {
      setIsAuthenticated(authStorage.isAuthenticated());
    };

    // Set initial auth state
    handleAuthChange();

    // Listen for auth state changes
    window.addEventListener(AUTH_CHANGE_EVENT, handleAuthChange);
    return () => window.removeEventListener(AUTH_CHANGE_EVENT, handleAuthChange);
  }, []);

  useEffect(() => {
    // If user is not authenticated, disconnect socket
    if (!isAuthenticated) {
      if (socketRef.current) {
        socketRef.current.removeAllListeners();
        socketRef.current.disconnect();
        socketRef.current = null;
      }
      return;
    }

    // If socket already exists and is connected, don't create a new one
    if (socketRef.current?.connected) {
      return;
    }

    // Clean up old socket if exists but not connected
    if (socketRef.current) {
      socketRef.current.removeAllListeners();
      socketRef.current.disconnect();
    }

    // Create new socket
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
      authStorage.clearAuth();
      navigate(ROUTES.LOGIN);
    });

    socketRef.current.on('bot-message', (data: { response: string; data?: BotData }) => {
      setMessages((prev) => [...prev, { type: 'bot', text: data.response, data: data.data }]);
    });

    socketRef.current.on('transfer-completed', () => {
      window.dispatchEvent(new CustomEvent('dashboard:refresh'));
    });

    // Cleanup on unmount
    return () => {
      if (socketRef.current) {
        socketRef.current.removeAllListeners();
        socketRef.current.disconnect();
        socketRef.current = null;
      }
    };
  }, [isAuthenticated, navigate]);

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
            {msg.type === 'bot' ? (
              <Box sx={botMarkdownSx}>
                <Markdown>{msg.text}</Markdown>
              </Box>
            ) : (
              msg.text
            )}
          </Box>
        ))}
        <div ref={messagesEndRef} />
      </Box>

      <Box sx={inputContainerSx}>
        <TextField
          size="small"
          fullWidth
          multiline
          maxRows={4}
          value={input}
          onChange={(e) => setInput(e.target.value)}
          onKeyDown={(e) => {
            if (e.key === 'Enter' && !e.shiftKey) {
              e.preventDefault();
              handleSend();
            }
          }}
          placeholder="Type a message..."
          sx={textFieldSx}
        />
        <IconButton
          color="primary"
          onClick={handleSend}
          disabled={!input.trim()}
          sx={{ opacity: input.trim() ? 1 : 0.4, transition: 'opacity 0.2s' }}
        >
          <Send />
        </IconButton>
      </Box>
    </Paper>
  );
};

export default ChatAssistant;
