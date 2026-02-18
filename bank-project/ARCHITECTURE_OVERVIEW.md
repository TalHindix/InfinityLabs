# סקירה ארכיטקטונית - Dubai-Bank

## סקירה כללית (Overview)

### מטרת הפרויקט
Dubai-Bank היא מערכת בנקאות דיגיטלית מאובטחת המאפשרת למשתמשים לנהל את הכספים שלהם בצורה בטוחה ויעילה. המערכת מספקת פתרון מלא לניהול פיננסי אישי עם דגש על אבטחה, אמינות וחוויית משתמש מעולה.

### הבעיה המרכזית
המערכת פותרת את הצורך במערכת בנקאות דיגיטלית מאובטחת המספקת:
- **ניהול זהויות מאובטח**: הרשמה, התחברות ואימות אימייל
- **העברות כספים אמינות**: העברות בין משתמשים עם שמירה על שלמות הנתונים
- **מעקב אחר פעילות**: היסטוריית עסקאות מפורטת עם pagination
- **תמיכה חכמה**: צ'אטבוט AI המסייע למשתמשים במידע על החשבון

### תכונות עיקריות
- **אימות משתמשים**: הרשמה, התחברות, אימות אימייל באמצעות Brevo
- **העברות כספים**: העברות מאובטחות בין משתמשים עם validation מלא
- **היסטוריית עסקאות**: צפייה בעסקאות עם pagination ו-filtering
- **צ'אטבוט AI**: עוזר וירטואלי מבוסס OpenAI לשאלות על יתרה ומידע כללי
- **אבטחה מתקדמת**: JWT authentication, rate limiting, CORS protection

---

## ארכיטקטורת מערכת

### מבנה כללי (3-Tier Architecture)
המערכת בנויה בארכיטקטורה קלאסית של שלוש שכבות:

```
┌─────────────────┐
│   Client (UI)   │  React 19 + TypeScript + Material-UI
│   Port: 5173    │
└────────┬────────┘
         │ HTTP/REST + WebSocket
         │
┌────────▼────────┐
│  Server (API)   │  Node.js + Express 5
│   Port: 3000    │
└────────┬────────┘
         │ Mongoose ODM
         │
┌────────▼────────┐
│  Database (DB)  │  MongoDB
│   Port: 27017   │
└─────────────────┘
```

### Stack טכנולוגי

#### Backend
- **Node.js**: Runtime environment אסינכרוני
- **Express 5**: Web framework קל ומהיר
- **MongoDB**: NoSQL database גמיש
- **Mongoose**: ODM (Object Document Mapper) ל-MongoDB
- **JWT (jsonwebtoken)**: Authentication stateless
- **bcrypt**: הצפנת סיסמאות
- **Socket.IO**: תקשורת real-time לצ'אטבוט
- **OpenAI API**: עיבוד שפה טבעית לצ'אטבוט
- **Brevo (Nodemailer)**: שליחת אימיילים
- **Winston**: Logging מקצועי
- **express-rate-limit**: הגנה מפני brute force attacks

#### Frontend
- **React 19**: ספריית UI מודרנית
- **TypeScript**: Type safety ו-better developer experience
- **Vite**: Build tool מהיר ויעיל
- **Material-UI (MUI)**: Component library מקצועי
- **React Router**: ניהול routing
- **Axios**: HTTP client
- **Socket.IO Client**: חיבור ל-real-time events
- **Emotion**: CSS-in-JS styling

### בחירות טכנולוגיות והסבר

#### Node.js + Express
**למה נבחר:**
- **אסינכרוניות**: Node.js מתאים מצוין ל-I/O operations מרובים (DB queries, API calls)
- **אקוסיסטם עשיר**: npm מספק חבילות מוכנות לכל צורך
- **מהירות פיתוח**: Express מספק abstraction layer נוח ל-REST APIs
- **Scalability**: תמיכה טובה ב-concurrent requests

**Trade-off:** Single-threaded model דורש תשומת לב ל-blocking operations, אבל מתאים מצוין ל-I/O bound applications כמו APIs.

#### MongoDB
**למה נבחר:**
- **גמישות סכמה**: שינויים בסכמה לא דורשים migrations מורכבים
- **ביצועים**: מהיר לקריאות (read-heavy workloads)
- **Document Model**: מתאים ל-JSON-like data structures
- **Transactions**: תמיכה ב-ACID transactions (מ-MongoDB 4.0+)

**Trade-off:** פחות strict מ-SQL מבחינת ACID guarantees, אבל פתרנו זאת באמצעות MongoDB sessions לפעולות קריטיות כמו העברות כספים.

#### React + TypeScript
**למה נבחר:**
- **Type Safety**: TypeScript מונע שגיאות runtime בשלב ה-compilation
- **Developer Experience**: IntelliSense, refactoring בטוח
- **Component Architecture**: קוד מודולרי ו-reusable
- **אקוסיסטם**: ספריות רבות ותמיכה קהילתית חזקה

**Trade-off:** עלות למידה ראשונית גבוהה יותר, אבל שווה את זה לטווח הארוך במונחי maintainability.

#### JWT + HTTP-only Cookies
**למה נבחר:**
- **Stateless**: לא צריך לאחסן sessions ב-server, מה שמאפשר scalability
- **אבטחה**: HTTP-only cookies מונעים גישה ל-JavaScript (XSS protection)
- **Cross-domain**: עובד טוב עם CORS

**Trade-off:** קושי ב-revocation של tokens (פתרנו עם expiration קצר של שעה), אבל היתרונות עולים על החסרונות.

---

## רכיבים מרכזיים (Backend Architecture)

### מבנה תיקיות

```
server/
├── src/
│   ├── controllers/          # טיפול בבקשות HTTP
│   │   ├── auth.controller.js
│   │   ├── user.controller.js
│   │   └── transaction.controller.js
│   │
│   ├── services/             # לוגיקה עסקית
│   │   ├── user.service.js
│   │   ├── transaction.service.js
│   │   ├── chatbot.service.js
│   │   └── openaiIntent.service.js
│   │
│   ├── models/               # Mongoose schemas
│   │   ├── user.model.js
│   │   └── transaction.model.js
│   │
│   ├── middleware/           # Cross-cutting concerns
│   │   ├── auth.middleware.js
│   │   ├── rateLimit.middleware.js
│   │   ├── logger.middleware.js
│   │   └── socketAuth.middleware.js
│   │
│   ├── routes/               # API endpoints definition
│   │   ├── auth.routes.js
│   │   ├── user.routes.js
│   │   └── transaction.routes.js
│   │
│   ├── utils/                # כלי עזר
│   │   ├── jwt.util.js
│   │   ├── email.util.js
│   │   ├── error.util.js
│   │   ├── validation.util.js
│   │   ├── response.util.js
│   │   └── logger.util.js
│   │
│   ├── config/               # הגדרות אפליקציה
│   │   ├── index.js
│   │   └── database.js
│   │
│   ├── constants/            # קבועים גלובליים
│   │   └── index.js
│   │
│   ├── socket/               # Socket.IO handlers
│   │   └── socket.handler.js
│   │
│   ├── app.js                # Express app configuration
│   └── server.js             # Entry point
```

### Separation of Concerns

#### Controllers (`/controllers`)
**תפקיד:** נקודת הכניסה לבקשות HTTP
- **Validation ראשוני**: בדיקת קיום שדות חובה
- **קריאה ל-Services**: העברת הלוגיקה העסקית ל-services
- **עיצוב Response**: עטיפת התוצאה בפורמט אחיד
- **Error Handling**: תפיסת שגיאות והעברתן ל-error handler

**דוגמה:**
```javascript
export const createTransaction = async (req, res) => {
  try {
    const { receiverEmail, amount, description } = req.body;
    const senderEmail = req.user.email;
    
    // Validation
    if (!receiverEmail) throw new AppError('Receiver email is required', 400);
    
    // Business logic in service
    const transaction = await executeTransfer(
      senderEmail, receiverEmail, amount, description
    );
    
    // Response formatting
    return response.created(res, { transaction });
  } catch (error) {
    // Error handling
    const { statusCode, message } = mapErrorToResponse(error);
    return res.status(statusCode).json({ success: false, error: message });
  }
};
```

#### Services (`/services`)
**תפקיד:** הלוגיקה העסקית של האפליקציה
- **Business Rules**: כללי עסק (למשל: לא ניתן להעביר לעצמך)
- **Data Manipulation**: עיבוד וטרנספורמציה של נתונים
- **Database Operations**: קריאות ל-Models
- **External APIs**: אינטגרציה עם שירותים חיצוניים (OpenAI, Email)

**דוגמה:**
```javascript
export const executeTransfer = async (senderEmail, receiverEmail, amount, description) => {
  const session = await mongoose.startSession();
  session.startTransaction();
  try {
    // Business rule
    if (receiverEmail.toLowerCase() === senderEmail.toLowerCase()) {
      throw new AppError('Cannot transfer to yourself', 400);
    }
    
    // Atomic operations
    await deductSenderBalance(senderEmail, amount, session);
    await addReceiverBalance(receiverEmail, amount, session);
    const transaction = await createTransactionRecord(
      senderEmail, receiverEmail, amount, description, session
    );
    
    await session.commitTransaction();
    return transaction;
  } catch (error) {
    await session.abortTransaction();
    throw error;
  } finally {
    session.endSession();
  }
};
```

#### Models (`/models`)
**תפקיד:** הגדרת מבנה הנתונים
- **Schema Definition**: הגדרת שדות, types, validations
- **Indexes**: הגדרת indexes לביצועים
- **Virtuals/Methods**: מתודות עזר ברמת ה-model

**דוגמה:**
```javascript
const userSchema = new mongoose.Schema({
  id: { type: String, required: true, unique: true, default: () => crypto.randomUUID() },
  email: { type: String, required: true, unique: true, lowercase: true },
  password: { type: String, required: true, minlength: 8, select: false },
  balance: { type: Number, default: 500, min: 0 },
  status: { type: String, enum: Object.values(USER_STATUS), default: USER_STATUS.PENDING },
  // ...
});
```

#### Middleware (`/middleware`)
**תפקיד:** Cross-cutting concerns
- **Authentication**: `protect` middleware - אימות JWT token
- **Rate Limiting**: הגנה מפני brute force ו-DoS
- **Logging**: רישום בקשות לניפוי באגים
- **Error Handling**: טיפול מרכזי בשגיאות

**דוגמה:**
```javascript
export const protect = async (req, res, next) => {
  try {
    const token = getTokenFromRequest(req);
    if (!token) return res.status(401).json({ success: false, error: 'Authentication failed' });
    
    const decoded = verifyToken(token);
    const user = await User.findOne({ id: decoded.id });
    if (!user || user.status !== USER_STATUS.ACTIVE) {
      return res.status(401).json({ success: false, error: 'Authentication failed' });
    }
    
    req.user = user;
    next();
  } catch (error) {
    return res.status(401).json({ success: false, error: 'Authentication failed' });
  }
};
```

#### Routes (`/routes`)
**תפקיד:** מיפוי URLs ל-controllers
- **Endpoint Definition**: הגדרת paths ו-HTTP methods
- **Middleware Application**: החלת middleware על routes ספציפיים
- **Route Grouping**: ארגון routes לפי domain

**דוגמה:**
```javascript
router.use(protect); // כל ה-routes דורשים authentication

router
  .route('/')
  .get(getTransactions)
  .post(transactionLimiter, createTransaction); // Rate limiting רק על POST

router.route('/:transactionId').get(getTransactionById);
```

---

## תבניות עיצוב ועקרונות (Design Patterns & Principles)

### RESTful API Principles
המערכת עוקבת אחר עקרונות REST:

- **Resource-Based URLs**: `/api/v1/transactions`, `/api/v1/auth`
- **HTTP Methods**: GET (read), POST (create), PUT/PATCH (update), DELETE (delete)
- **Stateless**: כל request מכיל את כל המידע הנדרש
- **JSON Responses**: פורמט אחיד של responses
- **Status Codes**: שימוש נכון ב-HTTP status codes (200, 201, 400, 401, 404, 500)

**דוגמה:**
```
GET    /api/v1/transactions        - רשימת עסקאות
POST   /api/v1/transactions        - יצירת העברה
GET    /api/v1/transactions/:id    - פרטי עסקה ספציפית
POST   /api/v1/auth/login          - התחברות
POST   /api/v1/auth/signup          - הרשמה
```

### Stateless JWT Authentication

**איך זה עובד:**
1. משתמש מתחבר עם email/password
2. Server מוודא credentials ויוצר JWT token
3. Token נשלח ב-HTTP-only cookie
4. כל request עתידי כולל את ה-cookie
5. `protect` middleware מאמת את ה-token ומחלץ את ה-user

**יתרונות:**
- **Scalability**: לא צריך shared session storage
- **Security**: HTTP-only cookies מונעים XSS attacks
- **Cross-domain**: עובד עם CORS

**הגבלות:**
- **Revocation**: קשה לבטל token לפני expiration (פתרנו עם expiration קצר)
- **Size**: JWT גדול יותר מ-session ID (אבל עדיין קטן)

### Transaction Pattern (Atomic Operations)

**הבעיה:** העברת כסף דורשת שתי פעולות אטומיות:
1. הפחתת יתרה מהשולח
2. הוספת יתרה למקבל

**הפתרון:** MongoDB Sessions עם Transactions

```javascript
const session = await mongoose.startSession();
session.startTransaction();
try {
  await deductSenderBalance(senderEmail, amount, session);
  await addReceiverBalance(receiverEmail, amount, session);
  await createTransactionRecord(..., session);
  await session.commitTransaction();
} catch (error) {
  await session.abortTransaction(); // Rollback אם יש שגיאה
  throw error;
} finally {
  session.endSession();
}
```

**יתרונות:**
- **ACID Guarantees**: כל הפעולות מצליחות או כולן נכשלות
- **Data Integrity**: אין מצב של "כסף נעלם"
- **Error Recovery**: Rollback אוטומטי בשגיאה

### Rate Limiting & Security

**הגנה מפני:**
- **Brute Force Attacks**: הגבלת ניסיונות התחברות (5 ניסיונות ב-15 דקות)
- **DoS Attacks**: הגבלת מספר requests (10 העברות בדקה)
- **Email Spam**: הגבלת ניסיונות אימות אימייל (10 ניסיונות בשעה)

**יישום:**
```javascript
export const authLimiter = rateLimit({
  windowMs: 15 * 60 * 1000, // 15 דקות
  max: 5, // 5 ניסיונות
  message: { success: false, error: 'Too many authentication attempts...' }
});
```

### Error Handling Pattern

**Custom Error Class:**
```javascript
export class AppError extends Error {
  constructor(message, statusCode) {
    super(message);
    this.statusCode = statusCode;
  }
}
```

**Centralized Error Handler:**
```javascript
export const errorHandler = (err, req, res, next) => {
  const { statusCode, message } = mapErrorToResponse(err);
  res.status(statusCode).json({ success: false, error: message });
};
```

**יתרונות:**
- **Consistency**: כל השגיאות מחזירות אותו פורמט
- **Security**: לא חושפים פרטים פנימיים ב-production
- **Maintainability**: שינוי אחד משפיע על כל המערכת

### Dependency Injection (Config Pattern)

**שימוש ב-config object:**
```javascript
// config/index.js
const config = {
  port: Number(process.env.PORT) || 3000,
  mongoUri: process.env.MONGO_URI || 'mongodb://localhost:27017/Dubai-Bank',
  jwt: { secret: process.env.JWT_SECRET, expiresIn: '1h' },
  // ...
};
Object.freeze(config.jwt); // Immutability
```

**יתרונות:**
- **Testability**: קל להחליף config ב-tests
- **Environment Management**: קל לניהול environments שונים
- **Type Safety**: אפשר להוסיף TypeScript types

---

## זרימת נתונים (Data Flow - Happy Path)

### דוגמה: העברת כסף (Money Transfer)

#### 1. Client Side (React)
```typescript
// משתמש ממלא טופס העברה
const handleTransfer = async () => {
  const response = await axios.post('/api/v1/transactions', {
    receiverEmail: 'receiver@example.com',
    amount: 100,
    description: 'Payment for services'
  });
  // עדכון UI עם התוצאה
};
```

#### 2. Network Layer
- **HTTP Request**: POST request ל-`/api/v1/transactions`
- **Headers**: Cookie עם JWT token (אוטומטי)
- **Body**: JSON עם פרטי ההעברה

#### 3. Express App (`app.js`)
```javascript
// Middleware chain
app.use(cors({ origin: config.clientUrl, credentials: true }));
app.use(cookieParser());
app.use(express.json());
app.use(requestLogger);
```

#### 4. Route Matching (`transaction.routes.js`)
```javascript
router.post('/', transactionLimiter, createTransaction);
// Rate limiting: בדיקה אם המשתמש לא חרג ממגבלה
```

#### 5. Authentication Middleware (`auth.middleware.js`)
```javascript
export const protect = async (req, res, next) => {
  const token = getTokenFromRequest(req); // מקריא מ-cookie או Authorization header
  const decoded = verifyToken(token); // מאמת JWT
  req.user = await User.findOne({ id: decoded.id }); // טוען user
  next(); // ממשיך ל-controller
};
```

#### 6. Controller (`transaction.controller.js`)
```javascript
export const createTransaction = async (req, res) => {
  // Validation
  const { receiverEmail, amount, description } = req.body;
  if (!receiverEmail) throw new AppError('Receiver email is required', 400);
  
  // Amount validation
  const amountValidation = validateTransactionAmount(amount);
  if (!amountValidation.isValid) {
    throw new AppError(amountValidation.error, 400);
  }
  
  // Business logic
  const transaction = await executeTransfer(
    req.user.email, // senderEmail מהמשתמש המאומת
    receiverEmail,
    amountValidation.sanitized,
    description
  );
  
  // Response
  return response.created(res, { transaction });
};
```

#### 7. Service Layer (`transaction.service.js`)
```javascript
export const executeTransfer = async (senderEmail, receiverEmail, amount, description) => {
  // מתחיל MongoDB session
  const session = await mongoose.startSession();
  session.startTransaction();
  
  try {
    // Business rule: לא ניתן להעביר לעצמך
    if (receiverEmail.toLowerCase() === senderEmail.toLowerCase()) {
      throw new AppError('Cannot transfer to yourself', 400);
    }
    
    // Atomic operation 1: הפחתת יתרה
    const sender = await User.findOneAndUpdate(
      { email: senderEmail, balance: { $gte: amount } },
      { $inc: { balance: -amount } },
      { session, new: true }
    );
    if (!sender) throw new AppError('Insufficient funds', 400);
    
    // Atomic operation 2: הוספת יתרה
    const receiver = await User.findOneAndUpdate(
      { email: receiverEmail },
      { $inc: { balance: amount } },
      { session, new: true }
    );
    if (!receiver) throw new AppError('Receiver not found', 404);
    
    // Atomic operation 3: יצירת רשומת transaction
    const transaction = await Transaction.create([{
      id: await getNextTransactionId(session),
      fromEmail: senderEmail,
      toEmail: receiverEmail,
      amount,
      description
    }], { session });
    
    // Commit: כל הפעולות מצליחות
    await session.commitTransaction();
    return transaction[0];
    
  } catch (error) {
    // Rollback: ביטול כל הפעולות אם יש שגיאה
    await session.abortTransaction();
    throw error;
  } finally {
    session.endSession();
  }
};
```

#### 8. Database Layer (MongoDB)
- **Transaction Start**: MongoDB מתחיל transaction
- **Write Operations**: כל ה-updates מתבצעים ב-transaction
- **Commit/Rollback**: בהתאם להצלחה או כשלון

#### 9. Response Flow
```javascript
// Controller מחזיר response
return response.created(res, { transaction });

// Response utility מעצב את ה-response
export const created = (res, data) => {
  return res.status(201).json({ success: true, ...data });
};

// Client מקבל:
{
  "success": true,
  "transaction": {
    "id": 12345,
    "fromEmail": "sender@example.com",
    "toEmail": "receiver@example.com",
    "amount": 100,
    "description": "Payment for services",
    "createdAt": "2026-02-18T10:30:00.000Z"
  }
}
```

#### 10. Client Update
```typescript
// React component מעדכן את ה-state
setTransactions([newTransaction, ...transactions]);
setBalance(balance - amount);
// UI מתעדכן אוטומטית
```

### תרשים זרימה (Flow Diagram)

```
User (React UI)
    │
    ▼
Axios POST /api/v1/transactions
    │
    ▼
Express Middleware Chain
    ├─ CORS
    ├─ Cookie Parser
    ├─ JSON Parser
    └─ Request Logger
    │
    ▼
Rate Limiter (transactionLimiter)
    │
    ▼
Auth Middleware (protect)
    ├─ Extract JWT from cookie
    ├─ Verify token
    └─ Load user → req.user
    │
    ▼
Controller (createTransaction)
    ├─ Validate input
    └─ Call Service
    │
    ▼
Service (executeTransfer)
    ├─ Start MongoDB Session
    ├─ Begin Transaction
    ├─ Deduct sender balance (atomic)
    ├─ Add receiver balance (atomic)
    ├─ Create transaction record (atomic)
    └─ Commit Transaction
    │
    ▼
Database (MongoDB)
    ├─ Execute all operations atomically
    └─ Return results
    │
    ▼
Service returns transaction
    │
    ▼
Controller formats response
    │
    ▼
Response sent to client
    │
    ▼
React updates UI
```

---

## שיקולי תכנון ופשרות (Trade-offs for Interview)

### MongoDB vs SQL (PostgreSQL/MySQL)

**למה MongoDB:**
- **גמישות סכמה**: שינויים בסכמה לא דורשים migrations מורכבים
- **מהירות פיתוח**: פחות overhead ב-setup ו-maintenance
- **Document Model**: מתאים ל-JSON-like structures
- **ביצועים**: מהיר לקריאות (read-heavy workloads)

**Trade-offs:**
- **ACID Guarantees**: פחות strict מ-SQL
- **Relationships**: אין foreign keys מובנים
- **Query Language**: לא SQL (אבל Mongoose מספק API נוח)

**איך פתרנו:**
- **MongoDB Transactions**: שימוש ב-sessions לפעולות קריטיות (העברות כספים)
- **Application-Level Validation**: בדיקות integrity ברמת ה-application
- **Indexes**: הגדרת indexes לביצועים ו-uniqueness

**טיפ לראיון:**
"בחרתי MongoDB בגלל גמישות סכמה ומהירות פיתוח, עם trade-off של פחות ACID guarantees. פתרתי זאת באמצעות MongoDB sessions לפעולות קריטיות כמו העברות כספים, מה שמבטיח atomicity ו-consistency. אם הייתי צריך complex joins או strict referential integrity, הייתי בוחר SQL."

### JWT vs Server-Side Sessions

**למה JWT:**
- **Stateless**: לא צריך shared session storage (Redis/Memcached)
- **Scalability**: קל להוסיף servers ללא shared state
- **Cross-domain**: עובד טוב עם CORS ו-microservices
- **Mobile-friendly**: עובד טוב עם mobile apps

**Trade-offs:**
- **Revocation**: קשה לבטל token לפני expiration
- **Size**: JWT גדול יותר מ-session ID
- **Security**: אם token נגנב, הוא תקף עד expiration

**איך פתרנו:**
- **Short Expiration**: Tokens תקפים לשעה אחת בלבד
- **HTTP-only Cookies**: מונעים XSS attacks
- **Secure Flag**: ב-production, cookies נשלחים רק ב-HTTPS

**טיפ לראיון:**
"בחרתי JWT בגלל scalability ו-stateless architecture, עם trade-off של קושי ב-revocation. פתרתי זאת עם expiration קצר (שעה) ו-HTTP-only cookies לאבטחה. אם הייתי צריך immediate revocation (למשל logout מכל המכשירים), הייתי משתמש ב-Redis blacklist או server-side sessions."

### Service Layer Pattern

**למה Service Layer:**
- **Separation of Concerns**: Controllers מטפלים ב-HTTP, Services מטפלים בלוגיקה עסקית
- **Testability**: קל לבדוק business logic ללא HTTP layer
- **Reusability**: Services יכולים להיקרא מ-multiple controllers או background jobs
- **Maintainability**: שינוי ב-business logic לא משפיע על HTTP layer

**Trade-offs:**
- **Complexity**: עוד שכבה בקוד
- **Overhead**: עוד function calls
- **Learning Curve**: צריך להבין את ה-separation

**איך פתרנו:**
- **Clear Boundaries**: Controllers רק validate ו-format, Services רק business logic
- **Consistent Patterns**: כל service עוקב אחר אותו pattern
- **Documentation**: קוד מלווה ב-comments ברורים

**טיפ לראיון:**
"הוספתי Service Layer כדי להפריד בין HTTP concerns ל-business logic. זה מוסיף שכבה אחת, אבל משפר משמעותית את ה-testability וה-maintainability. Controllers נשארים דקים וקלים לבדיקה, ו-Services מכילים את כל הלוגיקה העסקית במקום אחד."

### HTTP-only Cookies vs LocalStorage

**למה HTTP-only Cookies:**
- **XSS Protection**: JavaScript לא יכול לגשת ל-cookies
- **Automatic**: Browser שולח cookies אוטומטית
- **Secure Flag**: אפשר לכפות HTTPS-only

**Trade-offs:**
- **CSRF Risk**: צריך CSRF protection (אבל לא רלוונטי ל-SPA עם CORS)
- **CORS Complexity**: צריך להגדיר CORS נכון
- **Size Limits**: Cookies מוגבלים ל-4KB

**איך פתרנו:**
- **CORS Configuration**: הגדרת `credentials: true` ב-client ו-server
- **SameSite Cookie**: הגדרת `sameSite: 'lax'` (או `'none'` ב-production cross-domain)
- **Secure Flag**: ב-production, cookies נשלחים רק ב-HTTPS

**טיפ לראיון:**
"בחרתי HTTP-only cookies בגלל הגנה מפני XSS attacks. JavaScript לא יכול לגשת ל-tokens, מה שמגן מפני XSS. Trade-off הוא מורכבות ב-CORS configuration, אבל פתרתי זאת עם `credentials: true` ו-`sameSite` configuration נכון."

### Error Handling: Custom Errors vs Standard HTTP

**למה Custom Error Class:**
- **Consistency**: כל השגיאות מחזירות אותו פורמט
- **Type Safety**: אפשר להוסיף TypeScript types
- **Centralized Handling**: שינוי אחד משפיע על כל המערכת
- **Security**: לא חושפים פרטים פנימיים ב-production

**Trade-offs:**
- **Overhead**: עוד class ו-logic
- **Learning Curve**: צריך להבין את ה-pattern

**איך פתרנו:**
- **AppError Class**: Custom error עם statusCode
- **Centralized Handler**: Error handler אחד מטפל בכל השגיאות
- **Error Mapping**: מפה שגיאות שונות ל-HTTP status codes נכונים

**טיפ לראיון:**
"יצרתי Custom Error Class כדי להבטיח consistency ו-security. כל השגיאות עוברות דרך אותו handler, מה שמבטיח שאנחנו לא חושפים פרטים פנימיים ב-production. זה מוסיף קצת overhead, אבל שווה את זה במונחי maintainability ו-security."

---

## סיכום

Dubai-Bank היא מערכת בנקאות דיגיטלית מאובטחת הבנויה על ארכיטקטורה מודרנית ו-scalable. המערכת משתמשת ב-best practices של תעשייה ומתמקדת באבטחה, אמינות ו-maintainability.

**נקודות מפתח:**
- **3-Tier Architecture**: הפרדה ברורה בין Client, Server ו-Database
- **Separation of Concerns**: כל שכבה אחראית על תחום אחד
- **Security First**: JWT authentication, rate limiting, HTTP-only cookies
- **Data Integrity**: MongoDB transactions לפעולות קריטיות
- **Scalability**: Stateless authentication, service layer pattern

**החלטות ארכיטקטוניות:**
- MongoDB עם transactions לפעולות קריטיות
- JWT עם HTTP-only cookies לאבטחה
- Service layer pattern ל-testability ו-maintainability
- Custom error handling ל-consistency ו-security

המערכת מוכנה ל-scale ו-maintain, עם focus על code quality ו-best practices.
