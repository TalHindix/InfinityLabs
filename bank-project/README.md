# Dubai-Bank — Full-Stack Project Overview

> **להצגה בראיון:** קובץ זה מסכם את ה-Stack ואת תהליך כל Route כדי שתוכל להסביר על הלוח.

---

## 1. Tech Stack

| Layer   | Technologies |
|---------|--------------|
| **Client** | React 19, TypeScript, Vite, React Router, MUI (Material-UI), Emotion, Axios, Socket.IO Client |
| **Server** | Node.js, Express 5, ES Modules |
| **Database** | MongoDB (Mongoose) |
| **Auth** | JWT (HTTP-only cookie; server sets cookie at login; optional Bearer header for protect middleware), bcrypt |
| **Email** | Brevo API (verification emails) |
| **Real-time** | Socket.IO (chatbot namespace `/chat`) |
| **AI** | OpenAI (gpt-4o-mini) for chatbot intent fallback |
| **DevOps** | dotenv, Winston (logs), express-rate-limit, CORS |

---

## 2. High-Level Architecture

```
[Browser]  →  Vite (port 5173)  →  React SPA
                  ↓
            API base: VITE_API_URL  →  Express (port 3000)  →  MongoDB
            (e.g. /api/v1 with Vite proxy, or full URL)
                  ↓
            Socket.IO  →  /chat namespace (auth + chatbot)
```

- **Client:** Single Page App. API base URL from `VITE_API_URL` (required). In dev, use e.g. `http://localhost:5173/api/v1` so Vite proxies `/api` to the server (see `client/vite.config.ts`). Axios `http-client` uses `withCredentials: true`; no request interceptor — auth is HTTP-only cookie only; on 401, response interceptor clears user from localStorage. Client API layer: `client/src/api/` (auth.service, transaction.service, user.service, http-client, auth.storage). Hooks live in screens (e.g. `screens/login-signup/useSignup.ts`, `useLogin.ts`; `screens/transfer-money/useTransfer.ts`; `screens/transaction-history/useTransactions.ts`, `useTransactionDetail.ts`; `screens/dashboard/useDashboardData.ts`). Shared hook: `shared/useAsyncOperation.ts`.
- **Server:** REST API under `/api/v1/auth`, `/api/v1` (user), `/api/v1/transactions`; JWT from cookie first, then `Authorization: Bearer`; 404 → notFoundHandler; errors → errorHandler.
- **Socket:** Only `/chat` namespace; auth via cookie or `handshake.auth.token`; then `user-message` → chatbot → `bot-message`.

---

## 3. API Routes — Process Per Route

### Base URL: `/api/v1`

---

### 3.1 Auth — `/api/v1/auth`

| Method | Path | Middleware | Process (step by step) |
|--------|------|------------|-------------------------|
| **POST** | `/signup` | authLimiter | 1. Read body (firstName, lastName, email, phone, password). 2. Hash password (bcrypt). 3. Generate verification token, hash it, save user (status PENDING). 4. Send verification email (Brevo, async). 5. Return 201 + "check your email". |
| **GET** | `/verify` | verifyLimiter | 1. Read `token` from query. 2. If missing → 400 HTML failure page. 3. Hash token, find user by hashed token + PENDING. 4. If not found → 400 HTML failure. 5. Set user status ACTIVE, clear verificationToken, save. 6. Return 200 HTML success page. |
| **POST** | `/resend-verification` | authLimiter | 1. Read `email` from body. 2. If missing → 400 AppError. 3. Find user by email + PENDING. 4. If not found → still 200 (no user enumeration). 5. If found: new token, hash, save; send email async. 6. Return 200 + same message either way. |
| **POST** | `/login` | authLimiter | 1. Read email, password. 2. If missing → 400. 3. Find user by email (with password). 4. If no user or status ≠ ACTIVE → 401 "Invalid credentials". 5. Compare password (bcrypt). 6. If invalid → 401. 7. Create JWT, set HTTP-only cookie (config.cookie.tokenName, path `/`, sameSite/secure/maxAge from config). 8. Return 200 + **user only** (id, firstName, lastName, email) — no token in body; client stores user in localStorage via authStorage.setUser. |
| **POST** | `/logout` | — | 1. Clear auth cookie (same options as set). 2. Return 200. Client calls authService.logout(); client may clear localStorage elsewhere or rely on 401/clearAuth. |

---

### 3.2 User — `/api/v1` (all routes protected)

| Method | Path | Middleware | Process (step by step) |
|--------|------|------------|-------------------------|
| **GET** | `/me` | protect | 1. Read user id from `req.user` (set by protect after JWT + DB lookup). 2. Find user by id. 3. If not found → 404. 4. Find recent transactions by user email (limit 10). 5. Return 200 + user (id, firstName, lastName, email, phone, balance, status) + recentTransactions. |

**Protect middleware:** Token from cookie or `Authorization: Bearer` → verify JWT → find user by decoded id → if no user or status ≠ ACTIVE → 401; else `req.user = user`, next().

---

### 3.3 Transactions — `/api/v1/transactions` (all routes protected)

| Method | Path | Middleware | Process (step by step) |
|--------|------|------------|-------------------------|
| **GET** | `/` | protect | 1. Parse query: page (default 1), limit (default from constants, max 100). 2. Build query: fromEmail OR toEmail = req.user.email. 3. Find transactions (sort createdAt desc, skip, limit) + countDocuments. 4. Return 200 + transactions, total, totalPages, currentPage. |
| **GET** | `/:transactionId` | protect | 1. Find transaction by id (number) AND (fromEmail or toEmail = req.user.email). 2. If not found → 404 AppError. 3. Return 200 + transaction. |
| **POST** | `/` | protect, transactionLimiter | 1. Read receiverEmail, amount, description from body. 2. If no receiverEmail → 400. 3. Validate amount (validation.util: number, 0.01–1M, max 2 decimals). 4. If invalid → 400 with message. 5. Start MongoDB session (transaction). 6. Validate sender ≠ receiver. 7. Deduct amount from sender (findOneAndUpdate balance >= amount). 8. Add amount to receiver. 9. Get next transaction id (counter), create transaction document. 10. Commit session. 11. Return 201 + transaction. (On any error: abort session, throw.) |

---

## 4. Socket.IO — Chatbot (`/chat` namespace)

| Step | What happens |
|------|----------------|
| **Connect** | Client connects to `/chat`. Server runs `authenticateSocket`: token from cookie or handshake.auth.token → verify JWT → find user, check ACTIVE → set socket.user. |
| **Connected** | Server emits `bot-message`: greeting text, intent `greeting`, timestamp. |
| **user-message** | Client sends message. Server: sanitize input → detect intent (keywords or OpenAI fallback) → if sensitive (e.g. balance): check userId; if no userId → emit "Please log in". Else handle intent (e.g. balance → getAccountSummary, mask userId, format balance) or standard response. Emit `bot-message` (response, intent, data, requiresAuth, timestamp). |
| **disconnect** | No cleanup; connection closed. |

---

## 5. Project Structure (Summary)

| Area | Path | Contents |
|------|------|----------|
| **Client** | `client/src/` | `api/` (auth.service, transaction.service, user.service, http-client, auth.storage), `screens/` (login-signup, dashboard, transfer-money, transaction-history), `shared/` (useAsyncOperation, formatters, messages, theme), `context/` (ThemeContext, ThemeContextProvider), `layout/`, `components/`, `constants/`, `types/` |
| **Server** | `server/src/` | `routes/` (auth, user, transaction), `controllers/`, `services/`, `models/` (user, transaction + Counter), `middleware/` (auth, rateLimit, error, logger, socketAuth), `utils/`, `config/`, `socket/` |

For full flow and file-level detail, see **ARCHITECTURE.md**.

---

## Documentation (Docker, Deploy, Dev)

תיעוד על Docker, Production (Vercel + Render + Atlas) וסביבת פיתוח מרוכז בתיקייה **`docs/`**. מפתח קבצים: **[docs/README.md](docs/README.md)**.

- **פיתוח יומי (Dev):** [docs/DEV_WORKFLOW.md](docs/DEV_WORKFLOW.md) — איך להריץ שרת + קליינט מקומית בלי Docker.
- **Docker מקומי:** [docs/DOCKER.md](docs/DOCKER.md) — הסבר על Dockerfile, docker-compose, nginx.
- **בדיקת image לפני Render:** [docs/LOCAL_DOCKER_TEST.md](docs/LOCAL_DOCKER_TEST.md) — סקריפט ופקודות ידניות.
- **Production:** [docs/DEPLOY.md](docs/DEPLOY.md) — משתני סביבה ב-Vercel ו-Render, CORS ו-cookies.

---

## 6. Rate Limits

| Limiter | Window | Max | Used on |
|---------|--------|-----|--------|
| authLimiter | 15 min | 5 (prod) / 999999 (dev) | signup, login, resend-verification |
| verifyLimiter | 1 hour | 10 | GET /auth/verify |
| transactionLimiter | 1 min | 10 | POST /transactions |

---

## 7. Error Handling (Server)

- **AppError:** Controllers throw with message + statusCode; error handler sends that status and message.
- **Mongoose:** ValidationError → 400 (joined messages); duplicate key (11000) → 409; CastError → 400.
- **JWT:** JsonWebTokenError / TokenExpiredError → 401 "Authentication failed".
- **Unknown:** 500 "Something went wrong" (no stack to client).
- **404:** No route matched → notFoundHandler → AppError 404 "Resource not found".

---

## 8. Quick Reference for the Board

**Stack in one line:**  
React (TS, Vite) + Express + MongoDB + JWT (cookie) + Socket.IO + Brevo + OpenAI.

**Auth flow:**  
Signup → email verify (link) → Login → server sets HTTP-only cookie, returns user only; client stores user in localStorage (authStorage). Protect on /me and /transactions reads token from cookie (or Bearer header).

**Transfer flow:**  
Validate amount → MongoDB transaction: deduct sender, add receiver, create transaction record → 201.

**Chatbot flow:**  
Connect (auth) → greeting → user-message → intent (keywords or AI) → auth check for balance → bot-message.

---

*Author: Tal Hindi*
