# Interview Prep — Dubai Bank Project

## What This Project Is

A full-stack banking web application built with **React + TypeScript** (frontend) and **Node.js + Express + MongoDB** (backend).

**Core features:**
- User registration with email verification
- JWT-based authentication
- Send money between accounts
- Transaction history with pagination
- AI-powered chatbot (OpenAI) with natural language transfers
- Real-time chat via Socket.io
- Video call after a transfer (WebRTC)
- Email notifications (Brevo API)
- **Email OTP / 2FA on login** ← new feature

---

## Feature 1: Email OTP (Two-Factor Authentication)

### What you built
After a user enters their email and password, instead of immediately logging them in, the system:
1. Generates a random 6-digit code
2. Hashes it with bcrypt and stores it in the database with a 5-minute expiry
3. Sends it to the user's email
4. The frontend switches to an OTP input screen
5. The user enters the code → backend verifies → JWT cookie is issued → user enters dashboard

### Files changed
| File | What changed |
|------|-------------|
| `user.model.js` | Added `otpHash`, `otpExpiry`, `otpAttempts` fields |
| `user.service.js` | Added `saveOtp()`, `verifyOtp()` helpers |
| `email.util.js` | Added OTP email template + async send function |
| `auth.controller.js` | `login` now sends OTP; added `verifyOtp`, `resendOtp` handlers |
| `auth.routes.js` | Added `POST /verify-otp` and `POST /resend-otp` |
| `auth.service.ts` | Added `verifyOtp()` and `resendOtp()` API calls |
| `useLogin.ts` | Added OTP state, handlers, and 60s resend cooldown |
| `LoginForm.tsx` | Conditionally renders OTP input step |

### Security decisions — be ready to explain each one

**"Why do you hash the OTP with bcrypt?"**
> "Because storing a plain OTP in the database means anyone with read access to the DB can steal active login codes. Hashing it means even a database leak doesn't expose the OTP."

**"Why a 5-minute expiry?"**
> "Short enough that a stolen OTP is useless by the time an attacker tries to use it, long enough that a user can check their email and log in comfortably."

**"Why lock after 5 failed attempts?"**
> "A 6-digit OTP has 1,000,000 combinations. Without a limit, an attacker who intercepts the OTP step could brute-force it. 5 attempts makes that infeasible."

**"Why clear the OTP after successful verification?"**
> "OTPs should be single-use. If the token wasn't deleted, the same code could be replayed to authenticate again — which defeats the purpose of 2FA."

**"Why does `resendOtp` return a generic message even if the email doesn't exist?"**
> "To prevent user enumeration. If we returned 'user not found', an attacker could probe which emails are registered in our system."

**"Why store OTP fields in the User model instead of a separate collection?"**
> "For simplicity — one active OTP per user, no separate table to join. The fields are nullable so they don't waste space when not in use."

**"Why use `authLimiter` on the OTP endpoints?"**
> "The existing rate limiter (already on login/signup) applies to the new endpoints too. This blocks automated OTP guessing attacks without writing any new code."

### How the frontend handles it
- `useLogin.ts` manages both login steps in a single hook using separate `useAsyncOperation` instances
- When the backend returns `{ otpRequired: true }`, the hook sets `otpRequired` state to `true`
- `LoginForm.tsx` conditionally renders either the email/password form or the OTP form — clean separation, no new page/route needed
- The resend button has a 60-second cooldown implemented with `useEffect` + `setTimeout`

---

## Feature 2: Video Call Email Fix

### What the bug was
After a successful transfer, the app automatically sent an email to the recipient with a video call link — even if the sender had no intention of starting a call. Clicking "Close" on the success dialog still resulted in the recipient getting an unsolicited video call invite.

### What you fixed
Moved the `sendNotification()` call from being automatic (fired right after transfer) to being intentional (fired only when the sender clicks "Video Call").

**Before:**
```
Transfer succeeds → email sent immediately (always includes video call link) → dialog opens
Close → just closes (email already sent)
Video Call → opens call (email already sent)
```

**After:**
```
Transfer succeeds → dialog opens (no email yet)
Close → just closes, nothing sent
Video Call → email sent with video call link → call opens
```

### Why this is a good talking point
It shows you think about **user intent** and **product behavior**, not just whether the code works. The code was technically correct — it sent the email without errors — but the behavior wasn't right from a user experience perspective.

---

## General Questions to Prepare

**"Walk me through your authentication flow."**
> "On signup, the user verifies their email via a tokenized link. On login, they enter email and password — if valid, a 6-digit OTP is sent to their email. Once they verify the OTP, the server issues a JWT stored in an HTTP-only cookie. Every protected route reads that cookie via middleware, validates the JWT, and attaches the user to the request."

**"Why HTTP-only cookies instead of localStorage for the JWT?"**
> "localStorage is accessible to JavaScript, which means an XSS attack can steal the token. HTTP-only cookies are not accessible to JS — the browser sends them automatically on each request but a script can't read them."

**"How does the chatbot work?"**
> "The user sends a message via Socket.io. The backend uses OpenAI's function calling feature to classify the intent — for example, 'send 100 to john@example.com'. If it's a transfer intent, the AI extracts the parameters and calls the same internal transfer function that the regular form uses. The result is sent back through the socket in real time."

**"What would you add next if you had more time?"**
> "Real-time balance updates — when someone sends you money, your dashboard balance updates without a page refresh. The Socket.io infrastructure is already there, it would just need a server-side emit after a transaction completes."

---

## Tech Stack Quick Reference

| Layer | Technology |
|-------|-----------|
| Frontend | React, TypeScript, Material UI |
| State / hooks | Custom hooks (`useAsyncOperation`, `useTransfer`, `useLogin`) |
| Backend | Node.js, Express |
| Database | MongoDB with Mongoose |
| Auth | JWT in HTTP-only cookies |
| Real-time | Socket.io |
| Email | Brevo (Sendinblue) API |
| AI | OpenAI API (intent classification + function calling) |
| Video | WebRTC via a room-based system |
| Security | bcrypt, rate limiting, input validation, HTTP-only cookies |

---

---

# Project Overview – מצגת

---

## מה הפרויקט

**Dubai Bank** היא אפליקציית בנקאות דיגיטלית שמאפשרת:

- הרשמה והתחברות עם **2FA** (OTP למייל)
- צפייה ביתרה והיסטוריית עסקאות עם pagination
- העברות כסף בין משתמשים
- **צ'אטבוט בנקאי** מבוסס AI שמבין שפה טבעית

**הבעיה שנפתרת:** בנקים קלאסיים דורשים ממשק מורכב לביצוע פעולות. כאן המשתמש יכול לכתוב "העבר 200 לדנה" ולקבל תוצאה.

---

## הסטאק

| Layer | Technology |
|-------|-----------|
| Frontend | React + Vite |
| Backend | Node.js + Express |
| Database | MongoDB + Mongoose |
| Authentication | JWT + httpOnly Cookie |
| Real-time | Socket.IO |
| AI | OpenAI GPT-4o-mini (Function Calling) |
| Email | Brevo API |
| Passwords | bcrypt (SALT_ROUNDS = 10) |

---

## ארכיטקטורה – Layers

```
┌─────────────────────────────────────────────┐
│              CLIENT (React)                 │
│   REST requests + Socket.IO connection      │
└──────────────────┬──────────────────────────┘
                   │ HTTP / WebSocket
┌──────────────────▼──────────────────────────┐
│              API LAYER                      │
│  app.js → Routes → Middleware → Controllers │
│                                             │
│  /api/v1/auth         auth.routes.js        │
│  /api/v1/             user.routes.js        │
│  /api/v1/transactions transaction.routes.js │
│  Socket.IO /chat      socket.handler.js     │
└──────────────────┬──────────────────────────┘
                   │
┌──────────────────▼──────────────────────────┐
│             SERVICE LAYER                   │
│  user.service.js         DB logic for users │
│  transaction.service.js  transfers + fetch  │
│  chatbot.service.js      entry + sanitize   │
│  openaiFunctionCall.service.js  AI + tools  │
└──────────────────┬──────────────────────────┘
                   │
┌──────────────────▼──────────────────────────┐
│              DB LAYER                       │
│  user.model.js          User schema         │
│  transaction.model.js   Transaction schema  │
│  MongoDB via Mongoose                       │
└─────────────────────────────────────────────┘
```

**למה ההפרדה?** Controllers לא יודעים על Mongoose. Services לא יודעים על HTTP.
אם מחר מחליפים MongoDB – רק ה-models משתנים.

---

## זרימת בקשה – Transfer

```
1. Client  →  POST /api/v1/transactions  { receiverEmail, amount }
                                          cookie: token=<JWT>

2. app.js  →  cors → cookieParser → express.json → requestLogger

3. protect middleware
   ├── קורא token מה-cookie
   ├── verifyToken(token) → מפענח JWT
   ├── findActiveUserById() → DB query
   └── req.user = { id, email, status }

4. transactionLimiter  →  10 העברות / דקה

5. createTransaction controller
   ├── senderEmail = req.user.email  ← מה-JWT, לא מה-body
   ├── validateTransactionAmount(amount)
   └── executeTransfer()

6. executeTransfer service  –  MongoDB ACID Session
   ├── startSession() → startTransaction()
   ├── deductSenderBalance()   findOneAndUpdate({ balance: { $gte: amount } })
   ├── addReceiverBalance()    findOneAndUpdate({ $inc: { balance: amount } })
   ├── createTransactionRecord()
   └── commitTransaction()  ← הכל או כלום

7. Client  ←  201 + transaction object
```

---

## זרימת בקשה – Chatbot

```
1. Client  →  socket.emit('user-message', "מה היתרה שלי?")

2. socket.handler.js
   ├── isTokenValid()       בדיקת JWT בכל הודעה
   ├── checkRateLimit()     10 הודעות / דקה
   └── processMessage()

3. chatbot.service.js
   ├── sanitizeInput()      trim, slice(250), הסרת HTML tags
   └── processWithFunctionCalling()

4. openaiFunctionCall.service.js
   ├── buildMessagesForOpenAI()  [system + history(20-) + user]
   ├── callOpenAI(messages, TOOLS)
   │
   │   OpenAI מחזיר: tool_calls → get_balance
   │
   ├── executeFunctionCall()  →  getAccountSummary()  →  DB
   ├── push result to messages
   └── callOpenAI שוב  →  "היתרה שלך היא 5,000 AED"

5. socket.emit('bot-message', { response, intent, timestamp })
   אם transferCompleted → socket.emit('transfer-completed')
```

---

## אבטחה

### JWT + httpOnly Cookie

```js
res.cookie('token', token, {
  httpOnly: true,   // JS לא יכול לקרוא → הגנה מ-XSS
  secure: true,     // HTTPS בלבד ב-production
  sameSite: 'lax',  // הגנה מ-CSRF
  maxAge: 3600000,  // 1 שעה
});
```

**למה לא localStorage?** נגיש ל-JS – כל script זדוני יוכל לקרוא.

---

### bcrypt – Passwords & OTP

```js
// גם סיסמה וגם OTP מאוחסנים כ-hash
const hashedPassword = await bcrypt.hash(password, SALT_ROUNDS);
const otpHash        = await bcrypt.hash(otp, SALT_ROUNDS);
```

אם מישהו פרץ ל-DB – הוא לא יכול להשתמש בערכים.

---

### CORS

```js
app.use(cors({
  origin: config.clientUrl,  // לא * – רק ה-client הידוע
  credentials: true,          // מאפשר שליחת cookies
}));
```

---

### Validation – שכבות מרובות

| שכבה | מנגנון |
|------|--------|
| Schema | minlength, maxlength, regex, enum, min, max |
| Controller | validateTransactionAmount() |
| Service | `balance: { $gte: amount }` בתוך ה-query |
| Rate Limit | authLimiter · transactionLimiter · verifyLimiter |

---

### senderEmail מה-JWT בלבד

```js
const senderEmail = req.user.email;  // ← מה-JWT, לא מה-body!
```

המשתמש לא יכול לזייף `senderEmail` ב-body ולהעביר כסף בשם אחר.

---

## ביצועים ואתגרים

### ACID Transactions

**הבעיה:** ניכוי הצליח אבל ה-DB קרס לפני ההוספה למקבל → כסף נעלם.

```js
session.startTransaction();
try {
  await deductSenderBalance(..., session);
  await addReceiverBalance(..., session);
  await createTransactionRecord(..., session);
  await session.commitTransaction();   // הכל
} catch {
  await session.abortTransaction();    // או כלום
}
```

MongoDB sessions דורשות **Replica Set** – לא עובד על standalone. זה היה אתגר ב-setup.

---

### Socket.IO – ניהול חיבורים מרובים

```js
const activeSocketsByUserId = new Map();  // userId → Set<socket>
```

משתמש יכול לפתוח כמה טאבים → כמה connections.
בעת logout – כל ה-sockets של המשתמש מנותקים:

```js
export const disconnectUser = (userId) => {
  for (const socket of activeSocketsByUserId.get(userId)) {
    emitBot(socket, 'You have been logged out.', 'error');
    socket.disconnect();
  }
};
```

---

### Safari JWT Bug

**הבעיה:** Safari לא שולח httpOnly cookies ב-WebSocket handshake ב-cross-origin.

**הפתרון:** שני מנגנוני אימות:

```js
// socketAuth.middleware.js
const token = socket.handshake.auth?.token    // Safari: token ב-auth object
           ?? getTokenFromCookie(cookieHeader); // Chrome/Firefox: מה-cookie
```

---

### Chatbot Evolution

| גרסה | גישה | בעיה |
|------|------|------|
| ראשונה | Intent Detection – AI מחזיר מילה אחת | לא מחלץ פרמטרים, if-else לכל feature |
| נוכחית | Function Calling – AI מחליט מה לקרוא | AI מנהל שיחה, חולץ פרמטרים, מאשר לפני transfer |

---

## משפטי פתיחה לראיון

> *"הפרויקט הוא banking app עם Node/Express ו-MongoDB. האתגר הכי מעניין היה להבטיח שהעברות כסף הן atomic – השתמשתי ב-MongoDB sessions כדי שניכוי מהשולח והוספה למקבל קורים ביחד, או לא קורים כלל."*

> *"ה-JWT שמור ב-httpOnly cookie להגנה מ-XSS. נתקלתי בבאג של Safari שלא שולח cookies ב-WebSocket handshake, אז הוספתי fallback שבו ה-client שולח את הtoken גם ב-auth object של Socket.IO."*

> *"הצ'אטבוט עבר evolution – בהתחלה ה-AI החזיר מילה אחת כ-intent. עברתי ל-Function Calling כי זה מאפשר לו לחלץ פרמטרים מהשיחה ולנהל confirmation flow לפני העברת כסף."*
