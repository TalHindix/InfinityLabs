# Local Docker test before deploying to Render

This guide walks you through testing the **server** Docker image locally: build, run with the right env, hit `/health`, and run a signup API test. Use this to catch missing env vars or MongoDB connection issues before deploying to Render.

---

## Prerequisites

- **Docker** installed and running.
- **`.env`** at project root (or `server/.env`) with at least:
  - `JWT_SECRET` — required (generate with command below).
  - `MONGO_URI` — either:
    - **Local MongoDB:** `mongodb://localhost:27017/Dubai-Bank` (MongoDB running on your machine). The script will use `--network host` on Linux so the container sees host's localhost, or `host.docker.internal` on Mac/Windows.
    - **Atlas:** your full connection string (e.g. `mongodb+srv://...`). The script leaves it as-is.

Create or copy env and set JWT_SECRET:

```bash
# From project root
cp server/.env.example .env
# Generate a secret and paste into .env as JWT_SECRET=...
node -e "console.log(require('crypto').randomBytes(64).toString('hex'))"
```

Edit `.env` and set `JWT_SECRET=<paste-the-output>`.

---

## Option A: One-command script (recommended)

From the **project root**:

```bash
chmod +x scripts/test-docker-server.sh
./scripts/test-docker-server.sh
```

**What the script does:**

1. Checks that `.env` exists and `JWT_SECRET` is set.
2. Builds the image: `docker build -t dubai-bank-server:test ./server`
3. Removes any existing test container, then runs a new one. On **Linux** with local MongoDB it uses `--network host` and `MONGO_URI=mongodb://localhost:27017/Dubai-Bank`. On Mac/Windows it uses `host.docker.internal`. For Atlas it uses your `.env` as-is.
4. Waits until `GET http://localhost:3000/health` returns 200.
5. Sends `POST /api/v1/auth/signup` with a test user and expects 201.
6. Stops and removes the test container.

If any step fails, the script prints container logs and exits with a non-zero code.

---

## Option B: Manual commands (step by step)

Run these from the **project root** (where `server/` and `.env` live).

### 1. Build the Docker image

```bash
docker build -t dubai-bank-server:test ./server
```

- **What it does:** Builds an image from `server/Dockerfile` and tags it as `dubai-bank-server:test`. Uses `server/` as build context; `.dockerignore` keeps `node_modules` and `.env` out of the image.

### 2. Run the container with environment variables

**If using local MongoDB** (MongoDB running on your host):

**Linux** (recommended: same as script):

```bash
docker run -d --name dubai-bank-server-test --network host \
  --env-file server/.env \
  -e MONGO_URI=mongodb://localhost:27017/Dubai-Bank \
  dubai-bank-server:test
```

**Mac/Windows:**

```bash
docker run -d --name dubai-bank-server-test -p 3000:3000 \
  --env-file .env \
  -e MONGO_URI=mongodb://host.docker.internal:27017/Dubai-Bank \
  dubai-bank-server:test
```

**If using MongoDB Atlas**, use your connection string and do not override `MONGO_URI`:

```bash
docker run -d --name dubai-bank-server-test -p 3000:3000 \
  --env-file .env \
  dubai-bank-server:test
```

(Ensure `.env` contains `MONGO_URI=mongodb+srv://...`.)

### 3. Verify the container is healthy

```bash
curl -s http://localhost:3000/health
```

Expected: `{"status":"ok","timestamp":"2026-02-08T..."}`

If you get "Connection refused" or no response:

- Wait a few seconds (server and DB connect after start).
- Check logs: `docker logs dubai-bank-server-test`
- If you see "Missing required environment variables", fix `.env` (e.g. JWT_SECRET).
- If you see MongoDB connection errors, ensure MongoDB is running locally or that `MONGO_URI` (Atlas or host) is correct.

### 4. Test a basic API call (signup)

```bash
curl -s -X POST http://localhost:3000/api/v1/auth/signup \
  -H "Content-Type: application/json" \
  -d '{"firstName":"Test","lastName":"User","email":"test@example.com","phone":"+972500000000","password":"TestPass123!"}'
```

Expected: HTTP 201 and a JSON body with a success message.

### 5. Clean up

```bash
docker stop dubai-bank-server-test
docker rm dubai-bank-server-test
```

---

## Common issues and fixes

| Issue | What to check |
|-------|-------------------------------|
| **"Missing required environment variables: JWT_SECRET"** | Set `JWT_SECRET=...` in `.env` (no quotes needed). Generate with `node -e "console.log(require('crypto').randomBytes(64).toString('hex'))"`. |
| **"Missing required environment variables: MONGO_URI"** | Add `MONGO_URI=...` to `.env` (local: `mongodb://localhost:27017/Dubai-Bank`; for container use script or override as above). |
| **MongoDB connection error / ECONNREFUSED** | **Local:** MongoDB must be running on the host. On Linux use `--network host` + `MONGO_URI=mongodb://localhost:27017/Dubai-Bank`. **Atlas:** Check connection string, IP allowlist, and user password. |
| **Health or signup returns connection refused** | Container may still be starting or crashed. Run `docker logs dubai-bank-server-test` and fix env/DB as above. |
| **Linux: host.docker.internal unknown** | Use `--network host` and `MONGO_URI=mongodb://localhost:27017/Dubai-Bank` instead (script does this). |

---

## Summary

- **Script:** `./scripts/test-docker-server.sh` from project root (after `chmod +x`).
- **Manual:** Build with `docker build -t dubai-bank-server:test ./server`, run with correct `MONGO_URI` (Linux: `--network host` + localhost; Mac/Win: host.docker.internal), then `curl /health` and `curl -X POST .../auth/signup`.
- **MongoDB:** Use local MongoDB (with --network host on Linux) or Atlas; never commit real `MONGO_URI` or `JWT_SECRET` to the repo.
- After all steps pass, the same image and env pattern are suitable for deployment to Render (set env vars in Render dashboard; see [DEPLOY.md](DEPLOY.md)).