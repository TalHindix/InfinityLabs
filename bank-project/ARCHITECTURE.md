---
name: Bank Project Code Review Guide
overview: A comprehensive guide explaining all 5 processes (signup, login, verifyMail, transferMoney, transaction), plus resend verification and logout; client-server architecture (client api/ layer, server routes mounted at /api/v1); MongoDB interactions; middleware; and React hooks with interview-ready explanations.
todos:
  - id: explain-signup
    content: "Explain signup process: client hook → api service → server route → controller → service → MongoDB → email"
    status: pending
  - id: explain-login
    content: "Explain login process: client hook → api service → server route → controller → JWT → HTTP-only cookie; client stores user in localStorage only"
    status: pending
  - id: explain-verify
    content: "Explain email verification: email link → server route → token validation → status update → HTML response"
    status: pending
  - id: explain-transfer
    content: "Explain transfer money: client hook → transactionService → server route → MongoDB transaction → atomic balance update"
    status: pending
  - id: explain-transactions
    content: "Explain transaction listing: client hook → transactionService → server route → MongoDB query with pagination; GET /me returns user + recentTransactions"
    status: pending
  - id: explain-middleware
    content: "Explain all middleware: protect (auth), rateLimit (abuse prevention), error (centralized handling), logger (monitoring)"
    status: pending
  - id: explain-hooks
    content: "Explain React hooks: useState, useEffect, useContext; useAsyncOperation for loading/error; hooks live in screens/*"
    status: pending
  - id: explain-mongodb
    content: "Explain MongoDB interactions: models, transactions, queries, indexes, validation"
    status: pending
isProject: false
---

# Bank Project Code Review Guide

## Overview

This guide explains the complete architecture and flow of your Dubai-Bank project, covering all 5 main processes, client-server communication, MongoDB interactions, middleware, and React hooks.

---

## Architecture Overview

### Tech Stack

- **Client**: 
  - **React 19 + TypeScript + Vite**: React 19 with TypeScript for type safety, Vite for fast builds and hot module replacement (HMR) during development
  - **React Router DOM**: Client-side routing for SPA, includes Protected Routes for securing pages that require authentication
  - **Material-UI (MUI) + Emotion**: Pre-built UI component library with Emotion for advanced styling, saves development time
  - **Axios**: HTTP client with interceptors (see Interceptors explanation below) - automatically adds Authorization headers and handles 401 errors, better than native fetch API
  - **Socket.IO Client**: Real-time bidirectional communication with server for chatbot functionality
- **Server**: 
  - **Node.js + Express 5 (ES Modules)**: Express 5 with ES Modules (import/export) instead of CommonJS, more modern code structure
  - **MongoDB (Mongoose ODM)**: Flexible NoSQL database, Mongoose provides validation, schemas, and transaction support
  - **dotenv**: Environment variable management (.env files) - separates development/production configs, security (keeps secrets out of code)
  - **cookie-parser**: Middleware to parse cookies from HTTP requests - required to read JWT from HTTP-only cookies
  - **cors**: Enables cross-origin requests from client, configured with credentials: true for cookies
  - **express-rate-limit**: Protection against brute-force attacks and abuse - limits requests per IP (5 per 15min for auth, 10 per minute for transfers)
  - **winston**: Professional logging system - structured logs, levels (info/warn/error), useful for debugging and monitoring
- **Auth**: 
  - **JWT (jsonwebtoken)**: Stateless token for user authentication - HTTP-only cookie (XSS protection) + Bearer header (fallback for cross-origin)
  - **bcrypt**: Password hashing - intentionally slow algorithm (10 rounds) to resist brute-force attacks, random salt per password
- **Email**: 
  - **Brevo API (via Axios)**: External email service for sending verification emails - more reliable than direct SMTP, simple API
- **Real-time**: 
  - **Socket.IO (`/chat` namespace)**: Real-time bidirectional communication for chatbot - separate namespace for chat, authentication middleware
- **AI**: 
  - **OpenAI (gpt-4o-mini)**: Fallback mechanism for chatbot - if keyword matching fails, sends to AI for intent detection, cost-effective and fast (gpt-4o-mini)

#### Axios Interceptors Explained

**What are Interceptors?**
Interceptors are functions that Axios calls automatically before sending a request (request interceptor) or after receiving a response (response interceptor). They allow you to modify requests/responses globally without changing each API call.

**Request interceptor** (current project: none):

- The current client does **not** use a request interceptor. Authentication relies on the **HTTP-only cookie** set by the server at login. With `withCredentials: true`, the browser sends the cookie automatically on same-origin requests (and on cross-origin when CORS credentials are allowed). The server `protect` middleware reads the token from the cookie first, then from the `Authorization: Bearer` header if present (for environments where cookies are not sent).

**Response Interceptor** (`httpClient.interceptors.response.use`):

- **When**: Runs AFTER every HTTP response is received
- **What it does**:
  - Success case: Returns response as-is (no modification)
  - Error case: Checks if status is 401 (Unauthorized)
  - If 401: Calls `authStorage.clearAuth()` (removes user from localStorage; cookie is cleared by server on logout or ignored when expired)
  - Logs network errors when no response is received
  - Returns rejected promise (so error handling in components works)
- **Why**:
  - Automatic logout when token expires or is invalid
  - Centralized handling for authentication failures; components can show errors or redirect

**Example Flow**:

```
Component calls: transactionService.getAll(page, limit)
  ↓
Axios sends GET with credentials (cookie sent automatically)
  ↓
Server protect middleware reads token from cookie (or Authorization header)
  ↓
Server responds (200 OK or 401 Unauthorized)
  ↓
Axios response interceptor runs → If 401, clears auth (localStorage user)
  ↓
Response/Error returned to component
```

### Request Flow

This diagram shows the **complete request lifecycle** - how data flows from user interaction in the browser all the way to the database and back. It demonstrates the **layered architecture** and **separation of concerns** in the application.

```
Browser → React Component → Custom Hook → API Service → HTTP Client (Axios) 
  → Express Route → Middleware → Controller → Service → MongoDB
  → Response flows back up
```

**What this shows:**

- **Request Path (Down)**: User action → UI layer → Hook (state + side effects) → API layer → Network → Server routing → Authentication → Controller → Database operations
- **Response Path (Up)**: Database → Service → Controller → Middleware → HTTP response → Client → UI update

**Layer Breakdown:**

1. **Browser**: User clicks button, fills form, etc.
2. **React Component**: UI layer - displays form, handles user input (e.g., SignupForm, LoginForm)
3. **Custom Hook**: React logic layer - manages state, calls API services; lives in screens (e.g., `screens/login-signup/useSignup.ts`, `useLogin.ts`); often uses shared `useAsyncOperation` for loading/error
4. **API Service**: Client API abstraction - defines endpoints (e.g., `client/src/api/auth.service.ts`, `transaction.service.ts`, `user.service.ts`)
5. **HTTP Client (Axios)**: Network layer - `client/src/api/http-client.ts`; baseURL from `VITE_API_URL`; `withCredentials: true`; response interceptor on 401 clears auth
6. **Express Route**: Server routing - routes mounted at `/api/v1/auth`, `/api/v1`, `/api/v1/transactions` (see app.js)
7. **Middleware**: Server processing - protect (auth), rate limit, request logger; error and notFound after routes
8. **Controller**: Request handler - extracts data, validates, calls service, returns response via `response.util`
9. **Service**: Business logic - e.g. `user.service.js` (`createUser`, `findAndVerifyUserByToken`), `transaction.service.js` (`executeTransfer`, `findTransactionsByUserEmail`)
10. **MongoDB**: Database layer - User, Transaction, Counter collections

**Why this architecture?**

- **Separation of Concerns**: Each layer has a single responsibility
- **Maintainability**: Easy to change one layer without affecting others
- **Testability**: Can test each layer independently
- **Scalability**: Can scale different layers separately (e.g., add more servers, optimize database)

---

## Process 1: SIGNUP

### Client-Side Flow

**File**: `client/src/screens/login-signup/useSignup.ts`

1. **useState** - Why?
  - `formData`: Stores form input (firstName, lastName, email, password, phone) via `SignupFormData` type
  - `activeStep`: Tracks UI state (0 = form, 1 = success message)
  - **Interview Answer**: useState manages component state that changes over time. When user types, formData updates, triggering re-render with new values.
2. **useAsyncOperation** (shared hook): Provides `loading`, `error`, and `execute(operation, onSuccess)`. Wraps async calls and sets error on failure.
3. **handleSubmit**:
  - Calls `execute(() => authService.signup(formData), () => setActiveStep(1))`
  - On success: `onSuccess` callback sets `activeStep(1)` to show success message
  - On error: `useAsyncOperation` sets error; component displays it
4. **goBack**: Resets `activeStep` to 0 to return to the form from the success step.

**File**: `client/src/api/auth.service.ts`

- `signup(data)`: POST to `/auth/signup` via `httpClient` (baseURL from `VITE_API_URL` includes `/api/v1`, so full path is `/api/v1/auth/signup`)

**File**: `client/src/api/http-client.ts`

- Axios instance with `baseURL` from `VITE_API_URL`, `withCredentials: true` (sends cookies)
- No request interceptor in current implementation; auth relies on HTTP-only cookie when sent by browser
- Response interceptor: On 401, calls `authStorage.clearAuth()`; logs network errorss

### Server-Side Flow

**File**: `server/src/routes/auth.routes.js`

- Auth routes are mounted at `/api/v1/auth`. Route: `POST /signup` (full path: `POST /api/v1/auth/signup`)
- Middleware: `authLimiter` (rate limiting - 5 requests per 15 min)

**File**: `server/src/middleware/rateLimit.middleware.js`

- **Why authLimiter?** Prevents brute-force attacks and spam signups
- Uses `express-rate-limit` to track requests per IP
- Returns 429 if limit exceeded

**File**: `server/src/controllers/auth.controller.js` → `signup()`

1. Extracts: firstName, lastName, email, phone, password from `req.body`
2. Calls `createUser()` service
3. Sends verification email asynchronously (fire-and-forget)
4. Returns 201 with message

**File**: `server/src/services/user.service.js` → `createUser()`

1. Hashes password with bcrypt (10 salt rounds)
2. Generates verification token (64 hex chars) via `generateVerificationToken()`
3. Hashes token with SHA-256 (stored hashed, sent plain)
4. Creates user in MongoDB with:
  - `status: PENDING` (can't login until verified)
  - `verificationToken: hashedToken`
  - `balance: DEFAULT_BALANCE` (from constants)
5. Returns `{ user, verificationToken }` (plain token for email)

**File**: `server/src/models/user.model.js`

- Mongoose schema with validation:
  - Email: unique, lowercase, regex validation
  - Password: min 8 chars, `select: false` (not returned by default)
  - Phone: regex validation
  - Balance: min 0, default from constants
  - `verificationToken`: optional string

**File**: `server/src/utils/email.util.js`

- `sendVerificationEmailAsync()`: Calls Brevo API
- Builds HTML email with verification link: `/api/v1/auth/verify?token=<plainToken>`
- Async (doesn't block response) - logs errors but doesn't fail request

### MongoDB Interaction

- `User.create()` inserts new document
- Email uniqueness enforced by MongoDB index
- If duplicate email: Mongoose throws 11000 error → caught by error handler → 409 response

---

## Process 2: LOGIN

### Client-Side Flow

**File**: `client/src/screens/login-signup/useLogin.ts`

1. **useState**:
  - `email`, `password`: Form inputs
  - `showVerifiedMsg`: Shows success message if redirected from verification
  - `showResendOption`: Shows resend verification option after failed login
  - `resendSuccess`: Tracks success of resend verification
2. **useAsyncOperation**: Provides `loading`, `error`, `execute`, `setError`. A second instance (`resendAsync`) is used for the resend-verification flow.
3. **useEffect** - Why?
  ```typescript
   useEffect(() => {
     if (searchParams.get('verified') === 'true') {
       setShowVerifiedMsg(true);
       navigate(ROUTES.LOGIN, { replace: true });
     }
   }, [searchParams, navigate]);
  ```
  - **Interview Answer**: useEffect runs after render. Checks URL params for `?verified=true` (from email verification). When found, shows success message. Dependency array ensures it only runs when searchParams/navigate change.
4. **handleSubmit**:
  - Calls `execute(() => authService.login(email, password), (data) => { authStorage.setUser(data.user); navigate(ROUTES.DASHBOARD); })`
  - On success: Stores only **user** in localStorage via `authStorage.setUser(data.user)`; server sets HTTP-only cookie with JWT; navigates to dashboard
  - On error: Uses `getIntelligentErrorMessage(error)` for user-facing message; clears password; sets `showResendOption(true)`
5. **handleResendVerification**: Calls `authService.resendVerification(email)` via `resendAsync.execute`; on success sets `resendSuccess`, on error sets error via `getIntelligentErrorMessage`.
6. **greeting**: `getTimeBasedGreeting()` from `shared/greetings` (called once per render; no useMemo in current code).

**File**: `client/src/api/auth.storage.ts`

- **User only** in localStorage (token is **not** stored on client; server sets HTTP-only cookie at login).
- `setUser(user)`: Stores user object in localStorage
- `getUser()`: Returns parsed user or null
- `clearAuth()`: Removes user from localStorage (called on 401 by response interceptor)
- `logout()`: Alias for `clearAuth()`; server clears cookie via `POST /auth/logout`
- `isAuthenticated()`: Returns `!!getUser()`

### Server-Side Flow

**File**: `server/src/routes/auth.routes.js`

- `POST /login` with `authLimiter`. Also: `POST /logout`, `POST /resend-verification` (see below).

**File**: `server/src/controllers/auth.controller.js` → `login()`

1. Validates email + password exist; throws `AppError` 400 if missing
2. Calls `findUserByEmailWithPassword(email)` - `.select('+password')` includes password field
3. Checks `user` exists and `user.status === USER_STATUS.ACTIVE` (rejects PENDING users with same 401 message)
4. Validates password with `validatePassword(password, user.password)` (bcrypt.compare)
5. Creates JWT with `createToken(user)`
6. Sets HTTP-only cookie via `res.cookie(config.cookie.tokenName, token, ...)`:
  - Cookie name from config (e.g. `token`)
  - `httpOnly: true` (XSS protection)
  - `secure`: true in production
  - `sameSite`: from env (e.g. `lax` for cross-origin)
  - `maxAge`: from config (e.g. 1 hour), `path: '/'`
7. Returns **user data only** (no token in body): `response.ok(res, { user: { id, firstName, lastName, email } })`

**Logout** (`logout()`): Clears the auth cookie with `res.clearCookie(config.cookie.tokenName, { path: '/', httpOnly, secure, sameSite })` and returns 200. Client calls `authService.logout()` then typically clears localStorage (or relies on 401/clearAuth elsewhere).

**Resend verification** (`resendVerification()`): `POST /resend-verification` with `authLimiter`. Reads `email` from body; calls `regenerateVerificationToken(email)`; if user exists and PENDING, sends new verification email; always returns same success message (no user enumeration).

**File**: `server/src/utils/jwt.util.js`

- `createToken()`: Signs JWT with user.id and user.email
- Expires in 1 hour (from config)
- Secret from environment variable

**File**: `server/src/middleware/auth.middleware.js` → `protect`

- **Why protect middleware?** Validates JWT on protected routes
- Gets token from cookie OR Authorization header
- Verifies token with `verifyToken()`
- Finds user in DB, checks status === ACTIVE
- Attaches `req.user` for controllers to use
- Returns 401 if invalid/expired/missing

---

## Process 3: VERIFY MAIL

### Flow

**User clicks email link**: `${SERVER_URL}/api/v1/auth/verify?token=<plainToken>`

**File**: `server/src/routes/auth.routes.js`

- Route: `GET /verify` (mounted at `/api/v1/auth`, so full path: `GET /api/v1/auth/verify`)
- Middleware: `verifyLimiter` (10 requests per hour)

**File**: `server/src/controllers/auth.controller.js` → `verifyEmail()`

1. Extracts token from `req.query.token`
2. If missing: Returns 400 HTML failure page
3. Calls `findAndVerifyUserByToken(token)`
4. If user not found: Returns 400 HTML failure page
5. If found: User status set to ACTIVE, verificationToken cleared
6. Returns 200 HTML success page with link to login

**File**: `server/src/services/user.service.js` → `findAndVerifyUserByToken()`

1. Hashes incoming token with SHA-256
2. Finds user where `verificationToken === hashedToken` AND `status === PENDING`
3. Sets `status = ACTIVE`, clears `verificationToken`
4. Saves to MongoDB

**File**: `server/src/utils/email.util.js` → `buildVerificationResultPage()`

- Returns HTML page (not JSON) - user sees this in browser
- Success page: Green checkmark, "Email Verified!", link to login with `?verified=true`
- Failure page: Red X, error message, link back to login

**Why HTML instead of JSON?**

- User clicks link in email → browser navigates to URL
- Browser expects HTML, not JSON
- Success page provides clear feedback and redirect

---

## Process 4: TRANSFER MONEY

### Client-Side Flow

**File**: `client/src/screens/transfer-money/useTransfer.ts`

1. **useState**:
  - `receiverEmail`, `amount`: Form inputs (amount as string for input; converted to number on submit)
  - `success`: UI state for success message
2. **useAsyncOperation**: Provides `loading`, `error`, `execute`.
3. **handleSubmit**:
  - Calls `execute(() => transactionService.create({ receiverEmail, amount: Number(amount) }), () => { setSuccess(true); setReceiverEmail(''); setAmount(''); })`
  - On success: Clears form, sets success to true
  - On error: useAsyncOperation sets error; component displays it
4. **setReceiverEmail**, **setAmount**: Exposed for controlled inputs.

**File**: `client/src/api/transaction.service.ts`

- `create(data)`: POST to `/transactions` with `{ receiverEmail, amount, description? }` (baseURL includes `/api/v1`, so full path: `POST /api/v1/transactions`)

### Server-Side Flow

**File**: `server/src/routes/transaction.routes.js`

- Transaction routes mounted at `/api/v1/transactions`. All routes use `router.use(protect)`.
- `POST /` (create transfer): Middleware `transactionLimiter` (10 per minute)
- `GET /`: List transactions (paginated). `GET /:transactionId`: Get one transaction by id (must belong to user)

**File**: `server/src/middleware/rateLimit.middleware.js`

- **Why transactionLimiter?** Prevents rapid-fire transfers (fraud protection, rate limiting)

**File**: `server/src/controllers/transaction.controller.js` → `createTransaction()`

1. Extracts `receiverEmail`, `amount`, `description` from body; throws if `receiverEmail` missing
2. Gets `senderEmail` from `req.user.email` (set by protect middleware)
3. Validates amount with `validateTransactionAmount(amount)` from `utils/validation.util.js`:
  - Must be number, finite, not NaN; 0.01 - 1,000,000; max 2 decimal places
  - Returns `{ isValid, error, sanitized }`; throws AppError 400 if invalid
4. Calls `executeTransfer(senderEmail, receiverEmail, amountValidation.sanitized, description)`

**File**: `server/src/services/transaction.service.js` → `executeTransfer()`

**MongoDB Transaction (Critical!)**:

```javascript
const session = await mongoose.startSession();
session.startTransaction();
try {
  // All operations use same session
  await deductSenderBalance(senderEmail, amount, session);
  await addReceiverBalance(receiverEmail, amount, session);
  const transaction = await createTransactionRecord(...);
  await session.commitTransaction();
} catch (error) {
  await session.abortTransaction();
  throw error;
} finally {
  session.endSession();
}
```

**Why MongoDB Transactions?**

- **Interview Answer**: Ensures atomicity. If any step fails (insufficient funds, receiver not found, etc.), ALL changes roll back. Prevents partial updates (e.g., money deducted but not added, or vice versa).

**Steps**:

1. `validateTransferRequest()`: Checks sender ≠ receiver
2. `deductSenderBalance()`: Uses `findOneAndUpdate` with condition `balance >= amount` (atomic check + update)
3. `addReceiverBalance()`: Adds amount to receiver
4. `createTransactionRecord()`: Gets next transaction ID from counter, creates transaction document
5. `session.commitTransaction()`: Commits all changes atomically

**File**: `server/src/models/transaction.model.js`

- Schema: id (Number, unique), fromEmail, toEmail, amount, description
- Indexes on `fromEmail` and `toEmail` for fast queries
- Counter collection for sequential IDs

---

## Process 5: TRANSACTION (List/View)

### Client-Side Flow

**File**: `client/src/screens/transaction-history/useTransactions.ts`

1. **useState**:
  - `transactions`: Array of transaction objects
  - `totalPages`, `loading`, `error`: UI state
2. **useSearchParams** (React Router): Reads `?page=` from URL; `currentPage = Number(searchParams.get('page')) || 1`; `handlePageChange(page)` updates URL via `setSearchParams`
3. **URL normalization**: A first `useEffect` ensures `?page=` is set so `currentPage` is stable and the fetch effect does not re-run unnecessarily
4. **useEffect** - Why?
  ```typescript
   useEffect(() => {
     let cancelled = false;
     const loadTransactions = async () => { ... };
     loadTransactions();
     return () => { cancelled = true; };
   }, [currentPage, pageSize]);
  ```
  - **Interview Answer**: Runs when component mounts AND when currentPage/pageSize change. Fetches data via `transactionService.getAll(currentPage, pageSize)`; cancellation flag avoids updating state after unmount. Dependency array ensures it re-runs when pagination changes.
5. **userEmail**: From `authStorage.getUser()?.email` for display (e.g. "Incoming/Outgoing" per row)
6. **loadingRef**: Used to avoid double-setting loading in edge cases

**File**: `client/src/screens/transaction-history/useTransactionDetail.ts`

- **useState**: `selectedTransaction` (single transaction or null)
- **useAsyncOperation**: Wraps `transactionService.getById(id)`; on success sets `selectedTransaction` from `data.transaction`; on failure sets `selectedTransaction` to null
- **loadTransactionDetail(id)**: Called when user selects a row; fetches one transaction by id

**File**: `client/src/api/transaction.service.ts`

- `getAll(page = 1, limit = 10)`: GET `/transactions?page=&limit=` (full path: `/api/v1/transactions?page=1&limit=10`)
- `getById(id)`: GET `/transactions/:id`
- Used by dashboard (first page) and transaction-history page (paginated)

### Server-Side Flow

**File**: `server/src/controllers/transaction.controller.js` → `getTransactions()`

1. Parses `page` and `limit` from query params (defaults, max 100)
2. Gets `userEmail` from `req.user.email`
3. Calls `findTransactionsByUserEmail(userEmail, page, limit)`

**File**: `server/src/services/transaction.service.js` → `findTransactionsByUserEmail()`

1. Builds query: `{ $or: [{ fromEmail: userEmail }, { toEmail: userEmail }] }`
2. Uses `Promise.all()` to fetch:
  - Transactions: `.find().sort({ createdAt: -1 }).skip().limit()`
  - Total count: `.countDocuments()`
3. Returns: `{ transactions, total, totalPages, currentPage }`

**Why $or query?**

- User sees transactions where they're sender OR receiver
- Single query instead of two separate queries

**File**: `server/src/controllers/transaction.controller.js` → `getTransactionById()`

- Route: `GET /:transactionId` (full path: `GET /api/v1/transactions/:transactionId`)
- Calls `findTransactionById(transactionId, req.user.email)`; returns `{ status: 'SUCCESS', data }` or `{ status: 'NOT_FOUND', data: null }`
- If NOT_FOUND, throws AppError 404 "Transaction not found"; otherwise returns 200 with `{ transaction }`

**Dashboard and GET /me**

- **Server**: `GET /api/v1/me` (user routes mounted at `/api/v1`) is protected; `getCurrentUser` returns `{ user: { id, firstName, lastName, email, phone, balance, status }, recentTransactions }` (user from `findUserById`, recent from `findRecentTransactions(user.email, 10)`).
- **Client**: `client/src/api/user.service.ts` exposes `getMe()` (GET `/me`). Dashboard uses `useDashboardData` in `client/src/screens/dashboard/useDashboardData.ts`: fetches in parallel `userService.getMe()` and `transactionService.getAll()` (first page), then sets `user` from userData.user and `transactions` from transactionsData.transactions; cancellation pattern in useEffect; runs once on mount.

---

## Middleware Deep Dive

### 1. `auth.middleware.js` → `protect`

**Why?** Validates JWT on protected routes

- **getTokenFromRequest(req)**: Reads token from `req.cookies.token` first, then from `Authorization: Bearer <token>` header (for environments where cookie is not sent)
- Verifies token with `verifyToken(token)` (jwt.util)
- Finds user with `User.findOne({ id: decoded.id }).select('id email status')`
- Checks `user.status === USER_STATUS.ACTIVE`
- Attaches `req.user` for controllers; returns 401 with `{ success: false }` if missing/invalid token or inactive user

**When used?** All routes under `/api/v1/transactions` (via `router.use(protect)`), and `/api/v1` (user routes: GET /me)

### 2. `rateLimit.middleware.js`

**Why?** Prevents abuse

- `authLimiter`: 5 requests/15min (signup/login)
- `verifyLimiter`: 10 requests/hour (email verification)
- `transactionLimiter`: 10 requests/minute (transfers)

**How?** Uses `express-rate-limit` to track IP addresses

### 3. `error.middleware.js`

**Why?** Centralized error handling

- **notFoundHandler**: Runs when no route matches (must be registered after all routes in app.js); returns 404
- **errorHandler**: Catches all errors passed via `next(error)`, logs them, sends safe response
- Handles Mongoose errors (ValidationError → 400, duplicate key → 409)
- Handles JWT errors (401)
- Unknown errors → 500 with generic message (no stack trace to client)

**Why centralized?** Consistent error format, security (no stack traces), logging

### 4. `logger.middleware.js` → `requestLogger`

**Why?** Logs all requests for debugging/monitoring

- Logs: `METHOD /path STATUS - duration`
- Uses Winston logger
- Warn level for 4xx/5xx, info for success

### 5. `socketAuth.middleware.js`

**Why?** Authenticates Socket.IO connections

- Verifies JWT from cookie or handshake.auth.token
- Attaches user to socket for chatbot

---

## React Hooks Explained

**Where hooks live:** Custom hooks are colocated with screens (e.g. `screens/login-signup/useSignup.ts`, `useLogin.ts`; `screens/transfer-money/useTransfer.ts`; `screens/transaction-history/useTransactions.ts`, `useTransactionDetail.ts`; `screens/dashboard/useDashboardData.ts`). Shared hook: `shared/useAsyncOperation.ts`.

### useAsyncOperation (shared)

**What?** Wraps async operations with loading and error state; optional onSuccess callback.
**Where?** `client/src/shared/useAsyncOperation.ts`
**Why?** DRY: avoid repeating loading/error/setError in every screen hook. Used by useSignup, useLogin, useTransfer, useTransactionDetail.
**Returns:** `{ loading, error, execute(operation, onSuccess?), setError }`. On failure, sets error via `getErrorMessage(err)` from types.

### useState

**What?** Manages component state
**Why?** React is declarative - when state changes, component re-renders with new UI

**Example**:

```typescript
const [email, setEmail] = useState('');
// email = current value
// setEmail = function to update it
// When setEmail('new@email.com') called, component re-renders with new email
```

**Interview Answer**: useState returns [value, setter]. When setter called, React schedules re-render. Component uses new value to render UI.

### useEffect

**What?** Side effects (API calls, subscriptions, DOM manipulation)
**Why?** React renders are pure - side effects happen in useEffect

**Dependency Array**:

- `[]`: Run once on mount
- `[dep1, dep2]`: Run when deps change
- No array: Run on every render (usually bad)

**Interview Answer**: useEffect runs after render. Empty deps = mount/unmount. With deps = runs when deps change. Cleanup function runs before next effect or unmount.

**Example**:

```typescript
useEffect(() => {
  loadTransactions();
}, [currentPage]); // Runs when currentPage changes
```

### useContext

**What?** Accesses context value without prop drilling
**Why?** Avoids passing props through many components

**File**: `client/src/context/ThemeContext.ts`

- Creates context: `ThemeContext`
- Custom hook: `useThemeContext()` throws if used outside provider

**File**: `client/src/context/ThemeContextProvider.tsx`

- Provides theme state (light/dark)
- Uses useState to manage mode
- useEffect saves to localStorage
- Any child component can call `useThemeContext()` to get theme

**Interview Answer**: useContext reads value from nearest Provider. Solves prop drilling - don't need to pass theme through every component. Provider wraps app, consumers access value directly.

**Why not props?** If 10 components deep need theme, would need to pass through all 10. Context allows direct access.

---

## Client-Server Communication

### HTTP Client Setup

**File**: `client/src/api/http-client.ts`

- Base URL from env: `VITE_API_URL` (must include `/api/v1` so relative paths like `/auth/login` and `/transactions` resolve correctly)
- `withCredentials: true`: Sends cookies (HTTP-only cookie set by server at login)
- No request interceptor in current implementation; server `protect` middleware reads token from cookie first, then from `Authorization: Bearer` if present
- Response interceptor: On 401, calls `authStorage.clearAuth()` (removes user from localStorage); logs network errors when no response

### Authentication Flow

1. Login → Server sets HTTP-only cookie (client does not store token; only stores user in localStorage via `authStorage.setUser(data.user)`)
2. Subsequent requests → Cookie sent automatically by browser (`withCredentials: true`)
3. Logout → Client calls `authService.logout()` (POST /auth/logout); server clears cookie; client may clear localStorage elsewhere or rely on 401/clearAuth
4. Server `protect` middleware reads token from cookie first, then from `Authorization: Bearer` header (for environments where cookie is not sent, e.g. some cross-origin or API clients)

**Why cookie-only on client?** XSS protection: JavaScript cannot read HTTP-only cookie. Token is never stored in localStorage.

---

## MongoDB Models

### User Model

**File**: `server/src/models/user.model.js`

- `id`: String, required, unique, default `crypto.randomUUID()` (UUID, not MongoDB _id)
- `firstName`, `lastName`: String, required, trim, min/max length
- `email`: Unique, lowercase, trim, regex match
- `password`: Required, min 8 chars, `select: false`
- `phone`: Required, regex match
- `status`: Enum PENDING | ACTIVE, default PENDING
- `verificationToken`: Optional string (hashed)
- `balance`: Number, default from constants, min 0
- `timestamps`: createdAt only

### Transaction Model

**File**: `server/src/models/transaction.model.js`

- `id`: Number, required, unique (from Counter collection via `getNextTransactionId`)
- `fromEmail`, `toEmail`: String, required, lowercase, trim, email regex
- `amount`: Number, required, min/max (0.01 - 1,000,000), custom validator (finite, positive)
- `description`: Optional string, trim, maxlength 500
- `timestamps`: createdAt only
- Indexes: `{ fromEmail: 1, createdAt: -1 }`, `{ toEmail: 1, createdAt: -1 }`

### Counter Collection

- Used for sequential transaction IDs
- `_id: 'transactions'`, `seq: number`
- Atomic increment ensures unique IDs

---

## Security Features

1. **Password Hashing**: bcrypt (10 rounds)
2. **JWT**: HTTP-only cookie (XSS protection) + Bearer header fallback
3. **Rate Limiting**: Prevents brute force
4. **Input Validation**: Mongoose schema + custom validation
5. **Email Verification**: Token hashing (can't guess tokens)
6. **MongoDB Transactions**: Atomic operations (no partial updates)
7. **Error Handling**: No stack traces to client
8. **CORS**: Configured for specific origin
9. **SameSite Cookies**: CSRF protection

---

## Key Interview Questions & Answers

**Q: Why useState instead of regular variables?**
A: Regular variables don't trigger re-renders. useState tells React "this value changed, re-render component."

**Q: Why useEffect for API calls?**
A: API calls are side effects. React renders are pure functions. useEffect separates side effects from rendering.

**Q: Why useContext?**
A: Avoids prop drilling. If 5 components deep need theme, don't pass through all 5. Context provides direct access.

**Q: Why MongoDB transactions?**
A: Ensures atomicity. Transfer = deduct sender + add receiver + create record. If any fails, all roll back. Prevents money disappearing.

**Q: Why rate limiting?**
A: Prevents abuse - brute force attacks, spam signups, rapid transfers. Protects server resources and user accounts.

**Q: Why HTTP-only cookies?**
A: JavaScript can't access them (XSS protection). Even if attacker injects script, can't steal token from cookie.

**Q: Why hash verification tokens?**
A: If database compromised, attacker sees hashed tokens, not plain tokens. Can't use hashed tokens to verify emails.

**Q: Why protect middleware?**
A: DRY principle. Don't repeat JWT validation in every controller. Single place to handle auth, attach req.user.

**Q: Why error middleware?**
A: Centralized handling. Consistent error format, security (no stack traces), logging. Controllers just throw errors.

**Q: Why async email sending?**
A: Email API might be slow. Don't block HTTP response. User gets immediate feedback, email sent in background.

---

## Sequence Diagram Descriptions

Below are detailed descriptions for each process that can be used to generate sequence diagrams. Each description includes all participants, messages, and flow details.

---

### Process 1: SIGNUP - Sequence Diagram Description

**Participants:**

- User (Browser)
- ReactComponent (SignupForm component)
- useSignupHook (Custom React hook in screens/login-signup/useSignup.ts)
- AuthService (client/src/api/auth.service.ts)
- HttpClient (client/src/api/http-client.ts; no request interceptor)
- ExpressRoute (Express route handler)
- RateLimitMiddleware (authLimiter)
- AuthController (signup controller function)
- UserService (createUser service function)
- BcryptUtil (Password hashing)
- GenerateUtil (Token generation)
- MongoDB (User collection)
- EmailUtil (Brevo email service)
- BrevoAPI (External email service)

**Flow:**

1. User fills signup form (firstName, lastName, email, phone, password)
2. User clicks submit button
3. ReactComponent calls useSignupHook.handleSubmit()
4. useSignupHook sets loading state to true
5. useSignupHook calls AuthService.signup(formData)
6. AuthService calls HttpClient.post('/auth/signup', formData) (baseURL includes /api/v1)
7. HttpClient sends POST request with credentials to ExpressRoute (no request interceptor; no token at signup)
8. ExpressRoute receives request at POST /api/v1/auth/signup
9. ExpressRoute passes request to RateLimitMiddleware
11. RateLimitMiddleware checks IP address against rate limit (5 per 15 min)
12. If limit exceeded: RateLimitMiddleware returns 429 error → flows back to User
13. If within limit: RateLimitMiddleware calls next() → continues to AuthController
14. AuthController.signup() extracts { firstName, lastName, email, phone, password } from req.body
15. AuthController calls UserService.createUser(userData)
16. UserService calls BcryptUtil.hash(password, 10 rounds)
17. BcryptUtil returns hashedPassword to UserService
18. UserService calls GenerateUtil.generateVerificationToken()
19. GenerateUtil returns plainToken (64 hex characters)
20. UserService calls GenerateUtil.hashToken(plainToken)
21. GenerateUtil returns hashedToken to UserService
22. UserService calls MongoDB.User.create() with:
  - firstName, lastName, email (lowercase), phone, password (hashed)
    - status: PENDING
    - verificationToken: hashedToken
    - balance: DEFAULT_BALANCE
23. MongoDB validates schema (email uniqueness, password min length, etc.)
24. If duplicate email: MongoDB throws 11000 error → caught by error middleware → 409 response
25. If valid: MongoDB creates user document and returns user object
26. UserService returns { user, verificationToken: plainToken } to AuthController
27. AuthController calls EmailUtil.sendVerificationEmailAsync(user.email, plainToken)
28. EmailUtil.sendVerificationEmailAsync() fires async (non-blocking)
29. AuthController immediately returns 201 response with { message: "Please check your email..." }
30. Response flows back: AuthController → ExpressRoute → HttpClient → AuthService → useSignupHook
31. useSignupHook receives success response
32. useSignupHook sets activeStep to 1 (shows success message)
33. useSignupHook sets loading to false
34. ReactComponent re-renders showing success message
35. Meanwhile (async): EmailUtil builds HTML email template with verification link
36. EmailUtil calls BrevoAPI.post('/smtp/email') with email payload
37. BrevoAPI sends email to user's inbox
38. EmailUtil logs success or error (doesn't affect HTTP response)

**Error Paths:**

- Duplicate email: MongoDB → Error Middleware → 409 response → User sees error
- Validation error: MongoDB → Error Middleware → 400 response → User sees error
- Rate limit exceeded: RateLimitMiddleware → 429 response → User sees error
- Network error: HttpClient → Promise rejection → useSignupHook catches → User sees error

---

### Process 2: LOGIN - Sequence Diagram Description

**Participants:**

- User (Browser)
- ReactComponent (LoginForm component)
- useLoginHook (Custom React hook in screens/login-signup/useLogin.ts)
- AuthService (client/src/api/auth.service.ts)
- HttpClient (client/src/api/http-client.ts)
- AuthStorage (client/src/api/auth.storage.ts; user in localStorage only; token is server-set cookie)
- ExpressRoute (Express route handler)
- RateLimitMiddleware (authLimiter)
- AuthController (login controller function)
- UserService (findUserByEmailWithPassword service function)
- BcryptUtil (Password comparison)
- JWTUtil (Token creation)
- MongoDB (User collection)
- CookieParser (Express cookie middleware)

**Flow:**

1. User enters email and password in login form
2. User clicks submit button
3. ReactComponent calls useLoginHook.handleSubmit()
4. useLoginHook sets loading state to true, clears error
5. useLoginHook calls AuthService.login(email, password)
6. AuthService calls HttpClient.post('/auth/login', { email, password }) (baseURL includes /api/v1)
7. HttpClient sends POST request with credentials (no request interceptor; no token at login)
8. ExpressRoute receives request at POST /api/v1/auth/login
9. ExpressRoute receives request at POST /api/v1/auth/login
10. CookieParser middleware parses cookies (none at login)
11. ExpressRoute passes request to RateLimitMiddleware
12. RateLimitMiddleware checks IP against rate limit (5 per 15 min)
13. If limit exceeded: Returns 429 → User sees error
14. If within limit: Continues to AuthController
15. AuthController.login() validates email and password exist in req.body
16. If missing: AuthController throws AppError(400) → Error Middleware → 400 response
17. AuthController calls UserService.findUserByEmailWithPassword(email)
18. UserService calls MongoDB.User.findOne({ email: lowercase }).select('+password')
19. MongoDB queries User collection by email
20. If user not found: MongoDB returns null → UserService returns null
21. If user found: MongoDB returns user document with password field
22. UserService returns user to AuthController
23. AuthController checks if user exists AND user.status === ACTIVE
24. If user is null OR status !== ACTIVE: AuthController throws AppError(401, "Invalid credentials")
25. AuthController calls BcryptUtil.compare(password, user.password)
26. BcryptUtil compares plain password with hashed password
27. If passwords don't match: BcryptUtil returns false
28. If passwords match: BcryptUtil returns true
29. If password invalid: AuthController throws AppError(401, "Invalid credentials")
30. If password valid: AuthController calls JWTUtil.createToken(user)
31. JWTUtil signs JWT with { id: user.id, email: user.email }, secret, expiresIn: 1h
32. JWTUtil returns token string to AuthController
33. AuthController calls res.cookie(config.cookie.tokenName, token, ...):
  - name from config (e.g. 'token')
  - httpOnly: true, secure: config.cookie.secure, sameSite: config.cookie.sameSite (e.g. 'lax'), maxAge from config, path: '/'
34. CookieParser sets Set-Cookie header in response (HTTP-only cookie with token)
35. AuthController returns 200 response with { user: { id, firstName, lastName, email } } (no token in body)
36. Response flows back: AuthController → ExpressRoute → HttpClient → AuthService → useLoginHook
37. HttpClient response interceptor checks status (200, no action)
38. useLoginHook receives { user } in onSuccess callback
39. useLoginHook calls AuthStorage.setUser(user) → stores user in localStorage only (token is in HTTP-only cookie, not stored on client)
40. useLoginHook calls navigate(ROUTES.DASHBOARD)
41. ReactComponent re-renders, user redirected to dashboard
42. useAsyncOperation sets loading to false

**Error Paths:**

- Wrong email: MongoDB returns null → AuthController throws 401 → User sees "Invalid credentials"
- Wrong password: BcryptUtil returns false → AuthController throws 401 → User sees "Invalid credentials"
- Unverified account (PENDING): AuthController checks status → throws 401 → User sees "Invalid credentials" + resend option
- Rate limit exceeded: RateLimitMiddleware → 429 → User sees error
- Network error: HttpClient → Promise rejection → useLoginHook catches → User sees error

---

### Process 3: VERIFY EMAIL - Sequence Diagram Description

**Participants:**

- User (Browser)
- EmailClient (Email inbox - Gmail, Outlook, etc.)
- Browser (Navigates to verification URL)
- ExpressRoute (Express route handler)
- RateLimitMiddleware (verifyLimiter)
- AuthController (verifyEmail controller function)
- UserService (findAndVerifyUserByToken service function)
- GenerateUtil (Token hashing for comparison)
- MongoDB (User collection)
- EmailUtil (buildVerificationResultPage)

**Flow:**

1. User receives verification email from BrevoAPI (sent during signup)
2. User opens email in EmailClient
3. User clicks verification link: `${SERVER_URL}/api/v1/auth/verify?token=<plainToken>`
4. Browser navigates to verification URL (GET request)
5. ExpressRoute receives GET request at /api/v1/auth/verify
6. ExpressRoute extracts token from req.query.token
7. ExpressRoute passes request to RateLimitMiddleware
8. RateLimitMiddleware checks IP against rate limit (10 per hour)
9. If limit exceeded: Returns 429 HTML error page → Browser displays error
10. If within limit: Continues to AuthController
11. AuthController.verifyEmail() checks if token exists in req.query
12. If token missing: AuthController returns 400 HTML failure page → Browser displays failure
13. AuthController calls UserService.findAndVerifyUserByToken(token)
14. UserService calls GenerateUtil.hashToken(token) to hash incoming token
15. GenerateUtil returns hashedToken to UserService
16. UserService calls MongoDB.User.findOne({
  verificationToken: hashedToken,
      status: PENDING
    })
17. MongoDB queries User collection
18. If user not found: MongoDB returns null → UserService returns null
19. If user found: MongoDB returns user document
20. UserService sets user.status = ACTIVE
21. UserService sets user.verificationToken = undefined
22. UserService calls MongoDB.user.save()
23. MongoDB updates user document (status: ACTIVE, verificationToken: removed)
24. MongoDB returns updated user to UserService
25. UserService returns user to AuthController
26. If user is null: AuthController returns 400 HTML failure page → Browser displays failure
27. If user found and updated: AuthController calls EmailUtil.buildVerificationResultPage(true)
28. EmailUtil builds HTML success page with:
  - Green checkmark emoji
    - "Email Verified!" message
    - Link to login page with ?verified=true query param
29. AuthController returns 200 response with HTML content
30. Response flows back: AuthController → ExpressRoute → Browser
31. Browser receives HTML response
32. Browser renders HTML page showing success message
33. User sees "Email Verified!" page
34. User clicks "Go to Login" link
35. Browser navigates to /login?verified=true
36. Login page detects ?verified=true → shows success message

**Error Paths:**

- Token missing: AuthController → 400 HTML failure page
- Invalid/expired token: MongoDB returns null → AuthController → 400 HTML failure page
- User already verified: MongoDB query finds no PENDING user → 400 HTML failure page
- Rate limit exceeded: RateLimitMiddleware → 429 HTML error page
- Database error: MongoDB throws error → Error Middleware → 500 HTML error page

---

### Process 4: TRANSFER MONEY - Sequence Diagram Description

**Participants:**

- User (Browser - authenticated)
- ReactComponent (TransferForm component)
- useTransferHook (client/src/screens/transfer-money/useTransfer.ts)
- transactionService (client/src/api/transaction.service.ts)
- HttpClient (client/src/api/http-client.ts; cookie sent with credentials)
- ExpressRoute (Express route handler)
- AuthMiddleware (protect middleware)
- RateLimitMiddleware (transactionLimiter)
- TransactionController (createTransaction controller function)
- ValidationUtil (server utils/validation.util.js - validateTransactionAmount)
- TransactionService (server executeTransfer service function)
- MongoDB (User collection, Transaction collection, Counter collection)
- MongooseSession (MongoDB transaction session)

**Flow:**

1. User is authenticated (has valid JWT cookie)
2. User fills transfer form (receiverEmail, amount, description)
3. User clicks submit button
4. ReactComponent calls useTransferHook.handleSubmit()
5. useTransferHook sets loading to true, clears error and success
6. useTransferHook calls transactionService.create({ receiverEmail, amount })
7. transactionService calls HttpClient.post('/transactions', data) (baseURL includes /api/v1)
8. HttpClient sends POST request with credentials (cookie sent automatically)
9. ExpressRoute receives POST request at /api/v1/transactions
11. AuthMiddleware extracts token from cookie OR Authorization header
12. AuthMiddleware calls JWTUtil.verifyToken(token)
13. JWTUtil verifies token signature and expiration
14. If token invalid/expired: AuthMiddleware returns 401 → User sees error
15. If token valid: AuthMiddleware calls MongoDB.User.findOne({ id: decoded.id })
16. MongoDB returns user document
17. AuthMiddleware checks user.status === ACTIVE
18. If not ACTIVE: AuthMiddleware returns 401
19. If ACTIVE: AuthMiddleware attaches req.user = user → calls next()
20. ExpressRoute passes request to RateLimitMiddleware
21. RateLimitMiddleware checks IP against rate limit (10 per minute)
22. If limit exceeded: Returns 429 → User sees error
23. If within limit: Continues to TransactionController
24. TransactionController.createTransaction() extracts receiverEmail, amount, description from req.body
25. TransactionController gets senderEmail from req.user.email (set by AuthMiddleware)
26. TransactionController calls ValidationUtil.validateTransactionAmount(amount)
27. ValidationUtil checks: is number, finite, not NaN; >= 0.01 and <= 1,000,000; max 2 decimal places
28. If invalid: ValidationUtil returns { isValid: false, error } → TransactionController throws AppError(400)
29. If valid: ValidationUtil returns { isValid: true, sanitized: amount }
30. TransactionController calls TransactionService.executeTransfer(senderEmail, receiverEmail, amount, description)
31. TransactionService calls MongooseSession.startSession()
32. MongooseSession creates MongoDB transaction session
33. TransactionService calls MongooseSession.startTransaction()
34. TransactionService calls validateTransferRequest(senderEmail, receiverEmail)
35. If sender === receiver: TransactionService throws AppError(400, "Cannot transfer to yourself")
36. TransactionService calls deductSenderBalance(senderEmail, amount, session)
37. deductSenderBalance calls MongoDB.User.findOneAndUpdate(
  { email: senderEmail, balance: { $gte: amount } },
      { $inc: { balance: -amount } },
      { session, new: true }
    )
38. MongoDB atomically checks balance >= amount AND updates balance
39. If insufficient funds: MongoDB returns null → TransactionService throws AppError(400, "Insufficient funds")
40. If sufficient: MongoDB returns updated sender user → TransactionService continues
41. TransactionService calls addReceiverBalance(receiverEmail, amount, session)
42. addReceiverBalance calls MongoDB.User.findOneAndUpdate(
  { email: receiverEmail },
      { $inc: { balance: amount } },
      { session, new: true }
    )
43. MongoDB updates receiver balance
44. If receiver not found: MongoDB returns null → TransactionService throws AppError(404, "Receiver not found")
45. If receiver found: MongoDB returns updated receiver → TransactionService continues
46. TransactionService calls createTransactionRecord(senderEmail, receiverEmail, amount, description, session)
47. createTransactionRecord calls MongoDB.Counter.findByIdAndUpdate('transactions', { $inc: { seq: 1 } }, { session })
48. MongoDB Counter collection atomically increments seq
49. MongoDB returns counter with new seq number
50. createTransactionRecord calls MongoDB.Transaction.create([{ id: seq, fromEmail, toEmail, amount, description }], { session })
51. MongoDB creates transaction document within session
52. MongoDB returns transaction document
53. TransactionService calls MongooseSession.commitTransaction()
54. MongoDB commits all changes atomically (sender balance decreased, receiver balance increased, transaction document created, counter incremented)
55. TransactionService calls MongooseSession.endSession()
56. TransactionService returns transaction to TransactionController
57. TransactionController returns 201 response with { transaction }
58. Response flows back: TransactionController → ExpressRoute → HttpClient → transactionService → useTransferHook
59. useTransferHook onSuccess: sets success to true, clears receiverEmail and amount
60. useAsyncOperation sets loading to false
61. ReactComponent re-renders showing success message

**Error Paths:**

- Not authenticated: AuthMiddleware → 401 → User redirected to login
- Invalid amount: ValidationUtil → TransactionController throws 400 → User sees error
- Insufficient funds: MongoDB findOneAndUpdate returns null → TransactionService throws 400 → MongooseSession.abortTransaction() → User sees error
- Receiver not found: MongoDB returns null → TransactionService throws 404 → MongooseSession.abortTransaction() → User sees error
- Transfer to self: TransactionService throws 400 → MongooseSession.abortTransaction() → User sees error
- Any error in transaction: MongooseSession.abortTransaction() → All changes rolled back → Error flows back → User sees error

---

### Process 5: TRANSACTION LIST/VIEW - Sequence Diagram Description

**Participants:**

- User (Browser - authenticated)
- ReactComponent (TransactionsPage component)
- useTransactionsHook (client/src/screens/transaction-history/useTransactions.ts)
- transactionService (client/src/api/transaction.service.ts)
- HttpClient (client/src/api/http-client.ts)
- ExpressRoute (Express route handler)
- AuthMiddleware (protect middleware)
- TransactionController (getTransactions/getTransactionById controller functions)
- TransactionService (findTransactionsByUserEmail/findTransactionById service functions)
- MongoDB (Transaction collection, User collection)

**Flow - List Transactions:**

1. User navigates to /transactions page (authenticated)
2. ReactComponent mounts, calls useTransactionsHook()
3. useTransactionsHook reads currentPage from URL search params (?page=1)
4. useTransactionsHook calls useEffect with [currentPage, pageSize] dependencies
5. useEffect calls loadTransactions()
6. useTransactionsHook sets loading to true
7. useTransactionsHook calls transactionService.getAll(currentPage, pageSize)
8. transactionService calls HttpClient.get(`/transactions?page=${currentPage}&limit=${pageSize}`) (baseURL includes /api/v1)
9. HttpClient sends GET request with credentials (cookie) to ExpressRoute
11. ExpressRoute receives GET request at /api/v1/transactions
12. ExpressRoute passes request to AuthMiddleware (protect)
13. AuthMiddleware validates JWT (same as transfer process)
14. If invalid: Returns 401 → User redirected to login
15. If valid: AuthMiddleware attaches req.user → calls next()
16. ExpressRoute passes request to TransactionController.getTransactions()
17. TransactionController extracts page and limit from req.query (defaults: page=1, limit=10, max=100)
18. TransactionController gets userEmail from req.user.email
19. TransactionController calls TransactionService.findTransactionsByUserEmail(userEmail, page, limit)
20. TransactionService builds query: { $or: [{ fromEmail: userEmail }, { toEmail: userEmail }] }
21. TransactionService calls Promise.all([
  MongoDB.Transaction.find(query).sort({ createdAt: -1 }).skip((page-1)*limit).limit(limit),
      MongoDB.Transaction.countDocuments(query)
    ])
22. MongoDB queries Transaction collection:
  - Finds transactions where user is sender OR receiver
    - Sorts by createdAt descending (newest first)
    - Skips records for pagination
    - Limits results
    - Counts total matching documents
23. MongoDB returns [transactions array, total count]
24. TransactionService calculates totalPages = Math.ceil(total / limit)
25. TransactionService returns { transactions, total, totalPages, currentPage }
26. TransactionController returns 200 response with data
27. Response flows back: TransactionController → ExpressRoute → HttpClient → TransactionsService → useTransactionsHook
28. useTransactionsHook receives { transactions, totalPages } (and currentPage)
29. useTransactionsHook sets transactions, totalPages state
30. useTransactionsHook sets loading to false
31. ReactComponent re-renders displaying transactions list
32. User sees paginated transaction list

**Flow - View Single Transaction:**

1. User clicks on a transaction row
2. ReactComponent calls useTransactionDetail (loadTransactionDetail(id))
3. Hook calls transactionService.getById(transactionId)
4. transactionService calls HttpClient.get(`/transactions/${transactionId}`)
5. HttpClient sends GET request to ExpressRoute
6. ExpressRoute receives GET request at /api/v1/transactions/:transactionId
7. ExpressRoute passes request to AuthMiddleware (protect)
8. AuthMiddleware validates JWT and attaches req.user
9. ExpressRoute passes request to TransactionController.getTransactionById()
10. TransactionController extracts transactionId from req.params
11. TransactionController gets userEmail from req.user.email
12. TransactionController calls TransactionService.findTransactionById(transactionId, userEmail)
13. TransactionService builds query: {
  id: Number(transactionId),
      $or: [{ fromEmail: userEmail }, { toEmail: userEmail }]
    }
14. TransactionService calls MongoDB.Transaction.findOne(query)
15. MongoDB queries Transaction collection
16. If transaction not found OR user doesn't own it: MongoDB returns null
17. If transaction found: MongoDB returns transaction document
18. TransactionService returns { status: 'SUCCESS', data: transaction } OR { status: 'NOT_FOUND', data: null }
19. TransactionController checks status
20. If NOT_FOUND: TransactionController throws AppError(404, "Transaction not found")
21. If SUCCESS: TransactionController returns 200 response with { transaction }
22. Response flows back to hook
23. Hook sets transaction state
24. ReactComponent displays transaction details

**Error Paths:**

- Not authenticated: AuthMiddleware → 401 → User redirected to login
- Transaction not found: MongoDB returns null → TransactionController throws 404 → User sees error
- User doesn't own transaction: MongoDB query returns null → TransactionController throws 404 → User sees error
- Network error: HttpClient → Promise rejection → Hook catches → User sees error

**Pagination Flow:**

- User clicks page 2 button
- useTransactionsHook.handlePageChange(2) updates URL search params
- URL changes to ?page=2
- useEffect detects currentPage changed (from dependency array)
- useEffect calls loadTransactions() again
- Process repeats with page=2

