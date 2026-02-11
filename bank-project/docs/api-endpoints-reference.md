# API Endpoints Quick Reference

Quick reference for all HTTP endpoints in the Dubai-Bank API. Base URL: `http://localhost:3000` (or `SERVER_URL`). API prefix: `/api/v1`.

---

## Health

| Method | Endpoint | Auth | Description |
|--------|----------|------|-------------|
| GET | / | No | Health check |
| GET | /health | No | Health check |

**Response:** `{ "status": "ok", "timestamp": "<ISO string>" }`

---

## API Endpoints

| Method | Endpoint | Auth | Rate limit | Description | Controller |
|--------|----------|------|------------|-------------|------------|
| POST | /api/v1/auth/signup | No | authLimiter | User registration | auth.controller.signup |
| GET | /api/v1/auth/verify | No | verifyLimiter | Verify email (query: token) | auth.controller.verifyEmail |
| POST | /api/v1/auth/resend-verification | No | authLimiter | Resend verification email | auth.controller.resendVerification |
| POST | /api/v1/auth/login | No | authLimiter | Login, set JWT cookie | auth.controller.login |
| POST | /api/v1/auth/logout | No | — | Clear auth cookie | auth.controller.logout |
| GET | /api/v1/me | Yes (protect) | — | Current user + recent transactions | user.controller.getCurrentUser |
| GET | /api/v1/transactions | Yes (protect) | — | Paginated transaction list | transaction.controller.getTransactions |
| POST | /api/v1/transactions | Yes (protect) | transactionLimiter | Create transfer | transaction.controller.createTransaction |
| GET | /api/v1/transactions/:transactionId | Yes (protect) | — | Get transaction by id | transaction.controller.getTransactionById |

---

## Auth and middleware

- **Auth (protect):** JWT from cookie `token` or header `Authorization: Bearer <token>`. Applied to all user and transaction routes. Implemented in `server/src/middleware/auth.middleware.js` (`protect`).
- **authLimiter:** 5 requests per 15 minutes (production). Applied to signup, resend-verification, login. See `server/src/middleware/rateLimit.middleware.js`.
- **verifyLimiter:** 10 requests per hour. Applied to GET /api/v1/auth/verify.
- **transactionLimiter:** 10 requests per minute. Applied to POST /api/v1/transactions.

---

## Route files

- Auth: `server/src/routes/auth.routes.js`
- User: `server/src/routes/user.routes.js`
- Transactions: `server/src/routes/transaction.routes.js`
- Mounting: `server/src/app.js`
