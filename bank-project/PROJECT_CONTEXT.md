# PROJECT_CONTEXT.md — Dubai-Bank Technical Manifest

> **Purpose:** Provide full codebase context to an AI or engineer unfamiliar with this project.
> **Generated:** 2026-03-15

---

## 1. Project Essence

**Dubai-Bank** is a full-stack web banking application that simulates core personal-banking operations: user registration with email verification, secure authentication, account balance management, peer-to-peer money transfers (ACID-compliant), paginated transaction history, an AI-powered chatbot that can execute real banking operations via OpenAI function calling, and integrated peer-to-peer video calling via Jitsi Meet.

**Primary goals:**
- Demonstrate production-grade patterns in a financial domain (atomic transactions, JWT auth, rate limiting, graceful shutdown).
- Showcase an agentic AI chatbot that goes beyond Q&A — it can read balances, pull transaction history, and execute transfers through a tool-call loop.
- Serve as a technical interview portfolio piece with comprehensive tests, Docker infrastructure, and deployment automation.

---

## 2. Tech Stack

### Frontend (client/)
| Category | Technology | Version |
|---|---|---|
| Language | TypeScript | ~5.9.3 |
| UI Framework | React | 19.2 |
| Build Tool | Vite | 7.2 |
| Component Library | MUI (Material UI) | 7.3 |
| CSS-in-JS | Emotion | 11.14 |
| Routing | React Router | 7.13 |
| HTTP Client | Axios | 1.13 |
| WebSocket | Socket.IO Client | 4.8 |
| Markdown Rendering | react-markdown | 10.1 |
| Video Calling | @jitsi/react-sdk | 1.4 |
| Testing | Vitest + Testing Library + jsdom | 2.1 / 16.1 / 25.0 |
| Linting | ESLint (flat config) | 9.39 |

### Backend (server/)
| Category | Technology | Version |
|---|---|---|
| Runtime | Node.js (ES Modules) | 20 |
| Framework | Express | 5.2 |
| Database ODM | Mongoose | 8.21 |
| Database | MongoDB (Atlas in prod) | 7 |
| Auth | jsonwebtoken + bcrypt | 9.0 / 6.0 |
| AI | OpenAI SDK (GPT-4o-mini) | 6.17 |
| WebSocket | Socket.IO | 4.8 |
| Email | Nodemailer (Brevo/Sendinblue SMTP) | 7.0 |
| Logging | Winston | 3.19 |
| Rate Limiting | express-rate-limit | 8.2 |
| Testing | Jest + Supertest + mongodb-memory-server | 29.7 / 7.0 / 10.0 |

### Infrastructure
| Concern | Solution |
|---|---|
| Client Hosting | Vercel (auto-deploy on push) |
| Server Hosting | Render (Docker-based) |
| Database Hosting | MongoDB Atlas (replica set) |
| Local Dev | Docker Compose (MongoDB 7 + server + Nginx-fronted client) |
| Container Build | Multi-stage Dockerfiles (Node 20 Alpine + Nginx Alpine for client) |

---

## 3. Architecture & Design

### Overall Pattern: Three-Tier Layered Architecture

```
┌────────────────────────────────────────────────────────┐
│  CLIENT (React SPA)                                    │
│  Axios HTTP ──────────── REST API ──────────────────┐  │
│  Socket.IO ──────────── /chat namespace ────────┐   │  │
│  Jitsi iframe ────────── meet.jit.si (WebRTC)   │   │  │
└─────────────────────────────────────────────────┼───┼──┘
                                                  │   │
┌─────────────────────────────────────────────────┼───┼──┐
│  SERVER (Express 5 + Socket.IO)                 │   │  │
│                                                 │   │  │
│  Middleware Chain:                               │   │  │
│    CORS → Cookie → JSON → Logger → RateLimit    │   │  │
│    → Auth → Routes → NotFound → ErrorHandler    │   │  │
│                                                 │   │  │
│  Controllers ──→ Services ──→ Models (Mongoose) │   │  │
│                      │                          │   │  │
│                      ├── OpenAI API (chatbot)   │   │  │
│                      └── Brevo API (email)      │   │  │
└─────────────────────────────────────────────────┼───┼──┘
                                                  │   │
┌─────────────────────────────────────────────────┼───┼──┐
│  DATA LAYER                                     │   │  │
│  MongoDB: users, transactions, counters         │   │  │
└─────────────────────────────────────────────────┴───┴──┘
```

### Server-Side Layers

| Layer | Responsibility | Files |
|---|---|---|
| **Routes** | HTTP verb + path mapping, middleware attachment | `routes/*.routes.js` |
| **Controllers** | Request parsing, input validation, response shaping | `controllers/*.controller.js` |
| **Services** | Business logic, database queries, external API calls | `services/*.service.js` |
| **Models** | Mongoose schemas, indexes, virtuals, statics | `models/*.model.js` |
| **Middleware** | Cross-cutting: auth, logging, rate-limiting, errors | `middleware/*.middleware.js` |
| **Utils** | Stateless helpers: JWT, hashing, validation, email | `utils/*.util.js` |

### Client-Side Layers

| Layer | Responsibility | Files |
|---|---|---|
| **Screens** | Page-level React components (one per route) | `screens/*/Page.tsx` |
| **Components** | Reusable UI pieces (ChatAssistant, VideoCall, dialogs) | `components/*.tsx` |
| **Hooks** | Stateful logic extraction (data fetching, form handling) | `screens/*/use*.ts`, `shared/use*.ts` |
| **API Layer** | Axios service modules wrapping REST calls | `api/*.service.ts`, `api/http-client.ts` |
| **Types** | Shared TypeScript interfaces | `types/index.ts` |
| **Layout** | Header, footer, theme provider | `layout/*.tsx`, `context/*.tsx` |
| **Shared** | Utility functions, MUI barrel exports, theme config | `shared/*.ts` |

### Key Design Patterns

| Pattern | Implementation |
|---|---|
| **Layered Architecture** | Controller → Service → Model separation on server |
| **Unit of Work (ACID Transactions)** | `executeTransfer()` uses MongoDB sessions — debit, credit, and record creation are atomic |
| **Sequence Generator** | `Counter` collection with `findByIdAndUpdate($inc)` for auto-incrementing transaction IDs |
| **Agentic Tool-Call Loop** | OpenAI function calling with up to 5 iterative rounds of tool execution |
| **Observer / Pub-Sub** | `authStorage` dispatches `auth-state-change` DOM events; `dashboard:refresh` custom events bridge Socket.IO → React state |
| **Command Pattern** | `useAsyncOperation` hook — wraps any async fn into `{loading, error, execute}` |
| **Immutable Configuration** | `Object.freeze()` on the entire config tree |
| **Fire-and-Forget Async** | Email functions catch errors internally and never propagate |
| **Graceful Shutdown** | SIGTERM/SIGINT handlers close HTTP server → close DB → exit |
| **Deterministic Room Generation** | SHA-256 hash of sorted email pair ensures the same two users always get the same video room |

---

## 4. Folder Structure

```
bank-project/
├── client/                         # React SPA (Vite + TypeScript)
│   ├── public/                     # Static assets served as-is
│   ├── src/
│   │   ├── api/                    # Axios HTTP client and service modules (auth, user, transaction)
│   │   ├── components/             # Shared React components (ChatAssistant, VideoCall, dialogs)
│   │   ├── constants/              # Route path constants
│   │   ├── context/                # React Context (ThemeContext + provider)
│   │   ├── layout/                 # App shell: header, footer
│   │   ├── screens/                # Route-level pages, each with sub-components and hooks
│   │   │   ├── dashboard/          # Balance card, recent transactions, data-fetching hook
│   │   │   ├── login-signup/       # Login form, signup stepper, auth hooks
│   │   │   ├── transaction-history/# Paginated table, detail panel, pagination hooks
│   │   │   ├── transfer-money/     # Transfer form, success dialog, transfer hook
│   │   │   └── video-call/         # Jitsi Meet integration page
│   │   ├── shared/                 # Reusable utilities, custom hooks, MUI theme, barrel exports
│   │   ├── types/                  # TypeScript interfaces (User, Transaction, API shapes)
│   │   └── __tests__/              # Vitest tests: api/, hooks/, unit/
│   ├── Dockerfile                  # Multi-stage: Node builder → Nginx Alpine
│   ├── nginx.conf                  # Nginx config for SPA routing
│   ├── vite.config.ts              # Vite dev server, proxy, Vitest config
│   └── package.json                # Client dependencies and scripts
│
├── server/                         # Express 5 REST API + Socket.IO
│   ├── server.js                   # Entry point: DB connect, HTTP listen, Socket.IO init, graceful shutdown
│   ├── src/
│   │   ├── app.js                  # Express app: middleware chain, route mounting, error handlers
│   │   ├── config/                 # Environment config (frozen object), MongoDB connection with retry
│   │   ├── constants/              # Shared constants (USER_STATUS, DEFAULT_BALANCE, SALT_ROUNDS)
│   │   ├── controllers/            # Request handlers: auth, user, transaction
│   │   ├── middleware/             # Auth guard, socket auth, error handler, logger, rate limiters
│   │   ├── models/                 # Mongoose schemas: User, Transaction, Counter
│   │   ├── routes/                 # Express routers: auth, user, transaction
│   │   ├── services/               # Business logic: user ops, transfer (ACID), chatbot, OpenAI function calling
│   │   ├── socket/                 # Socket.IO /chat namespace handler (message routing, rate limiting)
│   │   ├── utils/                  # Stateless helpers: JWT, bcrypt, validation, email (Brevo), Winston logger
│   │   └── __tests__/              # Jest tests: integration/ (routes, auth), unit/ (jwt, validation)
│   │       └── helpers/            # Test helpers (mongodb-memory-server setup)
│   ├── Dockerfile                  # Node 20 Alpine production image
│   └── package.json                # Server dependencies and scripts
│
├── docs/
│   └── function-calling-guide.md   # Detailed guide on the OpenAI function-calling implementation
│
├── docker-compose.yml              # Local dev stack: MongoDB 7 + server + client (Nginx)
├── docker-compose.override.yml     # Local overrides (gitignored)
├── .env                            # Root environment variables
│
├── ARCHITECTURE_OVERVIEW.md        # Architecture deep-dive (Hebrew)
├── PROJECT_PRESENTATION.md         # Presentation-ready overview with mermaid diagrams
├── PROJECT_SUMMARY_FOR_NOTEBOOKLM.md # Narrative summary for NotebookLM
├── FUNCTION_TOOLING_EXPLANATION.md # High-level OpenAI function calling explanation
├── REPOSITORY_PATTERN_EXPLANATION.md # Repository pattern explanation
├── FRONTEND_INTERVIEW_QUESTIONS.md # 38 categorized frontend interview questions
├── FRONTEND_INTERVIEW_ANSWERS.md   # Detailed answers with project code examples
└── TECHNICAL_INTERVIEW_REPORT.md   # Full technical report: architecture, deployment, data flow
```

---

## 5. Core Logic & Key Files

### 5.1 Server Entry & Bootstrap

**`server/server.js`** — Top-level entry point.
1. Calls `connectDB()` (with retry logic).
2. Starts the HTTP server on the configured port.
3. Attaches a Socket.IO `Server` instance with CORS configuration.
4. Calls `initChatbotSocket(io)` to wire the `/chat` namespace.
5. Registers `SIGTERM`/`SIGINT` handlers for graceful shutdown (close HTTP → close Mongoose → exit).

**`server/src/app.js`** — Express application assembly.
- Configures: `trust proxy`, CORS with credentials, `cookie-parser`, JSON body (10kb limit), request logging.
- Mounts: `/api/v1/auth`, `/api/v1/me`, `/api/v1/transactions`.
- Terminates with `notFoundHandler` and global `errorHandler`.
- Exposes `/` and `/health` for health checks.

### 5.2 Configuration

**`server/src/config/index.js`** — Loads `.env`, validates `JWT_SECRET` and `MONGO_URI` are present, exports a deeply `Object.freeze()`-d config object with sections: `jwt`, `cookie`, `email`, and base settings (`port`, `nodeEnv`, `mongoUri`, `clientUrl`, `serverUrl`, `openAiApiKey`).

**`server/src/config/database.js`** — `connectDB()` retries connection up to N times with configurable delay. Exits the process if all retries fail.

### 5.3 Mongoose Models

**`User` (`models/user.model.js`)**
| Field | Type | Constraints |
|---|---|---|
| `id` | String | UUID via `crypto.randomUUID()`, unique, immutable |
| `firstName`, `lastName` | String | 2–50 chars, trimmed |
| `email` | String | Unique, lowercase, regex-validated |
| `password` | String | Min 8 chars, `select: false` (excluded from queries by default) |
| `phone` | String | International format regex |
| `status` | Enum | `PENDING` / `ACTIVE` |
| `verificationToken` | String | SHA-256 hash of raw token |
| `balance` | Number | Default 500, min 0 |

**`Transaction` (`models/transaction.model.js`)**
| Field | Type | Constraints |
|---|---|---|
| `id` | Number | Auto-incrementing via `Counter` collection |
| `fromEmail` | String | Sender email |
| `toEmail` | String | Receiver email |
| `amount` | Number | 0.01–1,000,000, finite, positive |
| `description` | String | Max 500 chars |

Compound indexes: `(fromEmail, createdAt)` and `(toEmail, createdAt)`.

**`Counter` (embedded in `transaction.model.js`)** — Separate collection storing a numeric sequence. `getNextTransactionId(session)` atomically increments and returns the next ID within a MongoDB session.

### 5.4 Auth Flow

**`auth.controller.js` + `user.service.js`**

- **Signup:** Validates fields → hashes password (bcrypt, 10 rounds) → generates verification token (32 random bytes hex) → stores SHA-256 hash of token in DB → fires async verification email (Brevo) → returns 201.
- **Email verification:** `GET /verify?token=X` → hashes token → finds PENDING user with matching hash → sets status to ACTIVE → returns a server-rendered HTML success page.
- **Resend verification:** Regenerates token for PENDING users. Always returns success (prevents email enumeration).
- **Login:** Validates credentials → checks status is ACTIVE → bcrypt compare → creates JWT (`{id, email}`, 1h expiry) → sets httpOnly secure cookie → returns user data.
- **Logout:** Optionally disconnects user's active Socket.IO connections → clears auth cookie.

**`auth.middleware.js`** — `protect` middleware extracts JWT from cookie or `Authorization: Bearer` header, verifies it, loads the active user, and attaches `req.user`.

### 5.5 Money Transfer (ACID)

**`transaction.service.js` → `executeTransfer()`** — The most critical function in the codebase.

```
1. Validate sender !== receiver
2. Start MongoDB session + transaction
3. deductSenderBalance() — atomic findOneAndUpdate with {balance >= amount} guard
4. addReceiverBalance() — atomic $inc on receiver balance
5. createTransactionRecord() — get next auto-increment ID within session, create document
6. Commit transaction (or abort on any error)
7. End session
```

The `{balance >= amount}` filter in `deductSenderBalance` prevents overdraft at the database level without race conditions — if two concurrent transfers deplete the same balance, only one succeeds.

### 5.6 AI Chatbot (OpenAI Function Calling)

**`openaiFunctionCall.service.js`** — Agentic tool-call engine.

**Registered tools:**
1. `get_balance` — Returns user's account balance via `getAccountSummary()`.
2. `get_transaction_history` — Returns recent transactions (configurable limit).
3. `transfer_money` — Executes a real money transfer via `executeTransfer()`.
4. `get_supported_services` — Returns the list of available capabilities.

**Processing loop:**
1. Builds messages: system prompt + last 20 chat history messages + new user message.
2. Calls OpenAI GPT-4o-mini with tool definitions (temperature 0.3).
3. If the response contains `tool_calls`, executes all in parallel via `Promise.all`.
4. Appends tool results to messages, calls OpenAI again.
5. Repeats up to `MAX_TOOL_CALL_ROUNDS` (5).
6. Extracts final text response and flags whether `transfer_money` was called.

The system prompt enforces a confirmation step before executing transfers.

**`chatbot.service.js`** — Thin wrapper: sanitizes user input (trims, caps at 250 chars, strips HTML), delegates to `processWithFunctionCalling`, returns `{intent, message, chatHistory, transferCompleted}`.

**`socket/socket.handler.js`** — Socket.IO `/chat` namespace.
- Uses `authenticateSocket` middleware (JWT from cookie or handshake auth).
- Maintains `activeSocketsByUserId: Map<userId, Set<Socket>>` in memory.
- Per-user rate limiting (10 messages / 60 seconds) via a `Map`.
- On `user-message`: validates, checks rate limit, calls `processMessage()`, emits `bot-message`. If transfer completed, emits `transfer-completed`.
- Exports `disconnectUser(userId)` called by `logout` to forcibly close sockets.

### 5.7 Client API Layer

**`http-client.ts`** — Axios instance with `withCredentials: true` and a response interceptor that clears auth on 401.

**`auth.storage.ts`** — Stores user object in `localStorage`. JWT lives in httpOnly cookie (never JS-accessible). Dispatches a custom `auth-state-change` DOM event on every mutation (Pub/Sub pattern).

**Service modules** (`auth.service.ts`, `user.service.ts`, `transaction.service.ts`) — Thin wrappers that call the HTTP client and unwrap the server's `{success, data}` envelope.

### 5.8 Client Hooks

| Hook | Location | Responsibility |
|---|---|---|
| `useAuth` | `shared/useAuth.ts` | Listens to `auth-state-change` events, provides reactive `{isAuthenticated}` |
| `useAsyncOperation` | `shared/useAsyncOperation.ts` | Generic async state wrapper: `{loading, error, execute, setError}` — used by nearly every data hook |
| `useLogin` | `screens/login-signup/useLogin.ts` | Login form state, error message transformation, verified-email URL param handling |
| `useSignup` | `screens/login-signup/useSignup.ts` | Two-step stepper state (form → "check email" confirmation) |
| `useDashboardData` | `screens/dashboard/useDashboardData.ts` | Parallel fetch of user profile + transactions via `Promise.all`; listens for `dashboard:refresh` DOM event |
| `useTransfer` | `screens/transfer-money/useTransfer.ts` | Transfer form state, submission, post-transfer notification to get video room name |
| `useTransactions` | `screens/transaction-history/useTransactions.ts` | Paginated fetch synced with URL `?page=N` param; race-condition guard via effect cleanup |
| `useTransactionDetail` | `screens/transaction-history/useTransactionDetail.ts` | Single transaction fetch for the detail side panel |
| `useChatSocket` | `components/useChatSocket.ts` | Socket.IO lifecycle: connect, message handling, `transfer-completed` → `dashboard:refresh` bridge, disconnect on auth change |

### 5.9 Client Screens

| Route | Screen | Key Components |
|---|---|---|
| `/login` | `LoginPage` | `LoginForm`, `DubaiBankLogo`, forgot-password placeholder, theme toggle |
| `/signup` | `SignupPage` | `SignupForm` with MUI Stepper (form → email verification prompt) |
| `/dashboard` | `DashboardPage` | `BalanceCard` (gradient card with balance), `RecentTransactions` (last 10), greeting |
| `/transfer` | `TransferPage` | `TransferForm`, `TransferSuccessDialog` (with video call option) |
| `/transactions` | `TransactionsPage` | `TransactionTable` (master), `TransactionDetailPanel` (detail), pagination |
| `/video-call/:roomName` | `VideoCallPage` | Full-screen `JitsiMeeting` component via `@jitsi/react-sdk` |

`ChatAssistant` is rendered globally inside `BrowserRouter` — a floating FAB that expands into a chat panel, available on all authenticated pages.

### 5.10 Theme System

**`ThemeContextProvider`** — React Context providing `{mode, isDark, toggleTheme}`. Persists mode to `localStorage`. Wraps MUI's `ThemeProvider`.

**`muiTheme.ts`** — Two complete MUI themes (light/dark) with Dubai-Bank branding: deep navy `#0D1B2A` primary, gold `#C9A227` secondary, gradient buttons, custom styles for tables/cards/alerts.

---

## 6. Data Flow

### 6.1 Money Transfer (Primary Flow)

```
Client                          Server                         Database
──────                          ──────                         ────────

TransferForm.onSubmit()
  │
  ├─► useTransfer.handleSubmit()
  │     │
  │     ├─► transactionService.create({receiverEmail, amount, description})
  │     │     │
  │     │     ├─► POST /api/v1/transactions
  │     │     │     │
  │     │     │     ├─► protect middleware (JWT verify → load user)
  │     │     │     ├─► transactionLimiter (10/min)
  │     │     │     ├─► transaction.controller.createTransaction()
  │     │     │     │     │
  │     │     │     │     ├─► validateTransactionAmount(amount)
  │     │     │     │     └─► executeTransfer(senderEmail, receiverEmail, amount, desc)
  │     │     │     │           │
  │     │     │     │           ├─► mongoose.startSession() + startTransaction()
  │     │     │     │           ├─► deductSenderBalance()  ──────► findOneAndUpdate({balance >= amount}, {$inc: -amount})
  │     │     │     │           ├─► addReceiverBalance()   ──────► findOneAndUpdate({email}, {$inc: +amount})
  │     │     │     │           ├─► createTransactionRecord() ──► Counter.findByIdAndUpdate({$inc}) → Transaction.create()
  │     │     │     │           ├─► commitTransaction()
  │     │     │     │           └─► return transaction
  │     │     │     │
  │     │     │     └─► 201 {success: true, data: {transaction}}
  │     │     │
  │     │     └─► return transaction
  │     │
  │     ├─► transactionService.sendNotification(transactionId)
  │     │     │
  │     │     ├─► POST /api/v1/transactions/:id/send-notification
  │     │     │     │
  │     │     │     ├─► Loads sender + receiver from DB
  │     │     │     ├─► generateVideoCallRoomName(sha256(sorted emails))
  │     │     │     ├─► sendTransferNotificationEmailAsync() (fire-and-forget → Brevo API)
  │     │     │     └─► 200 {roomName}
  │     │     │
  │     │     └─► return roomName
  │     │
  │     └─► Open TransferSuccessDialog (with "Start Video Call" option → /video-call/:roomName)
```

### 6.2 AI Chatbot Flow

```
Client                        Server                             External
──────                        ──────                             ────────

ChatAssistant: user types message
  │
  ├─► useChatSocket: socket.emit('user-message', {message})
  │     │
  │     ├─► [Socket.IO /chat namespace]
  │     │     ├─► authenticateSocket middleware (JWT)
  │     │     ├─► Rate limit check (10/60s per user)
  │     │     └─► chatbot.service.processMessage()
  │     │           │
  │     │           ├─► Sanitize input (trim, 250 char cap, strip HTML)
  │     │           └─► openaiFunctionCall.processWithFunctionCalling()
  │     │                 │
  │     │                 ├─► Build messages [system + history(20) + user msg]
  │     │                 ├─► callOpenAI(messages, tools) ────────────────► OpenAI GPT-4o-mini
  │     │                 │                                                      │
  │     │                 │   ◄── response with tool_calls ◄─────────────────────┘
  │     │                 │
  │     │                 ├─► [LOOP up to 5 rounds]
  │     │                 │     ├─► processAllToolCalls() (parallel)
  │     │                 │     │     ├─► get_balance → user.service.getAccountSummary() → DB
  │     │                 │     │     ├─► get_transaction_history → transaction.service → DB
  │     │                 │     │     ├─► transfer_money → executeTransfer() → DB (ACID)
  │     │                 │     │     └─► get_supported_services → static list
  │     │                 │     │
  │     │                 │     ├─► Append tool results to messages
  │     │                 │     └─► callOpenAI(messages, tools) ─────────► OpenAI GPT-4o-mini
  │     │                 │
  │     │                 └─► Return {message, chatHistory, transferCompleted}
  │     │
  │     ├─► socket.emit('bot-message', {message})
  │     │
  │     └─► [if transferCompleted] socket.emit('transfer-completed')
  │
  ├─► useChatSocket: append bot message to state
  │
  └─► [if transfer-completed] dispatch 'dashboard:refresh' DOM event
        └─► useDashboardData: re-fetches user + transactions
```

### 6.3 Authentication Flow

```
Signup: POST /auth/signup → hash password → generate token → store user (PENDING) → send email (Brevo)
Verify: GET /auth/verify?token=X → hash token → find+update user (PENDING→ACTIVE) → render HTML page
Login:  POST /auth/login → find user by email → bcrypt compare → create JWT → set httpOnly cookie → return user
Guard:  Every protected request → extract JWT (cookie or header) → verify → load active user → attach req.user
Logout: POST /auth/logout → disconnect sockets → clear cookie
```

---

## 7. API Endpoints

| Method | Path | Auth | Rate Limit | Purpose |
|---|---|---|---|---|
| POST | `/api/v1/auth/signup` | No | 5/15min | Register new user |
| GET | `/api/v1/auth/verify` | No | 10/hr | Verify email token |
| POST | `/api/v1/auth/resend-verification` | No | 5/15min | Resend verification email |
| POST | `/api/v1/auth/login` | No | 5/15min | Authenticate and get JWT cookie |
| POST | `/api/v1/auth/logout` | No | — | Clear session |
| GET | `/api/v1/me` | Yes | — | Get current user profile + recent transactions |
| GET | `/api/v1/transactions` | Yes | — | Paginated transaction list |
| POST | `/api/v1/transactions` | Yes | 10/min | Create a money transfer |
| GET | `/api/v1/transactions/:id` | Yes | — | Get single transaction by ID |
| POST | `/api/v1/transactions/:id/send-notification` | Yes | — | Send email notification + get video room name |

WebSocket namespace: `/chat` (JWT-authenticated, 10 messages/60s per user).

---

## 8. Environment Variables

### Server (`server/.env`)
| Variable | Purpose |
|---|---|
| `PORT` | HTTP listen port (default 3000) |
| `NODE_ENV` | `development` / `production` |
| `MONGO_URI` | MongoDB connection string (required) |
| `JWT_SECRET` | JWT signing secret (required) |
| `JWT_EXPIRES_IN` | Token TTL (default `1h`) |
| `COOKIE_SAME_SITE` | Cookie SameSite policy (default `lax`) |
| `OPENAI_API_KEY` | OpenAI API key for chatbot |
| `BREVO_API_KEY` | Brevo/Sendinblue transactional email API key |
| `EMAIL_FROM` | Sender email address |
| `CLIENT_URL` | Frontend URL for CORS and email links |
| `SERVER_URL` | Backend URL |

### Client (`client/.env.development`, `client/.env.production`)
| Variable | Purpose |
|---|---|
| `VITE_API_URL` | Backend API base URL |
| `VITE_SOCKET_URL` | Socket.IO server URL |

---

## 9. Testing Infrastructure

### Server (Jest + Supertest + mongodb-memory-server)
- **Integration tests** use an in-memory MongoDB replica set (required for transaction support).
- Files: `auth.routes.test.js`, `login.test.js`, `me.test.js`, `auth.middleware.test.js`, `transactions.test.js`.
- **Unit tests**: `jwt.util.test.js`, `validation.util.test.js`.
- Helper: `db.helper.js` — sets up/tears down the in-memory DB between test suites.

### Client (Vitest + Testing Library + jsdom)
- **API layer tests**: `auth.service.test.ts`, `http-client.test.ts`, `transaction.service.test.ts`, `user.service.test.ts`, `auth.storage.test.ts`.
- **Hook tests**: `useLogin.test.ts`, `useSignup.test.ts`, `useTransfer.test.ts`, `useDashboardData.test.ts`, `useTransactions.test.ts`, `useTransactionDetail.test.ts`.
- **Unit tests**: `displayFormatters.test.ts`, `timeBasedGreeting.test.ts`, `useAsyncOperation.test.ts`.
- Setup: `__tests__/setup.ts` configures the test environment.

**Total: 21 test files** (7 server, 14 client).

---

## 10. Current State & Roadmap

### Fully Implemented
- User registration with email verification (Brevo integration)
- JWT authentication with httpOnly cookies and dual extraction (cookie + Bearer header)
- ACID-compliant peer-to-peer money transfers with overdraft protection
- Paginated transaction history with detail panel
- AI chatbot with OpenAI function calling (balance, history, transfers, service discovery)
- Real-time chat via Socket.IO with per-user rate limiting
- Post-transfer email notifications with embedded video call links
- Jitsi Meet video calling with deterministic room names
- Dark/light theme with full MUI theming
- Docker Compose local development environment
- Production deployment (Vercel + Render + MongoDB Atlas)
- Comprehensive test suites (unit + integration, client + server)
- Rate limiting on auth, verification, and transaction endpoints
- Graceful server shutdown
- Winston structured logging

### Known TODOs / Not Yet Implemented
- **Forgot password flow** — placeholder exists in `LoginForm.tsx` (`Forgot password? (TODO)`).
- **CI/CD pipeline** — no automated test execution on PRs; deployment is auto-triggered by push but untested.
- **End-to-end tests** — no Cypress/Playwright tests exist.
- **OpenAI intent service** — `openaiIntent.service.js` is fully commented out (deprecated in favor of function calling), kept for reference.
- **Input sanitization hardening** — chatbot strips HTML but no dedicated XSS library (e.g., DOMPurify) is used.
- **Pagination on `/me` endpoint** — `getCurrentUser` returns the 10 most recent transactions with no pagination.
- **WebSocket reconnection** — Socket.IO client relies on default reconnection; no custom backoff or UI indicator for disconnection state.

### Recent Development Focus (from git history)
The most recent work focused on chatbot UI polish, OpenAI function tooling integration, Jitsi video call setup, comprehensive testing, Docker infrastructure, and frontend refactoring.
