# Dubai-Bank — API

REST API for the Dubai-Bank app: auth (signup, login, email verification), user profile, transfers, and transaction history. Uses JWT in HTTP-only cookies, MongoDB, and optional real-time (Socket.IO) and AI chatbot (OpenAI).

## Stack

- **Node.js** · **Express 5** · **MongoDB** (Mongoose)
- **JWT** (HTTP-only cookies) · **bcrypt** · **Winston**
- **Brevo** (email) · **OpenAI** (chatbot, optional)

## Setup

```bash
npm install
cp .env.example .env
# Edit .env: MONGO_URI, JWT_SECRET, BREVO_API_KEY, OPENAI_API_KEY (optional), CLIENT_URL, SERVER_URL
```

Generate a JWT secret:

```bash
node -e "console.log(require('crypto').randomBytes(64).toString('hex'))"
```

## Scripts

| Command        | Description        |
|----------------|--------------------|
| `npm run dev`  | Dev server (watch) |
| `npm start`    | Production         |
| `npm test`     | Jest               |

## API (base: `/api/v1`)

| Method | Endpoint | Description |
|--------|----------|-------------|
| POST   | `/auth/signup` | Register |
| GET    | `/auth/verify?token=` | Verify email |
| POST   | `/auth/login`  | Login |
| POST   | `/auth/logout` | Logout |
| GET    | `/me`          | Current user (protected) |
| GET    | `/transactions`| List transactions, paginated (protected) |
| POST   | `/transactions`| Create transfer (protected) |
| GET    | `/transactions/:id` | Transaction details (protected) |
| GET    | `/health` (root) | Health check |

Rate limits apply on auth and transfer endpoints. CORS is restricted to `CLIENT_URL`.

## Environment

See `.env.example`. Required: `PORT`, `NODE_ENV`, `MONGO_URI`, `JWT_SECRET`, `CLIENT_URL`, `SERVER_URL`. For email: `BREVO_API_KEY`, `EMAIL_FROM`. For chatbot: `OPENAI_API_KEY`. For cross-origin production: `COOKIE_SAME_SITE=none`.
