# AI Chatbot

The AI assistant uses OpenAI GPT-4o-mini with function calling to perform real banking operations through a real-time chat interface.

## Architecture

```
Client (ChatAssistant.tsx)
  |
  |-- Socket.IO "user-message" -->  socket.handler.js
                                      |
                                      v
                                    chatbot.service.js (sanitize input)
                                      |
                                      v
                                    openaiFunctionCall.service.js
                                      |
                                      |-- OpenAI API (chat completions)
                                      |     |
                                      |     v
                                      |   Tool calls? --> Execute functions
                                      |     |               |
                                      |     |   <-----------+
                                      |     v
                                      |   Final text response
                                      |
  <-- Socket.IO "bot-message" ------+
```

## Available Tools

The chatbot has access to four tools via OpenAI function calling:

| Tool | Parameters | Description |
|------|-----------|-------------|
| `get_balance` | none | Returns the user's current account balance |
| `get_transaction_history` | `limit` (default 5) | Returns recent transactions |
| `transfer_money` | `recipientEmail` (required), `amount` (required), `description` (optional) | Executes a real money transfer (ACID) |
| `get_supported_services` | none | Lists available banking services |

## Processing Flow

1. **Input Sanitization** -- Trim whitespace, cap at 250 characters, strip HTML tags
2. **Message Construction** -- System prompt + last 20 chat history messages + user message
3. **OpenAI API Call** -- `gpt-4o-mini`, temperature 0.3, with tool definitions
4. **Tool Call Loop** -- If the model requests tool calls:
   - Execute all requested tools in parallel (`Promise.all`)
   - Append tool results to the message history
   - Call OpenAI again with updated messages
   - Repeat up to 5 rounds
5. **Response** -- Extract final text response and return to client

## System Prompt Behavior

- Concise, professional, and friendly tone
- Formats monetary values with AED currency
- **Always asks for confirmation before executing transfers**
- Handles out-of-scope requests gracefully by suggesting available services

## Function Execution

Each tool maps to real service layer functions:

- `get_balance` -> Queries user model for current balance
- `get_transaction_history` -> Calls `transactionService.findTransactions()`
- `transfer_money` -> Calls `transactionService.executeTransfer()` (full ACID transaction)
- `get_supported_services` -> Returns static list of services

When `transfer_money` executes successfully, the service sets a `transferCompleted` flag. The socket handler emits a `transfer-completed` event, which triggers the client to refresh dashboard data.

## Client Integration

### `ChatAssistant.tsx`
Floating action button (FAB) that expands into a chat panel. Positioned fixed at bottom-right.

### `useChatSocket.ts`
Custom hook managing:
- Socket.IO connection to `/chat` namespace (with credentials)
- Message sending and receiving
- Chat history state
- `transfer-completed` event listener that dispatches `dashboard:refresh` DOM event

## Security

- Socket connections authenticated via JWT (same httpOnly cookie as REST)
- Input sanitized server-side before processing
- Rate limited: 10 messages per 60 seconds per user
- Transfer confirmation required by system prompt instruction
- All transfers go through the same ACID-compliant service as REST transfers
