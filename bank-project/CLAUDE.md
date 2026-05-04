# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Commands

### Server (in `server/`)
```bash
npm run dev           # start with --watch (auto-restart)
npm test              # Jest
npm run test:watch    # Jest watch mode
npm run test:coverage
# Run a single test file (tests live in src/__tests__/unit/ and src/__tests__/integration/):
node --experimental-vm-modules node_modules/.bin/jest src/__tests__/unit/auth.util.test.js
node --experimental-vm-modules node_modules/.bin/jest src/__tests__/integration/login.test.js
```

### Client (in `client/`)
```bash
npm run dev           # Vite dev server on :5173
npm run build         # TypeScript check + Vite build
npm run lint          # ESLint
npm test              # Vitest
npm run test:ui       # Vitest with browser UI
npm run test:coverage
# Run a single test file:
npx vitest run src/__tests__/auth.storage.test.ts
```

### Docker (local testing only)
```bash
docker compose up --build   # MongoDB (rs0 replica set) + server + client (Nginx)
```

The Vite dev server proxies `/api` → `http://localhost:3000`, so client and server can run independently without CORS issues during development.

## Architecture

Full-stack monorepo with independent `client/` and `server/` packages (no root package.json). Client deploys to Vercel, server to Render, database to MongoDB Atlas.

### Server (`server/`)

Express 5 + Socket.IO + Mongoose.

**Layer flow:** `routes/` → `controllers/` → `services/` → `models/`

- Entry: `server.js` bootstraps HTTP + Socket.IO; `src/app.js` configures Express
- Config is frozen at startup (`src/config/index.js`); auto-detects cross-origin by comparing `CLIENT_URL` vs `SERVER_URL` and adjusts cookie `SameSite`/`Secure` accordingly
- Errors: all code throws `AppError` or subclasses (`src/utils/error.util.js`); a single global `errorHandler` middleware catches everything
- Logging: Winston (`src/utils/logger.util.js`); `requestLogger` middleware logs method/path/status/duration at `info` level, `warn` for status ≥ 400

**Auth (2-step JWT + OTP):**
1. `POST /api/v1/auth/login` — verifies password, sends 6-digit OTP via email (bcrypt-hashed, 5-min TTL, max 5 attempts)
2. `POST /api/v1/auth/verify-otp` — verifies OTP, sets httpOnly cookie (`token`, 1 hr), **and returns the token in the response body** so the client can populate its in-memory token for cross-origin `Authorization: Bearer` requests
3. Every protected request: `protect` middleware (`src/middleware/auth.middleware.js`) calls `authenticate()` from `src/utils/auth.util.js`, which checks cookie first then `Authorization: Bearer` header

HTTP routes also have rate limiters via `express-rate-limit`: `authLimiter` (5 req / 15 min), `verifyLimiter` (10 req / hr), `transactionLimiter` (10 req / min) — all disabled in development.

**WebSocket (`/chat` namespace):**
- `authenticateSocket` middleware runs `authenticate()` (JWT + DB lookup) at handshake
- Per-message: token is **re-validated on every `user-message` event** via `verifyTokenOrThrow()` — JWT signature + expiry only, no DB lookup. A deactivated account mid-session won't be caught until token expiry
- `SocketRegistry` class tracks multiple sockets per user (supports multi-tab); used for force-disconnect on logout
- In-process `RateLimiter` class: 10 messages / 60 s per user
- AI chatbot uses OpenAI function calling with 4 tools (balance, transactions, transfer, supported-services); max 5 tool-call rounds per message, max 20 messages in history; responds in the user's detected language

**Database:**
- MongoDB via Mongoose; replica set required for ACID transactions (the `docker-compose.yml` sets up local `rs0`)
- Money transfers use `mongoose.startSession()` + multi-document transactions — deduct → credit → record, or full rollback
- Transaction IDs are sequential integers via a `Counter` collection (atomic `findOneAndUpdate` with `$inc`)
- Video call room names are deterministic SHA-256 hashes of sorted sender+recipient email pairs (generated in `transaction.service.js` and included in transfer notification emails)

### Client (`client/`)

React 19 + TypeScript + Vite. MUI 7 for UI, React Router v7 for routing, Recharts for analytics charts, `react-markdown` for bot message rendering, `@jitsi/react-sdk` for in-app video calls.

**Auth state (non-obvious):**
- JWT token lives **in memory only** (`#token` private field in `src/api/auth.storage.ts`) — intentionally lost on page refresh to prevent XSS; the httpOnly cookie re-authenticates on the first protected request
- `isAuthenticated()` checks for a `StoredUser` in `localStorage`, **not** for the in-memory token. On page refresh the user is still "authenticated" (localStorage survives), the socket reconnects immediately, and the cookie silently repopulates `#token` on the next request
- `localStorage` stores only `StoredUser`: `{ id, firstName, lastName }`. Email, phone, and balance are excluded and fetched from the server
- `AUTH_CHANGE_EVENT` dispatched on `window` notifies all subscribers on login, logout, and 401 intercept

**HTTP client (`src/api/http.client.ts`):**
- Axios with `withCredentials: true`
- Request interceptor attaches Bearer token from memory
- Response interceptor unwraps server envelope `{ data: ... }` so callers receive the payload directly; 401 → `authStorage.clearAuth()`

**WebSocket (`src/components/useChatSocket.ts`):**
- Connects to `/chat` namespace with `withCredentials: true`
- Lifecycle tied to `isAuthenticated`: connects on login, disconnects + cleans up all listeners on logout
- `transfer-completed` event from socket dispatches `DASHBOARD_REFRESH_EVENT` on `window`, which `useDashboardRefresh` picks up to refetch dashboard data without navigation

**Global event bus:** `window` `CustomEvent` is used for two cross-component signals:
- `AUTH_CHANGE_EVENT` — auth state changes
- `DASHBOARD_REFRESH_EVENT` — dashboard data should reload

**Screen pattern:** each screen in `src/screens/` owns a custom hook (`useLogin`, `useTransfer`, `useDashboardData`, etc.) that contains all state, form logic, and data fetching; the page component is a thin composition root. Shared async state is managed via `useAsyncOperation` (`src/shared/useAsyncOperation.ts`).

## Testing

**Server:** Jest with `mongodb-memory-server`. Tests are split into `src/__tests__/unit/` (pure logic) and `src/__tests__/integration/` (real Mongoose connections).

**Client:** Vitest + React Testing Library + jsdom. Setup file: `src/__tests__/setup.ts`. Tests mock axios and auth storage.

## Environment variables

- **Always required:** `JWT_SECRET`
- **Required in production (validated at startup):** `MONGO_URI`, `BREVO_API_KEY`, `EMAIL_FROM`
- **Functionally required but not validated at startup:** `CLIENT_URL` (CORS + cookie SameSite detection; defaults to `http://localhost:5173`), `SERVER_URL` (used in verification email links — throws at send time if absent), `OPENAI_API_KEY` (chatbot silently unavailable without it)
- **Client build-time:** `VITE_API_URL`, `VITE_SOCKET_URL`
