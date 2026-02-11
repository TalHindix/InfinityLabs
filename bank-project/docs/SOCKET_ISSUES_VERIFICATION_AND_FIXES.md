# Socket.IO Auth & Reconnection — Code Review & Fix Proposal

## Issue 1 (Critical): No token re-verification after socket connection

**Status:** ✅ **CONFIRMED**

**Evidence:**
- `server/src/socket/socket.handler.js` Line 22: `userId` extracted once on connection
- `server/src/socket/socket.handler.js` Lines 30-48: `user-message` handler uses `userId` from connection time (Line 32: `const context = { userId }`)
- No token verification in `user-message` handler
- `getTokenFromCookie` is already exported (Line 8 in `socketAuth.middleware.js`)
- `verifyToken` is imported (Line 3) but not used

**Problem:**
Socket remains functional indefinitely after JWT expires. User can continue sending messages even though their HTTP requests would fail with 401.

**Proposed Fix:**

### File: `server/src/socket/socket.handler.js`

**Before:**
```javascript
import { processMessage } from '../services/chatbot.service.js';
import { authenticateSocket  , getTokenFromCookie } from '../middleware/socketAuth.middleware.js';
import { verifyToken } from '../utils/jwt.util.js';

// ... existing code ...

socket.on('user-message', async (message) => {
  try {
    const context = { userId };
    const result = await processMessage(message, context);
    socket.emit('bot-message', {
      response: result.message,
      intent: result.intent,
      data: result.data ?? null,
      requiresAuth: result.requiresAuth ?? false,
      timestamp: getTimestamp(),
    });
  } catch {
    socket.emit('bot-message', {
      response: ERROR_FALLBACK_MESSAGE,
      intent: 'error',
      timestamp: getTimestamp(),
    });
  }
});
```

**After:**
```javascript
import { processMessage } from '../services/chatbot.service.js';
import { authenticateSocket, getTokenFromCookie } from '../middleware/socketAuth.middleware.js';
import { verifyToken } from '../utils/jwt.util.js';

// ... existing code ...

socket.on('user-message', async (message) => {
  try {
    // Re-verify token on each message
    const cookieHeader = socket.handshake.headers?.cookie;
    const token = getTokenFromCookie(cookieHeader);
    
    if (!token) {
      socket.emit('bot-message', {
        response: 'Your session has expired. Please refresh the page and log in again.',
        intent: 'error',
        timestamp: getTimestamp(),
      });
      socket.disconnect();
      return;
    }

    try {
      verifyToken(token); // Throws if expired or invalid
    } catch (error) {
      if (error.name === 'TokenExpiredError' || error.name === 'JsonWebTokenError') {
        socket.emit('bot-message', {
          response: 'Your session has expired. Please refresh the page and log in again.',
          intent: 'error',
          timestamp: getTimestamp(),
        });
        socket.disconnect();
        return;
      }
      throw error; // Re-throw non-JWT errors
    }

    const context = { userId };
    const result = await processMessage(message, context);
    socket.emit('bot-message', {
      response: result.message,
      intent: result.intent,
      data: result.data ?? null,
      requiresAuth: result.requiresAuth ?? false,
      timestamp: getTimestamp(),
    });
  } catch {
    socket.emit('bot-message', {
      response: ERROR_FALLBACK_MESSAGE,
      intent: 'error',
      timestamp: getTimestamp(),
    });
  }
});
```

**Note:** `getTokenFromCookie` is already exported, so no changes needed to `socketAuth.middleware.js`.

---

## Issue 2 (Critical): Logout doesn't disconnect active sockets

**Status:** ✅ **CONFIRMED**

**Evidence:**
- `server/src/controllers/auth.controller.js` Lines 133-141: `logout` function only clears cookie, no socket references
- `server/src/socket/socket.handler.js`: No socket tracking mechanism (no Map of userId → sockets)
- `server/src/routes/auth.routes.js` Line 18: `router.post('/logout', logout)` - **NO `protect` middleware**, so `req.user` won't exist

**Problem:**
1. Active sockets remain connected after logout
2. Logout route doesn't use `protect` middleware, so `req.user.id` is unavailable
3. No way to identify which sockets belong to a user

**Proposed Fix:**

### File: `server/src/socket/socket.handler.js`

**Before:**
```javascript
export const initChatbotSocket = (io) => {
  const chatNamespace = io.of('/chat');
  chatNamespace.use(authenticateSocket);

  chatNamespace.on('connection', (socket) => {
    const userId = socket.user.id;
    // ... rest of handler
  });
};
```

**After:**
```javascript
import { processMessage } from '../services/chatbot.service.js';
import { authenticateSocket, getTokenFromCookie } from '../middleware/socketAuth.middleware.js';
import { verifyToken } from '../utils/jwt.util.js';

const getTimestamp = () => new Date().toISOString();

/** Message sent when a client first connects to the chat. */
const INITIAL_GREETING = 'Hello! I\'m your virtual banking assistant 🏦';

/** Message sent when processing a user message fails. */
const ERROR_FALLBACK_MESSAGE = 'Sorry, something went wrong. Please try again.';

// Track active sockets per user: userId → Set<socket>
const activeSockets = new Map();

/**
 * Disconnects all sockets for a given user ID.
 * @param {string} userId - User ID to disconnect
 */
export const disconnectUser = (userId) => {
  const sockets = activeSockets.get(userId);
  if (sockets) {
    sockets.forEach((socket) => {
      socket.emit('bot-message', {
        response: 'You have been logged out. Please refresh the page.',
        intent: 'error',
        timestamp: getTimestamp(),
      });
      socket.disconnect();
    });
    activeSockets.delete(userId);
  }
};

/**
 * Sets up the /chat Socket.IO namespace: auth on connect, then greeting; user-message -> processMessage -> bot-message.
 */
export const initChatbotSocket = (io) => {
  const chatNamespace = io.of('/chat');
  chatNamespace.use(authenticateSocket);

  chatNamespace.on('connection', (socket) => {
    const userId = socket.user.id;

    // Track socket
    if (!activeSockets.has(userId)) {
      activeSockets.set(userId, new Set());
    }
    activeSockets.get(userId).add(socket);

    socket.emit('bot-message', {
      response: INITIAL_GREETING,
      intent: 'greeting',
      timestamp: getTimestamp(),
    });

    socket.on('user-message', async (message) => {
      // ... (token re-verification code from Issue 1)
    });

    socket.on('disconnect', () => {
      // Remove socket from tracking
      const sockets = activeSockets.get(userId);
      if (sockets) {
        sockets.delete(socket);
        if (sockets.size === 0) {
          activeSockets.delete(userId);
        }
      }
    });
  });
};
```

### File: `server/src/routes/auth.routes.js`

**Before:**
```javascript
router.post('/logout', logout);
```

**After:**
```javascript
import { protect } from '../middleware/auth.middleware.js';

// ... existing routes ...

router.post('/logout', protect, logout);
```

### File: `server/src/controllers/auth.controller.js`

**Before:**
```javascript
/** Clears the auth cookie and returns 200. */
export const logout = (req, res) => {
  res.clearCookie(config.cookie.tokenName, {
    path: '/',
    httpOnly: true,
    secure: config.cookie.secure,
    sameSite: config.cookie.sameSite,
  });
  return response.ok(res, null);
};
```

**After:**
```javascript
import { disconnectUser } from '../socket/socket.handler.js';

// ... existing code ...

/** Clears the auth cookie, disconnects active sockets, and returns 200. */
export const logout = (req, res) => {
  const userId = req.user.id;
  
  // Disconnect all sockets for this user
  disconnectUser(userId);
  
  res.clearCookie(config.cookie.tokenName, {
    path: '/',
    httpOnly: true,
    secure: config.cookie.secure,
    sameSite: config.cookie.sameSite,
  });
  return response.ok(res, null);
};
```

---

## Issue 3 (Medium): Client doesn't redirect on connect_error

**Status:** ✅ **CONFIRMED**

**Evidence:**
- `client/src/components/ChatAssistant.tsx` Lines 46-55: `connect_error` handler shows error message and disconnects
- Does NOT call `authStorage.clearAuth()`
- Does NOT redirect to login page
- App uses React Router (`react-router-dom` v7.13.0)

**Problem:**
User sees error message but remains on protected page. Auth state not cleared, so UI may show inconsistent state.

**Proposed Fix:**

### File: `client/src/components/ChatAssistant.tsx`

**Before:**
```typescript
import { useState, useEffect, useRef } from 'react';
import { io, Socket } from 'socket.io-client';
// ... other imports ...
import { authStorage } from '../api/auth.storage';

// ... component code ...

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
```

**After:**
```typescript
import { useState, useEffect, useRef } from 'react';
import { useNavigate } from 'react-router-dom';
import { io, Socket } from 'socket.io-client';
// ... other imports ...
import { authStorage } from '../api/auth.storage';
import { ROUTES } from '../constants/routePaths';

const ChatAssistant = () => {
  const navigate = useNavigate();
  // ... existing state ...

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
      authStorage.clearAuth();
      navigate(ROUTES.LOGIN);
    });

    // ... rest of handlers ...
  }, [navigate]);
```

**Note:** Added `navigate` to dependency array (Line 67 → `}, [navigate]);`)

---

## Issue 4 (Medium): Socket not recreated after re-login

**Status:** ✅ **CONFIRMED**

**Evidence:**
- `client/src/components/ChatAssistant.tsx` Line 67: `useEffect` dependency array is `[]` (empty)
- Socket is created once on component mount
- If user logs out and logs back in without page refresh, socket is never recreated

**Problem:**
User must refresh page to reconnect socket after re-login.

**Proposed Fix:**

### File: `client/src/components/ChatAssistant.tsx`

**Before:**
```typescript
useEffect(() => {
  if (!authStorage.isAuthenticated()) {
    return;
  }

  socketRef.current = io(`${SOCKET_URL}/chat`, {
    withCredentials: true,
  });

  // ... handlers ...

  return () => {
    if (socketRef.current) {
      socketRef.current.disconnect();
      socketRef.current = null;
    }
  };
}, []);
```

**After:**
```typescript
useEffect(() => {
  const isAuthenticated = authStorage.isAuthenticated();
  
  if (!isAuthenticated) {
    // Disconnect socket if user logs out
    if (socketRef.current) {
      socketRef.current.disconnect();
      socketRef.current = null;
    }
    return;
  }

  // Create socket if authenticated and not already connected
  if (!socketRef.current || !socketRef.current.connected) {
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
  }

  return () => {
    if (socketRef.current) {
      socketRef.current.disconnect();
      socketRef.current = null;
    }
  };
}, [authStorage.isAuthenticated(), navigate]);
```

**Alternative Simpler Fix (if auth state doesn't change reactively):**

If `authStorage.isAuthenticated()` is not reactive, use a state variable or listen to storage events:

```typescript
useEffect(() => {
  const checkAuthAndConnect = () => {
    if (!authStorage.isAuthenticated()) {
      if (socketRef.current) {
        socketRef.current.disconnect();
        socketRef.current = null;
      }
      return;
    }

    if (!socketRef.current || !socketRef.current.connected) {
      socketRef.current = io(`${SOCKET_URL}/chat`, {
        withCredentials: true,
      });

      // ... handlers ...
    }
  };

  checkAuthAndConnect();

  // Listen for storage changes (when authStorage is updated)
  const handleStorageChange = () => {
    checkAuthAndConnect();
  };
  window.addEventListener('storage', handleStorageChange);

  return () => {
    window.removeEventListener('storage', handleStorageChange);
    if (socketRef.current) {
      socketRef.current.disconnect();
      socketRef.current = null;
    }
  };
}, []);
```

**Note:** The simpler approach may not work if `authStorage` updates are synchronous (localStorage changes don't trigger `storage` event on same tab). Better to use a state management solution or make `authStorage` reactive.

**Recommended:** Use a state variable that updates when login/logout occurs, or use React Context for auth state.

---

## Issue 5 (Minor): Bug in TextField onChange

**Status:** ❌ **NOT AN ISSUE**

**Evidence:**
- `client/src/components/ChatAssistant.tsx` Line 128: `onChange={(e) => setInput(e.target.value)}`
- Uses `e.target.value` correctly, NOT `e.value`

**Verdict:** The claim is incorrect. Code is correct.

---

## Issue 6 (Minor): socket.user vs socket.data.user convention

**Status:** ✅ **CONFIRMED** (Minor - works but not Socket.IO convention)

**Evidence:**
- `server/src/middleware/socketAuth.middleware.js` Line 37: `socket.user = user`
- `server/src/socket/socket.handler.js` Line 22: `const userId = socket.user.id`
- No usage of `socket.data` anywhere in codebase

**Problem:**
Socket.IO documentation recommends using `socket.data` for custom properties. `socket.user` works but is not the official pattern.

**Proposed Fix (Optional):**

### File: `server/src/middleware/socketAuth.middleware.js`

**Before:**
```javascript
socket.user = user;
```

**After:**
```javascript
socket.data.user = user;
```

### File: `server/src/socket/socket.handler.js`

**Before:**
```javascript
const userId = socket.user.id;
```

**After:**
```javascript
const userId = socket.data.user.id;
```

**Note:** This is a minor refactor. The current code works fine, but following Socket.IO conventions improves maintainability and avoids potential conflicts with Socket.IO internals.

---

## Summary

| Issue | Status | Priority | Files to Change |
|-------|--------|----------|----------------|
| 1. No token re-verification | ✅ Confirmed | Critical | `server/src/socket/socket.handler.js` |
| 2. Logout doesn't disconnect sockets | ✅ Confirmed | Critical | `server/src/socket/socket.handler.js`, `server/src/controllers/auth.controller.js`, `server/src/routes/auth.routes.js` |
| 3. Client doesn't redirect on error | ✅ Confirmed | Medium | `client/src/components/ChatAssistant.tsx` |
| 4. Socket not recreated after re-login | ✅ Confirmed | Medium | `client/src/components/ChatAssistant.tsx` |
| 5. TextField onChange bug | ❌ Not an issue | N/A | None |
| 6. socket.user vs socket.data.user | ✅ Confirmed | Minor | `server/src/middleware/socketAuth.middleware.js`, `server/src/socket/socket.handler.js` |

**Total files to modify:** 5 files (4 server-side, 1 client-side)
