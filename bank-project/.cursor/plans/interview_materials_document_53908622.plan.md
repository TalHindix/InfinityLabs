---
name: Interview Materials Document
overview: Create a single, actionable markdown document containing 10 sections of interview-ready materials (elevator pitch, code walkthrough, Q&A, behavioral, debugging, tradeoffs, next steps, deep-dives, money-transfer narrative, red-flag defenses), all grounded in the Dubai-Bank codebase with exact file paths and function names.
todos: []
isProject: false
---

# Interview-Ready Materials: Single Document Plan

## Deliverable

**One markdown file** at [docs/INTERVIEW_MATERIALS.md](docs/INTERVIEW_MATERIALS.md) (create `docs/` if missing). All content will reference your actual codebase: real file paths under `server/` and `client/`, real function and route names (e.g. `executeTransfer`, `protect`, `createToken`, `POST /api/v1/transactions`).

---

## Section 1: Elevator pitch (2 minutes)

- **Structure**: Opening (10s) → Stack (30s) → Features (40s) → Technical highlights (30s) → Closing (10s).
- **Content**: Single prose script, word-for-word speakable. Name the project (Dubai-Bank), why you built it (demo banking to show auth, consistency, security). Stack: Node.js, Express 5, MongoDB, Mongoose, JWT in HTTP-only cookies, React 19, TypeScript, Vite, MUI, Docker Compose. Features: signup with email verification (Brevo), login, transfers with atomic MongoDB transaction, dashboard, transaction history with pagination, optional Socket.IO chatbot. Highlights: atomic transfers in `transaction.service.js`, `protect` middleware and cookie vs Bearer in `auth.middleware.js`, rate limiters on auth and transfer routes, central error handler. Closing: what you learned and “next steps” (e.g. refresh tokens, idempotency, E2E).
- **Sources**: [server/README.md](server/README.md), [client/README.md](client/README.md), [server/src/services/transaction.service.js](server/src/services/transaction.service.js), [server/src/middleware/auth.middleware.js](server/src/middleware/auth.middleware.js), [server/src/routes/auth.routes.js](server/src/routes/auth.routes.js), [server/src/routes/transaction.routes.js](server/src/routes/transaction.routes.js).

---

## Section 2: Code walkthrough (5 minutes)

- **Format**: Ordered list of “Show: &lt;path&gt;” plus 2–3 sentence script per file.
- **Order and key files**:
  1. [server/src/app.js](server/src/app.js) – Express app, CORS, cookieParser, body limit, routes mount, error handlers.
  2. [server/src/routes/transaction.routes.js](server/src/routes/transaction.routes.js) – `router.use(protect)`, `transactionLimiter` on POST, GET list and by id.
  3. [server/src/controllers/transaction.controller.js](server/src/controllers/transaction.controller.js) – `createTransaction`: validate amount via `validation.util`, call `executeTransfer`, return 201.
  4. [server/src/services/transaction.service.js](server/src/services/transaction.service.js) – `executeTransfer`: session, startTransaction, deductSenderBalance, addReceiverBalance, createTransactionRecord with `getNextTransactionId(session)`, commit/abort.
  5. [server/src/middleware/auth.middleware.js](server/src/middleware/auth.middleware.js) – `getTokenFromRequest` (cookie then Bearer), `verifyToken`, User lookup, `req.user`.
  6. [client/src/App.tsx](client/src/App.tsx) – Routes, `RequireAuth`, `authStorage.isAuthenticated()`.
  7. [client/src/api/http-client.ts](client/src/api/http-client.ts) – Axios withCredentials, 401 interceptor → `authStorage.clearAuth()`.
  8. [client/src/screens/transfer-money/TransferPage.tsx](client/src/screens/transfer-money/TransferPage.tsx) and [useTransfer.ts](client/src/screens/transfer-money/useTransfer.ts) – Form state, `transactionService.create`, `useAsyncOperation`.
- **Deep dive**: One end-to-end trace for **POST /api/v1/transactions** (money transfer): browser → `TransferForm` submit → `useTransfer.handleSubmit` → `transactionService.create` (client) → http-client (cookie sent) → Express → `protect` → `transactionLimiter` → `createTransaction` → `validateTransactionAmount` → `executeTransfer` (session, deduct, add, create record, commit) → 201 → client success handler, form reset. Reference exact function and file names.

---

## Section 3: Interview Q&A (15 questions)

- **Source**: The 10 questions from the existing plan (Section 9: 5 technical + 5 behavioral). Add **5 more** in the same style: e.g. rate limiting and why auth is strict, central error handling and safe messages, Socket.IO auth (cookie/Bearer in `socketAuth.middleware.js`), pagination (skip/limit and indexes), CORS and credentials.
- **Per question**: (1) Question text. (2) “What they’re testing” (one line). (3) Complete ~30-second answer, word-for-word, referencing your code (e.g. `executeTransfer` in `transaction.service.js`, `getTokenFromRequest` in `auth.middleware.js`). Where relevant, end with “if I had more time I’d add X” (idempotency, refresh tokens, E2E, etc.).

---

## Section 4: Behavioral questions (5)

- **Questions**: (1) Why MongoDB over PostgreSQL? (2) Tell me about a technical challenge (e.g. atomic transfer or auth/cookie). (3) What would you do differently if you started over? (4) How did you prioritize features? (5) What was the most complex part to implement?
- **Per question**: STAR answer (Situation, Task, Action, Result) in prose, plus 2–3 technical details to weave in (e.g. MongoDB transaction in `executeTransfer`, HTTP-only cookie in `auth.controller.js`, rate limiters in `rateLimit.middleware.js`).

---

## Section 5: Debugging scenarios (3)

- **Scenario 1 – “Money deducted but not received”**: Steps: (1) Find transaction by sender email, amount, createdAt. (2) If exists, check receiver balance (User by toEmail). (3) If no transaction, check server logs (Winston, request logger). (4) Consider duplicate (no idempotency). Mention: Transaction model, User model, `executeTransfer` abort path, logs in `logger.util.js` / `logger.middleware.js`. Commands: MongoDB queries for transactions and users; `docker compose logs server` or `tail logs/combined.log`.
- **Scenario 2 – “Server slow under load”**: Steps: (1) Request duration from request logger. (2) Identify slow route (e.g. GET /transactions with large page). (3) MongoDB slow query log; indexes on Transaction (`fromEmail`, `toEmail`, `createdAt`). (4) Rate limiters (auth 5/15min). Mention: [transaction.model.js](server/src/models/transaction.model.js) indexes, [rateLimit.middleware.js](server/src/middleware/rateLimit.middleware.js). Prevention: cursor pagination, cap page size (you have MAX_PAGE_SIZE 100).
- **Scenario 3 – “JWT expired mid-transaction”**: How it’s handled: request hits `protect` → `verifyToken` throws TokenExpiredError → error middleware 401 → client 401 interceptor `clearAuth()` → RequireAuth redirect to login. Files: [auth.middleware.js](server/src/middleware/auth.middleware.js), [error.middleware.js](server/src/middleware/error.middleware.js), [http-client.ts](client/src/api/http-client.ts). Improvement: refresh token and retry once before logout.

---

## Section 6: Tradeoffs table

- **Rows (5–7)**: Auth (JWT in HTTP-only cookie vs localStorage/sessions); Database (MongoDB vs PostgreSQL); Transfer consistency (MongoDB transaction vs two-phase/eventual); API style (REST + optional Socket.IO); State management (local/hooks vs Redux); Deployment (Docker Compose, Nginx + Node); Rate limiting (strict auth 5/15min vs looser).
- **Columns**: Decision | What I chose | Why | Tradeoff | Alternative. Filled from existing plan Section 2 and code (e.g. [auth.controller.js](server/src/controllers/auth.controller.js) cookie options, [transaction.service.js](server/src/services/transaction.service.js) session, [rateLimit.middleware.js](server/src/middleware/rateLimit.middleware.js)).

---

## Section 7: “Next steps” talking points (5)

- **Items**: (1) Idempotency for POST /transactions. (2) Refresh token + short-lived access token. (3) E2E tests (e.g. Playwright/Cypress) for login → transfer → list. (4) Verification token expiry (e.g. 24h). (5) Structured/correlation logging or server healthcheck in Docker Compose.
- **Per item**: One-sentence improvement; why it matters (business/tech); high-level how (1–2 sentences). No generic filler; tie to your stack (Express, Mongo, Docker).

---

## Section 8: Technical deep-dives (3 topics)

- **Topic 1 – MongoDB transaction (transfer)**: What it does (atomic debit/credit/record). How: `mongoose.startSession()`, `startTransaction()`, `deductSenderBalance` (findOneAndUpdate with `balance: { $gte: amount }`), `addReceiverBalance`, `createTransactionRecord` with `getNextTransactionId(session)`, commit/abort in try/catch/finally. Code refs: [transaction.service.js](server/src/services/transaction.service.js) `executeTransfer`, Counter in [transaction.model.js](server/src/models/transaction.model.js). Edge cases: self-transfer rejected, insufficient funds, receiver not found. Improve: idempotency key.
- **Topic 2 – JWT auth flow**: Creation in login (`createToken` in [jwt.util.js](server/src/utils/jwt.util.js)), cookie set in [auth.controller.js](server/src/controllers/auth.controller.js) (httpOnly, secure, sameSite, maxAge). Validation: [auth.middleware.js](server/src/middleware/auth.middleware.js) `getTokenFromRequest` → cookie or Bearer → `verifyToken` → User.findOne by id, status ACTIVE. No refresh; expiry 1h (config). Edge cases: missing token, invalid/expired, inactive user → same 401. Improve: refresh token and rotation.
- **Topic 3 – Socket.IO chatbot auth**: What: /chat namespace, auth on connect. How: [socketAuth.middleware.js](server/src/middleware/socketAuth.middleware.js) – token from `handshake.auth.token` or Cookie header parse, `verifyToken`, User by id, status ACTIVE, set `socket.user`. [socket.handler.js](server/src/socket/socket.handler.js) uses `socket.user.id` for context. Edge cases: no token, expired → connection rejected. Improve: same as JWT (refresh not applicable to socket connect).

---

## Section 9: “Walk me through a feature” – Money transfer

- **Narrative**: One continuous story, start to finish.
- **Frontend**: User on Transfer page → fills recipient, amount, description (required in [TransferForm.tsx](client/src/screens/transfer-money/TransferForm.tsx)) → Confirm dialog → Submit → [useTransfer.ts](client/src/screens/transfer-money/useTransfer.ts) `handleSubmit` → `transactionService.create` in [transaction.service.ts](client/src/api/transaction.service.ts) → `httpClient.post('/transactions', data)` with cookie from [http-client.ts](client/src/api/http-client.ts).
- **Backend**: Request hits [app.js](server/src/app.js) → [transaction.routes.js](server/src/routes/transaction.routes.js) → `protect` ([auth.middleware.js](server/src/middleware/auth.middleware.js)) → `transactionLimiter` → [transaction.controller.js](server/src/controllers/transaction.controller.js) `createTransaction` → `validateTransactionAmount` ([validation.util.js](server/src/utils/validation.util.js)) → [transaction.service.js](server/src/services/transaction.service.js) `executeTransfer` → MongoDB session, deduct/add/create record, commit.
- **Response**: 201 with transaction → client receives → `onSuccess` in useTransfer clears form, sets success → user sees success alert. Error path: 4xx/5xx → `getErrorMessage` in client → Alert, no clearAuth unless 401.

---

## Section 10: Red flags – defense script

- **Red flags** (from existing plan Section 10): No idempotency; Auth state localStorage + cookie; JWT not revocable; Verification token not expired; Logger creates `logs/` at load; No E2E tests; Transfer form requires description (API optional).
- **Per flag**: (1) “How they might phrase it” (one short question). (2) Honest, strategic 2–3 sentence answer acknowledging the gap and referencing your code where relevant. (3) Pivot: one sentence on what you’d do next or what you learned (growth mindset).

---

## Implementation notes

- **Tone**: Natural, speakable prose; no emojis; copy-paste ready.
- **References**: Use your real paths and names (e.g. `server/src/routes/transaction.routes.js`, not “transfer.routes”; `executeTransfer`, `protect`, `getTokenFromRequest`, `createToken`, `validateTransactionAmount`, `authStorage.clearAuth()`).
- **File location**: Create [docs/INTERVIEW_MATERIALS.md](docs/INTERVIEW_MATERIALS.md); add `docs/` to repo if needed. Do not modify [.cursor/plans/banking_app_interview_prep_b0f13664.plan.md](.cursor/plans/banking_app_interview_prep_b0f13664.plan.md).
- **Length**: Section 1 ~250–300 words; Section 2 ~400–500 (scripts); Section 3 ~100–150 words per Q&A; Section 4 ~150–200 per STAR; Section 5 ~200–300 per scenario; Section 6 one table; Section 7 ~50 words per item; Section 8 ~200–300 per topic; Section 9 ~300–400 words; Section 10 ~80–120 words per flag. Total document on the order of 6,000–8,000 words.

