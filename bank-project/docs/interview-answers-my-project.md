# REST API Interview Questions - Based on My Banking Project

Answers are written in first person and reference the actual Dubai-Bank codebase (file paths and function names). Each answer is intended to be about 30–60 seconds when spoken.

---

## Table of Contents

1. [API design and structure](#1-explain-your-api-design-for-the-banking-system)
2. [Authentication](#2-how-do-you-handle-authentication)
3. [Authorization and protected routes](#3-how-do-you-protect-routes-and-what-is-attached-to-the-request)
4. [Transfer atomicity](#4-how-do-you-ensure-a-transfer-doesnt-leave-the-sender-debited-and-the-receiver-not-credited)
5. [Error handling](#5-how-do-you-handle-errors-in-your-api)
6. [Validation](#6-how-do-you-validate-request-data)
7. [Rate limiting](#7-why-is-rate-limiting-strict-on-auth-and-how-is-it-implemented)
8. [JWT in cookie vs localStorage](#8-why-put-the-jwt-in-a-cookie-instead-of-returning-it-in-the-response-body)
9. [CORS and credentials](#9-how-does-your-auth-middleware-work-when-the-frontend-and-api-are-on-different-origins)
10. [Transaction IDs](#10-how-do-you-generate-sequential-transaction-ids-under-concurrency)
11. [Indexes](#11-what-indexes-do-you-have-on-transactions-and-why)
12. [Pagination](#12-how-do-you-paginate-the-transaction-list)
13. [Security measures](#13-what-security-measures-did-you-implement)
14. [Response format](#14-how-are-success-and-error-responses-structured)
15. [Versioning](#15-do-you-version-your-api-and-how)

---

## 1. Explain your API design for the banking system

**My Answer:**

In my banking application I use a REST-style API with a single version prefix. All endpoints live under `/api/v1`. I have three resource areas: auth under `/api/v1/auth`, user under `/api/v1`, and transactions under `/api/v1/transactions`. Auth handles signup, email verification, resend verification, login, and logout. User exposes only GET `/me` for the current user and their recent transactions. Transactions support GET for a paginated list, POST to create a transfer, and GET by id. I chose this so auth is isolated, the current user is a single endpoint, and transfers are clearly under transactions. Routes are defined in `server/src/routes/auth.routes.js`, `user.routes.js`, and `transaction.routes.js`, and mounted in `server/src/app.js`. There is no separate account resource; balance is on the user and returned by GET `/me`.

---

## 2. How do you handle authentication?

**My Answer:**

I use JWT-based authentication. On login, in `server/src/controllers/auth.controller.js`, I validate email and password, then call `createToken` from `server/src/utils/jwt.util.js`, which signs a payload with the user id and email and an expiry from config, default one hour. I set the token in an HTTP-only cookie named `token` with secure and sameSite from config so the client never reads it. For protected routes, the `protect` middleware in `server/src/middleware/auth.middleware.js` runs first. It gets the token via `getTokenFromRequest`, which checks the cookie and then the Authorization Bearer header. It verifies the token with `verifyToken`, loads the user by decoded id, checks the user exists and status is ACTIVE, then attaches the user to `req.user`. If anything fails, I return 401 with the same message so we don’t leak information. There is no refresh token; after expiry the user must log in again.

---

## 3. How do you protect routes and what is attached to the request?

**My Answer:**

Protected routes use the `protect` middleware from `server/src/middleware/auth.middleware.js`. In the user and transaction routers I call `router.use(protect)` so every route in those routers requires a valid JWT. After `protect` runs, `req.user` is set to the Mongoose user document with at least `id`, `email`, and `status`. Controllers use `req.user.id` or `req.user.email` to scope data, for example in `server/src/controllers/transaction.controller.js` the sender is always `req.user.email`, and in `getCurrentUser` in `server/src/controllers/user.controller.js` we use `req.user.id` to load the full user and recent transactions.

---

## 4. How do you ensure a transfer doesn't leave the sender debited and the receiver not credited?

**My Answer:**

I use a MongoDB transaction in `server/src/services/transaction.service.js`. The `executeTransfer` function starts a session and `startTransaction`. Inside that transaction we validate that sender and receiver are not the same, then we deduct from the sender’s balance with `findOneAndUpdate` using a condition that balance is at least the amount, add to the receiver’s balance, and create the transaction record. The transaction id comes from `getNextTransactionId` in `server/src/models/transaction.model.js`, and we pass the same session so the counter increment is part of the same transaction. If any step throws, we call `abortTransaction` and rethrow; on success we `commitTransaction`. We always call `endSession` in a finally block. So either all three operations commit or none do; there’s no partial state.

---

## 5. How do you handle errors in your API?

**My Answer:**

I use a central error handler in `server/src/middleware/error.middleware.js`. Every error is passed to `errorHandler`, which uses an internal `getUserSafeMessage` to map known errors to a status code and a safe message. Mongoose ValidationError becomes 400 with the field messages; duplicate key 11000 becomes 409 with a message like “This email is already registered”; CastError becomes 400; JWT errors become 401 with “Authentication failed”; our own AppError keeps its status code and message; and any unknown error becomes 500 with “Something went wrong”. The handler then sends JSON `{ success: false, error: message }` with that status. Controllers and services throw `new AppError(message, statusCode)` for business rules. Unmatched routes go to `notFoundHandler`, which passes a 404 AppError. We never send stack traces or internal details to the client.

---

## 6. How do you validate request data?

**My Answer:**

Validation happens in a few places. In the auth controller we check required fields manually and throw AppError, for example “All fields are required” for signup and “Email and password are required” for login. The User and Transaction Mongoose models in `server/src/models/user.model.js` and `transaction.model.js` define schema validation—required fields, lengths, email and phone regex, amount min and max. For transfers, we require receiverEmail in the controller and use a dedicated `validateTransactionAmount` in `server/src/utils/validation.util.js` for the amount: it must be a number between 0.01 and 1,000,000 with at most two decimal places, and it returns an object with isValid, error, and sanitized amount. We don’t use Joi or express-validator; we use Mongoose plus these controller and util checks.

---

## 7. Why is rate limiting strict on auth and how is it implemented?

**My Answer:**

We use express-rate-limit in `server/src/middleware/rateLimit.middleware.js`. Auth endpoints use `authLimiter`, which in production allows 5 requests per 15 minutes per IP to reduce brute force; the response is 429 with a JSON body like “Too many authentication attempts. Please try again after 15 minutes.” Verification uses `verifyLimiter`, 10 per hour, and POST transactions use `transactionLimiter`, 10 per minute. The app sets `trust proxy` to 1 so behind a reverse proxy we use the correct client IP. We apply the limiters on the routes in `server/src/routes/auth.routes.js` and `transaction.routes.js` so they run before the controllers.

---

## 8. Why put the JWT in a cookie instead of returning it in the response body?

**My Answer:**

So that even if there’s an XSS bug, JavaScript cannot read the token. The token is set in the login controller in `server/src/controllers/auth.controller.js` with httpOnly true, so the browser sends it automatically with credentials but scripts can’t access it. We also set secure and sameSite from config for production. We don’t put the JWT in the response body at all, so the client never stores it in localStorage. The tradeoff is we have to get CORS and sameSite right when the frontend and API are on different origins; we support both cookie and Authorization Bearer in `getTokenFromRequest` so API or mobile clients can send the token in the header if needed.

---

## 9. How does your auth middleware work when the frontend and API are on different origins?

**My Answer:**

In `server/src/middleware/auth.middleware.js`, `getTokenFromRequest` looks at the cookie first, then the Authorization Bearer header. When frontend and API are same-origin, the cookie is sent with credentials. When they’re cross-origin, the cookie is only sent if sameSite is none and secure is true; the server CORS in `server/src/app.js` is set to a single origin from config with credentials true. If the client can’t send the cookie in a cross-origin setup, the client could store the token after login and send it in the Authorization header; our middleware already supports that. So we support both cookie and header for flexibility.

---

## 10. How do you generate sequential transaction IDs under concurrency?

**My Answer:**

We have a Counter collection in `server/src/models/transaction.model.js` with a document whose _id is the string `'transactions'` and a `seq` field. `getNextTransactionId` does a findByIdAndUpdate with `$inc: { seq: 1 }`. When we create a transfer we pass the same MongoDB session we use for the transfer into `getNextTransactionId`, so the counter increment runs inside the same transaction as the balance updates and the transaction record. That way the id is unique and monotonic and commits or rolls back with the rest. Under very high write load the counter document could be a hotspot; for that we could look at a more distributed approach later.

---

## 11. What indexes do you have on transactions and why?

**My Answer:**

In `server/src/models/transaction.model.js` we have two compound indexes: one on fromEmail ascending and createdAt descending, and one on toEmail ascending and createdAt descending. The list query in `findTransactionsByUserEmail` in `server/src/services/transaction.service.js` uses a query that matches either fromEmail or toEmail and sorts by createdAt descending. These indexes let MongoDB satisfy the filter and sort from the index instead of doing a full collection scan. We don’t index amount or description because we don’t query by those.

---

## 12. How do you paginate the transaction list?

**My Answer:**

GET `/api/v1/transactions` accepts query params `page` and `limit`. In `server/src/controllers/transaction.controller.js`, we default page to 1 and limit to DEFAULT_PAGE_SIZE from constants, which is 10, and we cap limit at 100. We pass those to `findTransactionsByUserEmail` in `server/src/services/transaction.service.js`, which uses skip and limit and also does a countDocuments so we return an object with transactions, total, totalPages, and currentPage. The response is wrapped in our standard format with success and data, so the client gets both the page of items and the info needed to build pagination UI.

---

## 13. What security measures did you implement?

**My Answer:**

Besides JWT in an HTTP-only cookie and the central error handler that doesn’t leak details, we use CORS restricted to a single origin from config with credentials. We limit JSON body size to 10kb in `server/src/app.js`. We have rate limiters on auth and verification and on creating transactions. We use the same 401 message for any auth failure so we don’t reveal whether the email exists or the password was wrong. We set trust proxy so rate limiting uses the correct client IP behind a reverse proxy. Passwords are hashed with bcrypt in `server/src/services/user.service.js` and never returned. Email verification is required before the user is ACTIVE and can use protected routes.

---

## 14. How are success and error responses structured?

**My Answer:**

Success responses use helpers from `server/src/utils/response.util.js`: `response.ok` sends 200 with `{ success: true, data }` and `response.created` sends 201 with the same shape. So every success has the same envelope. Errors always go through the error handler in `server/src/middleware/error.middleware.js`, which sends `{ success: false, error: message }` with the appropriate status code. Rate limit responses from `server/src/middleware/rateLimit.middleware.js` use the same JSON shape with a specific message. So the client can always check `success` and then either use `data` or show `error`.

---

## 15. Do you version your API and how?

**My Answer:**

Yes. All API routes are under `/api/v1` in `server/src/app.js`. Auth is mounted at `/api/v1/auth`, user routes at `/api/v1`, and transaction routes at `/api/v1/transactions`. So we have a single version prefix and can add v2 later by mounting new route files under `/api/v2` and leaving v1 in place for backward compatibility. We don’t use version in headers or query params; the path prefix is the version.

---

*Answers are based on the Dubai-Bank server codebase. Paths and behavior refer to the files under `server/src`.*
