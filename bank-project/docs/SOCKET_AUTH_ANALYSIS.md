# Socket.IO JWT Authentication Analysis

## A) HTTP Auth Flow

### 1. JWT Creation/Signing (Login/Register)

**File:** `server/src/utils/jwt.util.js`
- **Function:** `createToken(user)`
  - Lines 5-14: Creates JWT with `{ id: user.id, email: user.email }`
  - Uses `config.jwt.secret` and `config.jwt.expiresIn` (default: '1h')
  - Throws `AppError` if user missing id/email

**File:** `server/src/controllers/auth.controller.js`
- **Function:** `login(req, res)` (Lines 88-130)
  - Line 101: Calls `createToken(user)` after password validation
  - Lines 102-108: Sets HTTP-only cookie via `res.cookie(config.cookie.tokenName, token, {...})`
  - Cookie config: `httpOnly: true`, `secure` (production), `sameSite`, `maxAge: 3600s`
  - Returns user object (without password) in response body

**Note:** Registration (`signup`) does NOT create JWT - user must verify email first, then login.

### 2. Token Storage/Transmission

**Server-side (Cookie):**
- **File:** `server/src/controllers/auth.controller.js` (Lines 102-108)
  - Token stored in HTTP-only cookie named `token` (from `config.cookie.tokenName`)
  - Cookie sent automatically with requests (same-origin or cross-origin with credentials)

**Client-side:**
- **File:** `client/src/api/auth.storage.ts`
  - Lines 4-25: Only stores user object in `localStorage` (NOT the JWT)
  - JWT is NEVER stored client-side - only in HTTP-only cookie
  - `clearAuth()` removes user from localStorage (cookie cleared by server on logout)

**File:** `client/src/api/http-client.ts`
- Line 12: `withCredentials: true` ensures cookies are sent with axios requests

### 3. /me Endpoint (Current User)

**Route:**
- **File:** `server/src/routes/user.routes.js`
  - Line 11: `router.use(protect)` - all routes protected
  - Line 12: `router.get('/me', getCurrentUser)`
  - Mounted at `/api/v1` (from `server/src/app.js` line 33)
  - Full path: `GET /api/v1/me`

**Controller:**
- **File:** `server/src/controllers/user.controller.js`
  - **Function:** `getCurrentUser(req, res)` (Lines 10-41)
  - Line 12: Uses `req.user.id` (set by `protect` middleware)
  - Calls `findUserById(userId)` and `findRecentTransactions(user.email, 10)`
  - Returns user data + recent transactions

**Auth Middleware:**
- **File:** `server/src/middleware/auth.middleware.js`
  - **Function:** `protect(req, res, next)` (Lines 18-39)
  - Lines 7-16: `getTokenFromRequest(req)` extracts token from:
    1. Cookie: `req.cookies?.token` (preferred)
    2. Authorization header: `Bearer <token>`
  - Line 23: Verifies token via `verifyToken(token)` (from `jwt.util.js`)
  - Lines 24-28: Loads user from DB, checks `USER_STATUS.ACTIVE`
  - Line 31: Sets `req.user = user` for downstream handlers
  - Returns 401 on any failure (no token, invalid token, expired token, inactive user)

### 4. Frontend 401 Handling

**File:** `client/src/api/http-client.ts`
- **Lines 15-25:** Axios response interceptor
  - Line 18: Checks `err.response?.status === 401`
  - Line 19: Calls `authStorage.clearAuth()` (removes user from localStorage)
  - Does NOT redirect automatically - components must handle redirects

**File:** `client/src/api/auth.storage.ts`
- **Function:** `clearAuth()` (Lines 14-16)
  - Removes 'user' from localStorage
  - Note: Cookie is cleared server-side via `/api/v1/auth/logout` endpoint

---

## B) Socket.IO Flow

### 1. Client Socket Connection

**File:** `client/src/components/ChatAssistant.tsx`
- **Lines 37-67:** `useEffect` hook sets up socket connection
- **Line 38:** Checks `authStorage.isAuthenticated()` before connecting
- **Line 42:** `io(\`${SOCKET_URL}/chat\`, { withCredentials: true })`
  - Connects to `/chat` namespace
  - `withCredentials: true` sends cookies (including JWT cookie)
- **Lines 46-55:** `connect_error` handler
  - Shows auth failure message
  - Disconnects socket on auth error

**Note:** Client does NOT explicitly pass token in `auth` object - relies on cookie.

### 2. Server Socket Setup

**File:** `server/server.js`
- **Lines 23-29:** Creates Socket.IO server
  - Attached to HTTP server
  - CORS: `origin: config.clientUrl`, `credentials: true`
- **Line 31:** Calls `initChatbotSocket(io)`

**File:** `server/src/socket/socket.handler.js`
- **Function:** `initChatbotSocket(io)` (Lines 15-52)
  - Line 16: Creates namespace: `io.of('/chat')`
  - Line 17: Applies auth middleware: `chatNamespace.use(authenticateSocket)`
  - Line 19: `connection` handler (only reached if auth succeeds)
  - Line 20: Uses `socket.user.id` (set by auth middleware)

### 3. Socket Auth Middleware

**File:** `server/src/middleware/socketAuth.middleware.js`
- **Function:** `authenticateSocket(socket, next)` (Lines 18-47)
- **Token extraction (Lines 20-21):**
  1. Checks `socket.handshake.auth?.token` (manual token pass)
  2. Falls back to `getTokenFromCookie(cookieHeader)` from `socket.handshake.headers?.cookie`
- **Lines 23-25:** Returns error if no token found
- **Line 27:** Verifies token via `verifyToken(token)` (same as HTTP middleware)
- **Lines 28-35:** Loads user from DB, checks `USER_STATUS.ACTIVE`
- **Line 37:** Sets `socket.user = user` (NOT `socket.data.user`)
- **Lines 39-46:** Error handling
  - Catches JWT errors (`JsonWebTokenError`, `TokenExpiredError`)
  - Calls `next(new Error(...))` which prevents connection

**Note:** When `next(error)` is called, Socket.IO prevents the connection and emits `connect_error` to client.

### 4. User Identity Usage in Socket Handlers

**File:** `server/src/socket/socket.handler.js`
- **Line 20:** `const userId = socket.user.id` (extracted once on connect)
- **Line 30:** Passes `{ userId }` to `processMessage(message, context)`
- **Lines 28-46:** `user-message` handler uses `userId` for all message processing

---

## C) Where Identity is Required (Protected Actions)

### Chatbot Service

**File:** `server/src/services/chatbot.service.js`

**Sensitive Intents (Line 19):**
- `SENSITIVE_INTENTS = ['balance']` - requires authentication

**Authorization Check:**
- **Function:** `isAuthorizedForIntent(intent, userId)` (Lines 60-61)
  - Returns `false` if intent is sensitive AND `userId` is null/undefined
  - Returns `true` for non-sensitive intents OR if userId exists

**Protected Handler:**
- **Function:** `handleBalanceIntent(userId)` (Lines 68-85)
  - Line 70: Calls `getAccountSummary(userId)` (from `user.service.js`)
  - Returns masked user ID and formatted balance
  - **File:** `server/src/services/user.service.js` (Lines 32-40)
    - `getAccountSummary(userId)` queries User model by `id`
    - Returns `{ userId, balance }` - USER-SPECIFIC DATA

**Message Processing:**
- **Function:** `processMessage(message, context)` (Lines 99-113)
  - Line 100: Extracts `userId` from context
  - Line 104: Checks `isAuthorizedForIntent(intent, userId)`
  - Line 107: Returns "Please log in" message if unauthorized
  - Line 111: Calls `handleBalanceIntent(userId)` for balance queries

**Current Intents:**
- `GREETING`, `HELP`, `GOODBYE`, `UNKNOWN` - NO auth required
- `BALANCE` - REQUIRES auth (userId must exist)

---

## D) What Breaks if Socket Auth is Removed

### Immediate Failures:

1. **Connection Handler Crash:**
   - **File:** `server/src/socket/socket.handler.js` (Line 20)
   - `const userId = socket.user.id` → **ReferenceError: socket.user is undefined**
   - Connection handler never executes

2. **Balance Queries Fail:**
   - **File:** `server/src/services/chatbot.service.js` (Line 111)
   - `handleBalanceIntent(userId)` called with `undefined`
   - **File:** `server/src/services/user.service.js` (Line 33)
   - `User.findOne({ id: undefined })` → returns `null`
   - **Line 36:** Throws `AppError('User not found', 404)`
   - Chatbot returns error message (Line 82)

3. **Authorization Check Always Fails:**
   - **File:** `server/src/services/chatbot.service.js` (Line 104)
   - `isAuthorizedForIntent(intent, undefined)` → returns `false` for balance
   - Users get "Please log in" message even if they ARE logged in

### Security Implications:

1. **Direct Socket Connection Without UI:**
   - **File:** `server/server.js` (Lines 23-29)
   - CORS allows `config.clientUrl` origin, but Socket.IO can be connected from ANY origin if:
     - Attacker knows server URL
     - CORS misconfigured (e.g., `origin: '*'`)
   - **Current protection:** `authenticateSocket` middleware blocks unauthenticated connections
   - **Without auth:** Anyone can connect and query balance (if they guess userId) OR crash the handler

2. **User Enumeration:**
   - Without auth, attacker could try different userIds in balance queries
   - Current code prevents this via auth middleware

---

## E) Token Expiry Scenarios

### 1. Token Expires BEFORE /me Call

**Scenario:** User logged in 2 hours ago, token expired, tries to access `/api/v1/me`

**Flow:**
- **File:** `server/src/middleware/auth.middleware.js` (Line 23)
- `verifyToken(token)` throws `TokenExpiredError`
- **Line 37:** Returns 401
- **File:** `client/src/api/http-client.ts` (Line 18-19)
- Interceptor calls `authStorage.clearAuth()`
- Frontend should redirect to login (handled by components)

**Expected:** ✅ Works correctly - user must re-login

### 2. Token Expires AFTER Socket Connected

**Scenario:** User connects to socket, token expires 30 minutes later, user sends message

**Current Behavior:**
- **File:** `server/src/middleware/socketAuth.middleware.js`
- Auth middleware runs ONLY on connection (Line 17: `chatNamespace.use(authenticateSocket)`)
- **NOT re-verified** on subsequent `user-message` events
- Socket remains connected, `socket.user` still set from initial connection
- **File:** `server/src/socket/socket.handler.js` (Line 30)
- `userId` extracted once on connect (Line 20), used for all messages
- **Result:** Expired token NOT detected - socket continues working

**Problem:** User can continue using socket indefinitely even after token expires.

**What SHOULD Happen:**
- Option A: Disconnect socket when token expires (requires periodic re-auth check)
- Option B: Re-verify token on each message (performance overhead)
- Option C: Use shorter-lived socket tokens (separate from HTTP JWT)

**Current Code:** ❌ No expiry detection after connection

### 3. Proposed Minimal Fix

**Option 1: Disconnect on Auth Failure (Recommended)**
```javascript
// In socket.handler.js, add periodic token check
socket.on('user-message', async (message) => {
  try {
    // Re-verify token (lightweight - just decode, no DB query)
    const token = getTokenFromCookie(socket.handshake.headers?.cookie);
    if (token) {
      try {
        verifyToken(token); // Throws if expired
      } catch (err) {
        if (err.name === 'TokenExpiredError') {
          socket.emit('bot-message', {
            response: 'Your session has expired. Please refresh the page.',
            intent: 'error',
            timestamp: getTimestamp(),
          });
          socket.disconnect();
          return;
        }
      }
    }
    // ... rest of handler
  } catch { ... }
});
```

**Files to modify:**
- `server/src/socket/socket.handler.js` - add token re-verification
- `server/src/middleware/socketAuth.middleware.js` - export `getTokenFromCookie` helper

---

## F) Alternatives to JWT for Socket Auth

### Option A: Cookie-Based Session (No JWT)

**What Changes:**

1. **Session Storage:**
   - Replace JWT with session ID stored in cookie
   - Store session data in Redis/Memory store
   - Session contains `userId`, `email`, `status`

2. **Files to Modify:**
   - `server/src/utils/jwt.util.js` → `server/src/utils/session.util.js`
     - `createSession(user)` → returns sessionId
     - `getSession(sessionId)` → returns user data
   - `server/src/controllers/auth.controller.js`
     - `login()`: Create session, set sessionId cookie
   - `server/src/middleware/auth.middleware.js`
     - `protect()`: Read sessionId from cookie, load session
   - `server/src/middleware/socketAuth.middleware.js`
     - `authenticateSocket()`: Read sessionId from cookie, load session
   - `server/src/controllers/auth.controller.js`
     - `logout()`: Destroy session

3. **Pros:**
   - Server can invalidate sessions immediately (logout, security breach)
   - No token expiry issues (server controls expiry)
   - Simpler token management

4. **Cons:**
   - Requires session store (Redis/Memory)
   - Not stateless (scaling considerations)
   - More server-side state

**Minimal Diff:**
- Add session store dependency
- Replace JWT create/verify with session create/get
- Update both HTTP and Socket middleware

---

### Option B: Short-Lived Socket Token (Issued from /me)

**What Changes:**

1. **New Endpoint:**
   - `GET /api/v1/me/socket-token` → returns short-lived token (5-15 min)
   - Client calls this before connecting socket
   - Passes token in `socket.handshake.auth.token`

2. **Files to Modify:**
   - `server/src/controllers/user.controller.js`
     - Add `getSocketToken(req, res)` handler
     - Creates JWT with shorter expiry (`expiresIn: '10m'`)
   - `server/src/routes/user.routes.js`
     - Add `router.get('/me/socket-token', getSocketToken)`
   - `client/src/components/ChatAssistant.tsx`
     - Before `io()`, call `/api/v1/me/socket-token`
     - Pass token: `io(url, { auth: { token } })`
   - `server/src/middleware/socketAuth.middleware.js`
     - Already supports `socket.handshake.auth.token` (Line 21)

3. **Pros:**
   - Separate expiry for socket (shorter = more secure)
   - Can refresh socket token without re-login
   - Keeps JWT architecture

4. **Cons:**
   - Extra HTTP call before socket connect
   - Still has expiry issue (just shorter window)
   - More complex client code

**Minimal Diff:**
- Add socket-token endpoint
- Update client to fetch and pass token
- No changes to socket middleware (already supports auth.token)

---

### Option C: No Auth (Public Chatbot)

**Feasibility Check:**

**File:** `server/src/services/chatbot.service.js`
- **Line 19:** `SENSITIVE_INTENTS = ['balance']`
- **Line 60-61:** `isAuthorizedForIntent()` allows non-sensitive intents without userId
- **Current intents:** `GREETING`, `HELP`, `GOODBYE`, `UNKNOWN` work without auth

**What Would Work:**
- General chatbot responses (greeting, help, goodbye)
- No balance queries
- No user-specific data

**What Would Break:**
- **File:** `server/src/socket/socket.handler.js` (Line 20)
- `socket.user.id` → undefined → crashes handler
- Must modify to handle missing user

**Required Changes:**
- Remove `chatNamespace.use(authenticateSocket)` from `socket.handler.js`
- Change Line 20: `const userId = socket.user?.id ?? null`
- Change Line 30: `const context = { userId: userId ?? null }`
- Balance queries would return "Please log in" (Line 107)

**Security:**
- ✅ Safe if balance queries are disabled
- ❌ Unsafe if balance queries remain (anyone can query any userId)

**Verdict:** Only viable if balance queries are removed OR moved to HTTP-only endpoint.

---

## Summary

**Why JWT is Needed:**
1. Balance queries require `userId` → `getAccountSummary(userId)` → user-specific data
2. Socket handler extracts `socket.user.id` on connect → crashes if undefined
3. Authorization check prevents unauthorized balance access
4. Security: Prevents direct socket connections without authentication

**Current Gap:**
- Token expiry NOT detected after socket connection
- Socket remains active even after JWT expires

**Recommended Fix:**
- Add token re-verification in `user-message` handler
- Disconnect socket on `TokenExpiredError`
- OR implement Option B (short-lived socket tokens) for better UX
