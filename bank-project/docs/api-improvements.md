# API Design Improvements and Best Practices

Review of the Dubai-Bank API implementation and concrete recommendations with references to the actual codebase.

---

## Table of Contents

1. [What You Do Well](#what-you-do-well)
2. [Potential Issues Found](#potential-issues-found)
3. [Recommended Improvements](#recommended-improvements)

---

## What You Do Well

1. **API versioning** — All routes are under `/api/v1` in `server/src/app.js`, so you can introduce v2 without breaking clients.

2. **Consistent response shape** — Success uses `{ success: true, data }` and errors use `{ success: false, error }` from `server/src/utils/response.util.js` and `server/src/middleware/error.middleware.js`.

3. **Central error handling** — A single `errorHandler` in `server/src/middleware/error.middleware.js` maps Mongoose, JWT, and AppError to safe client messages and never exposes stack traces.

4. **JWT in HTTP-only cookie with Bearer fallback** — Token is set in `server/src/controllers/auth.controller.js` with httpOnly; `getTokenFromRequest` in `server/src/middleware/auth.middleware.js` supports both cookie and Authorization header for cross-origin or API clients.

5. **Rate limiting** — authLimiter, verifyLimiter, and transactionLimiter in `server/src/middleware/rateLimit.middleware.js` protect auth and transfer endpoints; production auth limit is strict (5 per 15 minutes).

6. **Pagination** — GET /api/v1/transactions uses `page` and `limit` with a cap (MAX_PAGE_SIZE 100) in `server/src/controllers/transaction.controller.js`, and `findTransactionsByUserEmail` in `server/src/services/transaction.service.js` returns total and totalPages.

7. **Atomic transfers** — `executeTransfer` in `server/src/services/transaction.service.js` uses a MongoDB session so balance updates and transaction record commit or roll back together.

8. **Dedicated amount validation** — `validateTransactionAmount` in `server/src/utils/validation.util.js` centralizes amount rules (0.01–1_000_000, 2 decimals) and returns a sanitized value, keeping controller logic clean.

---

## Potential Issues Found

1. **No refresh token** — Session length is fixed (e.g. 1h via `config.jwt.expiresIn`). Users must log in again after expiry; there is no way to extend session without re-entering credentials.

2. **Logout does not invalidate JWT server-side** — Logout in `server/src/controllers/auth.controller.js` only clears the cookie. The token remains valid until it expires, so a captured token could still be used until then.

3. **No idempotency on POST /transactions** — Double submit (e.g. user double-clicks) can create two transfers. `executeTransfer` has no idempotency key check.

4. **GET /auth/verify returns HTML** — The endpoint returns an HTML page from `buildVerificationResultPage`. For a strictly JSON API, this is inconsistent; mobile or API-only clients may prefer a JSON response.

5. **Optional description and client behavior** — The API allows optional `description` on create transfer, but the client in `client/src/screens/transfer-money/useTransfer.ts` returns early when description is empty. That is a client-side choice; the API is fine, but the two could be aligned (e.g. send null/empty string) for clarity.

---

## Recommended Improvements

### 1. Versioning (already in place; document and plan for v2)

**Current:** Routes are mounted under `/api/v1` in `server/src/app.js`:

```javascript
app.use('/api/v1/auth', authRoutes);
app.use('/api/v1', userRoutes);
app.use('/api/v1/transactions', transactionRoutes);
```

**Recommendation:** Keep as is. When adding v2, add new route files and mount them under `/api/v2` in the same `app.js`, and keep v1 mounted for backward compatibility. No code change required now; only a short note in the README or API overview that v1 is stable and v2 will be additive.

---

### 2. Pagination: add total count header (optional)

**Current:** `getTransactions` in `server/src/controllers/transaction.controller.js` returns `result` from `findTransactionsByUserEmail`, which already includes `total`. Response body: `{ transactions, total, totalPages, currentPage }`.

**Recommendation:** If you want clients to get total count without parsing the body (e.g. for headers), set a custom header in the same controller before `response.ok`:

```javascript
// In server/src/controllers/transaction.controller.js, inside getTransactions:
const result = await findTransactionsByUserEmail(userEmail, page, limit);
res.set('X-Total-Count', String(result.total));
return response.ok(res, result);
```

Optional: add `Link` header for next/prev page using `result.totalPages` and `result.currentPage` (RFC 5988). No change to the service layer.

---

### 3. Refresh token (extend session without re-login)

**Current:** `createToken` in `server/src/utils/jwt.util.js` signs with `expiresIn` from config (e.g. 1h). There is no refresh endpoint or refresh token storage.

**Recommendation:** Add a refresh-token flow:

- **Where:** New route in `server/src/routes/auth.routes.js`, e.g. `POST /api/v1/auth/refresh`, and a new controller function (e.g. in `server/src/controllers/auth.controller.js`). Store refresh tokens in the database (e.g. a `RefreshToken` model or a field on User) with expiry (e.g. 7 days).
- **Flow:** Login returns both access token (cookie) and refresh token (e.g. in body or another httpOnly cookie). Client calls POST /auth/refresh with the refresh token; server validates it, issues a new access token (and optionally a new refresh token), and invalidates the old refresh token.
- **Where to add validation:** In the same auth controller, validate refresh token and check it exists and is not expired in the DB before calling `createToken` again. Keep existing login and protect middleware unchanged; protect continues to validate only the access JWT in cookie/header.

Example sketch for the new controller (conceptual; you would add a refresh token model and service):

```javascript
// server/src/controllers/auth.controller.js - new export
export const refresh = async (req, res, next) => {
  try {
    const refreshToken = req.cookies?.refreshToken || req.body?.refreshToken;
    if (!refreshToken) throw new AppError('Refresh token required', 401);
    const payload = await validateRefreshToken(refreshToken); // you implement: DB lookup + expiry
    const user = await findUserById(payload.userId);
    if (!user || user.status !== USER_STATUS.ACTIVE) throw new AppError('Authentication failed', 401);
    const token = createToken(user);
    res.cookie(config.cookie.tokenName, token, { /* same options as login */ });
    return response.ok(res, { user: { id: user.id, firstName: user.firstName, lastName: user.lastName, email: user.email } });
  } catch (error) {
    next(error);
  }
};
```

Mount the route in `server/src/routes/auth.routes.js`: `router.post('/refresh', authLimiter, refresh);` (or a dedicated refreshLimiter if you prefer).

---

### 4. Idempotency key for POST /transactions

**Current:** `createTransaction` in `server/src/controllers/transaction.controller.js` does not check any idempotency key; each request calls `executeTransfer` and creates a new transaction.

**Recommendation:** Accept an optional `Idempotency-Key` header (or body field). Store processed keys (e.g. in memory, Redis, or a small Mongo collection) with the response status and body (or transaction id). Before calling `executeTransfer`, check if the key was already used; if yes, return the stored response (e.g. 201 with same transaction); if no, run the transfer, store the key and response, then return.

**Where to add:**

- **Controller:** At the start of `createTransaction` in `server/src/controllers/transaction.controller.js`, read `req.headers['idempotency-key']` (or `req.body.idempotencyKey`). If present, call a small helper e.g. `getIdempotentResult(key)`; if it returns a result, send that and return. After a successful `executeTransfer`, call `setIdempotentResult(key, statusCode, responseBody)`.
- **Storage:** Add a simple store, e.g. `server/src/utils/idempotencyStore.js` (in-memory Map with TTL, or Mongoose model with key + createdAt). Key format: e.g. `userId:key` so keys are per user. TTL 24h is common.

Example (in-memory, per process):

```javascript
// server/src/utils/idempotencyStore.js (new file)
const store = new Map();
const TTL_MS = 24 * 60 * 60 * 1000;

export function getIdempotentResult(key) {
  const entry = store.get(key);
  if (!entry) return null;
  if (Date.now() > entry.expiresAt) {
    store.delete(key);
    return null;
  }
  return { statusCode: entry.statusCode, body: entry.body };
}

export function setIdempotentResult(key, statusCode, body) {
  store.set(key, { statusCode, body, expiresAt: Date.now() + TTL_MS });
}
```

In `createTransaction`, use the key only for POST; GET list and GET by id stay unchanged.

---

### 5. Structured body validation (e.g. express-validator)

**Current:** Validation is done in controllers (e.g. `!email || !password`) and in `validateTransactionAmount` in `server/src/utils/validation.util.js`; Mongoose handles schema validation on create. No shared schema library for request bodies.

**Recommendation:** Add express-validator (or Joi) for auth and transaction bodies so validation rules live in one place and error messages stay consistent. Keep using your existing error format so the central handler still receives something like `AppError(message, 400)`.

**Where to add:**

- **Auth:** In `server/src/routes/auth.routes.js` or in the controller, add a validation middleware that runs before the controller. Example for login:

```javascript
// Example: server/src/middleware/validateAuth.middleware.js (new file)
import { body, validationResult } from 'express-validator';

export const validateLogin = [
  body('email').isEmail().normalizeEmail().withMessage('Please provide a valid email'),
  body('password').notEmpty().withMessage('Password is required'),
  (req, res, next) => {
    const errors = validationResult(req);
    if (!errors.isEmpty()) {
      const message = errors.array().map(e => e.msg).join('. ');
      return next(new AppError(message, 400));
    }
    next();
  },
];
```

Then in `server/src/routes/auth.routes.js`: `router.post('/login', authLimiter, validateLogin, login);`. Do the same for signup (firstName, lastName, email, phone, password) and resend-verification (email). Keep the same `AppError` and response shape so `error.middleware.js` is unchanged.

- **Transactions:** For POST /transactions, add validators for `receiverEmail` (isEmail) and `amount` (optional custom validator that reuses the same rules as `validateTransactionAmount`), then in the controller you can still call `validateTransactionAmount(amount)` for the exact 0.01–1_000_000 and 2-decimal rules, or replicate those rules in the validator. Either way, throw `AppError` with the same messages so the API contract does not change.

---

### 6. OpenAPI (Swagger) spec

**Current:** No OpenAPI file; documentation is markdown and code.

**Recommendation:** Add an OpenAPI 3.0 spec (e.g. `docs/openapi.yaml` or `server/openapi.json`) that describes base URL, paths, request/response bodies, and error format. You can maintain it by hand or generate it from the same route definitions (e.g. with a small script that reads your route list and writes YAML/JSON). This gives you a single source of truth for the API and allows codegen or Postman import. Paths to document: all 9 endpoints from `docs/api-endpoints-reference.md`, with request bodies and responses taken from `server/src/controllers/*.js` and `server/src/middleware/error.middleware.js`. No change to server code required; this is additive documentation.

---

*All recommendations are scoped to the existing Dubai-Bank codebase and file paths. Implement only what fits your roadmap and security requirements.*
