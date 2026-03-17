# API Reference

Base URL: `/api/v1`

## Authentication

All protected endpoints require a valid JWT, sent as an httpOnly cookie (`token`) or `Authorization: Bearer <token>` header.

### Auth Routes (`/api/v1/auth`)

| Method | Endpoint | Auth | Rate Limit | Description |
|--------|----------|:----:|-----------|-------------|
| POST | `/signup` | No | 5/15min | Register a new user |
| GET | `/verify?token=X` | No | 10/hr | Verify email address |
| POST | `/resend-verification` | No | 5/15min | Resend verification email |
| POST | `/login` | No | 5/15min | Login, receive JWT cookie |
| POST | `/logout` | No | -- | Clear auth cookie, disconnect sockets |

#### POST `/signup`
```json
// Request
{ "firstName": "string", "lastName": "string", "email": "string", "phone": "string", "password": "string" }

// Response 201
{ "success": true, "data": { "message": "Please check your email to verify your account." } }
```

#### POST `/login`
```json
// Request
{ "email": "string", "password": "string" }

// Response 200 (+ Set-Cookie: token=JWT)
{ "success": true, "data": { "user": { "id": "uuid", "firstName": "string", "lastName": "string", "email": "string" } } }
```

#### POST `/logout`
```json
// Response 200
{ "success": true, "data": null }
```

### User Routes (`/api/v1`)

| Method | Endpoint | Auth | Description |
|--------|----------|:----:|-------------|
| GET | `/me` | Yes | Get current user profile + 10 recent transactions |

#### GET `/me`
```json
// Response 200
{ "success": true, "data": { "user": { "id": "uuid", "firstName": "string", "lastName": "string", "email": "string", "phone": "string", "balance": 500, "status": "ACTIVE" }, "recentTransactions": [...] } }
```

### Transaction Routes (`/api/v1/transactions`)

| Method | Endpoint | Auth | Rate Limit | Description |
|--------|----------|:----:|-----------|-------------|
| GET | `/` | Yes | -- | Get paginated transactions |
| POST | `/` | Yes | 10/min | Create a money transfer |
| GET | `/:transactionId` | Yes | -- | Get transaction by ID |
| POST | `/:transactionId/send-notification` | Yes | -- | Send email notification + get video room name |

#### GET `/`
Query params: `page` (default 1), `limit` (default 10, max 100)
```json
// Response 200
{ "success": true, "data": { "transactions": [...], "total": 48, "totalPages": 5, "currentPage": 1 } }
```

#### POST `/`
```json
// Request
{ "receiverEmail": "string", "amount": 100.50, "description": "string" }

// Response 201
{ "success": true, "data": { "transaction": { "id": 1, "fromEmail": "sender@example.com", "toEmail": "receiver@example.com", "amount": 100.50, "description": "string", "createdAt": "ISO-date" } } }
```

#### GET `/:transactionId`
```json
// Response 200
{ "success": true, "data": { "transaction": { "id": 1, "fromEmail": "string", "toEmail": "string", "amount": 100.50, "description": "string", "createdAt": "ISO-date" } } }
```

### Health Check

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/` or `/health` | Server health check |

```json
// Response 200
{ "status": "ok", "timestamp": "ISO-date" }
```

## WebSocket Events

Namespace: `/chat`

Connection requires JWT authentication (cookie-based).

### Client -> Server

| Event | Payload | Description |
|-------|---------|-------------|
| `user-message` | `{ message: string, chatHistory: array }` | Send message to AI assistant |

### Server -> Client

| Event | Payload | Description |
|-------|---------|-------------|
| `bot-message` | `{ message: string, chatHistory: array }` | AI assistant response |
| `transfer-completed` | `{}` | Notification that chatbot executed a transfer |

### Rate Limit
10 messages per 60 seconds per user.

## Error Responses

All errors follow this format:
```json
{ "success": false, "error": "Description of the error" }
```

| Status | Meaning |
|--------|---------|
| 400 | Validation error (invalid input, insufficient balance) |
| 401 | Unauthorized (missing/invalid/expired token) |
| 404 | Resource not found |
| 429 | Rate limit exceeded |
| 500 | Internal server error |
