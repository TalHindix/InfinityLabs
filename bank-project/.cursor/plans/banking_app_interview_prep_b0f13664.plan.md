---
name: Banking App Interview Prep
overview: "A senior-engineer-level interview preparation document for the Dubai-Bank fullstack application: business context, architecture, design decisions, backend/frontend deep-dives, testing, Docker, debugging scenarios, code review angles, simulated Q&A, and honest red flags with improvement ideas."
todos: []
isProject: false
---

# Dubai-Bank: Technical Interview Preparation

This plan is a single reference you can use to present the project confidently and answer depth questions. It is based on a full pass over the codebase and is written in an interview-ready, honest tone.

---

## 0. Business context

**Why a banking app?**

- Demonstrates **sensitive domain** skills: auth, money movement, consistency, and security.
- Real-world features you implemented:
  - **Identity**: Signup with email verification (Brevo), login, JWT in HTTP-only cookies, active/pending status.
  - **Money**: Transfer between users by email, atomic MongoDB transaction (deduct sender, credit receiver, create record), balance on user, paginated transaction history.
  - **UX**: Dashboard (user + recent transactions), transfer with confirmation dialog, transaction detail panel, optional AI chatbot (Socket.IO + OpenAI).
- Intentional scope: single-currency (AED), internal transfers only (no card/payment gateway), no 2FA, no audit log beyond transaction records, no admin panel.

**What you left out and why (good interview answers):**

- **No real payment rails**: Focus was on app logic and data consistency, not PCI or bank integrations.
- **No 2FA**: Would add TOTP or SMS in production; omitted for scope.
- **No refresh tokens**: 1h JWT; you’d add refresh flow for production.
- **Email verification only**: No KYC/ID verification; sufficient for a demo.
- **No idempotency keys on transfers**: Risk of double-submit; you’d add idempotency for production.

---

## 1. System architecture

**High-level**

- **Client**: React 19 + TypeScript, Vite, MUI, React Router, Axios (withCredentials), Socket.IO client. SPA served by Nginx in Docker or Vercel (client-only).
- **Server**: Node.js, Express 5, Mongoose. REST at `/api/v1`. Optional Socket.IO on same process for `/chat` namespace. Winston logging, MongoDB with retry + backoff.
- **DB**: MongoDB; collections: users, transactions, counters (for transaction id sequence).
- **Docker**: [docker-compose.yml](docker-compose.yml): `mongo`, `server`, `client` (Nginx). Server depends on Mongo healthcheck. Single `mongo_data` volume.

**Data flow – Login**

1. User submits email/password to `POST /api/v1/auth/login`.
2. Auth routes use `authLimiter` (e.g. 5/15min prod).
3. Controller: lookup user by email (with password), check active, `bcrypt.compare`, then `createToken(user)` (JWT with id, email, 1h).
4. Response sets HTTP-only cookie (`token`, secure/sameSite from config) and returns user summary; no JWT in body by design.
5. Client stores user in localStorage (for UI only); actual auth is cookie. Subsequent requests send cookie via `withCredentials: true`.

**Data flow – Money transfer**

1. User submits transfer form; client calls `POST /api/v1/transactions` with `receiverEmail`, `amount`, `description` (cookie sent).
2. `protect` middleware: read token from cookie or `Authorization: Bearer`, verify JWT, load user by `id`, check status ACTIVE; set `req.user`.
3. `transactionLimiter` (e.g. 10/min), then controller: validate amount (validation.util), then `executeTransfer(senderEmail, receiverEmail, amount, description)`.
4. Service: `mongoose.startSession()`, `startTransaction()`; validate not self-transfer; `deductSenderBalance` (findOneAndUpdate with `balance: { $gte: amount }`), `addReceiverBalance`, `createTransactionRecord` (next id from Counter); commit or abort.
5. Response 201 with transaction; client can show success and clear form.

**Elevator pitch (about 2 minutes)**  
“Dubai-Bank is a fullstack demo banking app: signup with email verification, login with JWT in HTTP-only cookies, and transfers between users. The backend is Node and Express with MongoDB; transfers run in a single MongoDB transaction so debits and credits stay consistent. The frontend is a React SPA with a dashboard, transfer flow with confirmation, and transaction history with pagination. I added rate limiting on auth and transfers, optional Socket.IO chatbot, and Docker Compose for local run. It’s built to show auth, consistency, and security choices in a finance-style domain.”

**5-minute technical walkthrough**  
“Architecture: React SPA talking to Express REST API and optional Socket.IO; MongoDB for users and transactions. Auth: signup creates a pending user and sends a verification link; login checks password and sets an HTTP-only cookie so the client never touches the JWT. Protected routes use a middleware that reads the cookie or Bearer token and loads the user. For transfers, the controller validates the amount and calls a service that runs in a MongoDB transaction: deduct from sender, add to receiver, create a transaction record with a sequential id from a counter collection; if any step fails we abort. The client uses a single Axios instance with credentials and a 401 interceptor that clears local auth and effectively logs the user out. I use Winston for logs, a central error handler that maps Mongoose/JWT/AppError to safe status codes, and rate limiters on auth and transfer endpoints. Docker Compose wires Mongo, the API, and the Nginx-served client with healthchecks and env for production-like runs.”

---

## 2. Key design decisions


| Choice                                    | Alternative(s)                               | Tradeoff                                                                                                                               |
| ----------------------------------------- | -------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------- |
| **JWT in HTTP-only cookie**               | JWT in localStorage or in response body only | Cookie: XSS can’t steal token; requires sameSite/secure and CORS credentials. You don’t expose JWT to JS.                              |
| **Bearer token fallback**                 | Cookie only                                  | Lets same backend work when cookie isn’t sent (e.g. cross-origin or some clients); you accept slightly more surface (token in header). |
| **MongoDB transaction for transfer**      | Two separate updates or eventual consistency | Single transaction gives atomicity: no “deducted but not credited”; more complexity and session handling.                              |
| **Counter collection for transaction id** | UUID or ObjectId                             | Sequential id is human-friendly and stable for “transaction #123”; counter must be in the same transaction as the insert.              |
| **User identity in JWT (id, email)**      | Only id or only email                        | You use id for DB lookup and email for display/transfer; minimal payload.                                                              |
| **Single Express process + Socket.IO**    | Separate Socket server                       | Simpler deploy and shared auth; less horizontal scaling flexibility for sockets.                                                       |
| **Rate limit on auth (5/15min prod)**     | No limit or higher                           | Reduces brute-force and enumeration; can lock out legitimate users if they mistype repeatedly.                                         |
| **CORS single origin (config.clientUrl)** | `*` or multiple origins                      | Tighter security; you must set CLIENT_URL correctly for each environment.                                                              |


**“If I had more time I would change…”**

- Add **refresh tokens** (store in DB or Redis) and short-lived access JWT to avoid long-lived cookies and improve revocation.
- Add **idempotency keys** for `POST /transactions` to prevent double-submit and duplicate transfers.
- **Structured request ids** and correlation id in logs for tracing a request across services.
- Consider **read-your-writes** for dashboard: after a transfer, invalidate or refetch so balance and list are consistent without full reload.

---

## 3. Backend deep-dive

**Auth flow**

- **Signup**: [auth.controller.js](server/src/controllers/auth.controller.js) – create user (bcrypt, status PENDING, hashed verification token), send email (Brevo), return generic message. No login until verified.
- **Verify**: GET `/auth/verify?token=...` – hash token, find user by verificationToken + PENDING, set ACTIVE and clear token, return HTML success/failure.
- **Login**: find user with password, check ACTIVE, compare password, create JWT, set cookie (path `/`, httpOnly, secure, sameSite, maxAge 1h), return user summary.
- **Logout**: clear cookie same options.
- **Middleware**: [auth.middleware.js](server/src/middleware/auth.middleware.js) – token from cookie or `Authorization: Bearer`, `verifyToken`, `User.findOne({ id })`, check ACTIVE, set `req.user`. Any failure → 401 same message (no enumeration).

**Validation and error handling**

- Amount: [validation.util.js](server/src/utils/validation.util.js) – type, NaN, finite, min/max, 2 decimal places; returns `{ isValid, error, sanitized }`.
- Controller checks required fields (e.g. receiverEmail) and uses AppError for 4xx. [error.middleware.js](server/src/middleware/error.middleware.js): ValidationError → 400, duplicate key → 409, CastError → 400, JWT errors → 401, AppError → statusCode + message, else 500 with generic message. All errors logged with stack.

**Transactions**

- [transaction.service.js](server/src/services/transaction.service.js): `startSession`, `startTransaction`, validate self-transfer, deduct (findOneAndUpdate with `$gte`), add, create record with `getNextTransactionId(session)`, commit or abort, `endSession`. Counter update and Transaction create use the same session.

**Security (OWASP-oriented)**

- **Done well**: HTTP-only cookie (XSS mitigation), bcrypt for passwords, hashed verification tokens, generic login/verify messages (no user enumeration), rate limiting (auth + verify + transfer), CORS single origin, credentials, body size limit 10kb, trust proxy for rate limiter, JWT expiry.
- **Gaps**: No explicit CSRF (mitigated by same-origin/cookie sameSite and state-changing APIs that expect JSON; not form POST from other sites). No Helmet (headers). No request signing or replay protection. Passwords only validated for length in schema (no complexity). Verification tokens are single-use but not time-limited in code (could add expiry). No audit log for sensitive actions.

**Database**

- **Models**: [user.model.js](server/src/models/user.model.js) – id (UUID), email unique, password select:false, balance default 500, status, verificationToken; [transaction.model.js](server/src/models/transaction.model.js) – id (number), fromEmail, toEmail, amount, description, indexes `(fromEmail, createdAt)`, `(toEmail, createdAt)`.
- **Indexes**: Transaction list uses `$or` on fromEmail/toEmail + sort by createdAt; compound indexes support that. User lookup by id (unique) and email (unique). Counter on `_id`.
- **Bottlenecks**: Large transaction history with skip/limit can get slower; cursor-based pagination would scale better. No index on user balance (not used for range queries). Counter is a single doc hotspot under very high write load.

---

## 4. Frontend (React)

**Structure**

- **Screens**: login-signup (LoginPage, SignupPage, forms, verification success), dashboard (BalanceCard, RecentTransactions, useDashboardData), transfer-money (TransferForm, useTransfer), transaction-history (table, pagination, detail panel, useTransactions, useTransactionDetail).
- **API layer**: [http-client.ts](client/src/api/http-client.ts) (Axios, baseURL from env, withCredentials, 401 → authStorage.clearAuth()), auth.service, user.service, transaction.service. Types in [types/index.ts](client/src/types/index.ts); getErrorMessage for user-facing errors.
- **Auth**: [auth.storage.ts](client/src/api/auth.storage.ts) – user in localStorage only; isAuthenticated() is “do we have user?”. RequireAuth in [App.tsx](client/src/App.tsx) redirects to login if not; no token in memory, cookie used by Axios.

**State**

- No global store (Redux/Zustand). Per-screen state (useState) and hooks (useDashboardData, useTransfer, useTransactions, useTransactionDetail, useLogin, useSignup). useAsyncOperation wraps async with loading/error and optional onSuccess.

**API integration**

- Services return unwrapped `data` (e.g. `res.data.data`). Hooks call services and set local state. 401 from any call triggers clearAuth(); RequireAuth then redirects to login.

**Performance**

- useCallback in useAsyncOperation for execute. useDashboardData and useTransactions use a cancelled flag in useEffect cleanup to avoid setState after unmount. No React.memo or useMemo on list items or heavy components; acceptable for current list sizes. Dashboard fetches user + transactions in parallel (Promise.all).

**Error/loading**

- useAsyncOperation sets error string and loading boolean; components show Alert and disabled buttons. useDashboardData/useTransactions expose loading and error; screens show spinners and messages. getErrorMessage prefers `response.data.error` then `message`.

**Note**: Transfer form requires description (front-end required + useTransfer early-return when empty); API allows missing description. Intentional UX choice; mention if asked.

---

## 5. Testing and quality

**What’s tested**

- **Server**: Integration (supertest + real Mongo via db.helper): auth (signup duplicates, validation), login (cookie, wrong password, unverified), protect (cookie, Bearer, no token, invalid, inactive user), GET/POST /transactions (transfer, auth, insufficient funds, self-transfer, validation, pagination, 404), GET /me (user + recent transactions). Unit: jwt.util, validateTransactionAmount.
- **Client**: Unit/hooks (Vitest, mocks): auth.storage, http-client (401 → clearAuth, no clearAuth on 500/network), auth/user/transaction services, useLogin, useSignup, useTransfer, useDashboardData, useTransactions, useTransactionDetail, useAsyncOperation, displayFormatters, timeBasedGreeting.

**Gaps**

- No E2E (Playwright/Cypress): no full login → transfer → verify in browser.
- No integration test for email verification flow (verify endpoint with token).
- No tests for error middleware (e.g. ValidationError → 400) in isolation.
- No load or chaos tests; no tests for MongoDB transaction abort path.
- Socket.IO chatbot not tested.

**One-sentence testing strategy**  
“I focused on backend integration tests for auth and money movement against a real DB, unit tests for validation and JWT, and frontend unit tests for API layer and hooks including cleanup and 401 handling; I’d add E2E for critical flows and more edge-case coverage for errors and transaction rollback.”

---

## 6. Docker and deployment

**Setup**

- [docker-compose.yml](docker-compose.yml): mongo:7 with volume and healthcheck; server build from [server/Dockerfile](server/Dockerfile) (node:20-alpine, npm ci --omit=dev, CMD node server.js); client build from [client/Dockerfile](client/Dockerfile) (multi-stage: build with VITE_API_URL, then nginx:alpine serving dist). Server env: NODE_ENV=production, MONGO_URI to mongo:27017, CLIENT_URL/SERVER_URL. Client gets VITE_API_URL at build time (e.g. [http://localhost:3000/api/v1](http://localhost:3000/api/v1)).
- [client/nginx.conf](client/nginx.conf): try_files for SPA; no proxy to API (API is separate origin in this setup).

**Production readiness**

- **Logging**: Winston to console and files (error.log, combined.log); request logger logs method, path, status, duration. No log aggregation configured.
- **Env**: Required JWT_SECRET, MONGO_URI validated at startup; .env.example documents all vars. Cookie sameSite can be set for cross-origin (e.g. Vercel + Render).
- **Rate limiting**: Applied on auth and transfer routes; trust proxy set for correct client IP behind proxy.
- **CORS**: Single origin from config; credentials true.
- **Failure**: Mongo connection retries with backoff; on final failure process exits. SIGTERM/SIGINT close HTTP server then Mongo connection. No health check on server in compose (only Mongo); no readiness vs liveness split.

**Improvements**

- Add server health in compose (e.g. depends_on with condition or healthcheck hitting GET /health). Consider separate health for “Mongo reachable” vs “ready to serve.”
- Avoid writing logs to container filesystem in prod or add a sidecar/volume for log shipping.
- Document that CLIENT_URL must match the actual frontend origin (e.g. Vercel URL) when API is on Render or another host.

---

## 7. Real-world debugging scenarios

**“User reports money deducted but not received”**

- Check whether a transaction record exists for that amount and time (fromEmail, toEmail, createdAt). If it exists and receiver balance was updated, the bug may be UI/cache (e.g. receiver’s dashboard not refetching).
- If transaction exists but receiver balance not updated: MongoDB transaction should have prevented that (atomic). Check logs for aborted transaction and any error after “addReceiverBalance”; if commit ran, both balance updates are committed. Consider rare cases: receiver deleted or status changed between add and commit (your code doesn’t handle that).
- If no transaction record: transfer failed after deduct (transaction aborted). Sender balance would be rolled back. Check server logs for the request (error stack, “Insufficient funds”, “Receiver not found”, etc.). Explain you’d add request/correlation id to trace the same request in logs.
- If duplicate deduction: no idempotency; user could have submitted twice. Recommend idempotency key and show how you’d store and reject duplicate keys.

**“Server responding slowly”**

- Check logs for slow requests (you have duration in request logger). Identify slow endpoints (e.g. GET /transactions with large page/limit or GET /me with many recent transactions).
- MongoDB: check slow query log; verify indexes on transactions (fromEmail/toEmail + createdAt). If pagination uses skip(n), large n can be slow; suggest cursor-based or capped page size.
- Check rate limiters (auth very strict in prod); confirm not hitting limits. Check Mongo connection pool and whether many concurrent transactions are blocking.
- If “slow” is only after deploy: check env (e.g. MONGO_URI, NODE_ENV), cold start, or missing indexes after schema change.

**“JWT expired mid-transaction”**

- Current design: no refresh; cookie has 1h expiry. If user opens transfer form and submits after 1h, request sends expired token; protect middleware calls verifyToken, which throws TokenExpiredError; error handler returns 401 “Authentication failed”; client 401 interceptor runs clearAuth() (clear localStorage user). User is effectively logged out and redirected to login on next navigation (RequireAuth). So: “We don’t refresh the token; the request fails with 401, we clear auth and send the user to login. I’d improve this by adding refresh tokens and retrying once with a new access token before logging out.”

---

## 8. Code review perspective

**Likely comments**

- **Auth**: “isAuthenticated() is based only on localStorage; if cookie is cleared server-side but client still has user, we show logged-in until next 401.” Defend: “We rely on 401 to sync; for a demo that’s acceptable. I’d add a periodic /me check or use cookie presence if we could read it.”
- **Transfer**: “Description required on frontend but optional in API.” Defend: “Intentional UX to force a reason; API stays flexible for future clients.”
- **useTransfer**: “Early return when description is empty prevents submit; backend would accept.” Acknowledge: “Consistent with UI requirement; we could relax if we make description optional in the form.”
- **Error handler**: “You log full stack for every error; in prod you might redact or sample.” Accept: “Would add log levels and avoid logging sensitive data in production.”
- **Logger**: “Creating logs directory at import can fail in read-only env.” Accept: “Would make it conditional or use a default path.”
- **Docker**: “Server has no healthcheck in compose.” Accept: “Would add a healthcheck and optional depends_on condition for client.”

**Defend**

- MongoDB transaction for transfer (correctness over simplicity).
- HTTP-only cookie + CORS single origin (security).
- Rate limiting on auth and transfer (abuse prevention).
- Central error handler and operational errors (consistency and safe messages).

**Refactor**

- Add idempotency for transfers; consider refresh token and structured logging; optional Helmet and verification token expiry.

---

## 9. Interview simulation – 10 questions

**Technical depth (5)**

1. **“How do you ensure a transfer doesn’t leave the sender debited and the receiver not credited?”**
  We run the whole transfer inside a single MongoDB transaction: deduct sender, add receiver, create transaction record and advance the counter, all in one session. We commit only if all steps succeed; otherwise we abort and roll back. So we get atomicity.
2. **“Why put the JWT in a cookie instead of returning it in the response body and storing it in localStorage?”**
  So any XSS that can read the page still can’t access the token; the browser sends the cookie automatically with credentials. We use httpOnly so JS can’t read it, and we set secure and sameSite for production. The tradeoff is we have to configure CORS and sameSite correctly for cross-origin.
3. **“How does your auth middleware work when the frontend and API are on different origins?”**
  The middleware accepts the token from either the cookie (same-site or sameSite=none with secure) or the Authorization Bearer header. So when the cookie isn’t sent cross-origin, the client can send the token in the header if we expose it (we don’t in the current flow; we’d need to for a cross-origin SPA or mobile client).
4. **“How do you generate sequential transaction IDs and keep them correct under concurrency?”**
  We have a Counter collection with a document (e.g. _id: 'transactions') and a seq field. We use findByIdAndUpdate with $inc: { seq: 1 } inside the same MongoDB session as the transfer transaction, so the counter increment and the transaction insert commit or roll back together. That gives unique, monotonic ids per transfer.
5. **“What indexes do you have on transactions and why?”**
  We have compound indexes (fromEmail, createdAt) and (toEmail, createdAt). Listing uses $or on fromEmail and toEmail with sort by createdAt desc; these indexes support that query pattern so we don’t do a full collection scan.

**Behavioral / design (5)**

1. **“What would you add or change first for production?”**
  I’d add refresh tokens and short-lived access tokens so we can revoke sessions and avoid long-lived JWTs. Then idempotency keys on transfers to prevent double-submit, and structured/correlation logging so we can trace a single request end-to-end.
2. **“How do you handle a user who says they were charged twice?”**
  Right now we don’t have idempotency, so double-submit could create two transfers. I’d look up transactions by sender, amount, and time window to see if there are duplicates, and I’d explain we’d add idempotency (client sends a key, we store it and reject duplicates) to prevent this going forward.
3. **“Why did you choose MongoDB over PostgreSQL for this app?”**
  For a demo I prioritized speed of development and a simple document model for user and transaction; MongoDB transactions are sufficient for this flow. For a real bank I’d consider PostgreSQL for ACID and relational constraints, and possibly a dedicated audit/event store.
4. **“Describe a tradeoff you made between simplicity and robustness.”**
  We use a single JWT with 1h expiry and no refresh. Simple, but if the user is active for more than an hour they get 401 and we clear auth. A more robust approach is refresh tokens and access token renewal, at the cost of more code and storage.
5. **“How do you prevent someone from enumerating valid emails via login or signup?”**
  For login we return the same “Invalid credentials” for wrong password, non-existent user, or unverified account. For signup we don’t say “email already exists”; we return a generic “check your email” and only send the verification email if the user exists and is pending. Resend verification uses the same generic message whether or not we sent an email.

---

## 10. Red flags and improvements

**Weak spots an interviewer might challenge**

- **No idempotency on transfers**: Double submit or retry can create two transfers. “I’d add an idempotency key header and store it; same key within a window returns 201 with the same transaction.”
- **Auth state is localStorage + cookie**: If cookie is cleared (e.g. server-side logout elsewhere) but client still has user, UI shows logged-in until next API call returns 401. “We rely on 401 to sync; I’d add a periodic /me or use a short-lived in-memory token check.”
- **JWT not revocable**: Until expiry, a leaked token is valid. “I’d introduce refresh tokens and an access token short TTL, and optionally a blocklist or DB check for critical actions.”
- **Verification token not expired**: Link could be used much later. “I’d add a verificationTokenExpiresAt and reject old links.”
- **Logger creates `logs/` at load**: Can fail in read-only or restricted environments. “I’d make it configurable or create only when writing.”
- **No E2E tests**: Critical path (login → transfer → see transaction) not automated. “I’d add Playwright or Cypress for the main flows.”
- **Transfer form requires description**: API allows optional description; frontend requires it. “Intentional UX; we could align by making description optional in the UI or required in the API.”

**Concrete “if I had more time” items**

- Idempotency for POST /transactions (key in header, store in Redis or DB with TTL).
- Refresh token (e.g. in DB or Redis) and access token rotation; logout invalidates refresh token.
- E2E test: login → dashboard → transfer → transaction list.
- Verification token expiry (e.g. 24h) and clear error message when expired.
- Optional: Helmet for security headers; request/correlation id in logs; cursor-based pagination for transactions.
- Healthcheck in Docker Compose for the server and document CLIENT_URL for each deployment environment.

---

## Summary

You have a coherent fullstack demo with real auth, atomic transfers, rate limiting, and clear structure. The main gaps to emphasize as “known and would fix for production” are: no refresh tokens, no idempotency on transfers, auth state tied to 401, and no E2E tests. Use the elevator pitch and 5-minute walkthrough as your opening, then lean on the design decisions and debugging answers when they go deep. Be ready to defend MongoDB transaction and HTTP-only cookie, and to propose refresh tokens and idempotency as the first improvements.