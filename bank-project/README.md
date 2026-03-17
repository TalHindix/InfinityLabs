# Dubai Bank

A full-stack banking application with real-time AI chatbot, money transfers, and video calling.

## Tech Stack

| Layer | Technology |
|-------|-----------|
| Frontend | React 19, TypeScript, Vite, Material-UI 7 |
| Backend | Node.js, Express 5, Socket.IO |
| Database | MongoDB (Mongoose ODM) |
| AI | OpenAI GPT-4o-mini with function calling |
| Auth | JWT + httpOnly cookies, bcrypt |
| Email | Nodemailer + Brevo SMTP |
| Video | Jitsi Meet |
| Testing | Jest + Supertest (server), Vitest + Testing Library (client) |

## Features

- **User Authentication** -- Signup with email verification, JWT login, secure logout
- **Dashboard** -- Account balance, recent transactions
- **Money Transfers** -- ACID-compliant transfers between users
- **Transaction History** -- Paginated, searchable transaction list with detail view
- **AI Chatbot** -- Real-time assistant that can check balances, view history, and execute transfers
- **Video Calling** -- Jitsi-based video calls between users
- **Dark/Light Theme** -- Persistent theme toggle

## Project Structure

```
client/           React frontend (Vite)
  src/
    api/          HTTP client & service layer
    components/   Reusable UI components
    screens/      Page-level components
    shared/       Hooks & utilities
    context/      Theme provider
    layout/       App header
    types/        TypeScript interfaces

server/           Express backend
  src/
    config/       App config & DB connection
    controllers/  Request handlers
    services/     Business logic
    models/       Mongoose schemas
    middleware/   Auth, rate-limit, logging, errors
    routes/       API route definitions
    socket/       Socket.IO chat handler
    utils/        JWT, validation, email utilities
```

## Quick Start

```bash
# Prerequisites: Node.js 20+, MongoDB 7+

# Server
cd server && npm install
cp .env.example .env   # Configure environment variables
npm run dev

# Client
cd client && npm install
npm run dev
```

See [docs/setup.md](docs/setup.md) for full setup instructions.

## Documentation

- [System Overview](docs/system-overview.md) -- High-level project summary
- [Architecture](docs/architecture.md) -- Layered architecture and design patterns
- [Backend](docs/backend.md) -- Server structure, middleware, services
- [Frontend](docs/frontend.md) -- Client structure, state management, routing
- [API Reference](docs/api.md) -- All REST endpoints and WebSocket events
- [AI Chatbot](docs/ai.md) -- OpenAI integration and function calling
- [Setup Guide](docs/setup.md) -- Environment variables, Docker, deployment
