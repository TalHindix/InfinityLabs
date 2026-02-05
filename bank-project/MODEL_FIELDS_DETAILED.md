# ניתוח מפורט של כל שדה במודלים

## מודל User (`server/src/models/user.model.js`)

---

### 1. `id` (String, UUID)

**הגדרה:**
```javascript
id: {
  type: String,
  required: true,
  unique: true,
  default: () => crypto.randomUUID(),
}
```

**למה השדה הזה קיים?**
- מזהה ייחודי למשתמש, **נפרד** מ-`_id` של MongoDB
- MongoDB יוצר `_id` אוטומטית (ObjectId), אבל אנחנו רוצים מזהה נוסף שאנחנו שולטים בו

**איך הוא עוזר בפרויקט?**

1. **אבטחה ב-JWT Tokens**
   - ב-`server/src/utils/jwt.util.js`: `{ id: user.id, email: user.email }`
   - לא חושפים את ה-`_id` הפנימי של MongoDB (ObjectId)
   - אם `_id` משתנה או נחשף, זה לא משפיע על ה-JWT

2. **יציבות**
   - UUID הוא יציב ולא משתנה (לעומת `_id` שיכול להשתנות)
   - משמש לזיהוי משתמשים ב-API responses

3. **שימוש ב-Frontend**
   - ב-`client/src/types/index.ts`: `id: string` - חלק מה-User interface
   - משמש לזיהוי משתמשים בצד הלקוח

**דוגמאות שימוש:**
- `server/src/controllers/user.controller.js`: `const userId = req.user.id`
- `server/src/socket/socket.handler.js`: `const userId = socket.user.id`
- `server/src/controllers/auth.controller.js`: מחזיר `id` ב-response

---

### 2. `firstName` (String)

**הגדרה:**
```javascript
firstName: {
  type: String,
  required: [true, 'First name is required'],
  trim: true,
  minlength: [2, 'First name must be at least 2 characters'],
  maxlength: [50, 'First name cannot exceed 50 characters'],
}
```

**למה השדה הזה קיים?**
- זיהוי המשתמש בממשק - שם פרטי

**איך הוא עוזר בפרויקט?**

1. **תצוגה אישית ב-Dashboard**
   - ב-`client/src/pages/DashboardPage.tsx`: `Welcome back, {user?.firstName || 'User'}`
   - חווית משתמש אישית ומזמינה

2. **הצגה ב-API Responses**
   - ב-`server/src/controllers/auth.controller.js`: מחזיר `firstName` ב-login response
   - ב-`server/src/controllers/user.controller.js`: מחזיר `firstName` ב-user profile

3. **Validation מחמיר**
   - `trim: true` - מסיר רווחים מיותרים
   - `minlength: 2` - לפחות 2 תווים (מונע ערכים כמו "A")
   - `maxlength: 50` - מקסימום 50 תווים (מונע שמות ארוכים מדי)

**דוגמאות שימוש:**
- `client/src/components/SignupForm.tsx`: שדה קלט ל-firstName
- `server/src/services/user.service.js`: `createUser` מקבל `firstName`

---

### 3. `lastName` (String)

**הגדרה:**
```javascript
lastName: {
  type: String,
  required: [true, 'Last name is required'],
  trim: true,
  minlength: [2, 'Last name must be at least 2 characters'],
  maxlength: [50, 'Last name cannot exceed 50 characters'],
}
```

**למה השדה הזה קיים?**
- שם משפחה - השלמת זיהוי המשתמש

**איך הוא עוזר בפרויקט?**

1. **הצגה מלאה של שם המשתמש**
   - ב-`server/src/controllers/auth.controller.js`: מחזיר `lastName` ב-login response
   - ב-`server/src/controllers/user.controller.js`: מחזיר `lastName` ב-user profile

2. **Validation זהה ל-firstName**
   - אותה הגנה מפני ערכים לא תקינים

**דוגמאות שימוש:**
- `client/src/components/SignupForm.tsx`: שדה קלט ל-lastName
- `server/src/services/user.service.js`: `createUser` מקבל `lastName`

---

### 4. `email` (String, unique, lowercase)

**הגדרה:**
```javascript
email: {
  type: String,
  required: [true, 'Email is required'],
  unique: true,
  lowercase: true,
  trim: true,
  match: [/^[^\s@]+@[^\s@]+\.[^\s@]+$/, 'Please provide a valid email address'],
}
```

**למה השדה הזה קיים?**
- זיהוי ייחודי למשתמש + אימות והתחברות

**איך הוא עוזר בפרויקט?**

1. **זיהוי ייחודי**
   - `unique: true` - MongoDB מבטיח שאין שני משתמשים עם אותו email
   - משמש כ-"username" של המשתמש

2. **Authentication Flow**
   - Login: `server/src/services/user.service.js` - `findUserByEmailWithPassword`
   - Signup: `server/src/services/user.service.js` - `createUser`
   - Verification: `server/src/services/user.service.js` - `findAndVerifyUserByToken`

3. **זיהוי שולח/מקבל בהעברות**
   - ב-`server/src/services/transaction.service.js`: `fromEmail`, `toEmail` משתמשים ב-email
   - Query: `$or: [{ fromEmail: userEmail }, { toEmail: userEmail }]`

4. **Normalization**
   - `lowercase: true` - כל ה-emails נשמרים באותיות קטנות
   - `trim: true` - מסיר רווחים מיותרים
   - מונע duplicates כמו: `User@Example.com` ו-`user@example.com`

5. **Validation**
   - `match` עם regex - בודק פורמט תקין של email

**דוגמאות שימוש:**
- `server/src/middleware/auth.middleware.js`: `req.user.email`
- `server/src/controllers/transaction.controller.js`: `const userEmail = req.user.email`
- `client/src/components/LoginForm.tsx`: שדה קלט ל-email

---

### 5. `password` (String, select: false)

**הגדרה:**
```javascript
password: {
  type: String,
  required: [true, 'Password is required'],
  minlength: [8, 'Password must be at least 8 characters'],
  select: false,
}
```

**למה השדה הזה קיים?**
- אחסון hash של הסיסמה (לא הסיסמה עצמה!)

**איך הוא עוזר בפרויקט?**

1. **אבטחה - `select: false`**
   - **זה חשוב מאוד!** - הסיסמה לא נשלפת אוטומטית בכל query
   - רק כשנדרש במפורש עם `.select('+password')`
   - מונע חשיפה בטעות של הסיסמה ב-responses

2. **Hashing**
   - ב-`server/src/services/user.service.js`: `bcrypt.hash(password, SALT_ROUNDS)`
   - הסיסמה נשמרת כ-hash, לא כטקסט פשוט
   - SALT_ROUNDS = 10 (מהקונסטנטים)

3. **Validation**
   - `minlength: 8` - לפחות 8 תווים
   - Frontend יכול להוסיף validation נוסף (uppercase, lowercase, number)

4. **Verification**
   - ב-`server/src/services/user.service.js`: `validatePassword` - `bcrypt.compare`
   - ב-`server/src/controllers/auth.controller.js`: בודק סיסמה ב-login

**דוגמאות שימוש:**
- `server/src/services/user.service.js`: `findUserByEmailWithPassword` - `.select('+password')`
- `server/src/services/user.service.js`: `createUser` - יוצר hash לפני שמירה
- `client/src/components/SignupForm.tsx`: שדה קלט ל-password

---

### 6. `phone` (String)

**הגדרה:**
```javascript
phone: {
  type: String,
  required: [true, 'Phone number is required'],
  trim: true,
  match: [/^\+?[1-9]\d{7,14}$/, 'Please provide a valid phone number'],
}
```

**למה השדה הזה קיים?**
- מספר טלפון לאימות נוסף ויצירת קשר

**איך הוא עוזר בפרויקט?**

1. **אימות נוסף**
   - יכול לשמש לאימות SMS בעתיד
   - שחזור חשבון דרך SMS

2. **Validation**
   - `match` עם regex: `/^\+?[1-9]\d{7,14}$/`
   - תומך בפורמט בינלאומי: `+971501234567`
   - 7-14 ספרות (לאחר קידומת)

3. **שימוש ב-Frontend**
   - ב-`client/src/components/SignupForm.tsx`: שדה קלט עם helper text
   - ב-`client/src/types/index.ts`: חלק מה-User interface

**דוגמאות שימוש:**
- `server/src/services/user.service.js`: `createUser` מקבל `phone`
- `server/src/controllers/user.controller.js`: מחזיר `phone` ב-user profile

---

### 7. `status` (Enum: PENDING/ACTIVE)

**הגדרה:**
```javascript
status: {
  type: String,
  enum: {
    values: Object.values(USER_STATUS),
    message: 'Invalid user status',
  },
  default: USER_STATUS.PENDING,
}
```

**למה השדה הזה קיים?**
- ניהול מצב המשתמש - אימות אימייל

**איך הוא עוזר בפרויקט?**

1. **PENDING - ממתין לאימות**
   - משתמש חדש שנרשם אבל לא אימת אימייל
   - Default value בעת יצירת משתמש חדש
   - לא יכול להתחבר או לבצע פעולות

2. **ACTIVE - מאומת**
   - משתמש שאימת את האימייל
   - יכול להתחבר ולבצע פעולות
   - ב-`server/src/services/user.service.js`: `findAndVerifyUserByToken` משנה ל-ACTIVE

3. **בדיקות אבטחה**
   - ב-`server/src/middleware/auth.middleware.js`: `if (!user || user.status !== USER_STATUS.ACTIVE)`
   - ב-`server/src/middleware/socketAuth.middleware.js`: בודק status לפני חיבור Socket
   - ב-`server/src/controllers/auth.controller.js`: בודק status ב-login

**דוגמאות שימוש:**
- `server/src/services/user.service.js`: `createUser` - יוצר עם status PENDING
- `server/src/services/user.service.js`: `findAndVerifyUserByToken` - משנה ל-ACTIVE
- `server/src/constants/index.js`: `USER_STATUS = { PENDING: 'PENDING', ACTIVE: 'ACTIVE' }`

---

### 8. `verificationToken` (String, optional)

**הגדרה:**
```javascript
verificationToken: { type: String }
```

**למה השדה הזה קיים?**
- אחסון token לאימות אימייל

**איך הוא עוזר בפרויקט?**

1. **יצירת Token**
   - ב-`server/src/services/user.service.js`: `generateVerificationToken()` - יוצר token אקראי
   - נשמר כ-hash: `hashToken(verificationToken)`
   - נשלח באימייל למשתמש

2. **אימות Token**
   - ב-`server/src/services/user.service.js`: `findAndVerifyUserByToken`
   - מחפש משתמש עם token מתאים ו-status PENDING
   - אם נמצא: משנה status ל-ACTIVE ומחק את ה-token

3. **ניקוי לאחר אימות**
   - `user.verificationToken = undefined` - השדה מוגדר ל-`undefined` לאחר אימות מוצלח
   - **הרשומה לא נמחקת** - רק השדה `verificationToken` מקבל `undefined`
   - ב-MongoDB/Mongoose: כששמים `undefined`, השדה בדרך כלל לא נשמר במסמך (הוא פשוט לא קיים)
   - לא נשאר מידע מיותר במסד הנתונים - השדה לא קיים יותר במסמך

4. **Resend Verification**
   - ב-`server/src/services/user.service.js`: `regenerateVerificationToken`
   - יוצר token חדש אם המשתמש לא קיבל את הראשון

**דוגמאות שימוש:**
- `server/src/services/user.service.js`: `createUser` - יוצר token בעת הרשמה
- `server/src/services/user.service.js`: `findAndVerifyUserByToken` - בודק token
- `server/src/controllers/auth.controller.js`: `sendVerificationEmailAsync` - שולח token באימייל

---

### 9. `balance` (Number)

**הגדרה:**
```javascript
balance: {
  type: Number,
  default: DEFAULT_BALANCE,
  min: [0, 'Balance cannot be negative'],
}
```

**למה השדה הזה קיים?**
- מעקב אחר יתרת המשתמש

**איך הוא עוזר בפרויקט?**

1. **Default Balance**
   - `DEFAULT_BALANCE = 500` (מ-`server/src/constants/index.js`)
   - כל משתמש חדש מקבל 500 AED

2. **תצוגה ב-Dashboard**
   - ב-`client/src/components/BalanceCard.tsx`: מציג את ה-balance
   - ב-`client/src/pages/DashboardPage.tsx`: מעביר balance ל-BalanceCard

3. **בדיקת מספיק כסף לפני העברה**
   - ב-`server/src/services/transaction.service.js`: `deductSenderBalance`
   - Query: `{ email: senderEmail, balance: { $gte: amount } }`
   - אם אין מספיק כסף → `AppError('Insufficient funds')`

4. **עדכון Atomically**
   - ב-`server/src/services/transaction.service.js`: `executeTransfer`
   - כל העדכונים (deduct, add, create transaction) בתוך MongoDB transaction
   - אם משהו נכשל → הכל מתבטל (atomicity)

5. **Validation**
   - `min: [0]` - לא יכול להיות שלילי
   - מונע יתרות שליליות

**דוגמאות שימוש:**
- `server/src/services/transaction.service.js`: `deductSenderBalance` - מפחית balance
- `server/src/services/transaction.service.js`: `addReceiverBalance` - מוסיף balance
- `server/src/services/user.service.js`: `getAccountSummary` - מחזיר balance ל-ChatBot
- `client/src/components/BalanceCard.tsx`: מציג balance

---

### 10. `createdAt` (Timestamp, auto)

**הגדרה:**
```javascript
{
  timestamps: { createdAt: true, updatedAt: false },
}
```

**למה השדה הזה קיים?**
- מעקב מתי נוצר החשבון

**איך הוא עוזר בפרויקט?**

1. **Audit Trail**
   - מעקב מתי נוצר כל משתמש
   - שימושי לניפוי באגים וניתוח

2. **דוחות וסטטיסטיקות**
   - יכול לשמש לניתוח: כמה משתמשים נרשמו השבוע/חודש
   - Analytics ו-reporting

3. **Auto-generated**
   - Mongoose יוצר את השדה אוטומטית בעת יצירת מסמך
   - לא צריך לספק אותו ידנית

**דוגמאות שימוש:**
- Mongoose יוצר אוטומטית בעת `User.create()`
- יכול לשמש למיון וסינון בעתיד

---

## מודל Transaction (`server/src/models/transaction.model.js`)

---

### 1. `id` (Number, unique)

**הגדרה:**
```javascript
id: {
  type: Number,
  required: true,
  unique: true,
}
```

**למה השדה הזה קיים?**
- מזהה ייחודי מספרי להעברה (לא `_id` של MongoDB)

**איך הוא עוזר בפרויקט?**

1. **User-friendly ID**
   - במקום ObjectId כמו `507f1f77bcf86cd799439011`
   - מספרים פשוטים: 1, 2, 3, 4...
   - קל יותר לזכור ולצטט

2. **יצירה ע"י Counter**
   - ב-`server/src/services/transaction.service.js`: `getNextTransactionId(session)`
   - Counter pattern מבטיח מספרים עוקבים וייחודיים
   - Thread-safe - מונע duplicates גם ב-concurrent requests

3. **שימוש ב-URLs**
   - ב-`server/src/controllers/transaction.controller.js`: `GET /transactions/:transactionId`
   - ב-`server/src/services/transaction.service.js`: `findTransactionById` - מחפש לפי `id: Number(transactionId)`

4. **Unique Constraint**
   - `unique: true` - MongoDB מבטיח שאין שני transactions עם אותו id
   - הגנה נוספת מפני duplicates

**דוגמאות שימוש:**
- `server/src/services/transaction.service.js`: `createTransactionRecord` - מקבל id מ-`getNextTransactionId`
- `server/src/services/transaction.service.js`: `findTransactionById` - מחפש לפי id
- `client/src/types/index.ts`: `id: number` - חלק מה-Transaction interface

---

### 2. `fromEmail` (String, indexed)

**הגדרה:**
```javascript
fromEmail: {
  type: String,
  required: [true, 'Sender email is required'],
  lowercase: true,
  trim: true,
  match: [/^[^\s@]+@[^\s@]+\.[^\s@]+$/, 'Invalid sender email format'],
}
```

**Index:**
```javascript
transactionSchema.index({ fromEmail: 1, createdAt: -1 });
```

**למה השדה הזה קיים?**
- זיהוי השולח בהעברה

**איך הוא עוזר בפרויקט?**

1. **זיהוי העברות יוצאות**
   - ב-`server/src/services/transaction.service.js`: `userTransactionQuery`
   - Query: `$or: [{ fromEmail: userEmail }, { toEmail: userEmail }]`
   - מציג גם העברות יוצאות וגם נכנסות

2. **Index לביצועים**
   - `{ fromEmail: 1, createdAt: -1 }` - Compound Index
   - מאפשר חיפוש מהיר לפי fromEmail + מיון לפי createdAt
   - **משפר ביצועים פי 100-1000** בכמות נתונים גדולה

3. **Normalization**
   - `lowercase: true` - כל ה-emails נשמרים באותיות קטנות
   - `trim: true` - מסיר רווחים מיותרים
   - מונע duplicates

4. **Validation**
   - `match` עם regex - בודק פורמט תקין של email
   - `required: true` - חובה למלא

5. **שימוש ב-Create Transaction**
   - ב-`server/src/services/transaction.service.js`: `createTransactionRecord`
   - מקבל `senderEmail` ומשתמש כ-`fromEmail`

**דוגמאות שימוש:**
- `server/src/services/transaction.service.js`: `userTransactionQuery` - ב-`findTransactionsByUserEmail`
- `server/src/services/transaction.service.js`: `createTransactionRecord` - יוצר transaction עם fromEmail
- `client/src/components/TransactionRow.tsx`: מציג fromEmail

---

### 3. `toEmail` (String, indexed)

**הגדרה:**
```javascript
toEmail: {
  type: String,
  required: [true, 'Receiver email is required'],
  lowercase: true,
  trim: true,
  match: [/^[^\s@]+@[^\s@]+\.[^\s@]+$/, 'Invalid receiver email format'],
}
```

**Index:**
```javascript
transactionSchema.index({ toEmail: 1, createdAt: -1 });
```

**למה השדה הזה קיים?**
- זיהוי המקבל בהעברה

**איך הוא עוזר בפרויקט?**

1. **זיהוי העברות נכנסות**
   - ב-`server/src/services/transaction.service.js`: `userTransactionQuery`
   - Query: `$or: [{ fromEmail: userEmail }, { toEmail: userEmail }]`
   - מציג גם העברות יוצאות וגם נכנסות

2. **Index לביצועים**
   - `{ toEmail: 1, createdAt: -1 }` - Compound Index
   - מאפשר חיפוש מהיר לפי toEmail + מיון לפי createdAt
   - **משפר ביצועים פי 100-1000** בכמות נתונים גדולה

3. **Normalization זהה ל-fromEmail**
   - `lowercase: true`, `trim: true`
   - `match` עם regex

4. **שימוש ב-Create Transaction**
   - ב-`server/src/services/transaction.service.js`: `createTransactionRecord`
   - מקבל `receiverEmail` ומשתמש כ-`toEmail`

**דוגמאות שימוש:**
- `server/src/services/transaction.service.js`: `userTransactionQuery` - ב-`findTransactionsByUserEmail`
- `server/src/services/transaction.service.js`: `createTransactionRecord` - יוצר transaction עם toEmail
- `client/src/components/TransactionRow.tsx`: מציג toEmail

---

### 4. `amount` (Number)

**הגדרה:**
```javascript
amount: {
  type: Number,
  required: [true, 'Amount is required'],
  min: [MIN_AMOUNT, 'Amount must be at least 0.01'],
  max: [MAX_AMOUNT, 'Amount cannot exceed 1,000,000'],
  validate: {
    validator: function (v) {
      return Number.isFinite(v) && v > 0;
    },
    message: 'Amount must be a positive number',
  },
}
```

**Constants:**
```javascript
const MIN_AMOUNT = 0.01;
const MAX_AMOUNT = 1_000_000;
```

**למה השדה הזה קיים?**
- סכום ההעברה

**איך הוא עוזר בפרויקט?**

1. **Validation מחמיר - 3 שכבות:**
   - **Schema validation**: `min: 0.01`, `max: 1,000,000`
   - **Custom validator**: `Number.isFinite(v) && v > 0` - בודק שהוא מספר חיובי וסופי
   - **Service validation**: ב-`server/src/utils/validation.util.js` - `validateTransactionAmount`

2. **שימוש ב-Transfer Flow**
   - ב-`server/src/services/transaction.service.js`: `executeTransfer`
   - `deductSenderBalance` - מפחית amount מה-balance של השולח
   - `addReceiverBalance` - מוסיף amount ל-balance של המקבל
   - `createTransactionRecord` - שומר את ה-amount ב-transaction

3. **תצוגה ב-Frontend**
   - ב-`client/src/components/TransactionRow.tsx`: מציג amount עם formatting
   - ב-`client/src/utils/formatters.ts`: `formatAmount` - מעצב את הסכום

4. **חישובי יתרות**
   - כל העברה משנה את ה-balance של שני המשתמשים
   - הכל atomically בתוך MongoDB transaction

**דוגמאות שימוש:**
- `server/src/services/transaction.service.js`: `executeTransfer` - מקבל amount
- `server/src/controllers/transaction.controller.js`: `createTransaction` - מקבל amount מה-body
- `client/src/components/TransferForm.tsx`: שדה קלט ל-amount

---

### 5. `description` (String, optional)

**הגדרה:**
```javascript
description: {
  type: String,
  trim: true,
  maxlength: [500, 'Description cannot exceed 500 characters'],
}
```

**למה השדה הזה קיים?**
- הערה אופציונלית על ההעברה

**איך הוא עוזר בפרויקט?**

1. **Optional Field**
   - לא חובה למלא - יכול להיות `undefined` או `null`
   - משתמש יכול להוסיף הערה או לא

2. **Validation**
   - `trim: true` - מסיר רווחים מיותרים
   - `maxlength: 500` - מקסימום 500 תווים
   - מונע תיאורים ארוכים מדי

3. **תצוגה ב-Frontend**
   - ב-`client/src/components/TransactionDetailPanel.tsx`: מציג description אם קיים
   - ב-`client/src/components/TransactionRow.tsx`: יכול להציג description

4. **שימוש עתידי**
   - יכול לשמש לחיפוש וסינון
   - Analytics - מה המשתמשים כותבים בהעברות

**דוגמאות שימוש:**
- `server/src/services/transaction.service.js`: `createTransactionRecord` - מקבל description (אופציונלי)
- `server/src/controllers/transaction.controller.js`: `createTransaction` - מקבל description מה-body
- `client/src/components/TransferForm.tsx`: שדה קלט ל-description (אופציונלי)

---

### 6. `createdAt` (Timestamp, auto)

**הגדרה:**
```javascript
{
  timestamps: { createdAt: true, updatedAt: false },
}
```

**למה השדה הזה קיים?**
- תאריך ושעה של ההעברה

**איך הוא עוזר בפרויקט?**

1. **מיון לפי תאריך**
   - ב-`server/src/services/transaction.service.js`: `.sort({ createdAt: -1 })`
   - תמיד מציגים את ההעברות החדשות ביותר קודם
   - Descending order (-1) = מהחדש לישן

2. **Index עם createdAt**
   - `{ fromEmail: 1, createdAt: -1 }` - Compound Index
   - מאפשר חיפוש + מיון מהיר ב-query אחד
   - לא צריך sort נפרד

3. **תצוגה ב-Frontend**
   - ב-`client/src/components/TransactionRow.tsx`: מציג createdAt עם formatting
   - ב-`client/src/utils/formatters.ts`: יכול לעצב תאריכים

4. **דוחות וסטטיסטיקות**
   - יכול לשמש לניתוח: כמה העברות היו השבוע/חודש
   - Analytics ו-reporting

**דוגמאות שימוש:**
- `server/src/services/transaction.service.js`: `findTransactionsByUserEmail` - מיון לפי createdAt
- `server/src/services/transaction.service.js`: `findRecentTransactions` - מיון לפי createdAt
- `client/src/types/index.ts`: `createdAt: string` - חלק מה-Transaction interface

---

## Counter Schema (בקובץ Transaction)

---

### 1. `_id` (String, required)

**הגדרה:**
```javascript
const counterSchema = new mongoose.Schema({
  _id: { type: String, required: true },
  seq: { type: Number, default: 0 },
});
```

**למה השדה הזה קיים?**
- מזהה קבוע ל-Counter - תמיד `'transactions'`

**איך הוא עוזר בפרויקט?**

1. **מזהה קבוע**
   - ב-`server/src/models/transaction.model.js`: `getNextTransactionId`
   - `Counter.findByIdAndUpdate('transactions', ...)`
   - תמיד אותו Counter - לא צריך ליצור חדש

2. **Upsert Pattern**
   - `upsert: true` - אם Counter לא קיים, יוצר אותו אוטומטית
   - אם קיים - משתמש בו

**דוגמאות שימוש:**
- `server/src/models/transaction.model.js`: `getNextTransactionId` - מחפש Counter עם `_id: 'transactions'`

---

### 2. `seq` (Number, default: 0)

**הגדרה:**
```javascript
seq: { type: Number, default: 0 }
```

**למה השדה הזה קיים?**
- המספר הנוכחי - מוגדל ב-1 כל פעם

**איך הוא עוזר בפרויקט?**

1. **Atomic Increment**
   - ב-`server/src/models/transaction.model.js`: `getNextTransactionId`
   - `{ $inc: { seq: 1 } }` - מוסיף 1 ל-seq
   - **Atomic operation** - מונע race conditions

2. **Thread-Safe**
   - גם אם יש concurrent requests, MongoDB מבטיח שהתוספת תהיה atomic
   - לא יהיו duplicates

3. **תמיכה ב-MongoDB Transactions**
   - הפונקציה מקבלת `session` - יכולה לרוץ בתוך transaction
   - אם ה-transaction נכשל, גם ה-increment מתבטל

4. **Sequential IDs**
   - מחזיר מספרים עוקבים: 1, 2, 3, 4...
   - User-friendly במקום ObjectIds

**דוגמאות שימוש:**
- `server/src/models/transaction.model.js`: `getNextTransactionId` - מחזיר `counter.seq`
- `server/src/services/transaction.service.js`: `createTransactionRecord` - מקבל id מ-`getNextTransactionId`

---

## סיכום - למה כל שדה חשוב

### מודל User:
- **id**: אבטחה ב-JWT, יציבות
- **firstName/lastName**: זיהוי משתמש, UX
- **email**: זיהוי ייחודי, authentication, העברות
- **password**: אבטחה (select: false, hashing)
- **phone**: אימות נוסף, שחזור חשבון
- **status**: ניהול אימות אימייל, אבטחה
- **verificationToken**: אימות אימייל
- **balance**: יתרת המשתמש, העברות
- **createdAt**: audit trail, analytics

### מודל Transaction:
- **id**: User-friendly ID, Counter pattern
- **fromEmail/toEmail**: זיהוי שולח/מקבל, Indexes לביצועים
- **amount**: סכום העברה, validation מחמיר
- **description**: הערה אופציונלית
- **createdAt**: מיון לפי תאריך, Index

### Counter:
- **_id**: מזהה קבוע ('transactions')
- **seq**: מספר עוקב, atomic increment

כל שדה תורם לפונקציונליות, אבטחה, או ביצועים של הפרויקט!
