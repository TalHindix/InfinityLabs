# Dubai-Bank API

A secure RESTful API for a banking application built with Node.js, Express, and MongoDB.

## Features

- User authentication (signup, login, email verification)
- JWT-based authorization
- Money transfers between users
- Transaction history with pagination
- Rate limiting protection
- Input validation at database level
- Centralized error handling

## Tech Stack

- **Runtime:** Node.js
- **Framework:** Express 5
- **Database:** MongoDB with Mongoose
- **Authentication:** JWT (jsonwebtoken)
- **Password Hashing:** bcrypt
- **Email:** Nodemailer
- **Logging:** Winston

## Project Structure

```
server/
├── src/
│   ├── config/          # Environment configuration
│   ├── constants/       # Application constants
│   ├── controllers/     # Request handlers
│   ├── middleware/      # Express middleware
│   │   ├── auth.middleware.js
│   │   ├── error.middleware.js
│   │   ├── logger.middleware.js
│   │   └── rateLimit.middleware.js
│   ├── models/          # Mongoose schemas
│   ├── routes/          # API routes
│   ├── services/        # Business logic
│   ├── utils/           # Utility functions
│   └── app.js           # Express app setup
├── logs/                # Application logs
├── server.js            # Entry point
├── package.json
└── .env.example         # Environment template
```

## Getting Started

### Prerequisites

- Node.js 18+
- MongoDB (with replica set for transactions)

### Installation

```bash
# Clone the repository
git clone <repository-url>
cd bank-project/server

# Install dependencies
npm install

# Create environment file
cp .env.example .env
# Edit .env with your configuration
```

### Environment Variables

```env
PORT=3000
NODE_ENV=development
MONGO_URI=mongodb://127.0.0.1:27017/Dubai-Bank?replicaSet=rs0
JWT_SECRET=<generate-a-64-character-random-string>
JWT_EXPIRES_IN=1h
EMAIL_USER=your-email@gmail.com
EMAIL_PASS=your-app-password
CLIENT_URL=http://localhost:5173
SERVER_URL=http://localhost:3000
```

Generate a secure JWT secret:
```bash
node -e "console.log(require('crypto').randomBytes(64).toString('hex'))"
```

### Running the Server

```bash
# Development (with auto-reload)
npm run dev

# Production
npm start
```

## API Endpoints

### Authentication

| Method | Endpoint | Description | Rate Limit |
|--------|----------|-------------|------------|
| POST | `/api/v1/auth/signup` | Register new user | 5/15min |
| GET | `/api/v1/auth/verify?token=` | Verify email | 10/hour |
| POST | `/api/v1/auth/login` | Login | 5/15min |
| POST | `/api/v1/auth/logout` | Logout | - |

### User (Protected)

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/v1/me` | Get current user profile |

### Transactions (Protected)

| Method | Endpoint | Description | Rate Limit |
|--------|----------|-------------|------------|
| GET | `/api/v1/transactions` | List transactions (paginated) | - |
| POST | `/api/v1/transactions` | Create transfer | 10/min |
| GET | `/api/v1/transactions/:id` | Get transaction details | - |

### Health Check

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/health` | Server health status |

## Request/Response Examples

### Signup
```bash
POST /api/v1/auth/signup
Content-Type: application/json

{
  "firstName": "John",
  "lastName": "Doe",
  "email": "john@example.com",
  "phone": "+1234567890",
  "password": "SecurePass123!"
}
```

### Login
```bash
POST /api/v1/auth/login
Content-Type: application/json

{
  "email": "john@example.com",
  "password": "SecurePass123!"
}

# Response
{
  "token": "eyJhbGciOiJIUzI1NiIs...",
  "user": {
    "id": "uuid",
    "firstName": "John",
    "lastName": "Doe",
    "email": "john@example.com"
  }
}
```

### Create Transaction
```bash
POST /api/v1/transactions
Authorization: Bearer <token>
Content-Type: application/json

{
  "receiverEmail": "jane@example.com",
  "amount": 100.50,
  "description": "Payment for lunch"
}
```

### Get Transactions (Paginated)
```bash
GET /api/v1/transactions?page=1&limit=10
Authorization: Bearer <token>
```

## Validation Rules

### User
| Field | Rules |
|-------|-------|
| firstName | 2-50 characters, required |
| lastName | 2-50 characters, required |
| email | Valid email format, unique |
| password | Minimum 8 characters |
| phone | International format (+1234567890) |

### Transaction
| Field | Rules |
|-------|-------|
| amount | 0.01 - 1,000,000 |
| description | Max 500 characters |
| receiverEmail | Valid email format |

## Security Features

- **CORS:** Restricted to configured client URL
- **Rate Limiting:** Protection against brute force attacks
- **Input Validation:** Mongoose schema validation
- **Password Hashing:** bcrypt with salt rounds
- **JWT Authentication:** Secure token-based auth
- **Error Handling:** No sensitive data in responses
- **Request Size Limit:** 10kb max body size

## Error Responses

All errors return a consistent format:
```json
{
  "success": false,
  "error": "Human-readable error message"
}
```

| Status | Description |
|--------|-------------|
| 400 | Bad Request / Validation Error |
| 401 | Unauthorized / Invalid Credentials |
| 404 | Resource Not Found |
| 409 | Conflict (duplicate email) |
| 429 | Too Many Requests |
| 500 | Internal Server Error |

## Logging

Logs are stored in the `logs/` directory:
- `error.log` - Error-level logs only
- `combined.log` - All logs

## License

ISC
