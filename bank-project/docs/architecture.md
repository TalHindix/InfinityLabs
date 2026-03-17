# Architecture

## Layered Architecture

The application follows a 3-tier layered architecture with strict separation of concerns:

```
Routes  -->  Controllers  -->  Services  -->  Models
(HTTP)       (Validation)     (Business)     (Database)
```

- **Routes** -- Define endpoints and attach middleware (auth, rate-limiting)
- **Controllers** -- Parse requests, call services, format responses
- **Services** -- Contain all business logic; reusable across REST and WebSocket
- **Models** -- Mongoose schemas and database operations

This separation means the chatbot and REST API share the same service layer. A transfer initiated via chat goes through the same `executeTransfer()` as one from the transfer form.

## Server Entry Flow

```
server.js
  ├── connectDB()           -- MongoDB connection with retry (3 attempts)
  ├── app.listen()          -- HTTP server
  ├── Socket.IO server      -- Attached to HTTP server
  ├── initChatbotSocket()   -- /chat namespace setup
  └── Graceful shutdown     -- SIGTERM/SIGINT handlers
```

## Express Middleware Pipeline

```
CORS --> Cookie Parser --> JSON Body --> Logger --> Routes (per-route Auth + Rate Limiters) --> Error Handler
```

1. **CORS** -- Configured for client origin with credentials
2. **Cookie Parser** -- Extracts JWT from httpOnly cookies
3. **Body Parser** -- JSON request parsing
4. **Logger** -- Winston structured logging
5. **Routes** -- Each router applies its own auth (`protect`) and rate limiters per-route
6. **Error Handler** -- Centralized error formatting

## Design Patterns

### Repository / Service Pattern
Controllers delegate to services, which encapsulate business logic and database operations. Controllers should not access models directly (one exception exists: `sendTransferNotification` in `transaction.controller.js` queries the User model directly).

### Command Pattern
The client's `useAsyncOperation` hook wraps async functions with loading/error state, used consistently across all data-fetching hooks.

### Pub/Sub (DOM Events)
Custom DOM events (`auth-state-change`, `dashboard:refresh`) provide loose coupling between unrelated components. The dashboard listens for refresh events from the chatbot without direct imports.

### Auto-Increment Counter
Transaction IDs use a separate `Counter` collection that atomically increments within the same MongoDB session as the transfer, ensuring unique sequential IDs.

## Authentication Architecture

```
Client                          Server
  |                               |
  |-- POST /login ------------->  |
  |                               |-- bcrypt.compare()
  |                               |-- sign JWT {id, email}
  |  <-- Set-Cookie: token=JWT -- |
  |                               |
  |-- GET /me (cookie) -------->  |
  |                               |-- extract JWT from cookie
  |                               |-- verify + load user
  |  <-- user profile ----------  |
```

- JWT payload: `{id, email}`, 1-hour expiry
- Transmitted via httpOnly cookie (primary) or Authorization Bearer header (fallback)
- Client stores user object in localStorage for UI state; cookie is the auth source of truth

## Database Design

**User Collection:**
- UUID primary key, email (unique), bcrypt password hash
- Status: `PENDING` (pre-verification) or `ACTIVE`
- Balance: default 500, min 0

**Transaction Collection:**
- Auto-incrementing integer ID (Counter pattern)
- fromEmail, toEmail, amount, description, createdAt
- Compound indexes on `(fromEmail, createdAt)` and `(toEmail, createdAt)`

**ACID Transfers:**
All money transfers run inside a MongoDB session. The session atomically decrements sender balance (with `$gte` guard), increments recipient balance, creates the transaction record, and increments the counter. Any failure rolls back all operations.
