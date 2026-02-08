# Docker production deployment on Render — checklist

Analysis of the Dubai Bank server for Render (Docker + MongoDB Atlas). Client on Vercel; server and env vars as below.

---

## 1. `server/Dockerfile` — exists and production-ready

**Status: OK**

- **Location:** [server/Dockerfile](../server/Dockerfile)
- **Content:** Node 20 Alpine, `npm ci --omit=dev`, `CMD ["node", "server.js"]`, `EXPOSE 3000`.
- **Verdict:** Suitable for production. No changes required.

---

## 2. `server/.dockerignore` — exists with proper exclusions

**Status: OK**

- **Location:** [server/.dockerignore](../server/.dockerignore)
- **Excludes:** `node_modules/`, `.env` and `.env.*`, logs, coverage, test files, `dist/`, `.git/`, IDE/OS files. Keeps `README.md` via `!README.md`.
- **Verdict:** Build context is clean; no secrets or dev cruft in the image. No changes required.

---

## 3. `/health` endpoint for Render health checks

**Status: OK**

- **Location:** [server/src/app.js](../server/src/app.js) (lines 26–29)
- **Existing code:**
  ```javascript
  // Health check (before API routes so GET / and GET /health are always available)
  app.get(['/', '/health'], (req, res) => {
    res.json({ status: 'ok', timestamp: new Date().toISOString() });
  });
  ```
- **Verdict:** Both `GET /` and `GET /health` return `{ status: 'ok', timestamp: '...' }`. In Render, set the health check path to **`/health`** (or `/`). No code changes needed.

---

## 4. Environment variables in `server/src/config/index.js`

**Status: OK**

- **Location:** [server/src/config/index.js](../server/src/config/index.js)
- **Referenced env vars:**

| Env var           | Config key / usage                    |
|-------------------|----------------------------------------|
| `PORT`            | `config.port`                          |
| `NODE_ENV`        | `config.nodeEnv`, `config.cookie.secure` |
| `MONGO_URI`       | `config.mongoUri` (required)           |
| `JWT_SECRET`      | `config.jwt.secret` (required)        |
| `JWT_EXPIRES_IN`  | `config.jwt.expiresIn`                 |
| `COOKIE_SAME_SITE`| `config.cookie.sameSite`               |
| `CLIENT_URL`      | `config.clientUrl` (CORS origin)       |
| `SERVER_URL`      | `config.serverUrl` (e.g. emails)      |
| `OPENAI_API_KEY`  | `config.openAiApiKey`                 |
| `BREVO_API_KEY`   | `config.email.brevoApiKey`            |
| `EMAIL_FROM`      | `config.email.from`                   |

- **Verdict:** All variables needed for Render + cross-origin cookies are read from `process.env`. No code changes needed.

---

## 5. CORS and cross-origin cookies

**Status: OK**

- **CORS — Location:** [server/src/app.js](../server/src/app.js) (lines 18–22)
  ```javascript
  app.use(cors({
    origin: config.clientUrl || 'http://localhost:5173',
    credentials: true,
  }));
  ```
  - `credentials: true` is set, so the browser will send cookies on cross-origin requests when the client (Vercel) is allowed by `origin`.

- **Cookie attributes (SameSite, Secure) — Location:** [server/src/config/index.js](../server/src/config/index.js) (lines 29–35)
  ```javascript
  cookie: {
    tokenName: 'token',
    maxAgeSeconds: 3600,
    sameSite: process.env.COOKIE_SAME_SITE || 'lax',
    secure: process.env.NODE_ENV === 'production',
  },
  ```
  - SameSite comes from `COOKIE_SAME_SITE` (set to `none` in production on Render).
  - Secure is `true` when `NODE_ENV === 'production'`.

- **Where cookie is set:** [server/src/controllers/auth.controller.js](../server/src/controllers/auth.controller.js) uses `config.cookie` when calling `res.cookie()` and `res.clearCookie()`, so production gets `sameSite: 'none'` and `secure: true` when configured via env.

- **Verdict:** CORS and cookie configuration support cross-origin production (Vercel + Render). No code changes needed; ensure Render has `COOKIE_SAME_SITE=none` and `NODE_ENV=production` (and correct `CLIENT_URL`).

---

## 6. `package.json` start script vs Dockerfile `CMD`

**Status: OK**

- **Location:** [server/package.json](../server/package.json)
  - `"start": "node server.js"`
- **Dockerfile:** `CMD ["node", "server.js"]`
- **Verdict:** Start script and CMD match. Render can use either `npm start` or the same command; the current Dockerfile is correct. No changes required.

---

## Summary

| # | Item                          | Status | Action        |
|---|-------------------------------|--------|---------------|
| 1 | server/Dockerfile             | OK     | None          |
| 2 | server/.dockerignore          | OK     | None          |
| 3 | /health endpoint              | OK     | None          |
| 4 | Env vars in config            | OK     | None          |
| 5 | CORS + cookie (credentials, SameSite) | OK | None (set env on Render) |
| 6 | start script vs CMD           | OK     | None          |

No code or file additions are required for Docker production deployment on Render. Ensure Render environment variables are set as in [DEPLOY.md](DEPLOY.md) (especially `MONGO_URI`, `JWT_SECRET`, `CLIENT_URL`, `SERVER_URL`, `COOKIE_SAME_SITE=none`, `NODE_ENV=production`), and set the Render health check path to **`/health`**.