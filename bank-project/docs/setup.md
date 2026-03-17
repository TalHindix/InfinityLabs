# Setup Guide

## Prerequisites

- Node.js 20+
- MongoDB 7+ (local) or MongoDB Atlas account
- OpenAI API key
- Brevo account (for email, optional)

## Environment Variables

### Server (`server/.env`)

| Variable | Required | Description |
|----------|:--------:|-------------|
| `PORT` | No | Server port (default: 3000) |
| `NODE_ENV` | No | `development` or `production` |
| `MONGO_URI` | Yes | MongoDB connection string |
| `JWT_SECRET` | Yes | Secret for signing JWTs (use 64+ char hex) |
| `OPENAI_API_KEY` | Yes | OpenAI API key for chatbot |
| `BREVO_API_KEY` | No | Brevo SMTP key for email verification |
| `EMAIL_FROM` | No | Sender email address |
| `CLIENT_URL` | Yes | Frontend URL for CORS (e.g., `http://localhost:5173`) |

### Client (`client/.env.development`)

| Variable | Required | Description |
|----------|:--------:|-------------|
| `VITE_API_URL` | Yes | Backend API URL (e.g., `http://localhost:3000/api/v1`) |
| `VITE_SOCKET_URL` | Yes | Backend Socket.IO URL (e.g., `http://localhost:3000`) |

## Local Development

### Without Docker

```bash
# 1. Start MongoDB locally
mongod --replSet rs0

# 2. Server
cd server
npm install
cp .env.example .env  # Edit with your values
npm run dev           # Starts on port 3000

# 3. Client
cd client
npm install
npm run dev           # Starts on port 5173
```

> **Note:** MongoDB must run as a replica set for transaction support. Initialize with:
> ```bash
> mongosh --eval "rs.initiate()"
> ```

### With Docker

```bash
# Start MongoDB + services
docker compose up -d

# Then start server and client as above
```

The `docker-compose.yml` provides a MongoDB instance configured for replica set mode.

## Running Tests

```bash
# Server tests (Jest + Supertest, uses mongodb-memory-server)
cd server && npm test

# Client tests (Vitest + Testing Library)
cd client && npm test
```

## Production Deployment

| Service | Platform |
|---------|----------|
| Client | Vercel |
| Server | Render |
| Database | MongoDB Atlas |

For production, set `NODE_ENV=production` and configure all environment variables on your hosting platform. The client build is generated with `npm run build` in the `client/` directory.
