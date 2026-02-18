# Production Dockerfile for Render Docker Service
# Render builds this image automatically on each deployment
FROM node:20-alpine

WORKDIR /app

COPY package.json package-lock.json ./
RUN npm ci --omit=dev

COPY . .

EXPOSE 3000

CMD ["node", "server.js"]
