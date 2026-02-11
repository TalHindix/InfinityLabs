---
name: Dubai-Bank Interview Questions
overview: A curated set of interview questions tailored to the Dubai-Bank project, designed to demonstrate your knowledge of the architecture, design decisions, and implementation details. These complement the existing INTERVIEW_MATERIALS.md with questions that probe deeper architectural understanding.
todos: []
isProject: false
---

# Dubai-Bank: Architecture & Knowledge Interview Questions

Interview questions designed to showcase your understanding of the Dubai-Bank fullstack banking application. Each question targets specific architecture, design, or implementation aspects of your codebase.

---

## Project Structure Questions (Interviewer Doesn't Know the Codebase)

Questions an interviewer might ask when first exploring the project to understand how it's built. Answer as if you're orienting someone who has never seen the structure.

### High-Level Structure

**"How is this project organized? What do I see when I open the repo?"**

At the root there are two main folders: `client/` (React frontend) and `server/` (Node/Express API). There's also `docs/` for documentation and `docker-compose.yml` to run the full stack. It's a monorepo-style layout with frontend and backend in the same repo.

**"How do the frontend and backend communicate?"**

The client makes HTTP requests to the API (base URL from `VITE_API_URL`). We use Axios with `withCredentials: true` so cookies are sent. Auth is JWT in an HTTP-only cookie. There's also Socket.IO for the chatbot on the `/chat` namespace.

**"What's the entry point for the app?"**

- **Client:** `client/src/main.tsx` renders the React app; `App.tsx` defines routes and layout.
- **Server:** `server/server.js` starts the HTTP server and attaches Express; `server/src/app.js` configures middleware and routes.

---

### Backend Structure (`server/`)

**"Walk me through the server folder. Where do I find what?"**

- `src/app.js` – Express app: middleware, route mounting, error handlers. Start here.
- `src/routes/` – Route definitions (auth, user, transactions). Each file mounts controller handlers and middleware.
- `src/controllers/` – Handle HTTP: parse request, validate, call service, send response.
- `src/services/` – Business logic: transfers, user creation, auth checks. No `req`/`res` here.
- `src/models/` – Mongoose schemas (User, Transaction).
- `src/middleware/` – Auth (`protect`), rate limiting, logging.
- `src/utils/` – Helpers: JWT, validation, error mapping, email.
- `src/config/` – DB connection, env variables.
- `src/socket/` – Socket.IO handler for the chatbot.

**"If I want to understand how a transfer works, which files do I read and in what order?"**

1. `routes/transaction.routes.js` – sees POST `/` goes to `createTransaction`
2. `controllers/transaction.controller.js` – extracts body, validates, calls `executeTransfer`
3. `services/transaction.service.js` – the actual transfer logic (deduct, add, create record)
4. `models/transaction.model.js` – Transaction schema and `getNextTransactionId`

---

### Frontend Structure (`client/src/`)

**"How is the frontend organized? Where is the UI vs the API logic?"**

- `api/` – HTTP client (Axios), auth storage, API services (auth, user, transaction).
- `screens/` – One folder per screen (login-signup, dashboard, transfer-money, transaction-history). Each usually has a page component, form, styles, and a custom hook for data/actions.
- `components/` – Reusable pieces: ChatAssistant, DubaiBankLogo.
- `layout/` – AppHeader, PageFooter.
- `context/` – ThemeContext for dark/light mode.
- `shared/` – Formatters, theme, shared hooks like `useAsyncOperation`.
- `constants/` – Route paths.
- `types/` – TypeScript types.
- `App.tsx` – Route definitions and RequireAuth wrapper.

**"Where does the transfer form live, and how does it talk to the API?"**

The transfer UI is in `screens/transfer-money/` – `TransferPage.tsx`, `TransferForm.tsx`, and `useTransfer.ts`. The hook calls `transactionService.create()` from `api/transaction.service.ts`, which uses the shared `httpClient` to POST to `/transactions`. The cookie is sent automatically.

**"Where is auth state stored on the client?"**

In `api/auth.storage.ts` – we store the user object in localStorage after login. The JWT itself is in an HTTP-only cookie; we never read it in JS. `authStorage.isAuthenticated()` checks if we have a user; `clearAuth` runs on 401.

---

### Quick Map: "Where is X?"


| Looking for...             | Location                                                                         |
| -------------------------- | -------------------------------------------------------------------------------- |
| API route definitions      | `server/src/routes/*.routes.js`                                                  |
| Transfer business logic    | `server/src/services/transaction.service.js`                                     |
| Auth middleware            | `server/src/middleware/auth.middleware.js`                                       |
| Error handling             | `server/src/utils/error.util.js`                                                 |
| Login/signup UI            | `client/src/screens/login-signup/`                                               |
| HTTP client + 401 handling | `client/src/api/http-client.ts`                                                  |
| Protected route guard      | `client/src/App.tsx` (RequireAuth)                                               |
| Chatbot socket             | `server/src/socket/socket.handler.js`, `client/src/components/ChatAssistant.tsx` |


---

## Simple Interview Questions (Quick Answers)

Short questions you can answer in 1-2 sentences. Good for warm-up or screening.

1. **What tech stack did you use?**
  React + TypeScript + Vite on the frontend; Node.js + Express + MongoDB on the backend.
2. **How do users log in?**
  Email and password; the server validates credentials and sets a JWT in an HTTP-only cookie.
3. **Where is the JWT stored?**
  In an HTTP-only cookie, so JavaScript cannot access it (XSS protection).
4. **How are API routes protected?**
  The `protect` middleware checks the JWT from the cookie or Bearer header and loads the user before allowing access.
5. **What happens when a transfer is executed?**
  We deduct from the sender, add to the receiver, and create a transaction record inside a MongoDB transaction so it's all-or-nothing.
6. **How do you prevent double-spending on transfers?**
  MongoDB transactions: if any step fails (e.g. insufficient funds), we abort and roll back everything.
7. **Why use a cookie instead of localStorage for the token?**
  HTTP-only cookies cannot be read by JavaScript, so XSS attacks cannot steal the token.
8. **What does `withCredentials: true` do in Axios?**
  It tells the browser to send cookies with cross-origin requests so the JWT cookie reaches the API.
9. **What happens when the client gets a 401?**
  The http-client interceptor clears auth (removes user from localStorage), so the user is redirected to login.
10. **How does pagination work for the transaction list?**
  We use `page` and `pageSize`; the server uses `skip` and `limit` on the MongoDB query.
11. **What is rate limiting used for?**
  To limit login/signup attempts (5 per 15 min) and transfers (10 per min) to reduce abuse and brute force.
12. **What is the role of the controller vs the service?**
  Controllers handle the request (parse body, validate input) and call services; services contain the business logic and database operations.
13. **How does the chatbot authenticate?**
  It uses the same JWT as the REST API; the Socket.IO connection validates the token from the cookie or handshake before accepting the connection.
14. **What is the purpose of `trust proxy` in Express?**
  So that `req.ip` reflects the real client IP behind a reverse proxy (needed for correct rate limiting).
15. **How do you ensure error messages are safe for the client?**
  A central error handler maps all errors to generic messages; we never expose stack traces or internal details.

---

## Architecture & System Design Questions (Detailed)

### Q1: Walk me through the request lifecycle when a user initiates a money transfer. What happens from the moment they click "Confirm" until they see the success message?

**What it tests:** End-to-end flow understanding, layered architecture, auth chain.

**Expected answer highlights:**

- Client: `handleSubmit` in [useTransfer.ts](client/src/screens/transfer-money/useTransfer.ts) → `transactionService.create` → `httpClient.post` with credentials
- Server: CORS + cookieParser + `express.json` → route `/api/v1/transactions` → `protect` middleware (cookie/Bearer token, verify, load user) → `transactionLimiter` → `createTransaction` controller
- Service: `validateTransactionAmount` → `executeTransfer` (session, deduct, add, create record, commit)
- Response: 201 → client `onSuccess` → clear form, show success

**Follow-up:** Why does the session need to be passed into `getNextTransactionId`? (Answer: counter increment must be in the same MongoDB transaction so it rolls back if the transfer fails.)

---

### Q2: Your backend uses a layered structure (routes → controllers → services → models). Why this separation, and where would you draw the line between controller and service responsibilities?

**What it tests:** Layered architecture rationale, separation of concerns.

**Expected answer highlights:**

- Routes: define paths and attach middleware (protect, limiters)
- Controllers: extract request data, validate input, call services, format response (no business logic)
- Services: business logic (transfer rules, balance updates), DB operations, throw AppError for business failures
- Models: schema, indexes, Mongoose layer
- Line: controller validates "is amount a valid number?"; service validates "does sender have enough balance?" and "is receiver different from sender?"

---

### Q3: How does your frontend "know" the user is authenticated, given the JWT lives in an HTTP-only cookie that JavaScript cannot read?

**What it tests:** Auth flow, client/server trust model.

**Expected answer highlights:**

- Client stores user object in localStorage after successful login (from response body, not the token)
- `authStorage.isAuthenticated()` checks presence of that user
- Protected routes use `RequireAuth` which redirects to login if no user
- The real source of truth is the cookie; if any API call returns 401, [http-client.ts](client/src/api/http-client.ts) interceptor calls `clearAuth` and removes the user
- So the client trusts localStorage for UI state until the server says "invalid" via 401; eventual convergence on next API call

---

### Q4: You have `trust proxy` set to 1 in Express. Why is that necessary, and what breaks without it?

**What it tests:** Production deployment, reverse proxies, rate limiting.

**Expected answer highlights:**

- Rate limiter (express-rate-limit) uses `req.ip` to identify clients
- Behind a proxy (Nginx, Render, Vercel), `req.ip` would be the proxy's IP, not the client's
- `trust proxy` makes Express trust `X-Forwarded-For` so `req.ip` reflects the real client
- Without it: all requests appear from one IP → rate limit would apply to all users collectively → legitimate users get 429 when a single IP hits the limit

---

### Q5: Describe the data flow for the chatbot. How does Socket.IO authentication differ from REST API authentication, and how do you keep them consistent?

**What it tests:** Real-time architecture, auth reuse across protocols.

**Expected answer highlights:**

- REST: token from cookie or Bearer header, verified in `protect` middleware per request
- Socket: token from `handshake.auth.token` or parsed from `Cookie` header via `getTokenFromCookie`, verified in `authenticateSocket` on connection
- Same JWT, same `verifyToken`, same User lookup and ACTIVE check
- Socket stores `socket.user` on connection; REST sets `req.user` per request
- Both ultimately rely on the same cookie when same-origin; Socket handshake includes cookies with `withCredentials: true`

---

## Deep-Dive Technical Questions

### Q6: In `deductSenderBalance`, you use `findOneAndUpdate` with `balance: { $gte: amount }` in the filter. Why not check the balance first with a separate `findOne` and then update?

**What it tests:** Concurrency, race conditions, atomic operations.

**Expected answer highlights:**

- Two-step approach (find then update) has a race: between read and write, another request could deduct funds; both could pass the "enough balance" check and over-deduct
- `findOneAndUpdate` with `balance: { $gte: amount }` is atomic: the update only runs if the condition holds at the moment of the write
- Combined with the MongoDB transaction, we get atomicity across multiple documents (sender, receiver, transaction record)

---

### Q7: Your error handler maps different error types to safe client messages. Walk me through how a Mongoose `ValidationError` versus a JWT `TokenExpiredError` would be handled differently.

**What it tests:** Error handling strategy, information leakage prevention.

**Expected answer highlights:**

- [error.util.js](server/src/utils/error.util.js): `mapErrorToResponse` centralizes mapping
- `AppError`: use statusCode and message as-is (business errors like "Insufficient funds", "Receiver not found")
- Mongoose `ValidationError`: 400, concatenate field-level messages (e.g. "Amount must be at least 0.01")
- JWT: not explicitly in mapErrorToResponse; auth middleware catches and returns generic 401 "Authentication failed" so we never leak "expired" vs "invalid"
- Unknown errors: 500, "Internal server error" — never expose stack or internal details to client

---

### Q8: You use compound indexes on `(fromEmail, createdAt)` and `(toEmail, createdAt)` for transactions. Your list query uses `$or` on both emails. How does MongoDB use these indexes, and what are the limitations?

**What it tests:** Database indexing, query optimization.

**Expected answer highlights:**

- Query: `$or: [{ fromEmail }, { toEmail }]` + sort `createdAt: -1`
- MongoDB can use index intersection or multiple index scans for `$or`; each branch can use one of the compound indexes
- Sort by `createdAt` is supported because it's the second field in each index
- Limitation: `skip` for pagination — for page 50 with pageSize 10, we skip 490 docs; gets slower as page grows (cursor-based pagination would avoid this)

---

### Q9: If you had to add idempotency for transfers, where in the stack would you implement it and what would you need (client, controller, service, storage)?

**What it tests:** System design, idempotency patterns.

**Expected answer highlights:**

- Client: generate UUID per transfer attempt, send as `Idempotency-Key` header; retry with same key on network failure
- Controller/middleware: extract header, check store (Redis or DB) for key
- If key exists: return stored 201 + transaction (same response as original)
- If not: call `executeTransfer`, store key + response with TTL (e.g. 24h), return 201
- Service: unchanged; idempotency is at the boundary before we touch money
- Storage: Redis preferred (fast, TTL); DB table with unique key + created_at also works

---

## Tradeoff & Design Questions

### Q10: You chose MongoDB over PostgreSQL. In what scenario would PostgreSQL (or a relational DB) have been a better fit for this app?

**What it tests:** Technology selection, tradeoff awareness.

**Expected answer highlights:**

- PostgreSQL better when: need strong referential integrity (foreign keys), complex joins, ACID with relations, audit trails with triggers
- For this demo: simple document model, multi-document transactions sufficient, fast iteration
- Real bank: might want users → accounts → transactions with FK constraints; double-entry bookkeeping with debits/credits; audit log as append-only table
- MongoDB choice was for speed and simplicity; would reassess for production banking

---

### Q11: The transfer form requires a description, but the API allows it to be optional. Is that a bug or intentional? How would you document it?

**What it tests:** API design, client/server contract.

**Expected answer highlights:**

- Intentional: UX decision to force a reason on the main client; API stays flexible for other clients (mobile, future features)
- Document in API spec: "description is optional; some clients may require it for UX"
- Document in form: comment explaining the early return if description empty
- Could align by making API required or form optional; current split is a conscious product choice

---

### Q12: Your `findTransactionsByUserEmail` runs `find` and `countDocuments` in parallel with `Promise.all`. Could `countDocuments` be a bottleneck, and what alternative would you consider for large datasets?

**What it tests:** Pagination, performance at scale.

**Expected answer highlights:**

- `countDocuments` scans matching documents (or uses index for count); for millions of docs it can be slow
- Alternative: cursor-based pagination — return "next" link with `createdAt` and `id` of last item; next request uses `createdAt < X` instead of skip
- Benefit: constant-time queries regardless of page depth
- Tradeoff: no "total pages" or "jump to page 5"; UX changes to "load more" or "next"
- For moderate data, current approach is fine; for very large datasets, cursor-based is better

---

## Diagram: High-Level Architecture

```mermaid
flowchart TB
    subgraph client [Client React + Vite]
        UI[Screens]
        Hooks[Custom Hooks]
        API[API Services]
        AuthStorage[authStorage]
    end
    
    subgraph server [Server Express]
        Routes[Routes]
        Middleware[Auth + Rate Limit]
        Controllers[Controllers]
        Services[Services]
    end
    
    subgraph db [MongoDB]
        Users[Users]
        Transactions[Transactions]
        Counter[Counter]
    end
    
    subgraph socket [Socket.IO]
        ChatNS[/chat namespace]
        SocketAuth[socketAuth]
    end
    
    UI --> Hooks
    Hooks --> API
    API -->|withCredentials| Routes
    Routes --> Middleware
    Middleware --> Controllers
    Controllers --> Services
    Services --> db
    
    API --> AuthStorage
    ChatNS --> SocketAuth
    SocketAuth --> Services
```



---

## Quick Reference: Files to Mention


| Topic              | Key Files                                                                                                                            |
| ------------------ | ------------------------------------------------------------------------------------------------------------------------------------ |
| Transfer atomicity | [transaction.service.js](server/src/services/transaction.service.js), [transaction.model.js](server/src/models/transaction.model.js) |
| Auth               | [auth.middleware.js](server/src/middleware/auth.middleware.js), [auth.controller.js](server/src/controllers/auth.controller.js)      |
| Error handling     | [error.util.js](server/src/utils/error.util.js)                                                                                      |
| Client auth sync   | [http-client.ts](client/src/api/http-client.ts), [auth.storage.ts](client/src/api/auth.storage.ts)                                   |
| App bootstrap      | [app.js](server/src/app.js)                                                                                                          |
| Chatbot flow       | [socket.handler.js](server/src/socket/socket.handler.js), [socketAuth.middleware.js](server/src/middleware/socketAuth.middleware.js) |


---

## Summary

**Project structure (for unfamiliar interviewer):** Questions that help you explain how the repo is organized, where to find things, and how frontend/backend connect. Includes a "where is X?" quick map.

**Simple questions (15):** One-sentence answers on tech stack, auth, cookies, transfers, pagination, rate limiting, and error handling. Good for quick screening.

**Detailed questions (12):** Deeper coverage of:

- **Request lifecycle** and layered architecture
- **Auth model** (cookie, localStorage, 401 sync)
- **Concurrency** (atomic updates, transactions)
- **Error handling** and safe client messages
- **Indexing** and pagination tradeoffs
- **Idempotency** and system design
- **Technology choices** and tradeoffs
- **API/UX contract** (description required vs optional)

They complement your existing [INTERVIEW_MATERIALS.md](docs/INTERVIEW_MATERIALS.md) by focusing on architectural reasoning and "why" decisions rather than rote answers.