# לימוד הפרויקט לעומק — חיבורים, פרוטוקולים, ספריות ומונחים

> מסמך זה מסביר איך כל חלק בפרויקט מתחבר לאחר, אילו פרוטוקולי תקשורת מעורבים, מה עושות הספריות החיצוניות ולמה נבחרו, וכל ה-keywords החשובים.

---

## חלק א׳ — אופן החיבורים בין חלקי הפרויקט

### 1. תמונה כללית של הזרימה

```
[משתמש בדפדפן]
       │
       ▼
[React SPA — Vite, פורט 5173]
       │  ┌─────────────────────────────────────────┐
       │  │ • React Router — ניווט בין דפים (Login,   │
       │  │   Signup, Dashboard, Transfer, Transactions)│
       │  │ • Hooks ב-screens — לוגיקה + קריאות API   │
       │  │ • api/ — auth.service, transaction.service,│
       │  │   user.service, http-client, auth.storage  │
       │  └─────────────────────────────────────────┘
       │
       │  HTTP (REST) — Axios, baseURL מ־VITE_API_URL
       │  עם withCredentials: true (שולח cookies)
       │
       ▼
[Express Server — פורט 3000]
       │  ┌─────────────────────────────────────────┐
       │  │ • Routes: /api/v1/auth, /api/v1,         │
       │  │   /api/v1/transactions                   │
       │  │ • Middleware: CORS, cookie-parser,       │
       │  │   express.json, requestLogger, protect,  │
       │  │   rate limit, errorHandler               │
       │  │ • Controllers → Services → DB / חיצוני  │
       │  └─────────────────────────────────────────┘
       │
       ├── HTTP (REST) ──────────────────────────────► [Brevo API] — שליחת מייל אימות
       │
       ├── Socket.IO (אותו שרת HTTP, upgrade) ───────► [Client Socket.IO] — צ'אטבוט
       │
       └── MongoDB Driver (Mongoose) ─────────────────► [MongoDB] — DB
```

### 2. חיבורים מפורטים

| מ | אל | איך | פרוטוקול / טכנולוגיה |
|---|-----|-----|------------------------|
| **דפדפן** | **React (Vite)** | טוען `index.html` → `main.tsx` → `App` | HTTP (GET), Same-Origin או VITE_API_URL |
| **React** | **Express** | Axios: `httpClient.get/post(...)` ל־`VITE_API_URL` + path (למשל `/auth/login`) | HTTP/1.1, REST (JSON), Cookies (SameSite) |
| **Express** | **MongoDB** | Mongoose: `User.findOne()`, `Transaction.find()`, sessions | TCP, MongoDB Wire Protocol |
| **Express** | **Brevo** | Axios: POST ל־`https://api.brevo.com/v3/...` עם `api-key` | HTTPS, REST |
| **Express** | **OpenAI** (אם בשימוש) | קריאה ל־API של OpenAI לצ'אטבוט | HTTPS, REST |
| **Client** | **Express (Socket)** | Socket.IO Client מתחבר ל־same origin (או ל־URL של השרת) | WebSocket (עם fallback), Socket.IO protocol |
| **שרת** | **Socket.IO namespace `/chat`** | אותו `httpServer`; Socket.IO מוצמד ל־server | Upgrade מ־HTTP ל־WebSocket |

### 3. זרימת בקשה לדוגמה (Login)

1. **דפדפן:** המשתמש לוחץ Submit בטופס Login.
2. **React (useLogin):** `handleSubmit` → `authService.login(email, password)`.
3. **auth.service:** `httpClient.post('/auth/login', { email, password })`.
4. **http-client (Axios):** שולח POST ל־`VITE_API_URL + '/auth/login'` (למשל `http://localhost:5173/api/v1/auth/login` עם proxy), עם `withCredentials: true`.
5. **Express:** מקבל את הבקשה → `cookie-parser` → `express.json` → `requestLogger` → route `POST /api/v1/auth/login` → `authLimiter` → `auth.controller.login`.
6. **Controller:** קורא ל־`user.service` (מציאת user, השוואת סיסמה), `jwt.util` (יצירת token), `res.cookie(...)` — שומר JWT ב־cookie.
7. **תשובה:** 200 + JSON עם `user` (בלי token בגוף); ה־Set-Cookie נשלח בדפדפן.
8. **Client:** Axios מחזיר את התשובה → `useLogin` ב־onSuccess קורא ל־`authStorage.setUser(data.user)` (localStorage) ו־`navigate` ל־Dashboard.

### 4. זרימת Socket.IO (צ'אטבוט)

1. **Client:** מתחבר ל־Socket.IO (אותו host/port כמו API), namespace `/chat`.
2. **שרת:** `initChatbotSocket(io)` — middleware אימות (cookie או `handshake.auth.token`), ואז האזנה ל־`user-message`.
3. **תקשורת:** Client שולח `user-message` → שרת מחשב intent (מילות מפתח או OpenAI) → שרת שולח `bot-message`.

---

## חלק ב׳ — פרוטוקולי תקשורת

| פרוטוקול / טכנולוגיה | איפה בפרויקט | תפקיד |
|----------------------|---------------|--------|
| **HTTP/1.1** | כל הבקשות מהדפדפן לשרת (REST) | GET/POST, headers, body (JSON), status codes |
| **HTTPS** | בפרודקשן; קריאות ל־Brevo ו־OpenAI | הצפנה; הגנה על API keys ותוכן |
| **REST** | API של Express (`/api/v1/...`) | משאבים כ־URLs; methods: GET, POST; תשובות JSON |
| **WebSocket** | Socket.IO (מתחת למכסה) | ערוץ דו־כיווני מתמשך; מתאים לצ'אט |
| **Socket.IO Protocol** | Client ↔ Server (צ'אטבוט) | מסגרת מעל WebSocket: namespaces, events, fallback ל־long polling |
| **MongoDB Wire Protocol** | Mongoose ↔ MongoDB | שאילתות, עדכונים, transactions — על גבי TCP |
| **Cookies (HTTP)** | JWT ב־auth | Set-Cookie (שרת), Cookie (בקשות); `httpOnly` — לא נגיש מ־JavaScript (XSS) |

### CORS (Cross-Origin Resource Sharing)

- **מה זה:** מנגנון בדפדפן שמאפשר לאתר ב־origin אחד לשלוח בקשות ל־origin אחר (למשל client ב־5173 ושרת ב־3000).
- **בפרויקט:** ב־Express מוגדר `cors({ origin: config.clientUrl, credentials: true })` — רק ה־origin של הלקוח מורשה, ו־`credentials: true` מאפשר שליחת cookies.
- **למה חשוב:** בלי CORS מתאים, דפדפן חוסם בקשות cross-origin; עם credentials חייבים origin מדויק ו־credentials: true.

---

## חלק ג׳ — ספריות חיצוניות: מה עושות ולמה נבחרו

### צד Client (React)

| ספרייה | גרסה (בערך) | מה עושה | למה דווקא היא |
|--------|--------------|---------|----------------|
| **react** | 19.x | בניית UI בצורה דקלרטיבית, קומפוננטות, state | סטנדרט תעשייתי; React 19 — עדכונים ומודל עדכני |
| **react-dom** | 19.x | מחבר את React ל־DOM (render, events) | חובה ל־React באתר |
| **react-router-dom** | 7.x | ניתוב בצד הלקוח (SPA): Routes, Navigate, useNavigate, useSearchParams | SPA עם דפים שונים בלי רענון; Protected routes (RequireAuth) |
| **axios** | 1.x | לקוח HTTP: get/post, interceptors, withCredentials | נוח מ־fetch; interceptors ל־401 ולניקוי auth; תמיכה טובה ב־cookies |
| **socket.io-client** | 4.x | חיבור ל־Socket.IO בצד השרת; namespaces, emit/on | אותו פרוטוקול כמו בשרת; אירועים וצ'אט בזמן אמת |
| **@mui/material** | 7.x | קומפוננטות UI (כפתורים, טפסים, כרטיסים) | פיתוח מהיר, עיצוב אחיד, נגישות |
| **@emotion/react** + **@emotion/styled** | 11.x | CSS-in-JS: styled components, theme | אינטגרציה עם MUI; סגנון דינמי וניהול ערכת עיצוב |
| **@mui/icons-material** | 7.x | אייקונים | עקביות עם MUI |
| **vite** | 7.x | Build tool: dev server, HMR, bundle ל־production | מהיר מ־CRA; HMR מעולה; תמיכה ב־ESM ו־TS |
| **typescript** | 5.x | טיפוסים ל־JavaScript | פחות באגים, אוטpletion, תיעוד בקוד |

### צד Server (Node.js)

| ספרייה | גרסה (בערך) | מה עושה | למה דווקא היא |
|--------|--------------|---------|----------------|
| **express** | 5.x | מסגרת HTTP: routes, middleware, תשובות | סטנדרט ל־Node; קל, גמיש, קהילה גדולה; גרסה 5 עם שיפורים |
| **mongoose** | 8.x | ODM ל־MongoDB: schemas, validation, middleware, transactions | מודלים מוגדרים; validation; תמיכה ב־sessions ל־transactions אטומיים |
| **jsonwebtoken** | 9.x | יצירה ואימות JWT (sign, verify) | אבטחה stateless; תאימות עם cookie ו־Bearer |
| **bcrypt** | 6.x |ashing לסיסמאות (hash, compare) | אלגוריתם מאושר; מלח אוטומטי; עמידות ל־brute force |
| **cookie-parser** | 1.x | מפרסר את header ה־Cookie ל־`req.cookies` | צריך כדי לקרוא את ה־JWT מ־cookie ב־protect |
| **cors** | 2.x | מוסיף headers ל־CORS ומטפל ב־preflight | שליטה ב־origin ו־credentials; הגנה מפני גישה לא מורשית |
| **dotenv** | 17.x | טוען `.env` ל־`process.env` | הפרדת סודות מהקוד; סביבות שונות (dev/prod) |
| **express-rate-limit** | 8.x | מגביל מספר בקשות לפי IP (חלון זמן) | הגנה מ־brute force ו־spam (login, signup, transfer) |
| **winston** | 3.x | לוגים: רמות (info, warn, error), פורמט, קבצים | ניפוי באגים וניטור בפרודקשן |
| **axios** | 1.x | קריאות HTTP מהשרת ל־Brevo (ולא OpenAI אם משתמשים ב־fetch/axios) | באימייל: שליחת מייל דרך Brevo API (REST) |
| **socket.io** | 4.x | שרת WebSocket: namespaces, events, אימות | צ'אטבוט בזמן אמת; אותו פרוטוקול עם הלקוח |
| **openai** | 6.x | SDK ל־OpenAI API (למשל gpt-4o-mini) | fallback לזיהוי intent בצ'אטבוט |

הערה: ב־`package.json` מופיעים גם `nodemailer` ו־`resend`; בקוד האימייל נשלח דרך **Brevo API** עם Axios (לא דרך nodemailer/resend). לכן בפועל — Brevo + Axios.

---

## חלק ד׳ — Keywords ומונחים שחשוב להבין

### אבטחה ואימות

| מונח | הסבר | בפרויקט |
|------|------|----------|
| **JWT (JSON Web Token)** | טוקן חתום (header.payload.signature); השרת יוצר ב־login ומאמת בכל בקשה. | `jsonwebtoken`: createToken, verifyToken; נשמר ב־cookie (httpOnly). |
| **HTTP-only cookie** | Cookie שהדפדפן שולח אוטומטית אבל JavaScript לא יכול לקרוא. | מגן מפני גניבת token ב־XSS. |
| **XSS (Cross-Site Scripting)** | הזרקת סקריפט לאתר; אם הטוקן ב־localStorage — הסקריפט יכול לגנוב אותו. | עם cookie + httpOnly הטוקן לא נגיש לסקריפט. |
| **CSRF (Cross-Site Request Forgery)** | אתר זר גורם לדפדפן לשלוח בקשה עם ה־cookies של המשתמש. | SameSite (למשל lax) מפחית סיכון; אפשר גם CSRF token. |
| **bcrypt** | פונקציית hash לסיסמאות עם מלח; איטית במכוון. | hash ב־signup, compare ב־login. |
| **Rate limiting** | הגבלת מספר בקשות ל־IP ב־חלון זמן. | authLimiter, verifyLimiter, transactionLimiter — הגנה מ־brute force. |
| **Protect middleware** | Middleware שבודק JWT ומגדיר `req.user`. | כל route של /me ו־/transactions עובר דרך protect. |

### רשת ו־API

| מונח | הסבר | בפרויקט |
|------|------|----------|
| **REST** | ארכיטקטורת API: משאבים כ־URLs, שימוש ב־HTTP methods, לרוב JSON. | GET/POST ל־/api/v1/auth, /api/v1/me, /api/v1/transactions. |
| **CORS** | מנגנון בדפדפן לאישור בקשות cross-origin. | שרת מחזיר Access-Control-Allow-Origin ו־Credentials. |
| **Credentials (cookies)** | `withCredentials: true` ב־Axios — הדפדפן מצרף cookies לבקשה. | חיוני כדי לשלוח את ה־JWT cookie בכל קריאה. |
| **Proxy (Vite)** | שרת הפיתוח מעביר בקשות ל־path מסוים (למשל /api) לשרת אחר. | ב־dev: `/api` → localhost:3000; כך VITE_API_URL יכול להיות same-origin. |
| **Interceptor (Axios)** | פונקציה שרצה לפני שליחת בקשה או אחרי קבלת תשובה. | Response interceptor: אם 401 — קורא ל־authStorage.clearAuth(). |

### שרת ו־Node

| מונח | הסבר | בפרויקט |
|------|------|----------|
| **Middleware** | פונקציה (req, res, next); רצה לפי סדר לפני ה־route handler. | cors, cookieParser, express.json, requestLogger, protect, authLimiter, errorHandler. |
| **ES Modules** | `import`/`export` (לא require). | `"type": "module"` ב־package.json; סיומת .js ב־import. |
| **Error handler** | Middleware עם 4 פרמטרים (err, req, res, next); תופס כל error שנזרק. | errorHandler: ממיר ל־status + message; לאFoundHandler ל־404. |
| **Trust proxy** | `app.set('trust proxy', 1)` — Express סומך על ה־proxy (Render/Vercel) ב־X-Forwarded-For. | נחוץ ל־rate limit עם IP נכון מאחורי proxy. |

### מסד נתונים

| מונח | הסבר | בפרויקט |
|------|------|----------|
| **MongoDB** | מסד NoSQL מבוסס מסמכים (JSON-like). | User, Transaction, Counter (ל־ID סידורי). |
| **Mongoose** | ODM: הגדרת schema, validation, מתודות (find, create, findOneAndUpdate). | user.model, transaction.model; sessions ל־transactions. |
| **MongoDB Transaction** | session.startTransaction(); כל הפעולות באותה session; commit או abort. | ב־executeTransfer: debit + credit + create record — אטומי. |
| **Atomic operation** | פעולה שלא ניתן לחלק; או שהכל מצליח או שהכל מתבטל. | findOneAndUpdate עם balance >= amount — בדיקה ועדכון ביחד. |

### React ו־Client

| מונח | הסבר | בפרויקט |
|------|------|----------|
| **SPA (Single Page Application)** | אפליקציה שבה דף אחד נטען והניווט מתבצע ב־JavaScript בלי טעינה מחדש. | React Router; כל המסכים הם קומפוננטות. |
| **Hook** | פונקציה שמתחילה ב־use ומשתמשת ב־state/effects (useState, useEffect). | useSignup, useLogin, useTransfer, useTransactions, useDashboardData. |
| **Custom Hook** | Hook שכתבת — מאגד לוגיקה ו־state לשימוש חוזר. | useAsyncOperation (loading, error, execute); hooks ב־screens. |
| **Context** | מנגנון להעברת ערך לכל העץ בלי prop drilling. | ThemeContextProvider — theme (light/dark) לכל האפליקציה. |
| **Protected Route** | Route שדורש התחברות; אחרת מפנה ל־login. | RequireAuth עוטף Dashboard, Transfer, Transactions. |
| **Controlled component** | ערך השדה מגיע מ־state; onChange מעדכן את ה־state. | טפסי Login, Signup, Transfer — value + onChange. |

### Real-time

| מונח | הסבר | בפרויקט |
|------|------|----------|
| **WebSocket** | פרוטוקול תקשורת דו־כיוונית מתמשך על גבי TCP. | Socket.IO משתמש ב־WebSocket (עם fallback). |
| **Socket.IO** | ספרייה שמספקת namespaces, events, reconnect, אימות. | namespace `/chat` לצ'אטבוט; אימות לפני קבלת ה־socket. |
| **Namespace** | חלוקה לוגית של חיבורים (path שונה). | `/chat` — רק צ'אטבוט; שאר ה־API ב־HTTP. |

### Build ו־Dev

| מונח | הסבר | בפרויקט |
|------|------|----------|
| **Vite** | כלי build: dev server עם HMR, bundle ל־production. | `npm run dev` — פורט 5173; `npm run build` — פלט ל־dist. |
| **HMR (Hot Module Replacement)** | עדכון מודולים בלי רענון מלא של הדף. | Vite מספק HMR ל־React. |
| **TypeScript** | JavaScript עם טיפוסים; מקומפל ל־JS. | client ב־TS; types ב־types/index.ts. |
| **Environment variable** | משתנה סביבה (למשל VITE_API_URL, JWT_SECRET). | dotenv בשרת; Vite חושף רק משתנים שמתחילים ב־VITE_ ל־client. |

---

## חלק ה׳ — סיכום זרימות מרכזיות

1. **Signup:** Client (useSignup → authService.signup) → HTTP POST /auth/signup → Express (authLimiter → controller → createUser, sendVerificationEmailAsync) → MongoDB (User.create) + Brevo (HTTP).  
2. **Verify:** משתמש לוחץ על לינק במייל → GET /auth/verify?token=... → Express → findAndVerifyUserByToken → HTML success/failure.  
3. **Login:** Client → POST /auth/login → Express → findUser, validatePassword, createToken, res.cookie → 200 + user; Client שומר user ב־localStorage.  
4. **Transfer:** Client (useTransfer → transactionService.create) → POST /transactions (עם cookie) → protect → transactionLimiter → controller → executeTransfer (MongoDB session: deduct, add, create record) → 201.  
5. **Transactions list:** Client (useTransactions → transactionService.getAll) → GET /transactions?page=&limit= → protect → findTransactionsByUserEmail → 200 + transactions, totalPages.  
6. **Chatbot:** Client מתחבר ל־Socket.IO /chat → אימות (cookie/token) → user-message → שרת (intent + OpenAI אם צריך) → bot-message.

---

## חלק ו׳ — קישור למסמכים נוספים

- **ARCHITECTURE.md** — פירוט מלא של כל תהליך (signup, login, verify, transfer, transactions), קבצים, middleware, hooks, MongoDB, ותיאור sequence לכל flow.
- **README.md** — סקירת Stack, API routes, rate limits, טיפול בשגיאות, ו־quick reference.

אם תרצה להעמיק בנושא מסוים (למשל רק auth, או רק Socket.IO), אפשר להרחיב את החלק הרלוונטי במסמך הזה או ב־ARCHITECTURE.
