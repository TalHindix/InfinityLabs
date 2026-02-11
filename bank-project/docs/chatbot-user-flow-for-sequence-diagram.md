# Chatbot User Flow (High-Level) — For Sequence Diagram

Use this description to generate a **sequence diagram** (e.g. Mermaid or PlantUML). Level: high-level for job interview.

---

## Actors

- **User** — The person using the banking app (browser).
- **Client** — Frontend (React): ChatAssistant component, Socket.IO client.
- **Server** — Backend: Express + Socket.IO server, namespace `/chat`.
- **SocketAuth** — Middleware: validates JWT and loads user before allowing connection.
- **ChatHandler** — Socket handler: connection event, user-message, bot-message.
- **ChatbotService** — Business logic: sanitize input, detect intent (keywords or AI), handle balance/help/greeting/etc., return response.

---

## Flow: From Opening the Chatbot to Receiving a Reply

### Phase 1: App load (user already logged in)

1. User is on the app; they are authenticated (JWT in HTTP-only cookie).
2. The page that shows the chat FAB (floating button) mounts the ChatAssistant component.
3. Client checks: `authStorage.isAuthenticated()`. If true, it creates a Socket.IO client and connects to `SERVER_URL/chat` with `withCredentials: true` (so the cookie is sent).
4. Server receives the connection attempt on the `/chat` namespace.
5. SocketAuth middleware runs: read JWT from cookie (or from `handshake.auth.token`), verify token, load user from DB, check user is ACTIVE. If any step fails, connection is rejected.
6. If auth fails: Client receives `connect_error` and shows "Authentication failed. Please log in again."; socket disconnects.
7. If auth succeeds: Server fires the `connection` event; `socket.user` is set (id, email, status).

### Phase 2: Connection established — initial greeting

8. ChatHandler, on `connection`, reads `userId` from `socket.user`.
9. Server immediately emits `bot-message` to the client with: initial greeting text (e.g. "Hello! I'm your virtual banking assistant"), intent `greeting`, timestamp.
10. Client receives `bot-message` and appends it to the messages list in state (the chat UI may still be closed at this point).

### Phase 3: User opens the chat UI

11. User clicks the chat FAB.
12. Client sets `isOpen = true` and shows the chat window (messages list + input). User sees the greeting that was already received (or that arrives when the window opens).

### Phase 4: User sends a message

13. User types a message and sends (button or Enter).
14. Client appends the message to the messages list as type "user" and emits `user-message` to the server with the message string; input is cleared.
15. Server (ChatHandler) receives `user-message` with the string and the socket’s `userId` in context.

### Phase 5: Server processes the message

16. ChatHandler calls ChatbotService `processMessage(message, { userId })`.
17. ChatbotService: sanitizes input (trim, max length, strip HTML).
18. ChatbotService: detects intent — first by keywords (e.g. balance, help, hello); if no match, optionally calls AI (OpenAI) to classify intent (e.g. greeting, balance, help, goodbye, unknown).
19. For sensitive intents (e.g. balance): if no `userId` (not authenticated), return a response like "Please log in to view this information" with `requiresAuth: true`.
20. For balance intent with valid userId: fetch account summary from UserService (balance), mask userId (e.g. ****1234), format balance (e.g. "1,234.56 AED"), return message + data.
21. For other intents (greeting, help, goodbye, unknown): return the predefined text for that intent.
22. ChatbotService returns `{ intent, message, data?, requiresAuth? }` to the ChatHandler.

### Phase 6: Bot reply to the user

23. ChatHandler emits `bot-message` to the client with: response text, intent, optional data (e.g. masked userId and formatted balance), optional requiresAuth, timestamp.
24. If processing threw an error, ChatHandler catches and emits `bot-message` with a generic error message and intent `error`.
25. Client receives `bot-message` and appends it to the messages list as type "bot", optionally rendering `data` (e.g. balance block).
26. User sees the bot’s reply in the chat window.

### Phase 7: Further messages or close

27. Steps 13–26 repeat for each new user message.
28. When the user closes the chat window, only the UI state changes; the socket can stay connected. When the user navigates away or the component unmounts, the client disconnects the socket and cleans up.

---

## Summary for diagram (short)

- **Client** connects to **Server** `/chat` with credentials → **SocketAuth** validates JWT and user → **Server** accepts connection → **ChatHandler** sends **Client** initial **bot-message** (greeting).
- **User** opens chat UI (FAB) → sees greeting.
- **User** sends text → **Client** emits **user-message** → **Server** (**ChatHandler**) calls **ChatbotService** (sanitize → detect intent → handle intent: balance from UserService or static reply) → **ChatHandler** emits **bot-message** → **Client** shows reply to **User**.

---

## Optional diagram focus

For the interview, the diagram can emphasize:

- Connection with auth (Client → Server → SocketAuth → DB for user → connection accepted/rejected).
- One round-trip: User sends message → Client → Server → ChatbotService (optional: UserService for balance, or OpenAI for intent) → Server → Client → User sees reply.

Use this text as the single source for drawing the sequence diagram (e.g. in Mermaid: User, Client, Server, SocketAuth, ChatHandler, ChatbotService, UserService, OpenAI as participants).
