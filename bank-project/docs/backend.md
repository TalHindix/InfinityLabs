# Backend

Express 5 REST API with Socket.IO, MongoDB, and OpenAI integration.

## Directory Structure

```
server/src/
  app.js              Express app setup (middleware + routes)
  config/
    index.js          Frozen config object (env vars)
    database.js       MongoDB connection with retry logic
  constants/
    index.js          USER_STATUS, DEFAULT_BALANCE, SALT_ROUNDS, PAGE_SIZE
  models/
    user.model.js     User schema (UUID, email, password, balance, status)
    transaction.model.js  Transaction schema (auto-increment ID, fromEmail, toEmail, amount)
  controllers/
    auth.controller.js        Signup, verify, login, logout
    user.controller.js        Get current user profile
    transaction.controller.js Transfers, transaction queries, notifications
  services/
    user.service.js                  User CRUD, password validation
    transaction.service.js           ACID transfers, queries, video room names
    chatbot.service.js               Input sanitization, message processing
    openaiFunctionCall.service.js    Tool-call loop, function execution
  middleware/
    auth.middleware.js       JWT extraction + verification
    socketAuth.middleware.js Socket.IO JWT validation
    error.middleware.js      404 + error handler
    logger.middleware.js     Winston request logging
    ratelimit.middleware.js  Rate limiters (auth, verify, transaction)
  routes/
    auth.routes.js           /api/v1/auth/*
    user.routes.js           /api/v1/me
    transaction.routes.js    /api/v1/transactions/*
  socket/
    socket.handler.js        /chat namespace, message handling, user tracking
  utils/
    jwt.util.js              createToken, verifyToken
    validation.util.js       Amount validation (0.01-1M, 2 decimals)
    email.util.js            Brevo SMTP email sending
    error.util.js            AppError class
    response.util.js         ok(), created() response helpers
    logger.util.js           Winston logger config
    generate.util.js         Token generation + hashing
```

## Middleware

### Authentication (`auth.middleware.js`)
Extracts JWT from the `token` cookie or `Authorization: Bearer` header. Verifies the token, loads the user from the database, checks `status === ACTIVE`, and attaches the user to `req.user`.

### Rate Limiting (`ratelimit.middleware.js`)
- **authLimiter** -- 5 requests / 15 minutes (login, signup)
- **verifyLimiter** -- 10 requests / hour (email verification)
- **transactionLimiter** -- 10 requests / minute (transfers)

### Error Handling (`error.middleware.js`)
- `notFoundHandler` -- Catches unmatched routes, returns 404
- `errorHandler` -- Catches all errors, maps known types (validation, duplicate key, JWT) to proper HTTP responses

## Services

### User Service
- `createUser()` -- Hash password, generate verification token, create user
- `findByEmail()` / `findById()` -- User lookups
- `validatePassword()` -- bcrypt comparison
- `verifyEmailToken()` -- Hash token, find matching PENDING user, activate

### Transaction Service
- `executeTransfer()` -- Full ACID transfer in a MongoDB session
- `findTransactions()` -- Paginated query for user's sent/received transactions
- `findById()` -- Single transaction lookup with authorization check
- `generateVideoRoomName()` -- Deterministic room name from sorted user IDs

### Chatbot Service
- `processMessage()` -- Sanitizes input (trim, 250 char cap, strip HTML), delegates to OpenAI service
- Returns `{message, chatHistory, transferCompleted}`

## Socket.IO

The `/chat` namespace handles real-time AI chat:

1. Connection authenticated via `socketAuth` middleware (JWT from cookie)
2. User tracked in `activeSocketsByUserId` map
3. `user-message` event triggers chatbot processing
4. `bot-message` event returns AI response
5. `transfer-completed` event fires if chatbot executed a transfer
6. Per-user rate limit: 10 messages / 60 seconds
7. `disconnectUser()` called on logout to force-close all sockets

## Testing

Jest + Supertest with `mongodb-memory-server` for integration tests. Tests cover auth flows, middleware, transactions, and utility functions.
