# מסמך הכנה טכנית לראיונות עבודה — טל הינדי

> **מטרה:** מסמך למידה מובנה עבור NotebookLLM. מכסה את כל מה שמופיע בקורות החיים ברמת עומק של ראיון טכני.
> **תאריך:** 2026-03-16

---

## כללי אמינות

- **"מהקורות חיים"** — מידע שמופיע ישירות בקורות החיים
- **"ידע מקצועי"** — הסבר טכני שנוסף כדי להעמיק הבנה
- **[צריך להשלים מידע]** — פרטים שלא מופיעים ב-CV וצריך להכין

---

## SECTION 1 — סיכום מהיר של הפרופיל

### 1.1 רקע (מהקורות חיים)

- בוגר הנדסת תעשייה וניהול (מערכות מידע) — המרכז האקדמי רופין (2020–2024)
- הכשרה אינטנסיבית ב-Full-Stack ותכנות מערכות — Infinity Labs (2025–2026)
- ניסיון כ-Backend Developer ב-C-Vision (2024–2025)
- שירות צבאי — קצין ביטחון, בסיס 5050

### 1.2 חוזקות טכניות מרכזיות (מהקורות חיים)

- **Full-Stack Development:** React, Node.js, Express, MongoDB, TypeScript
- **System Programming:** C, OS internals, multi-threaded/multi-process, memory management
- **Industrial Automation:** ASP.NET, C#, WinForms, SCADA, PLC, MS-SQL
- **AI Integration:** OpenAI API, function calling, agentic programming patterns
- **DevOps:** Docker, Git, Linux/Bash, CI/CD

### 1.3 תחומי ניסיון (מהקורות חיים)

- פיתוח מערכות ייצור (MES) בתעשייה
- בניית אפליקציות בנקאיות Full-Stack
- אינטגרציית SCADA/PLC לאוטומציה תעשייתית
- צ'אטבוט AI עם יכולות פעולה אמיתיות (function calling)

### 1.4 טכנולוגיות שסביר שיידונו בראיון

| עדיפות גבוהה | עדיפות בינונית | עדיפות נמוכה |
|---|---|---|
| React, Node.js, Express | C#, ASP.NET | SCADA, PLC |
| MongoDB, MS-SQL | WinForms | Automation Studio |
| TypeScript, JWT | Docker, CI/CD | STL (Structured Text) |
| REST API, WebSockets | C, OS internals | |
| OpenAI API | Design Patterns, SOLID | |

**בדיקת דיוק:**
- האם סדר העדיפויות תואם את התפקידים שאתה מתמיין אליהם?
- האם יש טכנולוגיה שחסרה כאן שאתה מרגיש חזק בה?

---

## SECTION 2 — ניסיון מקצועי

---

### 2.1 Backend Developer | C-Vision (2024–2025)

**תפקיד (מהקורות חיים):**
Backend Developer בחברת C-Vision

**מה עשיתי בפועל (מהקורות חיים):**

- הובלת פיתוח מערכת ניטור תעשייתית עבור Vishay, לתמיכה בהעברת ייצור קבלים לישראל
- תכנון ופיתוח מערכת מעקב אוטומטית לתהליכי בדיקת תנורים (oven test), החלפת תהליכים ידניים
- פיתוח אפליקציות ASP.NET, C#, SQL ו-WinForms עבור Manufacturing Execution Systems (MES)
- אינטגרציית מערכות SCADA עם בקרי PLC (Structured Text, Automation Studio) לאוטומציה תעשייתית
- פיתוח מערכת מעקב והתראות בזמן אמת לתהליכי oven test
- יצירת דשבורדים וכלי דיווח בהתאמה אישית לנראות ייצור בזמן אמת
- אופטימיזציה של ביצועי MS-SQL ואוטומציה של תהליכים

**הבעיה העסקית / הטכנית שנפתרה (ידע מקצועי):**

- Vishay העבירה קו ייצור קבלים לישראל — נדרשה מערכת חדשה לניטור ובקרת איכות
- תהליכי בדיקת תנורים היו ידניים — סיכון לשגיאות אנושיות, חוסר נראות, איטיות
- הפתרון: מערכת MES אוטומטית שמחליפה תהליכים ידניים בניטור דיגיטלי בזמן אמת

**הארכיטקטורה ברמה גבוהה (ידע מקצועי):**

```
┌──────────────┐     ┌──────────────┐     ┌──────────────┐
│   WinForms   │────►│  ASP.NET     │────►│   MS-SQL     │
│   (UI/HMI)   │     │  Backend     │     │   Database   │
└──────────────┘     └──────┬───────┘     └──────────────┘
                            │
                     ┌──────▼───────┐
                     │  SCADA/PLC   │
                     │  (Automation │
                     │   Studio)    │
                     └──────────────┘
```

[צריך להשלים מידע] — מה בדיוק הארכיטקטורה? כמה שרתים? איך WinForms תקשר עם ה-Backend? האם יש message queue? האם יש microservices?

**החלטות טכנולוגיות — למה נבחרו הכלים האלו (ידע מקצועי):**

- **ASP.NET + C#:** סטנדרט בתעשייה עבור enterprise applications, אינטגרציה טובה עם Windows ו-SQL Server
- **WinForms:** ממשק desktop יציב לאופרטורים ברצפת הייצור — לא צריך דפדפן, עובד offline
- **MS-SQL:** בסיס נתונים רלציוני חזק עם stored procedures, מתאים לנתונים מובנים של ייצור
- **SCADA + PLC:** שכבת בקרה תעשייתית — SCADA מנטר ומציג, PLC שולט בחומרה בפועל
- **Structured Text (STL):** שפת תכנות סטנדרטית ל-PLC (IEC 61131-3), מתאימה ללוגיקה מורכבת

**אתגרים טכניים אפשריים (ידע מקצועי):**

- סנכרון בין SCADA למערכת ה-MES — עדכונים בזמן אמת מהתנורים
- אמינות של מערכת מעקב שרצה 24/7 בסביבת ייצור
- טיפול בנתונים לא תקינים מהחיישנים
- ביצועי MS-SQL תחת עומס של writes רציפים מקווי ייצור

**נקודות חולשה שמראיין עלול לתקוף:**

- "מה הניסיון שלך בסקייל? כמה תנורים/מכונות המערכת תמכה?"
- "איך טיפלת ב-failure של PLC? מה קורה כשהתקשורת נופלת?"
- "למה WinForms ולא Web Application?"
- "מה ההבדל בין Polling ל-Push עבור עדכוני SCADA?"

**שאלות ריאיון אפשריות:**

1. תאר את הארכיטקטורה של המערכת שבנית ב-C-Vision
2. איך עשית אופטימיזציה ל-MS-SQL? תן דוגמה ספציפית
3. מה ההבדל בין SCADA ל-PLC? מה התפקיד של כל אחד?
4. איך המערכת טיפלה ב-real-time data מהתנורים?
5. מה הכי קשה היה בפרויקט הזה טכנולוגית?

**בדיקת דיוק:**
- האם תיאור הארכיטקטורה מדויק? מה חסר?
- כמה תנורים/מכונות המערכת תמכה?
- האם היו אתגרים נוספים שלא צוינו?

---

### 2.2 הכשרת Full-Stack & System Programming | Infinity Labs (2025–2026)

**תפקיד (מהקורות חיים):**
הכשרה אינטנסיבית ב-Full-Stack ותכנות מערכות

**מה למדתי בפועל (מהקורות חיים):**

- תכנות C ו-system programming: multi-threaded/multi-process, ניהול זיכרון, debugging ברמה נמוכה
- מבני נתונים ואלגוריתמים עם דגש על מימוש ואנליזת סיבוכיות
- OOP, Design Patterns, SOLID — בארכיטקטורות cross-platform ו-multi-tier
- Agile, CI/CD, code reviews, אסטרטגיות בדיקה (unit, regression, integration)
- Secure AI Programming, אינטגרציית LLM, ו-agentic programming patterns

**למה זה חשוב לראיון (ידע מקצועי):**

- ידע ב-C ו-OS internals מראה הבנה עמוקה של מה שקורה "מתחת למכסה המנוע"
- מראיינים אוהבים לשאול על threads vs processes, memory leaks, ו-race conditions
- הרקע ב-system programming מבדיל אותך ממפתחי Full-Stack רגילים

**שאלות ריאיון אפשריות:**

1. מה ההבדל בין process ל-thread?
2. מה זה race condition? תן דוגמה מהניסיון שלך
3. מה זה memory leak? איך מאתרים?
4. הסבר את עקרונות SOLID עם דוגמאות
5. מה Design Pattern אחד שהשתמשת בו ולמה?

**בדיקת דיוק:**
- מה הפרויקטים הספציפיים שעשית ב-Infinity Labs?
- האם יש פרויקט גמר או פרויקט C משמעותי שאפשר להזכיר?

---

### 2.3 פרויקט Dubai-Bank (מהקורות חיים)

**תיאור (מהקורות חיים):**
אפליקציית בנקאות Full-Stack: React, TypeScript, Node.js, Express, MongoDB עם JWT authentication, WebSocket chat בזמן אמת, צ'אטבוט AI עם OpenAI function calling, ו-Docker deployment.

> ניתוח מעמיק בסעיף 3.

---

## SECTION 3 — DEEP DIVE: פרויקט Dubai-Bank

---

### 3.1 System Architecture (מהקורות חיים + ידע מקצועי)

**ארכיטקטורה: Three-Tier Layered Architecture**

```
┌────────────────────────────────────────┐
│  CLIENT (React SPA)                     │
│  - Axios HTTP ──► REST API              │
│  - Socket.IO ──► /chat namespace        │
│  - Jitsi iframe ──► WebRTC video        │
└──────────────────┬─────────────────────┘
                   │
┌──────────────────▼─────────────────────┐
│  SERVER (Express 5 + Socket.IO)         │
│  Middleware Chain:                       │
│    CORS → Cookie → JSON → Logger →      │
│    RateLimit → Auth → Routes →          │
│    NotFound → ErrorHandler              │
│                                         │
│  Controllers → Services → Models        │
│         │                               │
│         ├── OpenAI API (chatbot)        │
│         └── Brevo API (email)           │
└──────────────────┬─────────────────────┘
                   │
┌──────────────────▼─────────────────────┐
│  DATA LAYER                             │
│  MongoDB: users, transactions, counters │
└─────────────────────────────────────────┘
```

**עקרונות ארכיטקטוניים:**

- **Separation of Concerns:** כל שכבה אחראית על דבר אחד — Routes ממפים HTTP, Controllers מפרסרים request, Services מכילים business logic, Models מגדירים schema
- **Immutable Configuration:** כל ה-config עטוף ב-`Object.freeze()` — אי אפשר לשנות בזמן ריצה
- **Graceful Shutdown:** טיפול ב-SIGTERM/SIGINT — סגירת HTTP server, סגירת חיבור DB, ואז יציאה
- **Fire-and-Forget Async:** שליחת emails לא חוסמת את ה-flow — שגיאות נתפסות פנימית

---

### 3.2 Frontend Structure — React (מהקורות חיים + ידע מקצועי)

**טכנולוגיות:**
React 19.2, TypeScript, Vite, MUI (Material UI), React Router, Axios, Socket.IO Client

**מבנה:**

| שכבה | אחריות |
|---|---|
| **Screens** | דף אחד לכל route — Login, Signup, Dashboard, Transfer, Transactions, VideoCall |
| **Components** | רכיבי UI משותפים — ChatAssistant, VideoCall, דיאלוגים |
| **Hooks** | חילוץ לוגיקה stateful — useAuth, useAsyncOperation, useTransfer וכו' |
| **API Layer** | Axios service modules שעוטפים REST calls |
| **Types** | TypeScript interfaces — User, Transaction, API shapes |
| **Context** | ThemeContext — dark/light mode |

**Patterns בצד הלקוח:**

- **Observer / Pub-Sub:** `authStorage` שולח `auth-state-change` DOM events. `dashboard:refresh` מגשר בין Socket.IO ל-React state
- **Command Pattern:** `useAsyncOperation` — עוטף כל פונקציה async ל-`{loading, error, execute}`
- **Deterministic Room Generation:** SHA-256 של זוג emails ממוין — אותם שני משתמשים תמיד מקבלים אותו חדר וידאו

**שאלות ריאיון אפשריות על הפרונט:**

1. למה React? מה היתרונות לעומת Angular/Vue?
2. מה זה custom hook? תן דוגמה מהפרויקט
3. איך ניהלת state? למה לא Redux?
4. איך עובד ה-auth flow בצד הלקוח?
5. מה זה `useAsyncOperation` ולמה יצרת אותו?

---

### 3.3 Backend Structure — Node + Express (מהקורות חיים + ידע מקצועי)

**טכנולוגיות:**
Node.js 20 (ES Modules), Express 5, Mongoose, Socket.IO, Winston, JWT, bcrypt

**שכבות בצד השרת:**

| שכבה | אחריות | קבצים |
|---|---|---|
| Routes | מיפוי HTTP verb + path, חיבור middleware | `routes/*.routes.js` |
| Controllers | פירסור request, ולידציה, עיצוב response | `controllers/*.controller.js` |
| Services | business logic, queries, API calls חיצוניים | `services/*.service.js` |
| Models | Mongoose schemas, indexes, statics | `models/*.model.js` |
| Middleware | auth, logging, rate-limiting, error handling | `middleware/*.middleware.js` |
| Utils | helpers חסרי state: JWT, hashing, validation, email | `utils/*.util.js` |

**Middleware Chain:**
```
CORS → Cookie Parser → JSON Body (10kb) → Logger → Rate Limit → Auth → Routes → NotFound → ErrorHandler
```

**Express 5:**
- תומך ב-async error handling מובנה — אין צורך ב-try/catch ידני בכל controller
- `trust proxy` מאפשר rate limiting נכון מאחורי reverse proxy

---

### 3.4 Database Design (מהקורות חיים + ידע מקצועי)

**MongoDB עם Mongoose ODM**

**User Schema:**

| שדה | סוג | אילוצים |
|---|---|---|
| `id` | String | UUID via `crypto.randomUUID()`, ייחודי, immutable |
| `firstName`, `lastName` | String | 2–50 תווים, trimmed |
| `email` | String | ייחודי, lowercase, regex validation |
| `password` | String | מינימום 8 תווים, `select: false` (לא חוזר בשאילתות) |
| `phone` | String | פורמט בינלאומי |
| `status` | Enum | `PENDING` / `ACTIVE` |
| `verificationToken` | String | SHA-256 hash של הטוקן המקורי |
| `balance` | Number | ברירת מחדל 500, מינימום 0 |

**Transaction Schema:**

| שדה | סוג | אילוצים |
|---|---|---|
| `id` | Number | auto-incrementing via Counter collection |
| `fromEmail` | String | email השולח |
| `toEmail` | String | email המקבל |
| `amount` | Number | 0.01–1,000,000, חיובי |
| `description` | String | עד 500 תווים |

**Indexes:** `(fromEmail, createdAt)` ו-`(toEmail, createdAt)` — compound indexes לשאילתות מהירות

**Counter Collection:** collection נפרד שמחזיק sequence מספרי. `getNextTransactionId(session)` מגדיל אטומית ומחזיר ID בתוך MongoDB session.

**למה MongoDB ולא SQL? (ידע מקצועי):**
- Schema גמיש — קל להוסיף שדות בלי migrations
- JSON-native — מתאים ל-Node.js stack
- MongoDB Atlas עם replica set תומך ב-ACID transactions
- עבור banking application אמיתי, SQL יכול להיות עדיף בגלל ACID מובנה ו-referential integrity

**שאלות ריאיון אפשריות:**

1. למה MongoDB ולא PostgreSQL לאפליקציית בנקאות?
2. מה היתרון של compound index? מתי הוא עוזר?
3. למה `select: false` על password?
4. למה Counter collection נפרד ולא ObjectId?

---

### 3.5 Authentication Flow — JWT (מהקורות חיים + ידע מקצועי)

**Flow מלא:**

```
Signup:
  1. ולידציית שדות
  2. Hash password (bcrypt, 10 rounds)
  3. יצירת verification token (32 random bytes hex)
  4. שמירת SHA-256 hash של הטוקן ב-DB
  5. שליחת email אימות (Brevo) — fire-and-forget
  6. החזרת 201

Email Verification:
  1. GET /verify?token=X
  2. Hash של הטוקן
  3. חיפוש user עם status=PENDING ו-hash תואם
  4. עדכון status ל-ACTIVE
  5. החזרת דף HTML הצלחה

Login:
  1. חיפוש user לפי email
  2. bcrypt compare של הסיסמה
  3. בדיקה שה-status הוא ACTIVE
  4. יצירת JWT ({id, email}, תוקף שעה)
  5. הגדרת httpOnly secure cookie
  6. החזרת user data

Auth Guard:
  1. חילוץ JWT מ-cookie או Authorization: Bearer header
  2. verify של הטוקן
  3. טעינת user פעיל
  4. הצמדה ל-req.user

Logout:
  1. ניתוק Socket.IO connections של המשתמש
  2. מחיקת auth cookie
```

**אבטחה (ידע מקצועי):**

- **httpOnly cookie:** ה-JWT לא נגיש מ-JavaScript — מונע XSS גניבת טוקן
- **bcrypt 10 rounds:** איטי מספיק להגנה מ-brute force, מהיר מספיק ל-UX
- **SHA-256 על verification token:** גם אם ה-DB נפרץ, הטוקנים לא שמישים
- **Dual extraction:** תומך גם ב-cookie וגם ב-Bearer header — גמישות לאינטגרציות
- **Rate limiting:** 5 ניסיונות login ל-15 דקות

**שאלות ריאיון אפשריות:**

1. למה JWT ולא session-based auth?
2. מה ההבדל בין httpOnly cookie ל-localStorage?
3. מה קורה כשה-JWT פג תוקף? יש refresh token?
4. למה bcrypt ולא SHA-256 לסיסמאות?
5. איך מונעים CSRF כשה-JWT ב-cookie?
6. מה ה-tradeoff של JWT — מה החיסרון?

**תשובה ל-tradeoff (ידע מקצועי):**
- JWT הוא stateless — אי אפשר לבטל טוקן ספציפי (למשל אחרי logout מרחוק) בלי מנגנון blacklist
- Session-based auth מאפשר invalidation מיידית אבל דורש מאגר sessions (Redis)
- בפרויקט הזה: JWT עם תוקף קצר (שעה) — פשרה סבירה

[צריך להשלים מידע] — האם מימשת refresh token? אם לא, צריך להכין תשובה למה לא ומה היית עושה.

---

### 3.6 Money Transfer Logic — ACID (מהקורות חיים + ידע מקצועי)

**זו הפונקציה הקריטית ביותר במערכת: `executeTransfer()`**

**Flow:**

```
1. Validate: sender !== receiver
2. Start MongoDB session + transaction
3. deductSenderBalance():
   - findOneAndUpdate({email: sender, balance >= amount}, {$inc: -amount})
   - אם ה-balance לא מספיק — הפעולה נכשלת ברמת ה-DB
4. addReceiverBalance():
   - findOneAndUpdate({email: receiver}, {$inc: +amount})
5. createTransactionRecord():
   - Counter.findByIdAndUpdate({$inc}) — קבלת ID הבא
   - Transaction.create() — יצירת רשומה
6. commitTransaction() — או abort בכל שגיאה
7. endSession()
```

**למה זה ACID (ידע מקצועי):**

- **Atomicity:** כל הפעולות (חיוב, זיכוי, רשומה) קורות יחד או לא קורות בכלל
- **Consistency:** `balance >= amount` guard מונע overdraft ברמת ה-DB
- **Isolation:** MongoDB session מבודדת את הפעולות מ-transactions אחרים
- **Durability:** אחרי commit, הנתונים שמורים ב-disk (replica set)

**מניעת Race Conditions:**
- ה-guard `{balance >= amount}` הוא atomic — שני transfers מקבילים לא יכולים לחרוג מהיתרה
- אם שני requests מנסים להוריד 300 מיתרה של 500, רק אחד יצליח

**שאלות ריאיון אפשריות:**

1. מה קורה אם השרת קורס באמצע transfer?
2. איך מנעת double-spending?
3. למה MongoDB session ולא שני update calls רגילים?
4. מה היית עושה בסקייל של מיליון transactions בשנייה?
5. מה ההבדל בין pessimistic ל-optimistic locking? מה השתמשת?

**תשובה מוכנה לסקייל (ידע מקצועי):**
- MongoDB transactions על replica set מתאימים לסקייל בינוני
- לסקייל גדול: שימוש ב-message queue (Kafka/RabbitMQ) עם event sourcing
- CQRS — הפרדת read/write models
- Sharding של MongoDB לפי user ID

---

### 3.7 WebSocket Communication (מהקורות חיים + ידע מקצועי)

**Socket.IO — /chat namespace**

**ארכיטקטורה:**

- **Authentication:** middleware שמחלץ JWT מ-cookie או handshake auth
- **Connection tracking:** `activeSocketsByUserId: Map<userId, Set<Socket>>` — משתמש יכול להיות מחובר ממספר tabs
- **Rate limiting:** 10 הודעות ל-60 שניות per user, via Map בזיכרון
- **Message flow:** `user-message` → validate → rate check → processMessage() → emit `bot-message`
- **Transfer bridge:** אם הצ'אטבוט ביצע העברה → emit `transfer-completed` → הלקוח שולח `dashboard:refresh`
- **Logout disconnect:** `disconnectUser(userId)` — סוגר את כל ה-sockets של המשתמש

**למה Socket.IO ולא WebSocket רגיל (ידע מקצועי):**

- Auto-reconnection מובנה
- Fallback ל-HTTP long-polling אם WebSocket לא זמין
- Namespaces — הפרדה לוגית (למשל `/chat`)
- Rooms — קיבוץ connections (לא בשימוש כאן אבל זמין)
- Event-based API — נוח יותר מ-raw WebSocket

**שאלות ריאיון אפשריות:**

1. מה ההבדל בין WebSocket ל-HTTP?
2. מה ההבדל בין Socket.IO ל-raw WebSocket?
3. איך ה-rate limiting עובד? מה הבעיה עם in-memory rate limiting?
4. מה קורה כשהשרת restarts? מה קורה ל-connections?
5. איך הייתה מסקלר את ה-WebSocket layer?

**תשובה לסקייל (ידע מקצועי):**
- בעיה: in-memory state (active sockets, rate limits) אבד ב-restart
- פתרון: Redis adapter ל-Socket.IO — מאפשר מספר instances לשתף state
- Sticky sessions אם משתמשים ב-load balancer

---

### 3.8 AI Chatbot Integration (מהקורות חיים + ידע מקצועי)

**OpenAI Function Calling — Agentic Tool-Call Loop**

**כלים רשומים:**

| כלי | מה עושה |
|---|---|
| `get_balance` | מחזיר יתרת חשבון |
| `get_transaction_history` | מחזיר היסטוריית transactions |
| `transfer_money` | מבצע העברת כסף אמיתית |
| `get_supported_services` | מחזיר רשימת יכולות |

**Loop עיבוד:**

```
1. בניית messages: system prompt + 20 הודעות אחרונות + הודעת משתמש חדשה
2. קריאה ל-OpenAI GPT-4o-mini עם tool definitions (temperature 0.3)
3. אם התשובה מכילה tool_calls:
   - ביצוע כולם במקביל (Promise.all)
   - הוספת תוצאות ל-messages
   - קריאה חוזרת ל-OpenAI
4. חזרה על שלב 3 עד MAX_TOOL_CALL_ROUNDS (5)
5. חילוץ תשובת טקסט סופית
```

**אבטחה בצ'אטבוט:**

- System prompt מכריח שלב אישור לפני transfers
- Input sanitization: trim, 250 תווים מקסימום, הסרת HTML
- Rate limiting ברמת Socket.IO

**למה function calling ולא intent-based (ידע מקצועי):**

- Intent-based: מפתח צריך לנתח ידנית מה המשתמש רוצה → שביר, לא מסקלר
- Function calling: המודל בוחר את הכלי הנכון בעצמו → גמיש, מסקלר, ניתן להרחבה
- ה-CV מציין שהייתה גרסת intent שהוחלפה — מראה growth ויכולת להעריך מחדש

**שאלות ריאיון אפשריות:**

1. מה זה function calling ב-OpenAI? מה ההבדל מ-regular prompting?
2. מה קורה אם ה-AI "הוזה" ומבצע transfer בלי אישור?
3. למה temperature 0.3 ולא 0?
4. מה ההגנות מפני prompt injection?
5. מה ה-tradeoff של קריאה ל-OpenAI בכל הודעה?

[צריך להשלים מידע] — האם יש הגנת prompt injection ספציפית? אם לא, צריך להכין תשובה מה היית מוסיף.

---

### 3.9 Docker Deployment (מהקורות חיים + ידע מקצועי)

**מבנה:**

| Component | Image | Hosting |
|---|---|---|
| Client | Multi-stage: Node builder → Nginx Alpine | Vercel (production) |
| Server | Node 20 Alpine | Render (Docker) |
| Database | MongoDB 7 (local), Atlas (production) | MongoDB Atlas |
| Local Dev | Docker Compose: MongoDB + Server + Nginx-fronted Client | |

**Docker Compose:**

- MongoDB 7 כ-service מקומי
- Server עם environment variables
- Client מאחורי Nginx (SPA routing)

**למה multi-stage build (ידע מקצועי):**

- שלב build: Node image מלא — להרצת `npm run build`
- שלב production: רק Nginx Alpine עם הקבצים הסטטיים — image קטן, מאובטח

**שאלות ריאיון אפשריות:**

1. מה ההבדל בין Docker image ל-container?
2. למה multi-stage build?
3. מה Docker Compose פותר?
4. מה ההבדל בין Vercel deployment ל-Docker deployment?
5. איך הייתה מוסיף CI/CD?

---

### 3.10 Security Considerations (ידע מקצועי)

**מה מיושם:**

- httpOnly cookies — מניעת XSS access ל-JWT
- bcrypt password hashing — 10 rounds
- Rate limiting — על auth, verification, ו-transactions
- Input validation — ברמת Mongoose schema ו-controller
- CORS — configured per environment
- `select: false` על password — לא חוזר בשאילתות
- SHA-256 על verification tokens
- `trust proxy` — לזיהוי IP נכון מאחורי proxy
- Input sanitization בצ'אטבוט

**מה חסר (ידע מקצועי — נקודות שמראיין יכול לתקוף):**

- אין CSRF token (הגנה חלקית מ-SameSite cookie)
- אין DOMPurify / XSS library ייעודי
- אין refresh token — JWT פג אחרי שעה, המשתמש צריך login מחדש
- אין IP blocking / account lockout אחרי ניסיונות login כושלים
- אין audit log מפורט
- אין encryption at rest

---

### 3.11 Scalability Considerations (ידע מקצועי)

**מצב נוכחי:**
- Single server instance
- In-memory rate limiting ו-socket tracking
- MongoDB Atlas replica set

**צווארי בקבוק פוטנציאליים:**

| בעיה | פתרון |
|---|---|
| WebSocket state in-memory | Redis adapter ל-Socket.IO |
| Rate limiting in-memory | Redis-based rate limiting |
| Single server | Horizontal scaling + load balancer |
| MongoDB writes | Sharding, read replicas |
| OpenAI latency | Caching, streaming responses |
| Large transaction tables | Partitioning by date, archiving |

**שאלות System Design אפשריות:**

1. "תעצב את המערכת הזו ל-10 מיליון משתמשים"
2. "מה קורה אם MongoDB נופל? מה ה-failover strategy?"
3. "איך הייתה מוסיף caching?"
4. "מה ההבדל בין vertical ל-horizontal scaling?"

**בדיקת דיוק כללית לסעיף 3:**
- האם כל הפרטים הטכניים תואמים את המימוש בפועל?
- האם יש features או אתגרים שלא צוינו?
- האם התיאור של ה-ACID flow מדויק?

---

## SECTION 4 — TECH STACK DEEP DIVE

---

### 4.1 React

**מה זה (ידע מקצועי):**
ספריית JavaScript לבניית ממשקי משתמש, מבוססת component architecture. מפותחת על ידי Meta.

**איך זה עובד מתחת למכסה המנוע:**
- Virtual DOM — עץ אובייקטים שמייצג את ה-UI בזיכרון
- Reconciliation — React משווה את ה-Virtual DOM הישן לחדש (diffing algorithm)
- רק שינויים בפועל מעודכנים ב-Real DOM — ביצועים טובים
- React 18+ משתמש ב-Fiber architecture — מאפשר concurrent rendering ועדיפות ל-updates

**מתי משתמשים:**
- SPA (Single Page Applications)
- ממשקים עם state מורכב שמשתנה הרבה
- כשצריך ecosystem גדול של ספריות

**Trade-offs:**
| יתרון | חיסרון |
|---|---|
| Component reuse | לא framework מלא — צריך להחליט על routing, state management וכו' |
| ביצועים (Virtual DOM) | Learning curve עם hooks ו-closures |
| Ecosystem ענק | Bundle size יכול לגדול |
| JSX — UI כ-JavaScript | SSR דורש פתרון נוסף (Next.js) |

**שאלות ריאיון קשות:**

1. מה ההבדל בין Virtual DOM ל-Real DOM? למה Virtual DOM מהיר יותר?
2. הסבר את ה-reconciliation algorithm
3. מה ההבדל בין `useState` ל-`useRef`?
4. מתי `useEffect` רץ? מה ה-cleanup function?
5. מה ההבדל בין controlled ל-uncontrolled component?
6. מה זה React.memo? מתי משתמשים?
7. מה הבעיה עם closure stale values ב-hooks?
8. מה ההבדל בין Context ל-Redux?

---

### 4.2 Node.js

**מה זה (ידע מקצועי):**
Runtime environment שמריץ JavaScript מחוץ לדפדפן, מבוסס על V8 engine של Chrome.

**איך זה עובד מתחת למכסה המנוע:**
- **Event Loop:** single-threaded, non-blocking I/O
- **libuv:** ספריית C שמטפלת ב-async I/O (file system, network, DNS) באמצעות thread pool
- **V8 Engine:** מקמפל JavaScript ל-machine code (JIT compilation)
- **Event Loop Phases:** timers → pending callbacks → idle → poll → check → close callbacks

**מתי משתמשים:**
- I/O intensive applications (APIs, chat, streaming)
- Microservices
- Real-time applications

**Trade-offs:**
| יתרון | חיסרון |
|---|---|
| Non-blocking I/O — throughput גבוה | לא מתאים ל-CPU intensive tasks |
| JavaScript בצד שרת — שפה אחת | Single-threaded — loop ארוך חוסם את כל השרת |
| NPM — ecosystem ענק | Callback hell (פחות רלוונטי עם async/await) |
| קל להתחיל | Memory leaks קשים לאתר |

**שאלות ריאיון קשות:**

1. הסבר את ה-Event Loop שלב אחרי שלב
2. מה ההבדל בין `process.nextTick()` ל-`setImmediate()`?
3. מה קורה כשפונקציה sync ארוכה חוסמת את ה-event loop?
4. מה ההבדל בין worker threads ל-child process?
5. איך Node.js מטפל ב-concurrency אם הוא single-threaded?
6. מה זה Streams? למה הם חשובים?

---

### 4.3 Express

**מה זה (ידע מקצועי):**
Framework מינימליסטי ל-Node.js לבניית web applications ו-APIs.

**איך זה עובד מתחת למכסה המנוע:**
- Middleware chain — כל request עובר דרך שרשרת פונקציות
- כל middleware מקבל `(req, res, next)` — יכול לשנות, לענות, או להעביר הלאה
- Router — מנתב requests לפי HTTP method ו-path
- Express 5 — async error handling מובנה, path matching משופר

**מתי משתמשים:**
- REST APIs
- Server-side rendering
- כל web application ב-Node.js

**Trade-offs:**
| יתרון | חיסרון |
|---|---|
| מינימליסטי וגמיש | צריך להוסיף הכל בעצמך (validation, auth וכו') |
| Middleware pattern חזק | אין structure מובנה — קל "להתבלגן" |
| Ecosystem ענק | ביצועים נמוכים יחסית ל-Fastify |
| קהילה ותיעוד | Express 4 ישן, Express 5 חדש ופחות מתועד |

**שאלות ריאיון קשות:**

1. הסבר את ה-middleware chain. מה קורה אם לא קוראים ל-`next()`?
2. מה ההבדל בין `app.use()` ל-`app.get()`?
3. איך error handling עובד ב-Express?
4. מה ההבדל בין Express 4 ל-Express 5?
5. למה Express ולא Fastify/Koa?

---

### 4.4 MongoDB

**מה זה (ידע מקצועי):**
NoSQL document database שמאחסן נתונים כ-JSON-like documents (BSON).

**איך זה עובד מתחת למכסה המנוע:**
- Documents מאוחסנים ב-BSON (Binary JSON) — תומך בסוגים נוספים (Date, ObjectId, Decimal128)
- WiredTiger storage engine — document-level locking, compression, in-memory caching
- Indexes מבוססים B-tree
- Replica Set — primary + secondaries, automatic failover
- Transactions (מגרסה 4.0) — ACID על replica set

**מתי משתמשים:**
- Schema גמיש — נתונים שמשתנים בתדירות
- Document-oriented data
- Rapid development
- Horizontal scaling עם sharding

**Trade-offs:**
| יתרון | חיסרון |
|---|---|
| Schema גמיש | אין referential integrity מובנה |
| Horizontal scaling (sharding) | Transactions יקרים יחסית |
| Document model מתאים ל-JSON APIs | Joins (lookups) איטיים |
| Atlas managed service | Consistency model מורכב |

**שאלות ריאיון קשות:**

1. מה ההבדל בין SQL ל-NoSQL? מתי כל אחד?
2. הסבר את ה-replica set. מה קורה כשה-primary נופל?
3. מה ההבדל בין embedding ל-referencing?
4. איך MongoDB transactions עובדים?
5. מה ההבדל בין `findOneAndUpdate` ל-`find` + `save`?
6. מה הבעיה עם `$inc` בלי session?

---

### 4.5 MS-SQL

**מה זה (ידע מקצועי):**
Relational Database Management System (RDBMS) של Microsoft.

**איך זה עובד מתחת למכסה המנוע:**
- טבלאות עם schema קבוע, rows ו-columns
- ACID transactions מובנה
- Query optimizer מתקדם — execution plans
- Storage: pages (8KB) → extents (64KB) → filegroups
- Stored procedures מקומפלים ורצים על השרת

**מתי משתמשים:**
- Enterprise applications
- נתונים מובנים עם relations מורכבים
- Reporting ו-analytics
- אינטגרציה עם .NET ecosystem

**Trade-offs:**
| יתרון | חיסרון |
|---|---|
| ACID מובנה ואמין | Vertical scaling (scale up, לא out) |
| Stored procedures — ביצועים | עלות רישיון גבוהה |
| Query optimizer מתקדם | Schema changes = migrations |
| Referential integrity | פחות גמיש מ-NoSQL |

**שאלות ריאיון קשות:**

1. מה ההבדל בין clustered ל-non-clustered index?
2. מה זה execution plan? איך קוראים אותו?
3. מה ההבדל בין stored procedure ל-function?
4. מה זה deadlock? איך מונעים?
5. מה עשית ב-query optimization ב-C-Vision?

---

### 4.6 Docker

**מה זה (ידע מקצועי):**
פלטפורמת containerization שמאפשרת לארוז אפליקציה + dependencies ל-container מבודד.

**איך זה עובד מתחת למכסה המנוע:**
- לא VM — containers חולקים את ה-kernel של ה-host
- Linux namespaces — בידוד process, network, filesystem
- cgroups — הגבלת משאבים (CPU, memory)
- Union filesystem (OverlayFS) — layers, כל layer immutable
- Image = blueprint, Container = running instance

**מתי משתמשים:**
- Consistent development environments
- Microservices deployment
- CI/CD pipelines
- בידוד אפליקציות

**Trade-offs:**
| יתרון | חיסרון |
|---|---|
| "Works on my machine" נפתר | Overhead (קטן אבל קיים) |
| Reproducible environments | Learning curve |
| קל לסקלר עם orchestration | Security — אם ה-kernel פגיע, כולם פגיעים |
| Ecosystem (Docker Hub) | State management (volumes) דורש תכנון |

**שאלות ריאיון קשות:**

1. מה ההבדל בין container ל-VM?
2. הסבר Docker layers — למה הסדר ב-Dockerfile חשוב?
3. מה ההבדל בין COPY ל-ADD?
4. למה multi-stage build?
5. מה Docker Compose? מה ההבדל מ-Kubernetes?
6. מה קורה כש-container מת? איפה ה-data?

---

### 4.7 JWT (JSON Web Token)

**מה זה (ידע מקצועי):**
תקן פתוח (RFC 7519) להעברת claims בין צדדים כ-JSON object חתום.

**איך זה עובד מתחת למכסה המנוע:**
- מורכב מ-3 חלקים: Header.Payload.Signature
- Header: algorithm + type (e.g., HS256)
- Payload: claims — id, email, exp, iat
- Signature: HMAC-SHA256(header + payload, secret)
- ה-server יוצר את הטוקן, הלקוח שולח אותו בכל request
- ה-server מוודא את החתימה — stateless authentication

**מתי משתמשים:**
- Authentication ב-APIs
- SSO (Single Sign-On)
- Information exchange בין services

**Trade-offs:**
| יתרון | חיסרון |
|---|---|
| Stateless — אין צורך ב-session store | אי אפשר לבטל טוקן ספציפי (בלי blacklist) |
| Self-contained — מכיל את כל המידע | Payload לא מוצפן — כל אחד יכול לקרוא |
| מתאים ל-microservices | גודל — גדול יותר מ-session ID |
| Cross-domain | אם ה-secret נחשף, כל הטוקנים פגיעים |

**שאלות ריאיון קשות:**

1. מה ההבדל בין JWT ל-session?
2. מה קורה אם מישהו שינה את ה-payload?
3. מה ההבדל בין HS256 ל-RS256?
4. למה JWT ב-httpOnly cookie ולא ב-localStorage?
5. איך מטפלים ב-token refresh?
6. מה קורה כשמשתמש עושה logout? הטוקן עדיין תקף?

---

### 4.8 WebSockets

**מה זה (ידע מקצועי):**
פרוטוקול תקשורת דו-כיווני (full-duplex) על חיבור TCP יחיד, מאפשר push מהשרת ללקוח.

**איך זה עובד מתחת למכסה המנוע:**
- מתחיל כ-HTTP request עם `Upgrade: websocket` header
- Server מחזיר `101 Switching Protocols`
- מכאן — חיבור TCP פתוח, שני הצדדים יכולים לשלוח בכל רגע
- Frame-based — כל הודעה נשלחת כ-frame עם opcode, length, ו-payload
- Ping/Pong frames לבדיקת חיבור

**מתי משתמשים:**
- Chat applications
- Real-time notifications
- Live data feeds (stocks, sports)
- Online games
- Collaborative editing

**Trade-offs:**
| יתרון | חיסרון |
|---|---|
| Low latency — אין HTTP overhead | Stateful — קשה לסקלר |
| Bi-directional | Load balancing מורכב יותר |
| Server push | ניהול connections בזיכרון |
| Efficient — אין header על כל הודעה | Debugging קשה יותר מ-HTTP |

**שאלות ריאיון קשות:**

1. מה ההבדל בין WebSocket ל-HTTP long-polling?
2. מה ההבדל בין WebSocket ל-Server-Sent Events (SSE)?
3. איך מסקלרים WebSocket connections?
4. מה קורה כשהחיבור נופל?
5. למה Socket.IO ולא raw WebSocket?

---

### 4.9 REST API

**מה זה (ידע מקצועי):**
Architectural style לבניית APIs על גבי HTTP, מבוסס על Resources ו-HTTP methods.

**איך זה עובד מתחת למכסה המנוע:**
- Resource-oriented: כל URL מייצג resource (`/users`, `/transactions`)
- HTTP methods מייצגים פעולות: GET (read), POST (create), PUT (replace), PATCH (update), DELETE
- Stateless: כל request מכיל את כל המידע הנדרש
- Status codes מייצגים תוצאות: 200, 201, 400, 401, 404, 500

**מתי משתמשים:**
- כמעט כל web API
- CRUD operations
- Public APIs
- Microservices communication

**Trade-offs:**
| יתרון | חיסרון |
|---|---|
| פשוט ומובן | Over-fetching / under-fetching |
| Cacheable (GET) | Multiple requests לנתונים מורכבים |
| Stateless | אין contract ברור (לעומת GraphQL/gRPC) |
| Tool support ענק | Versioning מורכב |

**שאלות ריאיון קשות:**

1. מה ההבדל בין PUT ל-PATCH?
2. מה ההבדל בין REST ל-GraphQL? מתי כל אחד?
3. איך עושים versioning ל-API?
4. מה ההבדל בין 401 ל-403?
5. מה זה idempotency? איזה HTTP methods הם idempotent?

---

### 4.10 Socket.IO

**מה זה (ידע מקצועי):**
ספרייה שמשלבת WebSocket עם fallbacks (HTTP long-polling), event-based API, ותמיכה ב-reconnection.

**איך זה עובד מתחת למכסה המנוע:**
- מנסה WebSocket קודם, נופל ל-long-polling אם לא זמין
- שכבת Engine.IO — מטפלת בתחבורה
- שכבת Socket.IO — מוסיפה namespaces, rooms, events, acknowledgements
- Heartbeat mechanism — בדיקת חיבור תקופתית

**Trade-offs:**
| יתרון | חיסרון |
|---|---|
| Auto-reconnection | לא תקן — רק Socket.IO client עובד |
| Namespaces ו-Rooms | Overhead על WebSocket רגיל |
| Fallback ל-polling | Debugging: שני פרוטוקולים |
| Event-based API נוח | הגדלת bundle size בצד הלקוח |

---

### 4.11 OpenAI API

**מה זה (ידע מקצועי):**
API לגישה למודלי שפה של OpenAI (GPT-4, GPT-4o-mini) — text generation, function calling, embeddings.

**איך זה עובד מתחת למכסה המנוע:**
- HTTP API שמקבל messages array (system, user, assistant, tool)
- Function calling: ה-model מחליט אילו כלים להפעיל ומחזיר JSON args
- Temperature שולט ברנדומליות: 0 = deterministic, 1 = creative
- Token-based pricing — input + output tokens

**מתי משתמשים:**
- Chatbots
- Content generation
- Data analysis
- Agentic applications (function calling)

**Trade-offs:**
| יתרון | חיסרון |
|---|---|
| יכולות NLP חזקות | עלות per-request |
| Function calling — agentic | Latency (1–10 שניות) |
| קל לאינטגרציה | אין שליטה מלאה — "hallucinations" |
| מתעדכן כל הזמן | תלות בספק חיצוני |

**שאלות ריאיון קשות:**

1. מה ההבדל בין function calling ל-regular prompting?
2. מה זה prompt injection? איך מתגוננים?
3. מה ה-tradeoff של temperature?
4. איך שולטים בעלויות?
5. מה קורה אם ה-API לא זמין? מה ה-fallback?

---

### 4.12 C

**מה זה (ידע מקצועי):**
שפת תכנות low-level, procedural, שמספקת גישה ישירה לזיכרון.

**איך זה עובד מתחת למכסה המנוע:**
- Compiled language — מתורגם ל-machine code
- Manual memory management — `malloc`/`free`
- Pointers — גישה ישירה לכתובות בזיכרון
- Stack ו-Heap — שני אזורי זיכרון שונים
- No garbage collection — האחריות על המתכנת

**מתי משתמשים:**
- Operating systems (Linux kernel)
- Embedded systems
- Performance-critical applications
- System programming

**שאלות ריאיון קשות:**

1. מה ההבדל בין Stack ל-Heap?
2. מה זה memory leak? איך מאתרים (valgrind)?
3. מה זה segmentation fault?
4. מה ההבדל בין `malloc` ל-`calloc`?
5. מה זה dangling pointer?
6. מה זה buffer overflow ולמה זה מסוכן?

---

### 4.13 C#

**מה זה (ידע מקצועי):**
שפת תכנות מונחית עצמים של Microsoft, רצה על .NET runtime.

**איך זה עובד מתחת למכסה המנוע:**
- Managed language — CLR (Common Language Runtime) מנהל זיכרון
- Garbage Collector אוטומטי — generations (Gen 0, 1, 2)
- JIT compilation — IL code מתורגם ל-native בזמן ריצה
- Strong typing עם generics

**מתי משתמשים:**
- Enterprise applications
- Windows desktop apps (WinForms, WPF)
- Web (ASP.NET)
- Game development (Unity)

**שאלות ריאיון קשות:**

1. מה ההבדל בין value type ל-reference type?
2. מה זה Garbage Collector? מה Generations?
3. מה ההבדל בין `abstract` ל-`interface`?
4. מה זה `async/await` ב-C#?
5. מה ההבדל בין `IEnumerable` ל-`IQueryable`?

---

### 4.14 ASP.NET

**מה זה (ידע מקצועי):**
Framework של Microsoft לבניית web applications ו-APIs על גבי .NET.

**איך זה עובד מתחת למכסה המנוע:**
- Request pipeline עם middleware (דומה ל-Express)
- MVC pattern: Model–View–Controller
- Web API: RESTful endpoints עם attribute routing
- Dependency Injection מובנה
- Kestrel web server

**מתי משתמשים:**
- Enterprise web applications
- REST APIs בסביבת .NET
- Microservices בסביבת Windows/Azure

---

### 4.15 WinForms

**מה זה (ידע מקצועי):**
Framework ל-Windows desktop applications עם GUI, חלק מ-.NET.

**איך זה עובד מתחת למכסה המנוע:**
- Event-driven programming — buttons, forms, events
- Windows message pump — כל interaction היא Windows message
- Single UI thread — כל עדכון UI חייב לקרות ב-main thread
- Designer — drag-and-drop ב-Visual Studio

**למה נבחר ב-C-Vision (ידע מקצועי):**
- אופרטורים ברצפת ייצור צריכים ממשק פשוט ויציב
- לא תלוי בדפדפן או באינטרנט
- אינטגרציה ישירה עם Windows ו-.NET libraries

---

### 4.16 SCADA

**מה זה (ידע מקצועי):**
Supervisory Control and Data Acquisition — מערכת לניטור ובקרה של תהליכים תעשייתיים.

**איך זה עובד מתחת למכסה המנוע:**
- שכבת שטח: חיישנים ו-PLCs אוספים נתונים
- שכבת תקשורת: פרוטוקולים תעשייתיים (OPC, Modbus)
- שכבת HMI: ממשק גרפי למפעיל — דשבורדים, alarming
- שכבת נתונים: Historian לשמירת נתונים היסטוריים

**מתי משתמשים:**
- ייצור תעשייתי
- תשתיות מים/חשמל
- בניינים חכמים
- כל מערכת בקרה תעשייתית

---

### 4.17 PLC (Programmable Logic Controller)

**מה זה (ידע מקצועי):**
מחשב תעשייתי שמיועד לבקרת תהליכים — מקבל inputs מחיישנים ומפעיל outputs (מנועים, שסתומים וכו').

**איך זה עובד מתחת למכסה המנוע:**
- Scan cycle: Read Inputs → Execute Program → Write Outputs → חזרה
- Structured Text (STL) — שפת תכנות דומה ל-Pascal, סטנדרט IEC 61131-3
- Deterministic execution — זמן ריצה קבוע ומובטח
- Redundancy — יכול לעבוד בלי מערכת ה-SCADA

**שאלות ריאיון אפשריות:**

1. מה ההבדל בין SCADA ל-PLC?
2. מה קורה אם ה-SCADA קורס? ה-PLC ממשיך?
3. מה Structured Text?
4. מה ה-scan cycle של PLC?

**בדיקת דיוק לסעיף 4:**
- האם רמת העומק מתאימה לתפקידים שאתה מתמיין אליהם?
- האם יש טכנולוגיה שאתה מרגיש שצריך הסבר עמוק יותר?

---

## SECTION 5 — יסודות מדעי המחשב

---

### 5.1 Array

**מה זה:** מבנה נתונים רציף בזיכרון שמאחסן אלמנטים מאותו סוג.

**איך זה נשמר בזיכרון:** בלוק רציף — כל אלמנט בגודל קבוע, גישה ישירה לפי index (base_address + index * element_size).

**סיבוכיות זמן:**
| פעולה | סיבוכיות |
|---|---|
| Access by index | O(1) |
| Search | O(n) |
| Insert at end | O(1) amortized (dynamic array) |
| Insert at position | O(n) |
| Delete | O(n) |

**שימושים בעולם האמיתי:** buffers, lookup tables, matrix operations, בסיס ל-Stack ו-Queue.

---

### 5.2 HashMap (Hash Table)

**מה זה:** מבנה נתונים שממפה keys ל-values באמצעות hash function.

**איך זה נשמר בזיכרון:** מערך של "דליים" (buckets). כל key עובר hash function שמחזיר index. Collisions נפתרים ב-chaining (linked list) או open addressing.

**סיבוכיות זמן:**
| פעולה | Average | Worst |
|---|---|---|
| Insert | O(1) | O(n) |
| Search | O(1) | O(n) |
| Delete | O(1) | O(n) |

**שימושים בעולם האמיתי:** caching, database indexing, counting frequencies, object storage ב-JavaScript.

---

### 5.3 Stack

**מה זה:** מבנה נתונים LIFO (Last In, First Out).

**איך זה נשמר בזיכרון:** מערך דינמי או linked list. ב-call stack — חלק מזיכרון ה-process.

**סיבוכיות זמן:**
| פעולה | סיבוכיות |
|---|---|
| Push | O(1) |
| Pop | O(1) |
| Peek | O(1) |

**שימושים בעולם האמיתי:** function call stack, undo operations, parsing (brackets matching), DFS.

---

### 5.4 Queue

**מה זה:** מבנה נתונים FIFO (First In, First Out).

**איך זה נשמר בזיכרון:** מערך מעגלי (circular buffer) או linked list.

**סיבוכיות זמן:**
| פעולה | סיבוכיות |
|---|---|
| Enqueue | O(1) |
| Dequeue | O(1) |
| Peek | O(1) |

**שימושים בעולם האמיתי:** message queues (RabbitMQ, Kafka), BFS, task scheduling, print queue.

---

### 5.5 Heap (Priority Queue)

**מה זה:** עץ בינארי שלם שמקיים heap property — בכל צומת, הערך גדול/קטן מהילדים.

**איך זה נשמר בזיכרון:** מערך — ילד שמאלי ב-`2i+1`, ילד ימני ב-`2i+2`, הורה ב-`(i-1)/2`.

**סיבוכיות זמן:**
| פעולה | סיבוכיות |
|---|---|
| Insert | O(log n) |
| Extract min/max | O(log n) |
| Peek | O(1) |
| Build heap | O(n) |

**שימושים בעולם האמיתי:** priority queues, scheduling algorithms, Dijkstra's algorithm, top-K problems.

---

### 5.6 Binary Tree

**מה זה:** מבנה נתונים הירארכי שבו לכל צומת יש לכל היותר 2 ילדים.

**איך זה נשמר בזיכרון:** nodes עם pointers (left, right, optionally parent). יכול להיות גם במערך (כמו heap).

**סיבוכיות זמן (BST):**
| פעולה | Average | Worst (unbalanced) |
|---|---|---|
| Search | O(log n) | O(n) |
| Insert | O(log n) | O(n) |
| Delete | O(log n) | O(n) |

**שימושים בעולם האמיתי:** BST לחיפוש מהיר, B-tree ל-database indexing, DOM tree בדפדפן.

---

### 5.7 Trie (Prefix Tree)

**מה זה:** עץ שבו כל edge מייצג תו. משמש לחיפוש מילים לפי prefix.

**איך זה נשמר בזיכרון:** כל node מכיל מערך/map של children (אחד לכל תו אפשרי) ו-flag "end of word".

**סיבוכיות זמן:**
| פעולה | סיבוכיות |
|---|---|
| Insert | O(m) — m = אורך המילה |
| Search | O(m) |
| Prefix search | O(m) |

**שימושים בעולם האמיתי:** autocomplete, spell check, IP routing tables, dictionary.

---

### 5.8 Union Find (Disjoint Set)

**מה זה:** מבנה נתונים ששומר קבוצות זרות ותומך בשתי פעולות: Find (מצא את הקבוצה) ו-Union (מאחד שתי קבוצות).

**איך זה נשמר בזיכרון:** מערך parent — כל אלמנט מצביע על ה-parent שלו. Root מצביע על עצמו.

**אופטימיזציות:**
- **Path compression:** כל Find מכוון ישירות ל-root
- **Union by rank:** מחבר עץ קטן לגדול

**סיבוכיות זמן:** O(alpha(n)) — כמעט O(1) עם שתי האופטימיזציות.

**שימושים בעולם האמיתי:** Kruskal's MST, cycle detection בגרפים, connected components, network connectivity.

---

### 5.9 Graph

**מה זה:** מבנה נתונים שמורכב מצמתים (vertices) וקשתות (edges).

**איך זה נשמר בזיכרון:**
- **Adjacency Matrix:** מטריצה n×n — O(n^2) מקום, O(1) בדיקת קשת
- **Adjacency List:** מערך של רשימות — O(V+E) מקום, O(degree) בדיקת קשת

**אלגוריתמים חשובים:**
- BFS — O(V+E) — shortest path בגרף לא משוקלל
- DFS — O(V+E) — cycle detection, topological sort
- Dijkstra — O((V+E) log V) — shortest path בגרף משוקלל
- Bellman-Ford — O(V*E) — negative weights

**שימושים בעולם האמיתי:** social networks, routing (GPS), dependency resolution, web crawling.

**בדיקת דיוק לסעיף 5:**
- האם אתה מרגיש בטוח עם כל מבנה הנתונים?
- על מה צריך לתרגל יותר?

---

## SECTION 6 — SYSTEM DESIGN & CONCURRENCY

---

### 6.1 Design: Monitoring System (רלוונטי לניסיון ב-C-Vision)

**שאלה:** "תעצב מערכת ניטור תעשייתית לעשרות מכונות"

**מתווה תשובה:**
- **Data collection:** Agents על כל מכונה שולחים metrics (CPU, temperature, status) כל X שניות
- **Message queue:** Kafka/RabbitMQ — buffer בין collectors לעיבוד
- **Processing:** Stream processing (aggregations, alerting rules)
- **Storage:** Time-series DB (InfluxDB/TimescaleDB) לנתונים היסטוריים
- **Alerting:** Rules engine שולח notifications (email, SMS, Slack)
- **Dashboard:** Real-time visualization (Grafana/custom)

---

### 6.2 Scaling WebSocket Systems

**שאלה:** "איך מסקלרים chat system עם WebSockets?"

**מתווה תשובה:**
- **בעיה:** WebSocket connections הם stateful — כל connection קשור לשרת ספציפי
- **פתרון 1:** Sticky sessions — load balancer מנתב את אותו user לאותו שרת
- **פתרון 2:** Redis Pub/Sub — כל שרת subscriber, הודעות עוברות דרך Redis
- **פתרון 3:** Socket.IO Redis adapter — מובנה, שקוף לקוד
- **Horizontal scaling:** מספר instances מאחורי load balancer
- **Connection limits:** כל process מוגבל ב-file descriptors — צריך tuning

---

### 6.3 Database Optimization

**שאלה:** "ה-DB איטי, מה עושים?"

**מתווה תשובה:**
1. **Identify:** slow query log, explain/execution plan
2. **Indexing:** compound indexes לשאילתות נפוצות
3. **Query optimization:** הימנע מ-N+1, השתמש ב-projection
4. **Caching:** Redis לשאילתות תכופות
5. **Connection pooling:** הגבלת connections פתוחים
6. **Read replicas:** הפרדת reads מ-writes
7. **Partitioning/Sharding:** לטבלאות גדולות

---

### 6.4 Concurrency Issues

**Threads vs Processes:**
- **Thread:** חולק memory space עם threads אחרים באותו process. קל ליצור, מסוכן (shared state)
- **Process:** memory space נפרד. מבודד, כבד יותר ליצור. תקשורת דרך IPC

**Race Condition:**
- שני threads/processes ניגשים למשאב משותף ותוצאה תלויה בסדר ביצוע
- דוגמה מהפרויקט: שני transfers מקבילים מאותו חשבון → ה-`{balance >= amount}` guard מונע

**Mutex / Semaphore:**
- **Mutex:** רק thread אחד יכול להחזיק — mutual exclusion. כמו מפתח לשירותים
- **Semaphore:** מונה — מאפשר N threads בו-זמנית. כמו חניון עם N מקומות
- **ההבדל:** Mutex = binary semaphore + ownership (רק מי שנעל יכול לפתוח)

**Deadlock:**
- שני threads ממתינים אחד לשני — שניהם תקועים
- **מניעה:** סדר נעילה קבוע, timeout, trylock

---

### 6.5 Microservices vs Monolith

**שאלה:** "מתי microservices ומתי monolith?"

**מתווה תשובה:**

| Monolith | Microservices |
|---|---|
| קוד אחד, deploy אחד | services נפרדים, deploys נפרדים |
| פשוט להתחיל | מורכב — networking, service discovery |
| קל לדבג | fault isolation |
| scaling של כל המערכת | scaling סלקטיבי |
| מתאים לצוותים קטנים | מתאים לצוותים גדולים |

**Dubai-Bank הוא monolith — ולמה זה OK (ידע מקצועי):**
- פרויקט של מפתח יחיד — microservices = over-engineering
- אפשר להפריד בעתיד: auth service, transaction service, chatbot service

**בדיקת דיוק לסעיף 6:**
- האם יש נושאי system design נוספים שרלוונטיים לתפקידים שאתה מתמיין?

---

## SECTION 7 — DEBUGGING & ENGINEERING THINKING

---

### 7.1 Debugging Strategy

**כשנשאלים "איך אתה ניגש לבאג?" — מתווה תשובה:**

1. **Reproduce:** תעתק את הבאג בצורה עקבית
2. **Isolate:** צמצם את האזור — binary search (מחצית הקוד עובד? בדוק את המחצית השנייה)
3. **Understand:** קרא logs, בדוק stack trace, הבן את ה-data flow
4. **Hypothesize:** צור השערה ובדוק אותה
5. **Fix:** תקן את root cause, לא את הסימפטום
6. **Verify:** וודא שהתיקון עובד ולא שבר משהו אחר
7. **Prevent:** הוסף test שתופס את הבאג

**דוגמה מהפרויקט (ידע מקצועי):**
- בעיה: race condition ב-transfers מקבילים
- ניתוח: שני requests מקבילים קוראים את אותה יתרה, שניהם חושבים שיש מספיק
- פתרון: MongoDB session + `{balance >= amount}` atomic guard

---

### 7.2 Production Failures

**מתווה תשובה:**

1. **Triage:** מה חומרת הבעיה? כמה משתמשים מושפעים?
2. **Mitigate:** פתרון מיידי — rollback, feature flag off, scale up
3. **Investigate:** logs, metrics, traces
4. **Root cause:** מצא את הסיבה האמיתית
5. **Post-mortem:** מה קרה, למה, ואיך מונעים בעתיד

---

### 7.3 Performance Problems

**מתווה תשובה:**

1. **Measure:** לא לנחש — profiler, metrics, load testing
2. **Identify bottleneck:** CPU? Memory? I/O? Network? DB?
3. **Common fixes:**
   - DB: indexes, query optimization, caching
   - API: response compression, pagination
   - Frontend: lazy loading, code splitting, memoization
   - Server: connection pooling, horizontal scaling

---

### 7.4 Monitoring & Observability

**שלושת עמודי התווך:**

- **Logs:** Winston בפרויקט. Structured logging (JSON). מה קרה ומתי
- **Metrics:** response times, error rates, throughput. כמה טוב המערכת עובדת
- **Traces:** מעקב אחרי request דרך כל השכבות. distributed tracing (Jaeger/Zipkin)

**בפרויקט Dubai-Bank (מהקורות חיים + ידע מקצועי):**
- Winston logging מובנה
- Health check endpoint (`/health`)
- Graceful shutdown

[צריך להשלים מידע] — האם הוספת monitoring/alerting? אם לא, צריך להכין תשובה מה היית מוסיף.

**בדיקת דיוק לסעיף 7:**
- האם יש דוגמאות debugging אמיתיות מ-C-Vision שאפשר להשתמש בהן?

---

## SECTION 8 — BEHAVIORAL INTERVIEW

---

### 8.1 ספר על עצמך

**מתווה תשובה (30-60 שניות):**

> אני טל, בוגר הנדסת תעשייה וניהול עם התמחות במערכות מידע.
> עבדתי כ-Backend Developer ב-C-Vision, שם פיתחתי מערכת MES תעשייתית עבור Vishay — אינטגרציה בין SCADA, PLC, ו-ASP.NET.
> במקביל, עברתי הכשרה אינטנסיבית ב-Infinity Labs בתכנות מערכות ו-Full-Stack.
> בניתי פרויקט בנקאות מלא — React, Node.js, MongoDB — כולל ACID transactions, צ'אטבוט AI עם function calling, ו-WebSocket.
> אני מחפש תפקיד פיתוח שבו אוכל לשלב את הרקע שלי ב-system programming עם Full-Stack development.

---

### 8.2 הפרויקט הכי מורכב שעבדת עליו

**אפשרות 1 — Dubai-Bank:**

> בניתי אפליקציית בנקאות Full-Stack מאפס.
> האתגר המרכזי היה לממש העברות כסף ACID-compliant — לוודא שכסף לא "נעלם" במקרה של שגיאה.
> השתמשתי ב-MongoDB sessions עם guard ברמת ה-DB למניעת overdraft.
> בנוסף, בניתי צ'אטבוט AI שיכול לבצע פעולות בנקאיות אמיתיות — function calling עם OpenAI — כולל שלב אישור מובנה לפני transfers.

**אפשרות 2 — C-Vision:**

> הובלתי פיתוח מערכת ניטור תעשייתית שתמכה בהעברת ייצור קבלים לישראל.
> [צריך להשלים מידע] — מה בדיוק האתגר הכי גדול? מה הפתרון? מה התוצאה?

---

### 8.3 אתגר טכני שפתרת

**מתווה STAR:**

- **Situation:** [תאר את ההקשר]
- **Task:** [מה היה הצורך]
- **Action:** [מה עשית — טכנית]
- **Result:** [מה התוצאה — מדידה אם אפשר]

**דוגמה — ACID transfers:**

> **S:** בניתי מערכת העברות כסף, וגיליתי שתחת עומס, שני transfers מקבילים יכולים לחרוג מהיתרה.
> **T:** צריך לוודא atomicity — או שכל ההעברה מצליחה, או שכולה נכשלת.
> **A:** מימשתי MongoDB sessions עם `{balance >= amount}` guard אטומי ב-`findOneAndUpdate`.
> **R:** אפס מקרים של overdraft, גם תחת concurrent requests.

---

### 8.4 איך אתה לומד טכנולוגיות חדשות

**מתווה תשובה:**

> אני מתחיל מהתיעוד הרשמי — מבין את ה-concepts מלמעלה.
> אחרי זה בונה פרויקט קטן — הדרך הטובה ביותר להבין זה לעשות.
> כשנתקע, אני קורא source code ומנסה להבין מה קורה מתחת למכסה המנוע.
> דוגמה: כשלמדתי OpenAI function calling, בניתי קודם צ'אטבוט פשוט, ואז הרחבתי ל-agentic loop עם כלים אמיתיים.

---

### 8.5 איך אתה ניגש לבאג מורכב

**מתווה תשובה:**

> **שלב 1:** תעתוק — לוודא שאני יכול לשחזר את הבעיה באופן עקבי.
> **שלב 2:** בידוד — לצמצם לאן בקוד הבעיה. Logs, breakpoints, binary search.
> **שלב 3:** השערה ובדיקה — לנסח תיאוריה ולבדוק אותה.
> **שלב 4:** תיקון root cause — לא patch, אלא תיקון אמיתי.
> **שלב 5:** test — לכתוב test שתופס את הבאג כדי שלא יחזור.

**בדיקת דיוק לסעיף 8:**
- האם התשובות הביהוויוראליות משקפות נכון את הניסיון שלך?
- האם יש סיפורים אמיתיים נוספים מ-C-Vision שכדאי להכין?

---

## SECTION 9 — KNOWLEDGE GAPS

---

### נושאים שכדאי להעמיק

| נושא | למה מראיינים ישאלו | עדיפות |
|---|---|---|
| **CI/CD Pipelines** | מופיע ב-CV כ-skill, אבל אין CI/CD מימושי בפרויקט. מראיין ישאל "תתאר pipeline שבנית" | גבוהה |
| **Testing Strategy** | יש tests אבל אין E2E. שאלה צפויה: "מה test pyramid שלך?" | גבוהה |
| **System Design at Scale** | הפרויקט הוא single-server. שאלות על scaling צפויות | גבוהה |
| **Refresh Token / OAuth** | אין refresh token. מראיין ישאל "מה קורה אחרי שעה?" | בינונית |
| **CSRF Protection** | SameSite cookie חלקי. שאלה צפויה על CSRF | בינונית |
| **Message Queues** | לא מופיע בפרויקט. רלוונטי ל-system design | בינונית |
| **Caching (Redis)** | לא מופיע בפרויקט. שאלה צפויה | בינונית |
| **Kubernetes** | Docker מופיע אבל לא K8s. שאלה צפויה: "מה ההבדל?" | נמוכה |
| **SQL Advanced** | Stored procedures מופיע ב-CV. שאלות על joins, window functions | בינונית |
| **Design Patterns — עומק** | SOLID ו-patterns מופיעים. שאלות ספציפיות צפויות | בינונית |
| **Prompt Injection** | AI בפרויקט. מראיין security ישאל | בינונית |
| **TypeScript Advanced** | Generics, utility types, conditional types | בינונית |

**בדיקת דיוק לסעיף 9:**
- האם יש נושאים נוספים שאתה מרגיש שחסרים?
- על מה אתה הכי פחות בטוח?

---

## SECTION 10 — RAPID INTERVIEW CHEAT SHEET

> **תקרא את זה 10 דקות לפני הראיון**

---

### Architecture — Dubai-Bank

- Three-Tier: React SPA → Express API + Socket.IO → MongoDB
- Controller → Service → Model separation
- JWT ב-httpOnly cookie, bcrypt 10 rounds
- ACID transfers עם MongoDB sessions + `{balance >= amount}` guard
- AI chatbot: OpenAI function calling, 4 tools, עד 5 rounds, confirmation step
- Socket.IO `/chat` namespace, per-user rate limiting (10/60s)

### Core Technologies

- **React:** Virtual DOM, reconciliation, hooks, component-based
- **Node.js:** Event loop, non-blocking I/O, single-threaded, V8, libuv
- **Express:** Middleware chain, `(req, res, next)`, Express 5 async errors
- **MongoDB:** Documents (BSON), replica set, WiredTiger, sessions for ACID
- **JWT:** Header.Payload.Signature, stateless, httpOnly, no refresh token
- **WebSocket/Socket.IO:** Full-duplex, upgrade from HTTP, namespaces, auto-reconnect
- **Docker:** Containers (not VMs), namespaces, cgroups, multi-stage builds

### System Design Answers — Key Points

- **Scale WebSocket:** Redis adapter, sticky sessions
- **Scale DB:** Indexes, caching (Redis), read replicas, sharding
- **Scale API:** Horizontal scaling, load balancer, rate limiting
- **High Availability:** Replica sets, health checks, graceful shutdown

### Common Interview Traps

| טראפ | תשובה מוכנה |
|---|---|
| "למה MongoDB לבנק? SQL עדיף" | "בפרויקט demo — MongoDB Atlas + sessions = ACID. בייצור אמיתי — SQL עדיף ל-referential integrity" |
| "אין refresh token" | "נכון, JWT פג אחרי שעה. הייתי מוסיף refresh token ב-httpOnly cookie נפרד + rotation" |
| "מה אם OpenAI לא זמין?" | "כרגע אין fallback. הייתי מוסיף circuit breaker pattern + הודעת שגיאה ידידותית" |
| "איך מונע prompt injection?" | "System prompt חזק, input sanitization, confirmation step לפני transfers. הייתי מוסיף output validation" |
| "אין CI/CD" | "Docker מאפשר reproducible builds. הייתי מוסיף GitHub Actions: lint → test → build → deploy" |
| "In-memory rate limiting לא מסקלר" | "נכון, עובד ל-single instance. ב-production הייתי עובר ל-Redis-based rate limiting" |
| "מה חסר בפרויקט?" | להכיר את ה-TODOs: forgot password, E2E tests, CI/CD, Redis caching, CSRF token |

### STAR Stories — מוכנות

1. **ACID Transfers:** Race condition → MongoDB sessions + atomic guard → zero overdrafts
2. **C-Vision MES:** [צריך להשלים] — אתגר ספציפי, פתרון, תוצאה
3. **AI Chatbot Evolution:** Intent-based → function calling → agentic loop — מראה growth

---

> **הערה חשובה:** כל סעיף שמסומן [צריך להשלים מידע] דורש ממך להוסיף פרטים מהניסיון האמיתי שלך. עבור על כל סעיף "בדיקת דיוק" ווודא שהתיאורים מדויקים.
