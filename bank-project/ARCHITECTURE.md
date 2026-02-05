---
name: Bank Project Code Review Guide
overview: A comprehensive guide explaining all 5 processes (signup, login, verifyMail, transferMoney, transaction), client-server architecture, MongoDB interactions, middleware, and React hooks with interview-ready explanations.
todos:
  - id: explain-signup
    content: "Explain signup process: client hook → service → server route → controller → service → MongoDB → email"
    status: pending
  - id: explain-login
    content: "Explain login process: client hook → service → server route → controller → JWT creation → cookie setting"
    status: pending
  - id: explain-verify
    content: "Explain email verification: email link → server route → token validation → status update → HTML response"
    status: pending
  - id: explain-transfer
    content: "Explain transfer money: client hook → service → server route → MongoDB transaction → atomic balance update"
    status: pending
  - id: explain-transactions
    content: "Explain transaction listing: client hook → service → server route → MongoDB query with pagination"
    status: pending
  - id: explain-middleware
    content: "Explain all middleware: protect (auth), rateLimit (abuse prevention), error (centralized handling), logger (monitoring)"
    status: pending
  - id: explain-hooks
    content: "Explain React hooks: useState (state management), useEffect (side effects), useContext (avoid prop drilling)"
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

**Request Interceptor** (`httpClient.interceptors.request.use`):

- **When**: Runs BEFORE every HTTP request is sent
- **What it does**: 
  - Reads token from `authStorage.getToken()` (checks localStorage/cookies)
  - If token exists, adds `Authorization: Bearer <token>` header to the request
  - Returns modified config
- **Why**: 
  - DRY principle - don't manually add token to every API call
  - Automatic - token added to ALL requests without remembering to do it
  - Fallback mechanism - if cookie isn't sent (cross-origin), Bearer header works

**Response Interceptor** (`httpClient.interceptors.response.use`):

- **When**: Runs AFTER every HTTP response is received
- **What it does**:
  - Success case: Returns response as-is (no modification)
  - Error case: Checks if status is 401 (Unauthorized)
  - If 401: Clears authentication (removes token and user from storage)
  - Returns rejected promise (so error handling in components works)
- **Why**:
  - Automatic logout when token expires or is invalid
  - User doesn't see confusing errors - automatically redirected to login
  - Centralized error handling for authentication failures

**Example Flow**:

```
Component calls: transactionsService.getAll()
  ↓
Axios request interceptor runs → Adds Authorization header
  ↓
Request sent to server with token
  ↓
Server responds (200 OK or 401 Unauthorized)
  ↓
Axios response interceptor runs → If 401, clears auth
  ↓
Response/Error returned to component
```

### Request Flow

This diagram shows the **complete request lifecycle** - how data flows from user interaction in the browser all the way to the database and back. It demonstrates the **layered architecture** and **separation of concerns** in the application.

```
Browser → React Component → Custom Hook → Service → HTTP Client (Axios) 
  → Express Route → Middleware → Controller → Service → MongoDB
  → Response flows back up
```

**What this shows:**

- **Request Path (Down)**: User action → UI layer → Business logic → Network → Server routing → Authentication → Controller → Database operations
- **Response Path (Up)**: Database → Service → Controller → Middleware → HTTP response → Client → UI update

**Layer Breakdown:**

1. **Browser**: User clicks button, fills form, etc.
2. **React Component**: UI layer - displays form, handles user input
3. **Custom Hook**: React logic layer - manages state, calls services (e.g., `useSignup`, `useLogin`)
4. **Service**: API abstraction layer - defines API endpoints (e.g., `authService.signup()`)
5. **HTTP Client (Axios)**: Network layer - makes HTTP request, adds interceptors (token headers)
6. **Express Route**: Server routing layer - matches URL to handler (e.g., `POST /api/v1/auth/signup`)
7. **Middleware**: Server processing layer - runs before controller (auth, rate limiting, logging)
8. **Controller**: Request handler layer - extracts data, calls service, returns response
9. **Service**: Business logic layer - contains core logic (e.g., `createUser()`, `executeTransfer()`)
10. **MongoDB**: Database layer - stores/retrieves data

**Why this architecture?**

- **Separation of Concerns**: Each layer has a single responsibility
- **Maintainability**: Easy to change one layer without affecting others
- **Testability**: Can test each layer independently
- **Scalability**: Can scale different layers separately (e.g., add more servers, optimize database)

---

## Process 1: SIGNUP

### Client-Side Flow

**File**: `client/src/hooks/useSignup.ts`

1. **useState** - Why?
  - `formData`: Stores form input (firstName, lastName, email, password, phone)
  - `activeStep`: Tracks UI state (0 = form, 1 = success message)
  - `error`: Stores error messages
  - `loading`: Tracks async operation state
  - **Interview Answer**: useState manages component state that changes over time. When user types, formData updates, triggering re-render with new values.
2. **handleSubmit**:
  - Calls `authService.signup(formData)` 
  - On success: `setActiveStep(1)` shows success message
  - On error: `setError()` displays error

**File**: `client/src/services/auth.service.ts`

- Makes POST request to `/api/v1/auth/signup` via `httpClient`

**File**: `client/src/services/httpClient.ts`

- Axios instance with `baseURL`, `withCredentials: true` (sends cookies)
- Request interceptor: Adds `Authorization: Bearer <token>` if token exists
- Response interceptor: On 401, clears auth and redirects

### Server-Side Flow

**File**: `server/src/routes/auth.routes.js`

- Route: `POST /api/v1/auth/signup`
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

**File**: `client/src/hooks/useLogin.ts`

1. **useState**:
  - `email`, `password`: Form inputs
  - `error`, `loading`: UI state
  - `showVerifiedMsg`: Shows success message if redirected from verification
  - `showResendOption`: Shows resend verification link after failed login
2. **useEffect** - Why?
  ```typescript
   useEffect(() => {
     if (searchParams.get('verified') === 'true') {
       setShowVerifiedMsg(true);
       navigate('/login', { replace: true });
     }
   }, [searchParams, navigate]);
  ```
  - **Interview Answer**: useEffect runs after render. Checks URL params for `?verified=true` (from email verification). When found, shows success message. Dependency array ensures it only runs when searchParams/navigate change.
3. **useMemo** - Why?
  ```typescript
   const greeting = useMemo(() => getTimeBasedGreeting(), []);
  ```
  - **Interview Answer**: Memoizes greeting calculation. Empty deps means it runs once on mount. Prevents recalculating on every render.
4. **handleSubmit**:
  - Calls `authService.login(email, password)`
  - On success: Stores user + token in localStorage/cookies, navigates to `/dashboard`
  - On error: Shows error, enables resend verification option

**File**: `client/src/services/auth.storage.ts`

- `setUser()`: Stores user object in localStorage
- `setToken()`: Stores token in cookie (1 hour expiry)
- `isAuthenticated()`: Checks if user exists in localStorage

### Server-Side Flow

**File**: `server/src/controllers/auth.controller.js` → `login()`

1. Validates email + password exist
2. Calls `findUserByEmailWithPassword()` - `.select('+password')` includes password field
3. Checks `user.status === ACTIVE` (rejects PENDING users)
4. Validates password with `bcrypt.compare()`
5. Creates JWT token with `createToken(user)`
6. Sets HTTP-only cookie with token:
  - `httpOnly: true` (JavaScript can't access - XSS protection)
  - `secure: true` in production (HTTPS only)
  - `sameSite: 'strict'` (CSRF protection)
  - `maxAge: 1 hour`
7. Returns user data + token (for Authorization header fallback)

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

**User clicks email link**: `/api/v1/auth/verify?token=<plainToken>`

**File**: `server/src/routes/auth.routes.js`

- Route: `GET /api/v1/auth/verify`
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

**File**: `client/src/hooks/useTransfer.ts`

1. **useState**:
  - `receiverEmail`, `amount`: Form inputs
  - `loading`, `error`, `success`: UI state
2. **handleSubmit**:
  - Calls `transactionsService.create({ receiverEmail, amount })`
  - On success: Clears form, shows success message
  - On error: Shows error message

**File**: `client/src/services/transactions.service.ts`

- POST `/api/v1/transactions` with `{ receiverEmail, amount, description? }`

### Server-Side Flow

**File**: `server/src/routes/transaction.routes.js`

- Route: `POST /api/v1/transactions`
- Middleware: `protect` (requires auth), `transactionLimiter` (10 per minute)

**File**: `server/src/middleware/rateLimit.middleware.js`

- **Why transactionLimiter?** Prevents rapid-fire transfers (fraud protection, rate limiting)

**File**: `server/src/controllers/transaction.controller.js` → `createTransaction()`

1. Extracts `receiverEmail`, `amount`, `description` from body
2. Gets `senderEmail` from `req.user.email` (set by protect middleware)
3. Validates amount with `validateTransactionAmount()`:
  - Must be number, 0.01 - 1,000,000
  - Max 2 decimal places
4. Calls `executeTransfer()` service

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

**File**: `client/src/hooks/useTransactions.ts`

1. **useState**:
  - `transactions`: Array of transaction objects
  - `loading`, `error`: UI state
  - `totalPages`: For pagination
2. **useSearchParams** (React Router):
  - Reads `?page=1` from URL
  - `handlePageChange()`: Updates URL params
3. **useEffect** - Why?
  ```typescript
   useEffect(() => {
     loadTransactions();
   }, [currentPage, pageSize]);
  ```
  - **Interview Answer**: Runs when component mounts AND when currentPage/pageSize change. Fetches new data when user navigates pages. Dependency array ensures it re-runs when pagination changes.

**File**: `client/src/services/transactions.service.ts`

- GET `/api/v1/transactions?page=1&limit=10`

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

- GET `/api/v1/transactions/:transactionId`
- Finds transaction by ID AND verifies user owns it (fromEmail OR toEmail matches)
- Returns 404 if not found or not owned

---

## Middleware Deep Dive

### 1. `auth.middleware.js` → `protect`

**Why?** Validates JWT on protected routes

- Checks cookie OR Authorization header
- Verifies token, finds user, checks ACTIVE status
- Attaches `req.user` for controllers
- Returns 401 if invalid

**When used?** All routes under `/api/v1/transactions`, `/api/v1/me`

### 2. `rateLimit.middleware.js`

**Why?** Prevents abuse

- `authLimiter`: 5 requests/15min (signup/login)
- `verifyLimiter`: 10 requests/hour (email verification)
- `transactionLimiter`: 10 requests/minute (transfers)

**How?** Uses `express-rate-limit` to track IP addresses

### 3. `error.middleware.js`

**Why?** Centralized error handling

- `errorHandler`: Catches all errors, logs them, sends safe response
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

**File**: `client/src/context/ThemeContext.tsx`

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

**File**: `client/src/services/httpClient.ts`

- Base URL from env: `VITE_API_URL`
- `withCredentials: true`: Sends cookies (for JWT)
- Request interceptor: Adds `Authorization: Bearer <token>` header
- Response interceptor: On 401, clears auth (token expired)

### Authentication Flow

1. Login → Server sets HTTP-only cookie
2. Subsequent requests → Cookie sent automatically (`withCredentials: true`)
3. If cookie missing → Request interceptor adds Bearer token from localStorage
4. Server reads cookie OR Authorization header

**Why both?** Cookie for same-origin, Bearer header for cross-origin or mobile apps

---

## MongoDB Models

### User Model

- `id`: UUID (not MongoDB _id)
- `email`: Unique, lowercase, validated
- `password`: Hashed, `select: false`
- `status`: PENDING or ACTIVE
- `verificationToken`: Hashed token
- `balance`: Number, min 0

### Transaction Model

- `id`: Sequential number (from counter)
- `fromEmail`, `toEmail`: Lowercase, validated
- `amount`: Number, 0.01 - 1,000,000, max 2 decimals
- `description`: Optional string
- Indexes on fromEmail and toEmail for performance

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
- useSignupHook (Custom React hook)
- AuthService (Client service layer)
- HttpClient (Axios instance with interceptors)
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
6. AuthService calls HttpClient.post('/api/v1/auth/signup', formData)
7. HttpClient request interceptor checks for token (none at signup)
8. HttpClient sends POST request to ExpressRoute with formData
9. ExpressRoute receives request at POST /api/v1/auth/signup
10. ExpressRoute passes request to RateLimitMiddleware
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
- useLoginHook (Custom React hook)
- AuthService (Client service layer)
- HttpClient (Axios instance)
- AuthStorage (localStorage + cookies)
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
6. AuthService calls HttpClient.post('/api/v1/auth/login', { email, password })
7. HttpClient request interceptor checks for token (none at login)
8. HttpClient sends POST request with credentials to ExpressRoute
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
33. AuthController calls res.cookie() to set HTTP-only cookie:
  - name: 'token'
    - value: token
    - httpOnly: true
    - secure: true (production)
    - sameSite: 'strict'
    - maxAge: 3600000 (1 hour)
34. CookieParser sets Set-Cookie header in response
35. AuthController returns 200 response with { user: {...}, token }
36. Response flows back: AuthController → ExpressRoute → HttpClient → AuthService → useLoginHook
37. HttpClient response interceptor checks status (200, no action)
38. useLoginHook receives { user, token }
39. useLoginHook calls AuthStorage.setUser(user) → stores in localStorage
40. useLoginHook calls AuthStorage.setToken(token) → stores in cookie
41. useLoginHook calls navigate('/dashboard')
42. ReactComponent re-renders, user redirected to dashboard
43. useLoginHook sets loading to false

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
- useTransferHook (Custom React hook)
- TransactionsService (Client service layer)
- HttpClient (Axios instance)
- ExpressRoute (Express route handler)
- AuthMiddleware (protect middleware)
- RateLimitMiddleware (transactionLimiter)
- TransactionController (createTransaction controller function)
- ValidationUtil (Amount validation)
- TransactionService (executeTransfer service function)
- MongoDB (User collection, Transaction collection, Counter collection)
- MongooseSession (MongoDB transaction session)

**Flow:**

1. User is authenticated (has valid JWT cookie)
2. User fills transfer form (receiverEmail, amount, description)
3. User clicks submit button
4. ReactComponent calls useTransferHook.handleSubmit()
5. useTransferHook sets loading to true, clears error and success
6. useTransferHook calls TransactionsService.create({ receiverEmail, amount })
7. TransactionsService calls HttpClient.post('/api/v1/transactions', data)
8. HttpClient request interceptor reads token from AuthStorage
9. HttpClient adds Authorization: Bearer  header
10. HttpClient sends POST request with credentials (cookie + header) to ExpressRoute
11. ExpressRoute receives POST request at /api/v1/transactions
12. ExpressRoute passes request to AuthMiddleware (protect)
13. AuthMiddleware extracts token from cookie OR Authorization header
14. AuthMiddleware calls JWTUtil.verifyToken(token)
15. JWTUtil verifies token signature and expiration
16. If token invalid/expired: AuthMiddleware returns 401 → User sees error
17. If token valid: AuthMiddleware calls MongoDB.User.findOne({ id: decoded.id })
18. MongoDB returns user document
19. AuthMiddleware checks user.status === ACTIVE
20. If not ACTIVE: AuthMiddleware returns 401
21. If ACTIVE: AuthMiddleware attaches req.user = user → calls next()
22. ExpressRoute passes request to RateLimitMiddleware
23. RateLimitMiddleware checks IP against rate limit (10 per minute)
24. If limit exceeded: Returns 429 → User sees error
25. If within limit: Continues to TransactionController
26. TransactionController.createTransaction() extracts receiverEmail, amount, description from req.body
27. TransactionController gets senderEmail from req.user.email (set by AuthMiddleware)
28. TransactionController calls ValidationUtil.validateTransactionAmount(amount)
29. ValidationUtil checks:
  - Is number? Is finite? Not NaN?
    - > = 0.01? <= 1,000,000?
    - Max 2 decimal places?
30. If invalid: ValidationUtil returns { isValid: false, error } → TransactionController throws AppError(400)
31. If valid: ValidationUtil returns { isValid: true, sanitized: amount }
32. TransactionController calls TransactionService.executeTransfer(senderEmail, receiverEmail, amount, description)
33. TransactionService calls MongooseSession.startSession()
34. MongooseSession creates MongoDB transaction session
35. TransactionService calls MongooseSession.startTransaction()
36. TransactionService calls validateTransferRequest(senderEmail, receiverEmail)
37. If sender === receiver: TransactionService throws AppError(400, "Cannot transfer to yourself")
38. TransactionService calls deductSenderBalance(senderEmail, amount, session)
39. deductSenderBalance calls MongoDB.User.findOneAndUpdate(
  { email: senderEmail, balance: { $gte: amount } },
      { $inc: { balance: -amount } },
      { session, new: true }
    )
40. MongoDB atomically checks balance >= amount AND updates balance
41. If insufficient funds: MongoDB returns null → TransactionService throws AppError(400, "Insufficient funds")
42. If sufficient: MongoDB returns updated sender user → TransactionService continues
43. TransactionService calls addReceiverBalance(receiverEmail, amount, session)
44. addReceiverBalance calls MongoDB.User.findOneAndUpdate(
  { email: receiverEmail },
      { $inc: { balance: amount } },
      { session, new: true }
    )
45. MongoDB updates receiver balance
46. If receiver not found: MongoDB returns null → TransactionService throws AppError(404, "Receiver not found")
47. If receiver found: MongoDB returns updated receiver → TransactionService continues
48. TransactionService calls createTransactionRecord(senderEmail, receiverEmail, amount, description, session)
49. createTransactionRecord calls MongoDB.Counter.findByIdAndUpdate('transactions', { $inc: { seq: 1 } }, { session })
50. MongoDB Counter collection atomically increments seq
51. MongoDB returns counter with new seq number
52. createTransactionRecord calls MongoDB.Transaction.create([{ id: seq, fromEmail, toEmail, amount, description }], { session })
53. MongoDB creates transaction document within session
54. MongoDB returns transaction document
55. TransactionService calls MongooseSession.commitTransaction()
56. MongoDB commits all changes atomically:
  - Sender balance decreased
    - Receiver balance increased
    - Transaction document created
    - Counter incremented
57. TransactionService calls MongooseSession.endSession()
58. TransactionService returns transaction to TransactionController
59. TransactionController returns 201 response with { transaction }
60. Response flows back: TransactionController → ExpressRoute → HttpClient → TransactionsService → useTransferHook
61. useTransferHook receives success response
62. useTransferHook sets success to true
63. useTransferHook clears receiverEmail and amount
64. useTransferHook sets loading to false
65. ReactComponent re-renders showing success message

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
- useTransactionsHook (Custom React hook)
- TransactionsService (Client service layer)
- HttpClient (Axios instance)
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
7. useTransactionsHook calls TransactionsService.getAll(currentPage, pageSize)
8. TransactionsService calls HttpClient.get(`/api/v1/transactions?page=${currentPage}&limit=${pageSize}`)
9. HttpClient request interceptor adds Authorization: Bearer  header
10. HttpClient sends GET request with credentials to ExpressRoute
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
28. useTransactionsHook receives { transactions, totalPages }
29. useTransactionsHook sets transactions state
30. useTransactionsHook sets totalPages state
31. useTransactionsHook sets loading to false
32. ReactComponent re-renders displaying transactions list
33. User sees paginated transaction list

**Flow - View Single Transaction:**

1. User clicks on a transaction row
2. ReactComponent calls useTransactionDetailHook (or similar)
3. Hook calls TransactionsService.getById(transactionId)
4. TransactionsService calls HttpClient.get(`/api/v1/transactions/${transactionId}`)
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

