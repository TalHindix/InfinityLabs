# Dubai-Bank -- Full Project Technical Report

> A comprehensive codebase analysis for technical interview preparation.

---

## 1. Project Executive Summary

### Core Purpose

Dubai-Bank is a **full-stack online banking application** that allows users to register (with email verification), log in, view their account balance and transaction history, transfer money to other users, receive email notifications with video-call links for completed transfers, and interact with an **AI-powered chatbot** that can answer banking questions and execute transactions via natural language.

### Tech Stack

| Layer | Technology | Key Details |
|-------|-----------|-------------|
| **Frontend** | React 19, TypeScript, Vite 7 | SPA with client-side routing |
| **UI Library** | MUI 7 + Emotion | Light/dark theme with gold accent palette |
| **Routing** | React Router 7 | Declarative routes with `RequireAuth` guards |
| **HTTP Client** | Axios | Centralized `httpClient` with credential forwarding and 401 interception |
| **Real-time** | Socket.IO Client | `/chat` namespace for the AI assistant |
| **Video** | Jitsi React SDK | Peer-to-peer video calls between sender/receiver |
| **Backend** | Node.js, Express 5 | ES Modules (`"type": "module"`), no transpiler |
| **Database** | MongoDB (Mongoose 8) | Atlas in production, local/Docker for dev |
| **Auth** | JWT (jsonwebtoken) + bcrypt | HTTP-only cookie, not localStorage |
| **AI** | OpenAI API (GPT) | Function-calling pattern for balance, history, transfers |
| **Email** | Brevo (Sendinblue) API | Verification emails, transfer notifications |
| **Logging** | Winston | Console + file transports, request-level middleware |
| **Testing** | Jest + Supertest (server), Vitest + Testing Library (client) | mongodb-memory-server for integration tests |
| **Containerization** | Docker, Docker Compose | Multi-stage builds; Nginx for client |

---

## 2. Architecture & Design

### Architectural Pattern

The project follows a **Layered Architecture** within a **Client-Server** model:

```
Client (React SPA)  <──HTTP/REST + WebSocket──>  Server (Express API)  <──Mongoose──>  MongoDB
```

The backend itself is organized in clean layers:

```
Routes  -->  Controllers  -->  Services  -->  Models (Mongoose)
                |
           Middleware (auth, rate-limit, logging, error handling)
```

- **Routes** define endpoints and attach middleware (rate limiters, auth guards).
- **Controllers** handle request/response parsing, validation, and delegation.
- **Services** contain business logic (user creation, password hashing, transfer execution with MongoDB transactions).
- **Models** define the Mongoose schemas and database-level validation.
- **Utilities** provide cross-cutting concerns (JWT, email, logging, error classes, response formatting).

### Folder Structure

```
bank-project/
├── client/                          # Frontend SPA
│   ├── src/
│   │   ├── api/                     # HTTP client, auth storage, service modules
│   │   ├── components/              # Shared components (ChatAssistant, VideoCall)
│   │   ├── constants/               # Route paths
│   │   ├── context/                 # React Context (Theme)
│   │   ├── layout/                  # AppHeader, PageFooter
│   │   ├── screens/                 # Feature-based pages
│   │   │   ├── dashboard/
│   │   │   ├── login-signup/
│   │   │   ├── transfer-money/
│   │   │   ├── transaction-history/
│   │   │   └── video-call/
│   │   ├── shared/                  # Hooks, theme, formatters
│   │   ├── types/                   # TypeScript interfaces
│   │   └── __tests__/               # Unit + hook tests
│   ├── Dockerfile                   # Multi-stage: Node builder + Nginx
│   └── vite.config.ts
│
├── server/                          # Backend API
│   ├── server.js                    # Entry: DB connect, HTTP listen, Socket.IO init
│   ├── src/
│   │   ├── app.js                   # Express app: middleware pipeline + route mounting
│   │   ├── config/                  # Env parsing, DB connection with retry
│   │   ├── constants/               # User statuses, defaults
│   │   ├── controllers/             # Request handlers
│   │   ├── middleware/              # Auth, error, logger, rate-limit, socket auth
│   │   ├── models/                  # Mongoose schemas (User, Transaction, Counter)
│   │   ├── routes/                  # Express routers
│   │   ├── services/                # Business logic + OpenAI integration
│   │   ├── socket/                  # Socket.IO handler
│   │   ├── utils/                   # JWT, email, validation, error, logger, response
│   │   └── __tests__/               # Integration + unit tests
│   └── Dockerfile                   # Single-stage Node Alpine
│
├── docker-compose.yml               # Local: MongoDB + server + client
└── docker-compose.override.yml      # Override for host-local MongoDB
```

### Entry Points

**Backend:** `server/server.js` -- connects to MongoDB, starts the Express HTTP server, initializes Socket.IO on the same HTTP server, and registers graceful shutdown handlers for `SIGTERM`/`SIGINT`.

**Frontend:** `client/src/main.tsx` -- renders `<App />` wrapped in `<StrictMode>` and `<ThemeContextProvider>`. `App.tsx` sets up `BrowserRouter`, defines all routes, and renders the global `<ChatAssistant />` component.

---

## 3. Component Communication & Data Flow

### Frontend-Backend Communication

The app uses two communication channels:

#### 3.1 RESTful HTTP API (via Axios)

Base path: `/api/v1`

| Method | Endpoint | Purpose |
|--------|----------|---------|
| `POST` | `/auth/signup` | Register a new user |
| `GET` | `/auth/verify?token=` | Email verification (server returns HTML) |
| `POST` | `/auth/resend-verification` | Re-send verification email |
| `POST` | `/auth/login` | Authenticate; sets HTTP-only cookie |
| `POST` | `/auth/logout` | Clear auth cookie; disconnect sockets |
| `GET` | `/me` | Get current user + 10 recent transactions |
| `GET` | `/transactions` | Paginated transaction list |
| `POST` | `/transactions` | Create a money transfer |
| `GET` | `/transactions/:id` | Single transaction detail |
| `POST` | `/transactions/:id/send-notification` | Email notification + video call room |

#### 3.2 WebSocket (Socket.IO)

Namespace: `/chat`

| Event | Direction | Payload |
|-------|-----------|---------|
| `user-message` | Client -> Server | Plain text message |
| `bot-message` | Server -> Client | `{ response, intent, data, requiresAuth, timestamp }` |
| `transfer-completed` | Server -> Client | (empty) -- triggers dashboard refresh |

### Tracing a Typical User Action: "Transfer Money"

Here is the complete flow when a user sends money:

1. **User clicks "Transfer"** on `TransferPage` and fills in the `TransferForm` (recipient email, amount, description).

2. **`useTransfer` hook** calls `transactionService.create({ receiverEmail, amount, description })`.

3. **Axios `httpClient`** sends `POST /api/v1/transactions` with the JSON body. The HTTP-only JWT cookie is automatically attached (`withCredentials: true`).

4. **Express routing** (`transaction.routes.js`) matches the route. The `protect` middleware runs first:
   - Extracts the JWT from the cookie (or `Authorization` header as fallback).
   - Calls `verifyToken()` to decode it.
   - Loads the user from MongoDB via `findActiveUserById()`.
   - Attaches `req.user` and calls `next()`.

5. **Rate limiter** (`transactionLimiter`) checks the IP has not exceeded 10 requests/minute.

6. **Controller** (`createTransaction`) validates inputs and calls `executeTransfer()` in the service layer.

7. **Service** (`executeTransfer`) starts a **MongoDB session/transaction** (ACID):
   - `deductSenderBalance()` -- atomically decrements sender's balance (fails if insufficient funds).
   - `addReceiverBalance()` -- atomically increments receiver's balance.
   - `createTransactionRecord()` -- gets the next sequential ID from the `Counter` collection and creates the `Transaction` document.
   - Commits the session. On any error, aborts.

8. **Response** flows back: `201 Created` with `{ success: true, data: { transaction } }`.

9. **Frontend** receives the response. `useTransfer` opens the `TransferSuccessDialog`, which offers to send an email notification and/or start a video call.

10. If the user opts to notify, `POST /transactions/:id/send-notification` triggers a Brevo email to the receiver with a video-call link.

### State Management (Frontend)

The frontend uses **no global state library** (no Redux, no Zustand). State is managed through:

- **React Context** -- `ThemeContextProvider` for light/dark mode, persisted in `localStorage`.
- **`authStorage`** -- a thin abstraction over `localStorage` for user info. The actual JWT lives in an HTTP-only cookie (not accessible to JS). Auth state changes are broadcast via a custom DOM event (`AUTH_CHANGE_EVENT`), and the `useAuth` hook subscribes to it.
- **Local component state** -- each screen has a dedicated custom hook (`useDashboardData`, `useTransfer`, `useTransactions`) that encapsulates API calls, loading/error states, and derived data.
- **Custom event bus** -- `dashboard:refresh` is dispatched by the chat socket when a transfer completes via the chatbot, causing the dashboard to re-fetch.

### Authentication & Authorization Flow

| Aspect | Implementation |
|--------|----------------|
| **Signup** | Password hashed with bcrypt (10 rounds). User created with `PENDING` status. Verification token hashed (SHA-256) and stored. Raw token emailed to user. |
| **Email Verification** | `GET /auth/verify?token=...` -- server hashes the query token, finds the matching `PENDING` user, updates status to `ACTIVE`. Returns an HTML result page. |
| **Login** | Validates credentials. Creates a JWT containing `{ id, email }`. Sets it as an HTTP-only, Secure (in production), SameSite cookie with 1-hour expiry. |
| **Request Auth** | `protect` middleware extracts JWT from cookie (or `Bearer` header), verifies it, loads the active user, and attaches to `req.user`. |
| **Socket Auth** | `authenticateSocket` middleware runs on WebSocket connection. Extracts JWT from cookie header or `socket.handshake.auth.token`. Each subsequent message also re-validates the token. |
| **Logout** | Server clears the cookie and disconnects all of that user's active WebSocket connections. Client clears `localStorage` and redirects to `/login`. |
| **401 Handling** | Axios response interceptor catches 401s, calls `authStorage.clearAuth()`, and rejects the promise. The `RequireAuth` wrapper then redirects to login. |

---

## 4. Deployment & Infrastructure

### Current Deployment Setup

| Component | Platform | Method |
|-----------|----------|--------|
| **Server** | Render (Docker Service) | Builds `server/Dockerfile`, runs `node server.js` |
| **Client** | Vercel | Auto-deploys from repo; Vite builds to static files |
| **Database** | MongoDB Atlas | Managed cloud cluster |

### How the Server Deploys to Render

Based on the `server/Dockerfile`:

```dockerfile
FROM node:20-alpine
WORKDIR /app
COPY package.json package-lock.json ./
RUN npm ci --omit=dev
COPY . .
EXPOSE 3000
CMD ["node", "server.js"]
```

Render detects the Dockerfile, builds the image on each push, and runs it as a Docker service. Environment variables (`MONGO_URI`, `JWT_SECRET`, `CLIENT_URL`, `OPENAI_API_KEY`, etc.) are configured in Render's dashboard. The `COOKIE_SAME_SITE=none` setting is critical because the client (Vercel) and API (Render) are on different origins.

### Why Render?

- **Zero-ops Docker hosting** -- push-to-deploy with automatic builds.
- **Free tier** available for side projects and portfolio apps.
- **Built-in HTTPS** -- no need to configure SSL certificates.
- **Environment variable management** in the dashboard.
- **Health checks and auto-restart** on crash.

### Interview Prep: Manual Deployment Alternative

If you were not using Render, here is the equivalent manual process:

1. **Provision a VPS** (e.g., AWS EC2, DigitalOcean Droplet) running Ubuntu.

2. **Install dependencies:**
   - Node.js 20 (via `nvm` or NodeSource).
   - Nginx as a reverse proxy.
   - PM2 as a Node.js process manager.
   - Certbot for SSL (Let's Encrypt).

3. **Clone and build:**
   ```bash
   git clone <repo> /var/www/dubai-bank
   cd /var/www/dubai-bank/server && npm ci --omit=dev
   cd /var/www/dubai-bank/client && npm ci && npm run build
   ```

4. **Configure PM2:**
   ```bash
   pm2 start server.js --name dubai-bank-api
   pm2 save && pm2 startup
   ```
   PM2 handles automatic restarts on crash, log rotation, and startup on reboot.

5. **Configure Nginx** as a reverse proxy:
   ```nginx
   server {
       listen 80;
       server_name api.dubai-bank.com;

       location / {
           proxy_pass http://127.0.0.1:3000;
           proxy_http_version 1.1;
           proxy_set_header Upgrade $http_upgrade;
           proxy_set_header Connection "upgrade";
           proxy_set_header Host $host;
           proxy_set_header X-Real-IP $remote_addr;
       }
   }

   server {
       listen 80;
       server_name dubai-bank.com;
       root /var/www/dubai-bank/client/dist;
       index index.html;

       location / {
           try_files $uri $uri/ /index.html;
       }
   }
   ```
   The `Upgrade` headers are essential for WebSocket (Socket.IO) to work through the proxy.

6. **SSL with Certbot:**
   ```bash
   certbot --nginx -d dubai-bank.com -d api.dubai-bank.com
   ```
   Certbot modifies the Nginx config to add TLS termination and sets up auto-renewal via cron/systemd timer.

7. **Database:** Either run MongoDB on the same server (not recommended for production) or use MongoDB Atlas. If self-hosting, configure authentication, replica sets (required for Mongoose transactions), and backups.

8. **CI/CD:** Set up a GitHub Actions workflow to SSH into the server, pull changes, install dependencies, and `pm2 restart`.

---

## 5. Database Schema

### Entities

There are **three MongoDB collections**: `users`, `transactions`, and `counters`.

#### User

| Field | Type | Notes |
|-------|------|-------|
| `id` | String (UUID) | Application-level unique ID (not `_id`) |
| `firstName` | String | 2-50 chars, trimmed |
| `lastName` | String | 2-50 chars, trimmed |
| `email` | String | Unique, lowercase, regex-validated |
| `password` | String | bcrypt hash, `select: false` (excluded from queries by default) |
| `phone` | String | International format regex |
| `status` | String | `PENDING` (default) or `ACTIVE` |
| `verificationToken` | String | SHA-256 hash of the raw email-verification token |
| `balance` | Number | Defaults to 500 AED, min 0 |
| `createdAt` | Date | Auto-generated |

#### Transaction

| Field | Type | Notes |
|-------|------|-------|
| `id` | Number | Sequential, from Counter collection |
| `fromEmail` | String | Sender's email |
| `toEmail` | String | Receiver's email |
| `amount` | Number | 0.01 -- 1,000,000 |
| `description` | String | Optional, max 500 chars |
| `createdAt` | Date | Auto-generated |
| **Indexes** | | `{ fromEmail: 1, createdAt: -1 }`, `{ toEmail: 1, createdAt: -1 }` |

#### Counter (Internal)

| Field | Type | Notes |
|-------|------|-------|
| `_id` | String | Always `"transactions"` |
| `seq` | Number | Auto-incremented for sequential transaction IDs |

### Relationships

```
User  ──(email)──>  Transaction.fromEmail   (one-to-many: a user sends many transactions)
User  ──(email)──>  Transaction.toEmail     (one-to-many: a user receives many transactions)
```

Relationships are **implicit** -- there are no Mongoose `ref` fields or `populate()` calls. Transactions reference users by their `email` string. This is a deliberate design choice: queries for "all transactions involving user X" use a single `$or` query on `fromEmail`/`toEmail`, which is efficient thanks to the compound indexes.

The trade-off is that if a user's email ever changed, historical transactions would become orphaned. Since the app does not support email changes, this is acceptable.

### Data Integrity

Money transfers use **MongoDB multi-document transactions** (sessions) to ensure atomicity:

- Sender balance is decremented.
- Receiver balance is incremented.
- Transaction record is created.

All three operations commit or abort together. The sender's balance check (`balance: { $gte: amount }`) is part of the atomic `findOneAndUpdate`, preventing race conditions and overdrafts.
