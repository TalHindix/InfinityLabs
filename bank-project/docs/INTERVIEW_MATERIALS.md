# Dubai-Bank: Interview-Ready Materials

Actionable, memorizable materials for presenting the Dubai-Bank fullstack banking application. All content references the actual codebase (file paths and function names).

---

## 1. Elevator Pitch (2 minutes)

**Word-for-word script:**

Dubai-Bank is a fullstack demo banking application I built to showcase how to handle auth, money movement, and security in a finance-style domain. I wanted something that felt real enough to demonstrate atomic transfers, JWT in HTTP-only cookies, and rate limiting, without building actual payment rails or compliance layers.

On the backend I use Node.js with Express 5 and MongoDB via Mongoose. Auth is JWT stored in an HTTP-only cookie so the client never touches the token; the server sets it on login in `server/src/controllers/auth.controller.js` and every protected route goes through the `protect` middleware in `server/src/middleware/auth.middleware.js`, which reads the token from the cookie or from the Authorization Bearer header. I chose MongoDB because I wanted a single transactional flow for transfers: in `server/src/services/transaction.service.js` the `executeTransfer` function runs inside a MongoDB session so we deduct from the sender, add to the receiver, and create the transaction record atomically. If any step fails we abort and roll back. On the frontend I use React 19 with TypeScript and Vite, MUI for the UI, and a single Axios instance in `client/src/api/http-client.ts` that sends credentials so the cookie goes with every request; when we get a 401 we clear local auth so the user is effectively logged out.

The main features are signup with email verification via Brevo, login, transfers between users by email, a dashboard that shows balance and recent transactions, and paginated transaction history. I also added an optional Socket.IO chatbot on the `/chat` namespace that uses the same JWT for auth via `server/src/middleware/socketAuth.middleware.js`. I'm most proud of the transfer atomicity, the consistent error handling in `server/src/middleware/error.middleware.js` that maps Mongoose and JWT errors to safe client messages, and the rate limiters in `server/src/middleware/rateLimit.middleware.js`—auth is capped at five attempts per fifteen minutes in production to reduce brute force. The whole stack runs in Docker Compose with Mongo, the API server, and an Nginx-served client.

What I learned was how much small decisions like cookie options and CORS matter when you have credentials. If I had more time I'd add refresh tokens so we can revoke sessions, idempotency keys on transfers to prevent double-submit, and E2E tests for the login-to-transfer flow.

---

## 2. Code Walkthrough (5 minutes)

**Order of files and what to say:**

**[Show: server/src/app.js]**  
"This is the Express app entry. We set trust proxy for rate limiting behind a reverse proxy, then CORS with a single origin from config and credentials true so the cookie is sent. We use cookieParser and a 10kb JSON body limit, then the request logger. Health check is at slash and slash health so it's always available. We mount auth at api/v1/auth, user routes at api/v1, and transaction routes at api/v1/transactions. At the end we have notFoundHandler and errorHandler so every error becomes a consistent JSON response with a safe message."

**[Show: server/src/routes/transaction.routes.js]**  
"All transaction routes are protected. We call router.use(protect) so every request must have a valid JWT from cookie or Bearer. POST to slash uses transactionLimiter—ten requests per minute—then createTransaction. GET slash is list transactions, GET slash colon transactionId is get by id. So the transfer endpoint is POST /api/v1/transactions with the limiter and protect in front."

**[Show: server/src/controllers/transaction.controller.js]**  
"The createTransaction controller gets receiverEmail, amount, and description from the body and senderEmail from req.user. We require receiverEmail and run validateTransactionAmount from validation.util—it returns isValid, error, and sanitized amount. If valid we call executeTransfer from the service with the sanitized amount and return 201 with the created transaction. Any error is passed to next so the central error handler can map it to a status code and message."

**[Show: server/src/services/transaction.service.js]**  
"This is where the money actually moves. executeTransfer starts a MongoDB session and startTransaction. We validate that sender and receiver aren't the same, then deductSenderBalance using findOneAndUpdate with balance gte amount so we only update if there's enough—that throws Insufficient funds if not. We addReceiverBalance the same way, then createTransactionRecord which calls getNextTransactionId with the session so the counter increment is in the same transaction. We commit on success and abort on any error in the try-catch, and we always endSession in finally. So all three operations are atomic."

**[Show: server/src/middleware/auth.middleware.js]**  
"Protect is the auth gate. getTokenFromRequest checks req.cookies.token first, then Authorization Bearer. We verify the token with verifyToken from jwt.util, then load the user by decoded.id and select only id, email, status. If there's no user or status isn't ACTIVE we return the same 401 message so we don't leak whether the user exists. Otherwise we set req.user and call next. Any exception—invalid or expired token—also returns that same 401."

**[Show: client/src/App.tsx]**  
"The app uses React Router. We have public routes for login and signup, and protected routes for dashboard, transfer, and transactions. RequireAuth checks authStorage.isAuthenticated—which is just whether we have a user in localStorage—and if not we Navigate to login. So the client doesn't read the cookie; we only know we're logged in because we stored the user after a successful login and we clear it on 401. Protected routes also render the ChatAssistant. Any unknown path redirects to login."

**[Show: client/src/api/http-client.ts]**  
"We create one Axios instance with baseURL from VITE_API_URL and withCredentials true so the cookie is sent on every request. The response interceptor: on success we pass through; on error, if status is 401 we call authStorage.clearAuth so the user is logged out, and we log network errors. We always reject so the calling code can show the error message. So any protected call that gets 401 will clear auth and the next navigation will hit RequireAuth and redirect to login."

**[Show: client/src/screens/transfer-money/TransferPage.tsx and useTransfer.ts]**  
"TransferPage uses the useTransfer hook and passes everything into TransferForm. In useTransfer we have local state for receiverEmail, amount, description, and success. handleSubmit prevents default, resets success, and if description is empty we return early—that's an intentional UX choice, the API allows optional description. We call execute from useAsyncOperation with transactionService.create and an onSuccess that sets success and clears the form fields. So the flow is: user submits, we POST with credentials, on 201 we show success and reset; on error we show the message from getErrorMessage."

**Deep dive – POST /api/v1/transactions (money transfer) end-to-end:**

User clicks Confirm in the transfer dialog. TransferForm calls onSubmit, which is handleSubmit from useTransfer in `client/src/screens/transfer-money/useTransfer.ts`. That calls `transactionService.create` from `client/src/api/transaction.service.ts`, which does `httpClient.post('/transactions', { receiverEmail, amount, description })`. The Axios instance in `client/src/api/http-client.ts` has withCredentials true so the cookie is sent. The request hits Express in `server/src/app.js`, which routes to `server/src/routes/transaction.routes.js`. The router has router.use(protect), so first `server/src/middleware/auth.middleware.js` protect runs: getTokenFromRequest gets the token from the cookie, verifyToken decodes it, User.findOne loads the user, req.user is set. Then transactionLimiter runs (10 per minute). Then createTransaction in `server/src/controllers/transaction.controller.js` runs: it validates amount with validateTransactionAmount from `server/src/utils/validation.util.js`, then calls executeTransfer in `server/src/services/transaction.service.js`. There we start a session, startTransaction, validateTransferRequest (no self-transfer), deductSenderBalance (findOneAndUpdate with balance gte amount), addReceiverBalance, createTransactionRecord with getNextTransactionId(session) from `server/src/models/transaction.model.js`, then commitTransaction. Response is 201 with the transaction. Back on the client, the promise resolves, onSuccess in useTransfer runs: setSuccess true and clear the form. The user sees the success alert.

---

## 3. Interview Q&A (15 questions)

### Technical depth

**Q1: How do you ensure a transfer doesn't leave the sender debited and the receiver not credited?**

**Testing:** Understanding of transactions, consistency, error handling.

**Answer (30 sec):** I use MongoDB transactions in `server/src/services/transaction.service.js`. executeTransfer starts a session and startTransaction, then we do three operations: deductSenderBalance and addReceiverBalance with findOneAndUpdate, and createTransactionRecord with getNextTransactionId passed the same session. All of them run inside that transaction. If any step throws—insufficient funds, receiver not found—we catch, abortTransaction, and rethrow. So we only commit when all three succeed; otherwise everything rolls back. If I had more time I'd add idempotency keys so a double-submit doesn't create two transfers.

---

**Q2: Why put the JWT in a cookie instead of returning it in the response body and storing it in localStorage?**

**Testing:** Security (XSS), cookie vs token storage.

**Answer (30 sec):** So that any XSS that can read the page still can't read the token. The browser sends the cookie automatically with credentials; we set it in the login controller in `server/src/controllers/auth.controller.js` with httpOnly true, so JavaScript can't access it. We also set secure and sameSite from config for production. The tradeoff is we have to get CORS and sameSite right when the frontend and API are on different origins. We don't put the JWT in the response body at all so the client never sees it.

---

**Q3: How does your auth middleware work when the frontend and API are on different origins?**

**Testing:** CORS, credentials, cookie vs header.

**Answer (30 sec):** In `server/src/middleware/auth.middleware.js`, getTokenFromRequest looks at the cookie first, then the Authorization Bearer header. So we support both. When they're same-origin the cookie is sent automatically with credentials. When they're cross-origin, sameSite has to be none and secure true for the cookie to be sent; if the client can't send the cookie we could expose the token in the login response and have the client send it in the Authorization header. We don't do that in the current flow; the app is built for same-origin or properly configured cross-origin with sameSite none.

---

**Q4: How do you generate sequential transaction IDs and keep them correct under concurrency?**

**Testing:** Concurrency, transaction scope, counter pattern.

**Answer (30 sec):** We have a Counter collection in `server/src/models/transaction.model.js` with a document _id of 'transactions' and a seq field. getNextTransactionId does findByIdAndUpdate with $inc seq 1, and we pass the same MongoDB session we use for the transfer. So the counter increment and the transaction create are in the same transaction—they commit or roll back together. That gives unique, monotonic IDs even under concurrent transfers. The counter is a single-doc hotspot at very high write load; for that we could consider a more distributed approach.

---

**Q5: What indexes do you have on transactions and why?**

**Testing:** Database design, query patterns.

**Answer (30 sec):** In `server/src/models/transaction.model.js` we have two compound indexes: fromEmail ascending, createdAt descending, and toEmail ascending, createdAt descending. The list query uses $or on fromEmail and toEmail with sort by createdAt descending, so we need to support that pattern. These indexes let MongoDB use an index for the filter and sort instead of a full collection scan. We don't have an index on amount or description because we're not querying by those.

---

**Q6: Why is rate limiting so strict on auth in production?**

**Testing:** Security, abuse prevention, tradeoffs.

**Answer (30 sec):** In `server/src/middleware/rateLimit.middleware.js` authLimiter is five requests per fifteen minutes in production. We're trying to limit brute force on login and signup and reduce enumeration. We don't skip successful requests so even successful logins count, which keeps the window consistent. The tradeoff is a user who mistypes five times is locked out for fifteen minutes; for a demo that's acceptable. I'd consider a higher limit or per-IP backoff in a real product.

---

**Q7: How do you make sure error messages are safe for the client?**

**Testing:** Error handling, information leakage.

**Answer (30 sec):** We have one central error handler in `server/src/middleware/error.middleware.js`. getUserSafeMessage maps Mongoose ValidationError to 400 with the validation messages, duplicate key to 409 with a generic message, CastError to 400, JWT invalid or expired to 401 with "Authentication failed", and our own AppError to its statusCode and message. Anything else—including unexpected errors—becomes 500 with "Something went wrong". We log the full error with stack in the handler so we can debug, but we never send stack or internal details to the client. So the client only ever sees statusCode and a safe message.

---

**Q8: How does Socket.IO auth work for the chatbot?**

**Testing:** Real-time auth, reuse of HTTP auth.

**Answer (30 sec):** The chat namespace is set up in `server/src/socket/socket.handler.js` and uses authenticateSocket from `server/src/middleware/socketAuth.middleware.js`. That middleware gets the token from socket.handshake.auth.token or by parsing the Cookie header with getTokenFromCookie. Then we verifyToken and load the user by id, check status is ACTIVE, and set socket.user. So it's the same JWT as the REST API—cookie or auth object—and the same User lookup. If the token is missing or invalid we call next with an error and the connection is rejected. No refresh flow; if the JWT is expired the user has to log in again and reconnect.

---

**Q9: How does pagination work for transaction list and could it be a bottleneck?**

**Testing:** Pagination, indexes, scalability.

**Answer (30 sec):** In `server/src/services/transaction.service.js`, findTransactionsByUserEmail takes userEmail, page, and pageSize. We compute skip as (page minus one) times pageSize and use find with the $or query, sort createdAt descending, skip and limit. We also countDocuments for the same query to return total and totalPages. The controller in `server/src/controllers/transaction.controller.js` caps page size at MAX_PAGE_SIZE 100. The compound indexes on fromEmail and toEmail with createdAt support this. The bottleneck is skip: for page 100 we're skipping 1000 documents. If I had more time I'd consider cursor-based pagination using the last createdAt or id for "next page" so we don't skip.

---

**Q10: Why do you use CORS with a single origin and credentials?**

**Testing:** Security, cross-origin, cookie behavior.

**Answer (30 sec):** In `server/src/app.js` we set cors with origin from config.clientUrl—one origin—and credentials true. Credentials true is required so the browser sends the HTTP-only cookie. Letting only one origin in prevents random sites from making authenticated requests to our API. We have to set CLIENT_URL correctly for each environment; for Vercel frontend and Render backend we'd set it to the Vercel URL. If we used origin star we couldn't use credentials; if we didn't use credentials the cookie wouldn't be sent. So single origin plus credentials is the right combination for our cookie-based auth.

---

### Behavioral / design

**Q11: What would you add or change first for production?**

**Testing:** Prioritization, production readiness.

**Answer (30 sec):** I'd add refresh tokens and short-lived access tokens so we can revoke sessions and avoid long-lived JWTs. Then idempotency keys on POST /transactions so the client can send a key and we return the same 201 for duplicates instead of creating two transfers. Third would be structured or correlation logging so we can trace one request across the server. All of that is in the codebase as known gaps—we have the central error handler and rate limiting in place, but token revocation and idempotency are the next steps I'd implement.

---

**Q12: How do you handle a user who says they were charged twice?**

**Testing:** Incident response, idempotency awareness.

**Answer (30 sec):** Right now we don't have idempotency, so a double submit or retry could create two transfers. I'd look up transactions in MongoDB by the sender's email, amount, and a tight time window to see if there are two records. If so I'd explain that we don't deduplicate today and we'd need to handle it operationally and then add idempotency. The fix is: client sends an Idempotency-Key header, we store it in Redis or the DB with a TTL, and if we see the same key again we return the same 201 and the stored transaction instead of running executeTransfer again.

---

**Q13: Why did you choose MongoDB over PostgreSQL for this app?**

**Testing:** Technology choice, tradeoffs.

**Answer (30 sec):** For this demo I wanted to move fast and keep the model simple—user and transaction documents, and MongoDB's multi-document transactions are enough for the transfer flow. So we get atomicity without introducing a separate queue or event log. For a real bank I'd consider PostgreSQL for stronger relational constraints and ACID, and possibly a separate audit or event store. Here the choice was development speed and a single DB with transactions that cover the critical path.

---

**Q14: Describe a tradeoff you made between simplicity and robustness.**

**Testing:** Self-awareness, design tradeoffs.

**Answer (30 sec):** We use a single JWT with one-hour expiry and no refresh. That's simple—one cookie, one verify call per request. But if the user is active for more than an hour they get 401 on the next request, the client clears auth, and they're sent to login. A more robust approach is refresh tokens and a short-lived access token so we can renew without re-login and revoke by invalidating the refresh token. I chose simplicity for the demo; I'd add refresh for production.

---

**Q15: How do you prevent someone from enumerating valid emails via login or signup?**

**Testing:** Security, user enumeration.

**Answer (30 sec):** For login, in `server/src/controllers/auth.controller.js` we return the same "Invalid credentials" for wrong password, non-existent user, or unverified account—we never say "user not found" or "wrong password". For signup we always return a generic "Please check your email to verify your account" and only send the verification email if the user was actually created. For resend verification we return the same message whether or not we sent an email. So an attacker can't tell from the response whether an email is registered or verified.

---

## 4. Behavioral Questions (5)

**Q1: Why did you choose MongoDB over PostgreSQL?**

**STAR:** I needed a database that could support atomic multi-document updates for transfers without adding a lot of infrastructure. The situation was a demo app where correctness of money movement was the main requirement. My task was to pick a stack that would let me show auth, validation, and consistency. I chose MongoDB because I could use Mongoose and run the whole transfer—deduct sender, add receiver, create transaction record, and increment the counter—inside one session with startTransaction and commit or abort. I didn't need complex joins; the query pattern was "transactions for this user" with indexes. The result was a single codebase with atomic transfers and no separate queue. I'd still consider PostgreSQL for a real bank for relational constraints and auditability, but for this project MongoDB was the right fit. *Technical details to mention: executeTransfer in transaction.service.js, getNextTransactionId(session), compound indexes on fromEmail and toEmail with createdAt.*

---

**Q2: Tell me about a technical challenge you faced in this project.**

**STAR:** The challenge was making sure a transfer could never leave the sender debited and the receiver not credited. The situation was that we have two balance updates and one transaction record; if any step failed we'd have inconsistent state. The task was to guarantee atomicity. I used MongoDB's multi-document transactions: start a session, startTransaction, run deductSenderBalance and addReceiverBalance and createTransactionRecord all with that session, then commit or abort. I had to pass the session into getNextTransactionId so the counter document update was part of the same transaction—otherwise a commit of the counter and a rollback of the rest could still happen. I also made deductSenderBalance use findOneAndUpdate with balance gte amount so we don't deduct if there's insufficient funds, and that throws inside the transaction so we abort. The result is that every transfer is all-or-nothing. *Technical details: transaction.service.js executeTransfer, try/catch/finally with abortTransaction and endSession, Counter in transaction.model.js.*

---

**Q3: What would you do differently if you started over?**

**STAR:** I'd introduce refresh tokens and idempotency from the start. The situation was building a demo under time constraints, so I prioritized the core flow: auth with JWT in a cookie and atomic transfers. What I'd do differently: first, add a refresh token stored in the DB or Redis and a short-lived access token so we can revoke sessions and avoid the "JWT valid until expiry" problem. Second, add idempotency for POST /transactions on day one so we never have to explain double charges. I'd also add request or correlation IDs in the logger so we can trace one request through the server. The result would be the same demo but with production-style token lifecycle and transfer safety. *Technical details: auth.controller.js sets only the access cookie today; we'd add a refresh endpoint and a table or Redis key for refresh tokens; transaction.controller.js would read Idempotency-Key and check a store before calling executeTransfer.*

---

**Q4: How did you prioritize features?**

**STAR:** I prioritized by risk and visibility. The situation was a banking-style app where wrong behavior would be obvious and security would be scrutinized. My task was to order work so the most critical paths were solid first. I put auth and transfer correctness at the top: signup with verification, login with HTTP-only cookie, protect middleware, and the MongoDB transaction for transfers. Then I added the rest of the UX—dashboard, transaction list with pagination, transfer form with confirmation—and rate limiting and error handling so we didn't leak information or allow abuse. The chatbot and theme were last because they're additive. The result was a demo where the interviewer can drill into auth and money movement and see deliberate choices. *Technical details: auth.routes.js and transaction.routes.js with limiters; error.middleware.js getUserSafeMessage; transaction.service.js executeTransfer.*

---

**Q5: What was the most complex part to implement?**

**STAR:** The most complex part was keeping the transfer atomic and the transaction ID correct. The situation was that we have three writes: two User balance updates and one Transaction insert, plus a Counter update for the next id. If we did them separately, a crash or error could leave partial state. The task was to make the whole thing atomic and to avoid duplicate or gaps in transaction IDs. I used a single MongoDB session, startTransaction, and passed the session into every operation including getNextTransactionId so the Counter update is part of the same transaction. I had to be careful with the order: validate self-transfer first, then deduct (which can throw Insufficient funds), then add (which can throw Receiver not found), then create the record. Any throw triggers abort and rethrow so the controller gets the error and the client sees the right message. The result is one clear code path in executeTransfer that either commits everything or rolls back. *Technical details: transaction.service.js executeTransfer, deductSenderBalance with findOneAndUpdate and balance $gte amount, createTransactionRecord with getNextTransactionId(session), finally endSession.*

---

## 5. Debugging Scenarios (3)

### Scenario 1: "A user reports money was deducted but not received"

**Step-by-step:**

1. **Find the transaction.** In MongoDB, query the transactions collection by the sender's email (fromEmail), approximate amount, and a time window around when they say they sent it. Example: `db.transactions.find({ fromEmail: "user@example.com", amount: 100 }).sort({ createdAt: -1 }).limit(10)`. If a matching transaction exists, note its toEmail, id, and createdAt.

2. **Check receiver balance.** If the transaction exists, load the receiver by toEmail from the users collection and check their balance. If the transfer was 100 and their balance reflects that credit, the backend did its job and the issue is likely on the receiver's side (e.g. they didn't refresh, or they're looking at a cached dashboard). If the transaction exists but the receiver's balance wasn't updated, that would contradict our atomic transaction—investigate logs for that request.

3. **If no transaction exists.** The transfer never committed. Either the request failed before executeTransfer (e.g. 401, 400 validation) or executeTransfer aborted (e.g. insufficient funds, receiver not found, self-transfer). Check server logs: Winston writes to `logs/combined.log` and `logs/error.log`, and the request logger in `server/src/middleware/logger.middleware.js` logs method, path, status, and duration on finish. Search for the sender's email or the time window and look for errors (e.g. "Insufficient funds", "Receiver not found"). The sender's balance would have been rolled back on abort.

4. **Consider duplicate deduction.** We don't have idempotency. If the user double-clicked or retried, there could be two transactions and two deductions. Search transactions for the same fromEmail, amount, and a short time window. If you see two, explain that we don't deduplicate and add idempotency as a fix.

5. **Commands.** `docker compose logs server` or `tail -n 500 server/logs/combined.log`. MongoDB: `mongosh` (or connect to the Mongo in compose), `use Dubai-Bank`, then the find queries above and `db.users.findOne({ email: "receiver@example.com" })` to check balance.

**Prevention in production:** Idempotency key on POST /transactions; store key in Redis or DB with TTL and return the same 201 for repeated keys. Correlation id in logs so one request can be traced across the server.

---

### Scenario 2: "The server is responding slowly under load"

**Step-by-step:**

1. **Identify slow requests.** The request logger in `server/src/middleware/logger.middleware.js` logs duration on res.on('finish'). Check logs for high duration (e.g. "POST /api/v1/transactions 201 - 3000ms"). That tells you which method and path are slow. If you have metrics, correlate with request volume.

2. **Correlate with routes.** Likely candidates: GET /api/v1/transactions with large page or high offset (skip), GET /api/v1/me (loads user and recent transactions). Check `server/src/services/transaction.service.js` findTransactionsByUserEmail: we use skip((page - 1) * pageSize). For page 50 and pageSize 10 we skip 490 documents; that can be slow even with indexes. Check `server/src/controllers/transaction.controller.js`: we cap limit at MAX_PAGE_SIZE 100, but skip still grows.

3. **Check MongoDB.** Enable slow query logging or use MongoDB profiler. Verify indexes: in `server/src/models/transaction.model.js` we have indexes on (fromEmail, createdAt) and (toEmail, createdAt). The list query uses $or and sort by createdAt desc; confirm the explain plan uses these indexes. If skip is the issue, consider cursor-based pagination (e.g. "transactions before createdAt X" or "before id Y").

4. **Rate limiters.** In `server/src/middleware/rateLimit.middleware.js`, authLimiter is 5 per 15 minutes in production. If many clients hit login/signup, they could get 429 and retry, adding load. transactionLimiter is 10 per minute per IP. Check whether slow periods correlate with many 429s or repeated auth attempts.

5. **Connection pool and concurrency.** Mongoose default pool size might be a bottleneck under many concurrent transfers. Check if slow requests coincide with many concurrent executeTransfer calls; consider tuning pool size or adding a queue for transfers if needed.

**Prevention:** Cursor-based pagination for GET /transactions; keep MAX_PAGE_SIZE and consider a lower default. Indexes are already in place; monitor slow query log. Optional: rate limit by user or endpoint if one client is overwhelming the server.

---

### Scenario 3: "JWT token expires mid-transaction"

**How the system handles it today:**

1. User fills the transfer form and clicks Confirm. Their JWT was issued more than an hour ago (or whatever JWT_EXPIRES_IN is in config). The request is sent with the cookie.

2. Request hits `server/src/middleware/auth.middleware.js` protect. getTokenFromRequest reads the cookie. verifyToken in `server/src/utils/jwt.util.js` is called; jwt.verify throws TokenExpiredError.

3. The catch in protect returns 401 with { success: false, error: 'Authentication failed' }. We don't distinguish "expired" from "invalid" in the response.

4. On the client, `client/src/api/http-client.ts` response interceptor sees status 401 and calls authStorage.clearAuth(), which removes the user from localStorage. The promise rejects so the transfer form shows an error (e.g. from getErrorMessage).

5. On the next navigation, RequireAuth in `client/src/App.tsx` runs; authStorage.isAuthenticated() is false (no user in localStorage), so we Navigate to login. The user is effectively logged out and must log in again. No retry with a new token; we don't have refresh tokens.

**Files involved:** auth.middleware.js (protect, verifyToken), error.middleware.js (TokenExpiredError mapped to 401), jwt.util.js (verifyToken), http-client.ts (401 → clearAuth), auth.storage.ts (clearAuth), App.tsx (RequireAuth).

**What I'd improve:** Add refresh tokens. Store refresh token in DB or Redis; return it on login (in a separate HTTP-only cookie or body if needed); have a refresh endpoint that validates the refresh token and issues a new access JWT. On 401 from a protected call, the client could call refresh once; if refresh succeeds, retry the original request with the new token; if refresh fails, then clearAuth and redirect to login. That way short-lived access tokens expire without logging the user out as long as the refresh token is valid.

---

## 6. Tradeoffs Table

| Decision | What I Chose | Why | Tradeoff | Alternative |
|----------|--------------|-----|----------|-------------|
| Auth | JWT in HTTP-only cookie | XSS can't steal token; browser sends cookie with credentials | Cookie and CORS/sameSite must be configured correctly; no revocation until expiry | localStorage + Bearer (simpler but XSS risk); server sessions (revocable but stateful) |
| Database | MongoDB | Fast to model user/transaction; multi-doc transactions enough for transfer | No relational constraints; counter is single-doc hotspot at very high write load | PostgreSQL (ACID, relations); separate audit store |
| Transfer consistency | MongoDB transaction (single session) | Atomic debit, credit, and record; rollback on any failure | More code (session, pass session everywhere); requires replica set | Two-phase commit; eventual consistency with reconciliation |
| API style | REST + optional Socket.IO | REST for CRUD; Socket for chat on same server, same auth | Single process for HTTP and Socket; scaling sockets means scaling the whole app | Separate Socket server; GraphQL for client |
| State management | Local state + hooks (no Redux) | Simple; each screen owns its data; 401 syncs via clearAuth | No global cache; refetch on navigation; auth "truth" is cookie, UI truth is localStorage until 401 | Redux or Zustand for user and cache; would need to invalidate on 401 |
| Deployment | Docker Compose (Mongo + Node + Nginx client) | One command to run full stack; healthcheck on Mongo | No server healthcheck in compose; logs to container filesystem | Add server healthcheck; log to stdout or sidecar for prod |
| Rate limiting | Strict auth (5/15min prod), 10/min transfer | Reduce brute force and abuse | Legit user locked out after 5 failed attempts for 15 min | Higher limit; per-IP backoff; skip successful login |

---

## 7. "Next Steps" Talking Points (5)

**1. Idempotency for POST /transactions**  
Why: Prevents duplicate transfers from double-submit or retry; directly addresses "charged twice" reports.  
How: Client sends Idempotency-Key header (e.g. UUID). In the transaction controller or a small middleware, check Redis or a DB table for that key within a TTL (e.g. 24h). If found, return 201 with the stored response. If not, run executeTransfer, store key and response, then return 201. Same key in window always returns the same result.

**2. Refresh token + short-lived access token**  
Why: Enables revocation (logout or compromise) and avoids long-lived JWTs; better security and UX than "expired = logged out".  
How: On login, create a refresh token (random, store in DB or Redis with user id and expiry). Set access JWT in cookie (e.g. 15 min) and optionally return or set refresh token in another HTTP-only cookie. Add POST /auth/refresh that accepts refresh token, validates it, issues new access JWT, optionally rotates refresh token. Client on 401 can call refresh once and retry; if refresh fails, clearAuth and redirect to login.

**3. E2E tests for critical flow**  
Why: Catches regressions on login, transfer, and list in a real browser; complements existing unit and integration tests.  
How: Add Playwright or Cypress. One flow: open app, fill login, submit, assert redirect to dashboard and balance visible; go to transfer, fill recipient/amount/description, confirm, assert success and optional navigation; go to transactions, assert latest transaction appears. Run in CI on every PR. Use a test DB or isolated Mongo.

**4. Verification token expiry**  
Why: Old verification links shouldn't work forever; reduces risk of stale links and forces re-signup or resend if needed.  
How: In the User model add verificationTokenExpiresAt (Date). When creating or regenerating the verification token, set it to now + 24h. In findAndVerifyUserByToken (or in the verify controller), check that the token exists and verificationTokenExpiresAt is greater than now; otherwise return invalid or expired. Clear message on the verification result page when expired so the user can request a new link.

**5. Structured/correlation logging or server healthcheck**  
Why: Correlation id lets you trace one request across logs; healthcheck lets Docker or a load balancer know the server is ready and Mongo is reachable.  
How: Middleware that generates or reads X-Request-Id (or similar), attaches to req, and logs it with every Winston log line for that request. For healthcheck, add a GET /health that returns 200 when the app is up; optionally hit Mongo (e.g. ping) and return 503 if Mongo is down. In docker-compose, add healthcheck for the server service that curls GET /health and use depends_on with condition service_healthy for the client if you want the client to wait for the API.

---

## 8. Technical Deep-Dives (3 topics)

### Topic 1: MongoDB transaction implementation (transfer)

**What it does:** Ensures a transfer is atomic: either the sender is debited, the receiver is credited, and a transaction record is created with the next sequential id, or none of that happens. No partial state.

**How it works:** In `server/src/services/transaction.service.js`, executeTransfer starts with `const session = await mongoose.startSession()` and `session.startTransaction()`. We validate that sender and receiver aren't the same (validateTransferRequest). deductSenderBalance does `User.findOneAndUpdate({ email: senderEmail, balance: { $gte: amount } }, { $inc: { balance: -amount } }, { session, new: true })`—so the update only runs if balance is at least amount; otherwise we get null and throw Insufficient funds. addReceiverBalance does a similar $inc for the receiver; if the receiver doesn't exist we throw Receiver not found. createTransactionRecord calls getNextTransactionId(session) from `server/src/models/transaction.model.js`, which does Counter.findByIdAndUpdate('transactions', { $inc: { seq: 1 } }, { session, new: true, upsert: true }) so the counter is part of the same transaction. We then Transaction.create([{ id, fromEmail, toEmail, amount, description }], { session }). On success we commitTransaction(); on any throw we abortTransaction() and rethrow; in finally we endSession(). So the session ties together the two User updates, the Counter update, and the Transaction create.

**Code snippets:** executeTransfer (lines ~118–141 in transaction.service.js), getNextTransactionId (transaction.model.js), deductSenderBalance / addReceiverBalance (same file).

**Edge cases:** Self-transfer: we throw "Cannot transfer to yourself" before any DB write. Insufficient funds: deductSenderBalance returns null, we throw, transaction aborts, sender balance unchanged. Receiver not found: addReceiverBalance throws, we abort, sender already deducted in the same transaction so the abort rolls them back. Receiver deleted between our checks and commit: MongoDB transaction isolation keeps the snapshot consistent for the duration of the transaction.

**What I'd improve:** Add idempotency so the same request (same key) doesn't run executeTransfer twice. Optionally add a unique constraint or check so we never double-apply the same business operation.

---

### Topic 2: JWT authentication flow (creation, validation, no refresh)

**What it does:** Login issues a JWT and sets it in an HTTP-only cookie. Every protected route and the Socket.IO chat namespace validate that token and load the user; invalid or expired token yields 401 and the client clears local auth.

**How it works:** Creation: In `server/src/controllers/auth.controller.js` login we call findUserByEmailWithPassword, check user exists and status is ACTIVE, compare password with bcrypt, then `const token = createToken(user)`. createToken in `server/src/utils/jwt.util.js` does jwt.sign({ id: user.id, email: user.email }, config.jwt.secret, { expiresIn: config.jwt.expiresIn }) (expiresIn is e.g. 1h from config). We never put the token in the response body; we set it on the response with res.cookie(config.cookie.tokenName, token, { httpOnly: true, secure: config.cookie.secure, sameSite: config.cookie.sameSite, maxAge: config.cookie.maxAgeSeconds * 1000, path: '/' }). Validation: In `server/src/middleware/auth.middleware.js`, getTokenFromRequest returns req.cookies.token or the Bearer token from Authorization header. protect calls verifyToken(token)—jwt.verify in jwt.util—then User.findOne({ id: decoded.id }).select('id email status'). If no user or user.status !== USER_STATUS.ACTIVE we return 401 with the same message. Otherwise we set req.user and next(). There is no refresh: we don't issue or validate refresh tokens; when the JWT expires the next request gets 401 and the client clears auth.

**Code snippets:** auth.controller.js login (createToken, res.cookie), jwt.util.js createToken and verifyToken, auth.middleware.js getTokenFromRequest and protect.

**Edge cases:** Missing token: 401. Invalid signature or malformed: jwt.verify throws, we catch and return 401. TokenExpiredError: same 401. User deleted or status changed to non-ACTIVE: we return 401. We always return the same "Authentication failed" so we don't leak whether the token was expired, invalid, or the user inactive.

**What I'd improve:** Add refresh tokens: store in DB or Redis, issue on login, add POST /auth/refresh, and have the client retry once with a new access token on 401 before clearing auth and redirecting to login.

---

### Topic 3: Real-time chatbot with Socket.IO (auth and flow)

**What it does:** A /chat Socket.IO namespace where the user can send messages and receive bot replies. Connection is gated by the same JWT as the REST API; the server uses the user id for context (e.g. balance lookup in the chatbot service).

**How it works:** In `server/server.js` we attach Socket.IO to the same HTTP server as Express and call initChatbotSocket(io). In `server/src/socket/socket.handler.js` we do io.of('/chat').use(authenticateSocket). authenticateSocket in `server/src/middleware/socketAuth.middleware.js` gets the token from socket.handshake.auth.token or from the Cookie header via getTokenFromCookie (regex match for token=). We verifyToken, load User by decoded.id, check status ACTIVE, set socket.user, and call next(). If anything fails we call next(error) and the connection is rejected. On connection we emit an initial bot-message. On 'user-message' we call processMessage (chatbot service) with the message and { userId: socket.user.id }, then emit 'bot-message' with the result. So the same JWT that protects REST routes protects the socket; we don't send the JWT in the response body for REST, but the browser sends the cookie on the socket handshake when same-origin or when sameSite is none and secure.

**Code snippets:** socket.handler.js initChatbotSocket, socketAuth.middleware.js authenticateSocket and getTokenFromCookie.

**Edge cases:** No token: next(error), connection fails. Expired or invalid token: verifyToken throws, we return a generic error. User not found or inactive: same. Once connected, socket.user is set so we don't re-verify on each message. If the JWT expires while the socket is open we don't proactively close it; the next REST request would get 401. We could add a heartbeat that re-validates the token and disconnect if it's expired.

**What I'd improve:** Same as REST: refresh tokens don't directly apply to the socket handshake, but with short-lived access tokens we could have the client refresh and then reconnect the socket with the new token when we get 401 on REST. For the socket itself we could optionally validate the token on each message or on a timer and disconnect if expired.

---

## 9. "Walk Me Through a Feature" – Money Transfer

**Narrative (use as a continuous story):**

The user is on the Transfer page. They've already logged in, so the cookie is set and RequireAuth in `client/src/App.tsx` let them through. The page is rendered by `client/src/screens/transfer-money/TransferPage.tsx`, which uses the useTransfer hook from `client/src/screens/transfer-money/useTransfer.ts`. The form in `client/src/screens/transfer-money/TransferForm.tsx` has fields for recipient email, amount, and transfer reason (description). Description is required on the frontend—we have an early return in useTransfer handleSubmit if description is empty—so the user has to fill all three. They click Send Money, which opens a confirmation dialog; when they click Confirm, the form calls onSubmit, which is handleSubmit from useTransfer.

handleSubmit calls execute from useAsyncOperation with two arguments: an async function that calls transactionService.create with receiverEmail, amount as a number, and description trimmed, and an onSuccess callback that sets success to true and clears the three form fields. transactionService is in `client/src/api/transaction.service.ts`; create does httpClient.post('/transactions', data). The http-client in `client/src/api/http-client.ts` has baseURL set to the API (e.g. /api/v1) and withCredentials true, so the browser sends the cookie with the JWT. The request goes to the server.

On the server, Express in `server/src/app.js` receives the request and routes it to `server/src/routes/transaction.routes.js` because the path is /api/v1/transactions. The router has router.use(protect), so first the protect middleware in `server/src/middleware/auth.middleware.js` runs. getTokenFromRequest reads the token from the cookie (or Bearer header). verifyToken decodes the JWT; User.findOne loads the user by id and checks status is ACTIVE. We set req.user. Then the route handler runs: for POST / we have transactionLimiter then createTransaction. The limiter allows 10 requests per minute per IP. createTransaction in `server/src/controllers/transaction.controller.js` reads receiverEmail, amount, and description from the body and senderEmail from req.user. It validates receiverEmail is present and runs validateTransactionAmount from `server/src/utils/validation.util.js` on amount. If validation fails we throw AppError and the error handler returns 400. If valid we call executeTransfer in `server/src/services/transaction.service.js` with senderEmail, receiverEmail, sanitized amount, and description.

In executeTransfer we start a MongoDB session and startTransaction. We validate the sender and receiver aren't the same. We call deductSenderBalance (findOneAndUpdate on User with balance gte amount, $inc balance by minus amount, with session)—if the sender doesn't have enough we get null and throw Insufficient funds. We call addReceiverBalance (same idea for the receiver; if receiver doesn't exist we throw). We call createTransactionRecord, which gets the next id from getNextTransactionId(session) so the Counter update is in the same transaction, then Transaction.create with that id and the transfer details, with session. We commitTransaction. If any step threw we abortTransaction and rethrow. We endSession in finally. The controller then returns response.created with the transaction, so the client gets 201 and a body with the new transaction.

Back on the client, the promise from httpClient.post resolves. The onSuccess callback runs: we set success to true and clear receiverEmail, amount, and description. The user sees a success alert and an empty form. If the request had failed—400, 401, 404, 500—the interceptor would not clear auth unless status was 401; for 401 we'd call authStorage.clearAuth and the user would be redirected to login on next navigation. For other errors we'd show the error message from the response body via getErrorMessage in the form.

---

## 10. Red Flags – Defense Script

**Flag 1: No idempotency on transfers**

*How they might phrase it:* "What happens if the user double-clicks or the request is retried? Can they be charged twice?"

*Answer:* Right now we don't have idempotency, so a double submit or retry could create two transfers and two deductions. I'd investigate by looking up transactions by the sender's email and amount in a short time window to see if there are duplicates. The fix I'd add is an idempotency key: the client sends an Idempotency-Key header, we store it in Redis or the DB with a TTL, and if we see the same key again we return the same 201 and the stored transaction instead of calling executeTransfer again. So we'd address both the incident and the gap.

*Pivot:* I prioritized atomicity first so we never have partial state; idempotency was the next step for production and I'd implement it the way I just described.

---

**Flag 2: Auth state is localStorage plus cookie**

*How they might phrase it:* "If the cookie is cleared on the server but the client still has the user in localStorage, wouldn't the UI show them as logged in until the next API call?"

*Answer:* Yes. We treat "logged in" on the client as "we have a user in localStorage" and we rely on the next API call to sync: when any request returns 401, the interceptor in http-client.ts calls authStorage.clearAuth(), so we remove the user and then RequireAuth redirects to login. So we eventually converge; we just don't proactively check that the cookie is still valid. For a demo that's acceptable. I'd improve it by adding a periodic /me call or a check on focus so we invalidate sooner if the cookie was cleared elsewhere.

*Pivot:* The important part for security is that we never send the JWT to the client; the cookie is httpOnly. The localStorage user is only for UI. I'd add that proactive check for production.

---

**Flag 3: JWT not revocable**

*How they might phrase it:* "If a token is leaked, you can't revoke it until it expires, right?"

*Answer:* Right. We don't have a blocklist or refresh tokens, so until the JWT expires—one hour in config—a leaked token is valid. I'd introduce refresh tokens stored in the DB or Redis and a short-lived access token. Logout would invalidate the refresh token, and we could optionally add a blocklist for access tokens if we needed to revoke immediately. That way we get revocation without checking the DB on every request for the common case.

*Pivot:* For the demo I chose simplicity: one token, one verify. For production I'd add the refresh flow and optionally a blocklist for high-risk actions.

---

**Flag 4: Verification token not expired**

*How they might phrase it:* "Can someone use the verification link years later?"

*Answer:* Yes. We hash and store the verification token and clear it when the user verifies, but we don't store an expiry. So an old link could still work if the user never verified and the token wasn't reused. I'd add a verificationTokenExpiresAt field on the user, set it when we create or regenerate the token to now plus 24 hours, and in the verify flow reject if it's past that time. We'd show a clear message so the user can request a new link.

*Pivot:* Single-use was the main goal so the token can't be reused; expiry is the next step I'd add for production.

---

**Flag 5: Logger creates logs/ at load**

*How they might phrase it:* "Your logger creates a logs directory at import time; what if the process doesn't have write access or is in a read-only container?"

*Answer:* Then the logger would throw when it tries to create the directory or write the file, and the server might not start or could crash on first log. I'd make the file transport conditional on config or only add it when the path is writable, or in production log only to stdout and let the orchestrator handle log aggregation. So we wouldn't depend on a local logs directory in restricted environments.

*Pivot:* For local development the current setup is fine; I'd add that conditional or stdout-only path for production deployments.

---

**Flag 6: No E2E tests**

*How they might phrase it:* "You have unit and integration tests but no end-to-end tests. How do you know the full flow works in a browser?"

*Answer:* We have backend integration tests with a real Mongo for auth and transfers, and frontend unit tests for the API layer and hooks including 401 handling and cleanup on unmount. What we don't have is a single test that drives the browser through login, transfer, and transaction list. I'd add Playwright or Cypress and implement that flow so we catch regressions in the full stack. I'd run it in CI and use a test DB or an isolated Mongo so we don't touch real data.

*Pivot:* I focused on the critical paths in isolation first—auth middleware, transfer service, HTTP client. E2E was the next phase; I'd add it exactly as above.

---

**Flag 7: Transfer form requires description; API allows optional**

*How they might phrase it:* "The API accepts transfers without a description but your form requires it. Isn't that inconsistent?"

*Answer:* It's intentional on the UX side: we wanted to force a reason for the transfer so users don't leave it blank. The API stays flexible so other clients or future features could create transfers without a description. So we have one strict client and a permissive API. We could align by making description optional in the form or required in the API schema; for the demo I chose strict UI and flexible API.

*Pivot:* I'd document that choice in the API spec and in the form so the next developer knows it's intentional. If the product requirement changed we could relax the form or tighten the API.
