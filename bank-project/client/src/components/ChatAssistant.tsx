import { useState, useEffect, useRef } from 'react';
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
import { useAuth } from '../shared/useAuth';
import { useChatSocket } from './useChatSocket';
import TransactionList from './TransactionList';

const ChatAssistant = () => {
  const { isAuthenticated } = useAuth();
  const { messages, sendMessage, emitTyping, isOtherTyping } = useChatSocket(isAuthenticated);

  const [isOpen, setIsOpen] = useState(false);
  const [input, setInput] = useState('');
  const messagesEndRef = useRef<HTMLDivElement | null>(null);

  useEffect(() => {
    if (isOpen && messages.length > 0 && messagesEndRef.current) {
      messagesEndRef.current.scrollIntoView({ behavior: 'smooth' });
    }
  }, [messages, isOpen]);

  const handleSend = () => {
    if (!input.trim()) return;
    sendMessage(input);
    setInput('');
  };

  if (!isAuthenticated) return null;

  if (!isOpen) {
    return (
      <Fab color="primary" onClick={() => setIsOpen(true)} sx={fabSx}>
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
        {messages.map((msg, index) => {
          const hasTransactions = msg.type === 'bot' && msg.transactions && msg.transactions.length > 0;
          return (
            <Box key={`${msg.type}-${index}`} dir="auto" sx={createMessageSx(msg.type === 'user')}>
              {msg.type === 'bot' ? (
                hasTransactions ? (
                  <Box>
                    {msg.text && (
                      <Box sx={{ ...botMarkdownSx, mb: 0.75 }}>
                        <Markdown>{msg.text}</Markdown>
                      </Box>
                    )}
                    <TransactionList transactions={msg.transactions!} summary={msg.summary} />
                  </Box>
                ) : (
                  <Box sx={botMarkdownSx}>
                    <Markdown>{msg.text}</Markdown>
                  </Box>
                )
              ) : (
                msg.text
              )}
            </Box>
          );
        })}
        <div ref={messagesEndRef} />
      </Box>

      {isOtherTyping && (
        <Typography sx={{ px: 2, pb: 1, fontSize: '0.75rem', color: 'text.secondary', fontStyle: 'italic' }}>
          Someone is typing...
        </Typography>
      )}

      <Box sx={inputContainerSx}>
        <TextField
          size="small"
          fullWidth
          multiline
          maxRows={4}
          value={input}
          onChange={(e) => { setInput(e.target.value); emitTyping(); }}
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
