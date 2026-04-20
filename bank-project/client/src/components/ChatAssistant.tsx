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

// Mirror of the server-side parser in openaiFunctionCall.service.js. The
// server normally extracts the transaction JSON before sending, but if a
// message slips through with raw text+JSON mixed (or arrives via an older
// socket path) we re-parse here so the chat bubble can still render cards.
function findBalancedJson(str: string, startIndex: number): string | null {
  if (startIndex < 0 || str[startIndex] !== '{') return null;
  let depth = 0;
  let inString = false;
  let escape = false;

  for (let i = startIndex; i < str.length; i++) {
    const ch = str[i];
    if (escape) { escape = false; continue; }
    if (ch === '\\' && inString) { escape = true; continue; }
    if (ch === '"') { inString = !inString; continue; }
    if (inString) continue;
    if (ch === '{') depth++;
    else if (ch === '}') {
      depth--;
      if (depth === 0) return str.slice(startIndex, i + 1);
    }
  }
  return null;
}

function extractJsonFromText(text: string): TransactionData | null {
  let pos = 0;

  while (pos < text.length) {
    const start = text.indexOf('{', pos);
    if (start === -1) break;

    const jsonStr = findBalancedJson(text, start);
    if (!jsonStr) break;

    try {
      const parsed = JSON.parse(jsonStr);
      if (parsed && Array.isArray(parsed.transactions)) {
        const before = text.slice(0, start).trim();
        const afterStart = start + jsonStr.length;
        const after = text.slice(afterStart).trim();

        // Extract message from any remaining JSON object (e.g. separate balance response)
        let extraMessage = '';
        if (after) {
          const extraJsonStr = findBalancedJson(after, after.indexOf('{'));
          if (extraJsonStr) {
            try {
              const extraParsed = JSON.parse(extraJsonStr);
              if (extraParsed.message) extraMessage = extraParsed.message;
            } catch { /* ignore */ }
          } else if (!after.startsWith('{')) {
            extraMessage = after;
          }
        }

        const parts = [before, parsed.message, extraMessage].filter(Boolean);
        return { text: parts.join('\n\n'), transactions: parsed.transactions, summary: parsed.summary };
      }
    } catch { /* not valid JSON, skip */ }

    pos = start + (jsonStr ? jsonStr.length : 1);
  }

  return null;
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
