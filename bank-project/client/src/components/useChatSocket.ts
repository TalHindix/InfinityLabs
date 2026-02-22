// Manages the chat socket connection, message state, and sending messages.
import { useState, useRef, useCallback, useEffect } from 'react';
import { useNavigate } from 'react-router-dom';
import { io, Socket } from 'socket.io-client';
import { authStorage } from '../api/auth.storage';
import { ROUTES } from '../constants/routePaths';

const SOCKET_URL = import.meta.env.VITE_SOCKET_URL || 'http://localhost:3000';

interface BotData {
  userId?: string;
  balance?: string;
}

export interface Message {
  type: 'user' | 'bot';
  text: string;
  data?: BotData;
}

export const useChatSocket = (isAuthenticated: boolean) => {
  const navigate = useNavigate();
  const [messages, setMessages] = useState<Message[]>([]);
  const socketRef = useRef<Socket | null>(null);

  const disconnectSocket = useCallback(() => {
    if (socketRef.current) {
      socketRef.current.removeAllListeners();
      socketRef.current.disconnect();
      socketRef.current = null;
    }
  }, []);

  useEffect(() => {
    disconnectSocket();

    if (!isAuthenticated) {
      setMessages([]);
      return;
    }

    setMessages([{ type: 'bot', text: "Hello! I'm your virtual banking assistant." }]);

    socketRef.current = io(`${SOCKET_URL}/chat`, {
      withCredentials: true,
    });

    socketRef.current.once('connect_error', () => {
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

    return () => {
      disconnectSocket();
    };
  }, [isAuthenticated, navigate, disconnectSocket]);

  const sendMessage = useCallback((text: string) => {
    if (!text.trim()) return;

    setMessages((prev) => [...prev, { type: 'user', text }]);
    socketRef.current?.emit('user-message', text);
  }, []);

  return { messages, sendMessage };
};
