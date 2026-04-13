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
import { useChatSocket, type Message, type Transaction } from './useChatSocket';
import TransactionList from './TransactionList';

interface TransactionData {
  text: string;
  transactions: Transaction[];
  summary?: string;
}

function extractJsonFromText(text: string): TransactionData | null {
  const start = text.indexOf('{');
  const end = text.lastIndexOf('}');
  if (start === -1 || end <= start) return null;

  try {
    const parsed = JSON.parse(text.slice(start, end + 1));
    if (!parsed || !Array.isArray(parsed.transactions)) return null;

    const before = text.slice(0, start).trim();
    const parts = [before, parsed.message].filter(Boolean);

    return { text: parts.join('\n\n'), transactions: parsed.transactions, summary: parsed.summary };
  } catch {
    return null;
  }
}

function getTransactionData(msg: Message): TransactionData | null {
  if (msg.transactions) {
    return { text: msg.text, transactions: msg.transactions, summary: msg.summary };
  }
  return extractJsonFromText(msg.text);
}

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
          const txData = msg.type === 'bot' ? getTransactionData(msg) : null;
          return (
            <Box key={`${msg.type}-${index}`} dir="auto" sx={createMessageSx(msg.type === 'user')}>
              {msg.type === 'bot' ? (
                txData ? (
                  <Box>
                    {txData.text && (
                      <Box sx={{ ...botMarkdownSx, mb: 0.75 }}>
                        <Markdown>{txData.text}</Markdown>
                      </Box>
                    )}
                    <TransactionList transactions={txData.transactions} summary={txData.summary} />
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
