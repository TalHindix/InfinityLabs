---
name: REST API Design Docs
overview: "Analyze the Dubai-Bank server (Express, MongoDB, JWT) and produce five documentation artifacts: API design overview, endpoints reference, improvements analysis, interview Q&A, and a Postman collection—all derived from the actual code."
todos: []
isProject: false
---

# REST API Design Documentation Plan

## Summary of What Was Found

**Project:** `dubai-bank-api` v1.0.0 ([server/package.json](server/package.json)). Express 5 app; API mounted at `/api/v1` ([server/src/app.js](server/src/app.js)). Base URL from env: `SERVER_URL` or `http://localhost:3000`.

**API structure:**

- **Auth** (`/api/v1/auth`): POST signup, GET verify, POST resend-verification, POST login, POST logout. Middleware: `authLimiter` on signup/resend/login; `verifyLimiter` on verify; logout has no rate limit.
- **User** (`/api/v1`): GET `/me` only. All routes behind `protect` ([server/src/routes/user.routes.js](server/src/routes/user.routes.js)).
- **Transactions** (`/api/v1/transactions`): GET `/` (paginated list), POST `/` (create transfer), GET `/:transactionId`. All behind `protect`; POST also has `transactionLimiter` ([server/src/routes/transaction.routes.js](server/src/routes/transaction.routes.js)).

There is **no separate Account resource**; balance lives on the User model and is returned in GET `/me` with `recentTransactions`.

**Auth:** JWT from cookie `token` or `Authorization: Bearer` ([server/src/middleware/auth.middleware.js](server/src/middleware/auth.middleware.js)); token payload `{ id, email }`, expiry from `config.jwt.expiresIn` (default `1h`). No refresh token.

**Responses:** Success: `{ success: true, data }` with 200/201 via [server/src/utils/response.util.js](server/src/utils/response.util.js). Errors: `{ success: false, error: string }` with status from [server/src/middleware/error.middleware.js](server/src/middleware/error.middleware.js) (400, 401, 404, 409, 500; Mongoose ValidationError/11000/CastError, JWT, AppError mapped).

**Validation:** Mongoose schema on User and Transaction; controller-level checks and `AppError`; custom `validateTransactionAmount` in [server/src/utils/validation.util.js](server/src/utils/validation.util.js) (min 0.01, max 1_000_000, 2 decimal places). No Joi/Zod/express-validator.

**Rate limits** ([server/src/middleware/rateLimit.middleware.js](server/src/middleware/rateLimit.middleware.js)): auth 5/15min (prod), verify 10/hour, transactions 10/min.

---

## Document 1: `docs/api-design-overview.md`

**Content (all from actual code):**

- **Project information:** Name, version from `package.json`; base URL from `config.serverUrl` / port 3000; API prefix `/api/v1`.
- **Current API structure** per resource:
  - **Authentication:** Each of the 5 auth endpoints with exact path, method, middleware, and request/response examples taken from [server/src/controllers/auth.controller.js](server/src/controllers/auth.controller.js) (e.g. signup body `firstName, lastName, email, phone, password`; login response `user: { id, firstName, lastName, email }`; verify returns HTML from `buildVerificationResultPage`; logout clears cookie and returns 200 with null data).
  - **User:** GET `/api/v1/me` — response shape from `getCurrentUser`: `user` (id, firstName, lastName, email, phone, balance, status) and `recentTransactions` (10 items).
  - **Transactions:** GET list (query `page`, `limit`, defaults from [server/src/constants/index.js](server/src/constants/index.js) and controller MAX_PAGE_SIZE 100); POST body `receiverEmail`, `amount`, `description`; GET by id. Response shapes from controller and [server/src/services/transaction.service.js](server/src/services/transaction.service.js).
- **Request/response examples:** Use real success/error payloads (e.g. 201 signup `data.message`, 401 `Invalid credentials`, 400 `Receiver email is required`, `Insufficient funds`, `Cannot transfer to yourself`, `Receiver not found` 404).
- **Authentication & authorization:** Document `getTokenFromRequest` (cookie then Bearer), `verifyToken`, `protect` flow, `req.user` shape (`id`, `email`, `status`); cookie options from [server/src/config/index.js](server/src/config/index.js) (tokenName, httpOnly, secure, sameSite, maxAge).
- **Error handling:** `getUserSafeMessage` mapping (ValidationError → 400, 11000 → 409, CastError → 400, JWT → 401, AppError → statusCode, unknown → 500); `notFoundHandler` 404; single JSON shape `{ success: false, error }`.
- **Validation:** What is validated where (signup required fields, login email/password, transaction amount via `validateTransactionAmount`, receiverEmail required); Mongoose validations (user/transaction models); no schema library.
- **Additional:** Optional short sections for example curl commands, edge cases from tests/controllers (duplicate email 409, invalid/expired verify token, rate limit message bodies), security (CORS, cookie, rate limits), and performance (paginated list, indexes on transactions, atomic transfer).

---

## Document 2: `docs/api-endpoints-reference.md`

**Content:** Single quick-reference table.


| Method | Endpoint                            | Auth | Description                        | Controller                                |
| ------ | ----------------------------------- | ---- | ---------------------------------- | ----------------------------------------- |
| POST   | /api/v1/auth/signup                 | No   | Register user                      | auth.controller.signup                    |
| GET    | /api/v1/auth/verify                 | No   | Verify email (query token)         | auth.controller.verifyEmail               |
| POST   | /api/v1/auth/resend-verification    | No   | Resend verification email          | auth.controller.resendVerification        |
| POST   | /api/v1/auth/login                  | No   | Login, set cookie                  | auth.controller.login                     |
| POST   | /api/v1/auth/logout                 | No   | Clear cookie                       | auth.controller.logout                    |
| GET    | /api/v1/me                          | Yes  | Current user + recent transactions | user.controller.getCurrentUser            |
| GET    | /api/v1/transactions                | Yes  | Paginated transactions             | transaction.controller.getTransactions    |
| POST   | /api/v1/transactions                | Yes  | Create transfer                    | transaction.controller.createTransaction  |
| GET    | /api/v1/transactions/:transactionId | Yes  | Transaction by id                  | transaction.controller.getTransactionById |


Include health: GET `/`, GET `/health` → `{ status: 'ok', timestamp }`. Note rate limiters per route where applicable.

---

## Document 3: `docs/api-improvements.md`

**Content (tied to this codebase):**

- **What you do well:** API versioning (`/api/v1`), consistent `{ success, data/error }`, central error handler and safe messages, JWT in httpOnly cookie + Bearer fallback, rate limiting on auth/verify/transactions, pagination on list transactions with cap, atomic transfers with MongoDB session, validation layer for amount.
- **Potential issues:** No refresh token (session fixed to 1h); logout does not invalidate JWT server-side; no idempotency key on POST transfer; GET verify returns HTML (not JSON) for API consistency; optional description required by some clients (e.g. [client useTransfer](client/src/screens/transfer-money/useTransfer.ts) early-return when empty).
- **Recommendations with examples from this repo:**
  1. **Versioning:** Already present; note how to add v2 in app.js.
  2. **Pagination:** Already implemented; document query params and suggest adding `Link` or `X-Total-Count` headers from `findTransactionsByUserEmail` result.
  3. **Refresh token:** Suggest flow and where to plug in (e.g. new route, cookie name) without changing existing login contract.
  4. **Idempotency:** Suggest `Idempotency-Key` header in `createTransaction` and key storage (e.g. in-memory or Redis) with example in transaction.controller.js.
  5. **Validation:** Option to add express-validator or Joi for body validation reusing same error format.
  6. **OpenAPI:** Suggest generating spec from same routes for consistency.

Use actual file paths and function names; no generic snippets.

---

## Document 4: `docs/interview-answers-my-project.md`

**Content:** Interview Q&A where each answer is written as “My Answer” using this project only.

- **API design:** “In my banking application…” — list actual routes and structure (auth, user /me, transactions CRUD), and why (REST, versioning, protect on sensitive routes).
- **Authentication:** “I implemented JWT…” — cookie + Bearer, `protect`, `createToken`/`verifyToken`, payload, no refresh; reference auth.middleware.js and auth.controller.js.
- **Transfer safety:** “I use MongoDB transactions…” — `executeTransfer`, session, deduct/add/create with same session, abort on error; reference transaction.service.js.
- **Error handling:** “We have a central error handler…” — `getUserSafeMessage`, AppError, status codes, no leaking stack; reference error.middleware.js.
- **Validation:** “We validate…” — Mongoose + `validateTransactionAmount` + controller checks; reference validation.util.js and models.
- **Rate limiting:** “We use express-rate-limit…” — authLimiter, verifyLimiter, transactionLimiter with actual windows and limits; reference rateLimit.middleware.js.
- **Security:** Cookie (httpOnly, secure, sameSite), CORS, body limit 10kb, same 401 message for auth failures.

Keep answers concise (e.g. 30–60 sec), with real file paths and function names. Can align with existing [docs/INTERVIEW_MATERIALS.md](docs/INTERVIEW_MATERIALS.md) but focus on REST/API angle.

---

## Document 5: `docs/POSTMAN_COLLECTION.json`

**Content:** Postman Collection v2.1.0.

- **Info:** name `Banking API - Dubai-Bank` (or “My Project”), schema URL.
- **Items:**
  - **Authentication:** 5 requests — POST signup (body raw JSON with firstName, lastName, email, phone, password), GET verify (query `token`), POST resend-verification (body `email`), POST login (body `email`, `password`), POST logout. Use `{{baseUrl}}` variable (e.g. `http://localhost:3000/api/v1`).
  - **User:** GET me (no body; note “Send cookie or Bearer token” in description).
  - **Transactions:** GET list (query `page`, `limit`), GET by id (path `:transactionId`), POST create (body `receiverEmail`, `amount`, `description`).
- **Health:** GET `{{baseUrl}}/../health` or separate base for root (GET `/health`).
- **Variables:** `baseUrl` = `http://localhost:3000/api/v1` (so auth requests use `{{baseUrl}}/auth/...`, user `{{baseUrl}}/me`, transactions `{{baseUrl}}/transactions`).

Use exact route paths and body keys from the codebase; no placeholder endpoints.

---

## Implementation Notes

- **Conventions:** 2 spaces, ES6+, existing response/error shapes; no emojis in docs (per user rules).
- **Sources:** All endpoint details, status codes, and message strings will be copied or derived from the server code and tests (e.g. auth.routes.test.js, validation.util.test.js) so the docs match the running API.
- **Placement:** All five files under the existing [docs/](docs/) folder; no changes to server or client code.

---

## File Output Summary


| Document        | Path                                                                         | Purpose                                              |
| --------------- | ---------------------------------------------------------------------------- | ---------------------------------------------------- |
| Design overview | [docs/api-design-overview.md](docs/api-design-overview.md)                   | Full REST API spec from project code                 |
| Quick reference | [docs/api-endpoints-reference.md](docs/api-endpoints-reference.md)           | Table of method, path, auth, controller              |
| Improvements    | [docs/api-improvements.md](docs/api-improvements.md)                         | Current review + concrete improvements for this repo |
| Interview Q&A   | [docs/interview-answers-my-project.md](docs/interview-answers-my-project.md) | REST/API interview answers citing this project       |
| Postman         | [docs/POSTMAN_COLLECTION.json](docs/POSTMAN_COLLECTION.json)                 | Importable collection for all API endpoints          |


