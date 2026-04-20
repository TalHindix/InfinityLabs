# Dubai-Bank

A full-stack banking web app with real-time AI chatbot, ACID money transfers, 2FA login, and peer-to-peer video calls.

**Live demo:** _add your Vercel URL here_  
**API:** https://dubai-bank-api-docker.onrender.com  
**Repo:** https://github.com/TalHindix/InfinityLabs

> A portfolio project built to demonstrate a production-shaped Node + React codebase: layered architecture, real-time transport, transactional data, AI integration, and end-to-end tests.

---

## Screenshots

_Add screenshots / GIFs here — dashboard, transfer, chatbot, video call._

---

## Architecture at a glance

- **Client** (Vite + React + MUI) talks to the API over HTTPS for REST and over a Socket.IO namespace (`/chat`) for the AI assistant.
- **API** (Express 5) is split into controllers → services → models. All money movement goes through a single `executeTransfer` service that wraps the changes in a MongoDB transaction, so a transfer either commits entirely or not at all.
- **Auth** is a JWT stored in an httpOnly cookie, with a Bearer-token fallback so the token survives when iOS Safari drops the cross-origin cookie. Login is two-step: password, then an email OTP.
- **AI chatbot** uses OpenAI function-calling. The same services that power REST endpoints (`getAccountSummary`, `findRecentTransactions`, `executeTransfer`) are registered as tools, so the assistant can answer _and_ act without bypassing validation or ACID rules.
- **Video calls** piggyback on a public Jitsi room whose name is a short hash of the two sorted emails — deterministic for the pair, unguessable for a stranger.

See [docs/architecture.md](docs/architecture.md) and [docs/system-overview.md](docs/system-overview.md) for the long version.

---

## Tech stack

| Layer    | Choice                           | Why                                                                   |
|----------|----------------------------------|-----------------------------------------------------------------------|
| Frontend | React 19 + Vite + TypeScript     | Fast dev loop, strict typing across the API/UI boundary               |
| UI       | Material-UI 7                    | Ships accessible primitives + theming (light/dark)                    |
| Backend  | Node 20 + Express 5              | Boring, well-documented, easy to deploy                               |
| Realtime | Socket.IO                        | Rooms, reconnection, cookie-based auth out of the box                 |
| DB       | MongoDB 7 (replica set)          | Multi-document transactions for money movement                        |
| ODM      | Mongoose                         | Schema-level validation of amounts, emails, etc.                      |
| AI       | OpenAI GPT-4o-mini + tool calls  | Small, cheap, multilingual; tools let the model act, not just answer  |
| Auth     | JWT (httpOnly cookie) + email OTP| Cookie is primary; Bearer fallback keeps mobile Safari working        |
| Email    | Brevo HTTP API                   | No SMTP plumbing; free tier covers dev                                |
| Video    | Jitsi Meet (iframe SDK)          | Zero infra; deterministic room names                                  |
| Testing  | Jest + Supertest / Vitest + RTL  | API integration with `mongodb-memory-server`; hook + component tests  |
| Deploy   | Vercel (client) + Render (API)   | Free tiers, separate scaling, Docker image for the API                |

---

## Features

- **Signup + email verification** — token is emailed plaintext but stored hashed, so a DB leak doesn't hand out verifications.
- **2FA login** — password, then a 6-digit code sent by email. 5 misses lock the code until the user requests a new one.
- **Dashboard** — live balance, recent transactions, spending analytics (spent vs received by month, top recipients).
- **Money transfer** — atomic check-and-decrement on the sender and a single Mongo transaction across both accounts + the ledger row.
- **Transaction history** — server-side pagination, detail view, email notification to the recipient with a one-click video-call link.
- **AI assistant** — sidebar chatbot can check balance, list transactions, initiate a transfer (with confirmation), and replies in the user's language.
- **Video calls** — join the same room as the person you just paid.
- **Dark / light theme** — persisted per browser.

---

## Quick start

```bash
# Prereqs: Node 20+, MongoDB 7+ running as a replica set

# API
cd server
npm install
cp .env.example .env        # fill in the secrets — see table below
npm run dev                 # http://localhost:3000

# Client
cd client
npm install
npm run dev                 # http://localhost:5173
```

Need a replica set locally?

```bash
mongod --replSet rs0
mongosh --eval "rs.initiate()"
```

Prefer Docker?

```bash
docker compose up -d        # Mongo (rs0) + API + client behind nginx
```

Full walkthrough: [docs/setup.md](docs/setup.md).

---

## Environment variables

### Server (`server/.env`)

| Variable          | Required | Description                                                       |
|-------------------|:--------:|-------------------------------------------------------------------|
| `PORT`            |    No    | API port (default 3000)                                           |
| `NODE_ENV`        |    No    | `development` or `production`                                     |
| `MONGO_URI`       |   Yes    | Mongo connection string; must point at a replica set              |
| `JWT_SECRET`      |   Yes    | 64+ char hex (`node -e "console.log(require('crypto').randomBytes(64).toString('hex'))"`) |
| `JWT_EXPIRES_IN`  |    No    | Default `1h`                                                      |
| `OPENAI_API_KEY`  |   Yes    | OpenAI key for the chatbot                                        |
| `BREVO_API_KEY`   |    No    | Required if you want verification / OTP / notification emails     |
| `EMAIL_FROM`      |    No    | Sender address used by Brevo                                      |
| `CLIENT_URL`      |   Yes    | Frontend origin for CORS + email links                            |
| `SERVER_URL`      |    No    | Public API origin (used in email verification links)              |
| `COOKIE_SAME_SITE`|    No    | Override; auto-set to `none` when client + API origins differ     |

### Client (`client/.env.development` / `client/.env.production`)

| Variable          | Required | Description                                  |
|-------------------|:--------:|----------------------------------------------|
| `VITE_API_URL`    |   Yes    | Base URL of the REST API (e.g. `/api/v1`)    |
| `VITE_SOCKET_URL` |   Yes    | Base URL of the Socket.IO server             |

---

## Testing

```bash
# Server — Jest + Supertest against mongodb-memory-server
cd server && npm test

# Client — Vitest + React Testing Library
cd client && npm test

# Client lint
cd client && npx eslint .
```

---

## Deployment

| Piece     | Platform       | Notes                                                              |
|-----------|----------------|--------------------------------------------------------------------|
| Client    | Vercel         | `vite build`; env vars from `client/.env.production`               |
| API       | Render         | Docker service built from `server/Dockerfile`                      |
| Database  | MongoDB Atlas  | Replica-set cluster; connection string goes into `MONGO_URI`       |

In production set `NODE_ENV=production` — the cookie flips to `Secure`, logging drops `debug`, and rate limits tighten.

---

## Documentation

- [System overview](docs/system-overview.md)
- [Architecture](docs/architecture.md)
- [Backend](docs/backend.md) — layers, middleware, services
- [Frontend](docs/frontend.md) — routing, hooks, state
- [API reference](docs/api.md) — REST endpoints + WebSocket events
- [AI chatbot](docs/ai.md) — OpenAI integration and tools
- [Setup guide](docs/setup.md) — env vars, Docker, deployment

---

## Author

**Tal Hindi** — [GitHub](https://github.com/TalHindix) · talhindix@gmail.com
