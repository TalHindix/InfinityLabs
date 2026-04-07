// Manages the chat socket connection, message state, and sending messages.
import { useState, useRef, useCallback, useEffect } from 'react';
import { useNavigate } from 'react-router-dom';
import { io, Socket } from 'socket.io-client';
import { authStorage } from '../api/auth.storage';
import { DASHBOARD_REFRESH_EVENT } from '../screens/dashboard/useDashboardData';
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
  const networkErrorShownRef = useRef(false);

  const disconnectSocket = useCallback(() => {
    if (socketRef.current) {
      socketRef.current.removeAllListeners()
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
    networkErrorShownRef.current = false;

    socketRef.current = io(`${SOCKET_URL}/chat`, {
      withCredentials: true,
      auth: { token: authStorage.getToken() },
    });

    socketRef.current.on('connect', () => {
      networkErrorShownRef.current = false;
    });

    socketRef.current.on('connect_error', (err: Error) => {
      const isAuthError =
        err.message.toLowerCase().includes('unauthorized') ||
        err.message.toLowerCase().includes('forbidden') ||
        err.message.toLowerCase().includes('authentication');

      if (isAuthError) {
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
      } else if (!networkErrorShownRef.current) {
        networkErrorShownRef.current = true;
        setMessages((prev) => [
          ...prev,
          {
            type: 'bot',
            text: 'Chat is temporarily unavailable. Trying to reconnect…',
          },
        ]);
      }
    });

    socketRef.current.on('bot-message', (data: { response: string; data?: BotData }) => {
      setMessages((prev) => [...prev, { type: 'bot', text: data.response, data: data.data }]);
    });

    socketRef.current.on('transfer-completed', () => {
      window.dispatchEvent(new CustomEvent(DASHBOARD_REFRESH_EVENT));
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
