# Socket.IO Reconnection Behavior Analysis (JWT Expiry)

## 1. Client-Side: `client/src/components/ChatAssistant.tsx`

### Full Code (Lines 1-142):

```typescript
1:  import { useState, useEffect, useRef } from 'react';
2:  import { io, Socket } from 'socket.io-client';
3:  import { Box, Fab, Paper, Typography, TextField, IconButton } from '@mui/material';
4:  import { Chat, Close, Send } from '@mui/icons-material';
5:  import {
6:    fabSx,
7:    windowSx,
8:    headerSx,
9:    closeButtonSx,
10:   messagesContainerSx,
11:   createMessageSx,
12:   inputContainerSx,
13:   textFieldSx,
14: } from './ChatAssistant.styles';
15:  import { authStorage } from '../api/auth.storage';
16:
17:  const SOCKET_URL = import.meta.env.VITE_SOCKET_URL || 'http://localhost:3000';
18:
19:  interface BotData {
20:    userId?: string;
21:    balance?: string;
22:  }
23:
24:  interface Message {
25:    type: 'user' | 'bot';
26:    text: string;
27:    data?: BotData;
28:  }
29:
30:  const ChatAssistant = () => {
31:    const [isOpen, setIsOpen] = useState(false);
32:    const [messages, setMessages] = useState<Message[]>([]);
33:    const [input, setInput] = useState('');
34:    const socketRef = useRef<Socket | null>(null);
35:    const messagesEndRef = useRef<HTMLDivElement | null>(null);
36:
37:    useEffect(() => {
38:      if (!authStorage.isAuthenticated()) {
39:        return;
40:      }
41:
42:      socketRef.current = io(`${SOCKET_URL}/chat`, {
43:        withCredentials: true,
44:      });
45:
46:      socketRef.current.on('connect_error', () => {
47:        setMessages((prev) => [
48:          ...prev,
49:          {
50:            type: 'bot',
51:            text: 'Authentication failed. Please log in again to use the chat assistant.',
52:          },
53:        ]);
54:        socketRef.current?.disconnect();
55:      });
56:
57:      socketRef.current.on('bot-message', (data: { response: string; data?: BotData }) => {
58:        setMessages((prev) => [...prev, { type: 'bot', text: data.response, data: data.data }]);
59:      });
60:
61:      return () => {
62:        if (socketRef.current) {
63:          socketRef.current.disconnect();
64:          socketRef.current = null;
65:        }
66:      };
67:    }, []);
68:
69:    useEffect(() => {
70:      if (isOpen && messages.length > 0 && messagesEndRef.current) {
71:        messagesEndRef.current.scrollIntoView({ behavior: 'smooth' });
72:      }
73:    }, [messages, isOpen]);
74:
75:    const handleOpen = () => {
76:      setIsOpen(true);
77:    };
78:
78:    const handleClose = () => {
80:      setIsOpen(false);
81:    };
82:
83:    const handleSend = () => {
84:      if (!input.trim()) return;
85:
86:      setMessages((prev) => [...prev, { type: 'user', text: input }]);
87:      socketRef.current?.emit('user-message', input);
88:      setInput('');
89:    };
90:
91:    if (!isOpen) {
92:      return (
93:        <Fab color="primary" onClick={handleOpen} sx={fabSx}>
94:          <Chat />
95:        </Fab>
96:      );
97:    }
98:
99:    return (
100:     <Paper elevation={4} sx={windowSx}>
101:       <Box sx={headerSx}>
102:         <Typography>Virtual Assistant 🏦</Typography>
103:         <IconButton size="small" onClick={handleClose} sx={closeButtonSx}>
104:           <Close />
105:         </IconButton>
106:       </Box>
107:
108:       <Box sx={messagesContainerSx}>
109:         {messages.map((msg, index) => (
110:           <Box key={`${msg.type}-${index}`} sx={createMessageSx(msg.type === 'user')}>
111:             {msg.text}
112:             {msg.data && (
113:               <Box sx={{ mt: 1, p: 1, bgcolor: 'rgba(0,0,0,0.1)', borderRadius: 1 }}>
114:                 {msg.data.userId && <div>Account: {msg.data.userId}</div>}
115:                 {msg.data.balance && <div>Balance: {msg.data.balance}</div>}
116:               </Box>
117:             )}
118:           </Box>
119:         ))}
120:         <div ref={messagesEndRef} />
121:       </Box>
122:
123:       <Box sx={inputContainerSx}>
124:         <TextField
125:           size="small"
126:           fullWidth
127:           value={input}
128:           onChange={(e) => setInput(e.value)}
129:           onKeyDown={(e) => e.key === 'Enter' && handleSend()}
130:           placeholder="Type a message..."
131:           sx={textFieldSx}
132:         />
133:         <IconButton color="primary" onClick={handleSend}>
134:           <Send />
135:         </IconButton>
136:       </Box>
137:     </Paper>
138:   );
139: };
140:
141:  export default ChatAssistant;
142:
```

### Reconnection Analysis:

**Line 42-44:** Socket.IO connection configuration
- **`io()` call:** Creates socket connection to `/chat` namespace
- **`withCredentials: true`:** Sends cookies (including JWT cookie) with handshake
- **Missing config:** No explicit `autoConnect`, `reconnection`, `reconnectionAttempts`, or `reconnectionDelay` options
- **Default behavior:** Socket.IO client defaults:
  - `autoConnect: true` (connects immediately)
  - `reconnection: true` (will attempt to reconnect on disconnect)
  - `reconnectionAttempts: Infinity` (keeps retrying)
  - `reconnectionDelay: 1000` (1 second initial delay)

**Line 46-55:** `connect_error` handler
- **Trigger:** Fired when connection fails (including auth failures)
- **Action:** 
  - Adds error message to UI (Lines 47-53)
  - **Manually disconnects socket** (Line 54: `socketRef.current?.disconnect()`)
- **Effect:** Calling `disconnect()` prevents Socket.IO's automatic reconnection
- **Problem:** After JWT expiry, if socket tries to reconnect (e.g., network blip), it will fail auth again, trigger `connect_error`, disconnect, and potentially retry in a loop

**Line 37-67:** `useEffect` hook
- **Dependency array:** `[]` (empty) - runs once on component mount
- **Condition:** Only creates socket if `authStorage.isAuthenticated()` is true
- **Cleanup:** Disconnects socket on component unmount (Lines 61-66)
- **Issue:** Socket is created once and never recreated if auth state changes

**Reconnection Behavior Summary:**
1. **On initial connection failure (JWT expired):**
   - `connect_error` fires → shows error message → manually disconnects
   - Socket.IO's default reconnection is **disabled** by manual disconnect
   - Socket will NOT automatically retry

2. **If socket disconnects for other reasons (network):**
   - Socket.IO will attempt to reconnect (default behavior)
   - On reconnect, auth middleware runs again
   - If JWT still expired → `connect_error` → manual disconnect → stops retrying

3. **After user re-logs in:**
   - New JWT cookie is set
   - **Socket is NOT recreated** (useEffect dependency is `[]`)
   - User must refresh page or component must remount to get new socket connection

---

## 2. Server-Side: `server/src/socket/socket.handler.js`

### Full Code (Lines 1-52):

```javascript
1:  import { processMessage } from '../services/chatbot.service.js';
2:  import { authenticateSocket } from '../middleware/socketAuth.middleware.js';
3:
4:  const getTimestamp = () => new Date().toISOString();
5:
6:  /** Message sent when a client first connects to the chat. */
7:  const INITIAL_GREETING = 'Hello! I\'m your virtual banking assistant 🏦';
8:
9:  /** Message sent when processing a user message fails. */
10: const ERROR_FALLBACK_MESSAGE = 'Sorry, something went wrong. Please try again.';
11:
12: /**
13:  * Sets up the /chat Socket.IO namespace: auth on connect, then greeting; user-message -> processMessage -> bot-message.
14:  */
15: export const initChatbotSocket = (io) => {
16:   const chatNamespace = io.of('/chat');
17:   chatNamespace.use(authenticateSocket);
18:
19:   chatNamespace.on('connection', (socket) => {
20:     const userId = socket.user.id;
21:
22:     socket.emit('bot-message', {
23:       response: INITIAL_GREETING,
24:       intent: 'greeting',
25:       timestamp: getTimestamp(),
26:     });
28:     socket.on('user-message', async (message) => {
29:       try {
30:         const context = { userId };
31:         const result = await processMessage(message, context);
32:         socket.emit('bot-message', {
33:           response: result.message,
34:           intent: result.intent,
35:           data: result.data ?? null,
36:           requiresAuth: result.requiresAuth ?? false,
37:           timestamp: getTimestamp(),
38:         });
39:       } catch {
40:         socket.emit('bot-message', {
41:           response: ERROR_FALLBACK_MESSAGE,
42:           intent: 'error',
43:           timestamp: getTimestamp(),
44:         });
45:       }
46:     });
47:
48:     socket.on('disconnect', () => {
49:       // No cleanup needed; user just left the chat
50:     });
51:   });
52: };
```

### Connection Handler Analysis:

**Line 15-17:** Namespace setup
- Creates `/chat` namespace
- Applies `authenticateSocket` middleware (runs BEFORE `connection` event)

**Line 19-51:** `connection` event handler
- **Only executes if auth middleware succeeds** (calls `next()` without error)
- **Line 20:** Extracts `userId` from `socket.user` (set by auth middleware)
- **Line 22-26:** Sends initial greeting immediately on connection

**Event Listeners Registered:**

1. **`user-message` (Lines 28-46):**
   - Listens for user messages
   - Uses `userId` from connection time (Line 30)
   - Calls `processMessage()` with user context
   - Emits `bot-message` response
   - **No token re-verification** - uses `userId` from initial connection

2. **`disconnect` (Lines 48-50):**
   - Empty handler - no cleanup logic
   - **No resource cleanup** (no user tracking, no room leaving, etc.)
   - Socket.IO automatically cleans up socket reference

**Missing Event Handlers:**
- No `disconnecting` handler (fires before disconnect)
- No `error` handler (socket-level errors)
- No periodic token validation
- No forced disconnect on token expiry

---

## 3. Socket Auth Middleware: `server/src/middleware/socketAuth.middleware.js`

### Full Code (Lines 1-47):

```javascript
1:  import User from '../models/user.model.js';
2:  import { USER_STATUS } from '../constants/index.js';
3:  import { verifyToken } from '../utils/jwt.util.js';
4:
5:  /**
6:   * Extracts the "token" cookie value from the Cookie header string (e.g. "token=abc123; other=...").
7:   */
8:  function getTokenFromCookie(cookieHeader) {
9:    if (!cookieHeader || typeof cookieHeader !== 'string') return null;
10:   const match = cookieHeader.match(/\btoken=([^;]+)/);
11:   return match ? decodeURIComponent(match[1].trim()) : null;
12: }
13:
14: /**
15:  * Socket.io auth middleware: requires a valid JWT (from socket.handshake.auth.token or cookie),
16:  * loads the user, checks they are active, then sets socket.user and calls next(). Otherwise calls next(error).
17:  */
18: export const authenticateSocket = async (socket, next) => {
19:   try {
20:     const cookieHeader = socket.handshake.headers?.cookie;
21:     const token = socket.handshake.auth?.token ?? getTokenFromCookie(cookieHeader);
22:
23:     if (!token) {
24:       return next(new Error('Authentication token is required'));
25:     }
25:
27:     const decoded = verifyToken(token);
28:     const user = await User.findOne({ id: decoded.id }).select('id email status');
29:
30:     if (!user) {
31:       return next(new Error('User not found'));
32:     }
33:     if (user.status !== USER_STATUS.ACTIVE) {
34:       return next(new Error('User account is not active'));
35:     }
35:
37:     socket.user = user;
38:     next();
39:   } catch (error) {
40:     const isJwtError =
41:       error.name === 'JsonWebTokenError' || error.name === 'TokenExpiredError';
42:     if (isJwtError) {
43:       return next(new Error('Invalid or expired authentication token'));
44:     }
45:     return next(new Error('Authentication failed'));
46:   }
47: };
```

### Exports Analysis:

**Exported:**
- `authenticateSocket` (Line 18) - main middleware function

**Private (Not Exported):**
- `getTokenFromCookie` (Line 8) - helper function, only used internally

**Token Extraction (Line 20-21):**
1. Checks `socket.handshake.auth?.token` (manual token pass)
2. Falls back to `getTokenFromCookie(cookieHeader)` from `socket.handshake.headers?.cookie`

**Error Handling (Lines 39-46):**
- Catches JWT errors (`JsonWebTokenError`, `TokenExpiredError`)
- Calls `next(new Error(...))` which prevents connection
- Client receives `connect_error` event with error message

**When JWT Expires:**
- **On initial connection:** Middleware catches `TokenExpiredError` → calls `next(error)` → connection rejected → client gets `connect_error`
- **After connection:** Middleware does NOT run again - socket remains connected even if token expires later

---

## 4. Logout: `server/src/controllers/auth.controller.js`

### Logout Function (Lines 132-141):

```javascript
132: /** Clears the auth cookie and returns 200. */
133: export const logout = (req, res) => {
134:   res.clearCookie(config.cookie.tokenName, {
135:     path: '/',
136:     httpOnly: true,
137:     secure: config.cookie.secure,
138:     sameSite: config.cookie.sameSite,
139:   });
140:   return response.ok(res, null);
141: };
```

### Socket Disconnect Analysis:

**What Happens:**
- Clears JWT cookie (Lines 134-139)
- Returns 200 OK
- **No socket disconnect logic**

**Active Socket Behavior:**
- Socket remains connected (cookie cleared but socket already authenticated)
- `socket.user` still set from initial connection
- Socket can continue sending/receiving messages
- **Security issue:** User can still use socket after logout until:
  1. Socket disconnects naturally (network issue, page close)
  2. Socket tries to reconnect (will fail auth on reconnect)
  3. Server restarts

**Missing Implementation:**
- No socket tracking (no map of `userId` → `socket[]`)
- No forced disconnect on logout
- No way to disconnect user's sockets from HTTP endpoint

**To Fix:** Would need to:
1. Track sockets per user (e.g., `Map<userId, Set<socket>>`)
2. Export socket namespace from `socket.handler.js`
3. In logout, find user's sockets and call `socket.disconnect()`

---

## Summary: Reconnection Flow from Token Expiry

### Scenario 1: Token Expires BEFORE Socket Connection

**Flow:**
1. User opens chat → `ChatAssistant` component mounts
2. `useEffect` runs → checks `authStorage.isAuthenticated()` → true (user still in localStorage)
3. Socket.IO attempts connection → sends cookie with expired JWT
4. **Server:** `authenticateSocket` middleware runs → `verifyToken()` throws `TokenExpiredError`
5. **Server:** Calls `next(new Error('Invalid or expired authentication token'))`
6. **Server:** Connection rejected (no `connection` event fired)
7. **Client:** `connect_error` event fires
8. **Client:** Handler (Line 46-55) shows error message → calls `disconnect()`
9. **Result:** Socket disconnected, no automatic reconnection (manual disconnect prevents it)

**User Experience:**
- Error message: "Authentication failed. Please log in again to use the chat assistant."
- Socket stays disconnected
- User must refresh page and re-login to reconnect

---

### Scenario 2: Token Expires AFTER Socket Connected

**Flow:**
1. User connects successfully (valid JWT)
2. Socket authenticated → `socket.user` set → connection handler runs
3. User chats normally
4. **Time passes** → JWT expires (1 hour default)
5. **Socket remains connected** (auth middleware only runs on initial connection)
6. User sends message → `user-message` handler processes it using `userId` from connection time
7. **No token re-verification** → message processed successfully
8. Socket continues working indefinitely

**User Experience:**
- Socket works normally even after token expiry
- HTTP requests fail with 401 (token expired)
- Socket bypasses expiry check

**Security Issue:**
- User can use socket indefinitely after token expires
- Only reconnection would trigger new auth check

---

### Scenario 3: Socket Disconnects (Network Issue) After Token Expiry

**Flow:**
1. Socket connected with valid JWT
2. Token expires
3. Network blip → socket disconnects
4. **Socket.IO default:** Attempts automatic reconnection
5. Reconnection attempt → sends expired JWT cookie
6. **Server:** `authenticateSocket` middleware runs → `TokenExpiredError`
7. **Server:** Connection rejected
8. **Client:** `connect_error` fires
9. **Client:** Handler calls `disconnect()` → stops reconnection attempts
10. **Result:** Socket stays disconnected

**User Experience:**
- Socket disconnects
- Reconnection fails silently (error handler shows message but doesn't retry)
- User must refresh page and re-login

---

### Scenario 4: User Logs Out While Socket Connected

**Flow:**
1. Socket connected and authenticated
2. User calls `/api/v1/auth/logout`
3. **Server:** Clears JWT cookie
4. **Server:** Returns 200 OK
5. **Socket:** Remains connected (already authenticated)
6. **Client:** `authStorage.clearAuth()` called (from HTTP interceptor)
7. **Socket:** Still functional - can send/receive messages
8. **Only disconnects if:**
   - User closes page/component unmounts
   - Network disconnects → reconnection fails (no cookie)
   - Server restarts

**User Experience:**
- HTTP requests fail (no cookie)
- Socket continues working
- Inconsistent state

---

## Key Findings

1. **No explicit reconnection config** - uses Socket.IO defaults (`reconnection: true`)
2. **Manual disconnect in `connect_error`** - prevents automatic reconnection after auth failure
3. **Socket created once** - `useEffect` dependency `[]` means socket never recreated on auth change
4. **No token re-verification** - socket remains active after token expiry
5. **No logout disconnect** - active sockets remain connected after logout
6. **`getTokenFromCookie` is private** - not exported, only used internally

## Recommendations

1. **Disable reconnection on auth errors:**
   ```typescript
   socketRef.current = io(`${SOCKET_URL}/chat`, {
     withCredentials: true,
     autoConnect: true,
     reconnection: true,
     reconnectionAttempts: 5, // Limit retries
   });
   
   socketRef.current.on('connect_error', (error) => {
     if (error.message.includes('expired') || error.message.includes('Authentication')) {
       socketRef.current?.disconnect(); // Stop retrying on auth errors
       // Show login prompt
     }
   });
   ```

2. **Recreate socket on auth change:**
   - Add `authStorage.isAuthenticated()` to `useEffect` dependency
   - Or listen to auth state changes and recreate socket

3. **Add token re-verification:**
   - Periodically check token expiry in socket handler
   - Disconnect socket if token expired

4. **Track sockets for logout:**
   - Maintain `userId` → `socket[]` mapping
   - Disconnect user's sockets on logout
