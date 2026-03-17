# System Overview

Dubai Bank is a full-stack banking web application built as a production-grade demo. It supports user registration with email verification, secure money transfers, real-time AI-powered chat assistance, and video calling.

## Core Capabilities

1. **Authentication** -- Signup, email verification, login, logout with JWT + httpOnly cookies
2. **Banking Operations** -- View balance, transfer money (ACID-compliant), transaction history
3. **AI Assistant** -- Chat-based assistant using OpenAI function calling to perform real banking operations
4. **Real-Time Communication** -- Socket.IO for chat, Jitsi Meet for video calls
5. **Notifications** -- Email notifications for transfers via Brevo SMTP

## High-Level Architecture

```
Browser (React SPA)
  |
  |-- REST API (Axios) --> Express Server --> MongoDB
  |-- WebSocket (Socket.IO) --> Chat Handler --> OpenAI API
  |-- Jitsi SDK --> Jitsi Meet Server
```

The client is a React 19 single-page application communicating with an Express 5 backend. REST handles auth and transactions. Socket.IO handles real-time chat with the AI assistant. The AI chatbot uses OpenAI's function calling to execute actual banking operations (balance checks, transfers) through the same service layer the REST API uses.

## Data Flow: Money Transfer

1. User submits transfer via REST or chatbot
2. Server validates amount and recipient
3. MongoDB session starts (ACID transaction)
4. Sender balance decremented, recipient balance incremented, transaction record created
5. Session commits (or rolls back on error)
6. If via chatbot, `transfer-completed` event emitted to refresh dashboard

## Key Design Decisions

| Decision | Rationale |
|----------|-----------|
| JWT in httpOnly cookies | Prevents XSS token theft while staying stateless |
| MongoDB sessions for transfers | ACID guarantees on financial operations |
| OpenAI function calling | AI decides when to invoke tools, not hardcoded logic |
| Socket.IO for chat | Bidirectional real-time communication |
| Service layer pattern | Business logic reusable across REST and chatbot paths |
| Pub/Sub DOM events | Loose coupling between components (e.g., dashboard refresh) |
