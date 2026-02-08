#!/usr/bin/env bash
# Local Docker test for server before deploying to Render.
# Run from project root: ./scripts/test-docker-server.sh
# Requires: .env with JWT_SECRET (and MONGO_URI for Atlas, or local MongoDB on host).

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$ROOT_DIR"

CONTAINER_NAME="dubai-bank-server-test"
IMAGE_TAG="dubai-bank-server:test"
ENV_FILE=".env"
if [[ ! -f "$ENV_FILE" ]]; then
  ENV_FILE="server/.env"
fi

echo "=== 1. Check .env and required variables ==="
if [[ ! -f "$ENV_FILE" ]]; then
  echo "Error: No .env found at project root or server/.env. Copy server/.env.example to .env and set JWT_SECRET."
  exit 1
fi
if ! grep -q 'JWT_SECRET=.\+' "$ENV_FILE" 2>/dev/null; then
  echo "Error: JWT_SECRET is empty or missing in $ENV_FILE. Add a value (e.g. run: node -e \"console.log(require('crypto').randomBytes(64).toString('hex'))\")."
  exit 1
fi
echo "Using env file: $ENV_FILE"

echo ""
echo "=== 2. Build Docker image from server/Dockerfile ==="
docker build -t "$IMAGE_TAG" ./server

echo ""
echo "=== 3. Stop and remove existing test container (if any) ==="
docker rm -f "$CONTAINER_NAME" 2>/dev/null || true

# Detect local MongoDB in .env (localhost or 127.0.0.1)
USE_LOCAL_MONGO=false
if grep -q 'MONGO_URI=mongodb://localhost' "$ENV_FILE" 2>/dev/null || grep -q 'MONGO_URI=mongodb://127.0.0.1' "$ENV_FILE" 2>/dev/null; then
  USE_LOCAL_MONGO=true
fi

# On Linux with local MongoDB: --network host so container sees host's localhost (same as your manual run).
# On Mac/Windows with local MongoDB: use host.docker.internal so container can reach host's MongoDB.
echo ""
echo "=== 4. Run container with env vars (port 3000) ==="
if [[ "$USE_LOCAL_MONGO" = true ]] && [[ "$(uname)" = "Linux" ]]; then
  echo "Detected local MongoDB on Linux; using --network host and MONGO_URI=localhost"
  docker run -d --name "$CONTAINER_NAME" --network host \
    --env-file "$ENV_FILE" \
    -e MONGO_URI=mongodb://localhost:27017/Dubai-Bank \
    "$IMAGE_TAG"
elif [[ "$USE_LOCAL_MONGO" = true ]]; then
  echo "Detected local MongoDB on Mac/Windows; using host.docker.internal"
  docker run -d --name "$CONTAINER_NAME" -p 3000:3000 \
    --env-file "$ENV_FILE" \
    -e MONGO_URI=mongodb://host.docker.internal:27017/Dubai-Bank \
    "$IMAGE_TAG"
else
  echo "Using MONGO_URI from $ENV_FILE (e.g. Atlas)"
  docker run -d --name "$CONTAINER_NAME" -p 3000:3000 \
    --env-file "$ENV_FILE" \
    "$IMAGE_TAG"
fi

echo "Waiting for server to be ready..."
for i in {1..30}; do
  if curl -sf http://localhost:3000/health >/dev/null 2>&1; then
    echo "Server is up."
    break
  fi
  if [[ $i -eq 30 ]]; then
    echo "Timeout waiting for /health. Container logs:"
    docker logs "$CONTAINER_NAME"
    docker rm -f "$CONTAINER_NAME" 2>/dev/null || true
    exit 1
  fi
  sleep 1
done

echo ""
echo "=== 5. Verify /health endpoint ==="
HEALTH_RESPONSE=$(curl -s http://localhost:3000/health)
echo "Response: $HEALTH_RESPONSE"
if ! echo "$HEALTH_RESPONSE" | grep -q '"status":"ok"'; then
  echo "Health check failed."
  docker logs "$CONTAINER_NAME" 2>&1 | tail -20
  docker rm -f "$CONTAINER_NAME" 2>/dev/null || true
  exit 1
fi
echo "Health check passed."

echo ""
echo "=== 6. Test API: POST /api/v1/auth/signup ==="
TEST_EMAIL="docker-test-$(date +%s)@example.com"
SIGNUP_RESPONSE=$(curl -s -w "\n%{http_code}" -X POST http://localhost:3000/api/v1/auth/signup \
  -H "Content-Type: application/json" \
  -d "{\"firstName\":\"Docker\",\"lastName\":\"Test\",\"email\":\"$TEST_EMAIL\",\"phone\":\"+972500000000\",\"password\":\"TestPass123!\"}")
HTTP_BODY=$(echo "$SIGNUP_RESPONSE" | head -n -1)
HTTP_CODE=$(echo "$SIGNUP_RESPONSE" | tail -n 1)
echo "HTTP status: $HTTP_CODE"
echo "Response body: $HTTP_BODY"
if [[ "$HTTP_CODE" != "201" ]]; then
  echo "Signup test failed (expected 201). Container logs:"
  docker logs "$CONTAINER_NAME" 2>&1 | tail -30
  docker rm -f "$CONTAINER_NAME" 2>/dev/null || true
  exit 1
fi
echo "Signup test passed."

echo ""
echo "=== All checks passed. Stopping and removing test container. ==="
docker rm -f "$CONTAINER_NAME" 2>/dev/null || true
echo "Done. Server image $IMAGE_TAG is ready for deployment (e.g. Render)."
