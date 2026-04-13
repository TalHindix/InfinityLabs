// Manages the chat socket connection, message state, and sending messages.
import { useState, useRef, useCallback, useEffect } from 'react';
import { useNavigate } from 'react-router-dom';
import { io, Socket } from 'socket.io-client';
import { authStorage } from '../api/auth.storage';
import { DASHBOARD_REFRESH_EVENT } from '../screens/dashboard/useDashboardData';
import { ROUTES } from '../constants/routePaths';

const SOCKET_URL = import.meta.env.VITE_SOCKET_URL || 'http://localhost:3000';

export interface Transaction {
  id: number;
  date: string;
  type: 'Sent' | 'Received';
  amount: string;
  counterpart: string;
  description: string;
}

export interface Message {
  type: 'user' | 'bot';
  text: string;
  transactions?: Transaction[];
  summary?: string;
}

export const useChatSocket = (isAuthenticated: boolean) => {
  const navigate = useNavigate();
  const [messages, setMessages] = useState<Message[]>([]);
  const [isOtherTyping, setIsOtherTyping] = useState(false);
  const socketRef = useRef<Socket | null>(null);
  const networkErrorShownRef = useRef(false);
  const typingTimerRef = useRef<ReturnType<typeof setTimeout> | null>(null);

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
      // eslint-disable-next-line react-hooks/set-state-in-effect -- reset state when auth is lost
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

    socketRef.current.on('bot-message', (data: { response: string; data?: { transactions?: Transaction[]; summary?: string } }) => {
      setMessages((prev) => [
        ...prev,
        {
          type: 'bot',
          text: data.response,
          transactions: data.data?.transactions,
          summary: data.data?.summary ?? undefined,
        },
      ]);
    });

    socketRef.current.on('transfer-completed', () => {
      window.dispatchEvent(new CustomEvent(DASHBOARD_REFRESH_EVENT));
    });

    socketRef.current.on('typing', () => setIsOtherTyping(true));
    socketRef.current.on('stop_typing', () => setIsOtherTyping(false));

    return () => {
      disconnectSocket();
    };
  }, [isAuthenticated, navigate, disconnectSocket]);

  const sendMessage = useCallback((text: string) => {
    if (!text.trim()) return;

    setMessages((prev) => [...prev, { type: 'user', text }]);
    socketRef.current?.emit('user-message', text);
  }, []);

  const emitTyping = useCallback(() => {
    socketRef.current?.emit('typing');

    if (typingTimerRef.current) {
      clearTimeout(typingTimerRef.current);
    }

    typingTimerRef.current = setTimeout(() => {
      socketRef.current?.emit('stop_typing');
    }, 1000);
  }, []);

  return { messages, sendMessage, emitTyping, isOtherTyping };
};
