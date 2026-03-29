# Two-Factor Authentication (2FA) — Email OTP
### הסבר מלא לראיון עבודה

---

## מה זה בכלל 2FA?

**Two-Factor Authentication** = אימות דו-שלבי.

הרעיון: גם אם מישהו גנב את הסיסמה שלך, הוא עדיין לא יכול להיכנס לחשבון, כי הוא צריך גם גישה לאימייל שלך.

מסביר בפשטות:
- **Factor 1 (משהו שאתה יודע):** אימייל + סיסמה
- **Factor 2 (משהו שיש לך):** גישה לתיבת הדואר האלקטרוני שלך

---

## ה-Flow המלא — שלב אחרי שלב

```
USER                    FRONTEND                    BACKEND                    DB / EMAIL
 |                          |                           |                          |
 |  enters email+password   |                           |                          |
 |------------------------->|                           |                          |
 |                          |  POST /auth/login         |                          |
 |                          |-------------------------->|                          |
 |                          |                           |  find user by email      |
 |                          |                           |------------------------->|
 |                          |                           |  validate password       |
 |                          |                           |  (bcrypt.compare)        |
 |                          |                           |                          |
 |                          |                           |  generate 6-digit OTP    |
 |                          |                           |  (Math.random)           |
 |                          |                           |                          |
 |                          |                           |  bcrypt.hash(otp)        |
 |                          |                           |  save: otpHash,          |
 |                          |                           |        otpExpiry (+5min) |
 |                          |                           |        otpAttempts = 0   |
 |                          |                           |------------------------->|
 |                          |                           |                          |
 |                          |                           |  send OTP email -------->| (Brevo API)
 |                          |                           |  (async, fire-and-forget)|
 |                          |                           |                          |
 |                          |  { otpRequired: true }    |                          |
 |                          |<--------------------------|                          |
 |   OTP screen appears     |                           |                          |
 |<-------------------------|                           |                          |
 |                          |                           |                          |
 |  enters 6-digit code     |                           |                          |
 |------------------------->|                           |                          |
 |                          |  POST /auth/verify-otp    |                          |
 |                          |  { email, otp }           |                          |
 |                          |-------------------------->|                          |
 |                          |                           |  find user by email      |
 |                          |                           |------------------------->|
 |                          |                           |  check attempts < 5      |
 |                          |                           |  check expiry < now      |
 |                          |                           |  bcrypt.compare(otp,hash)|
 |                          |                           |                          |
 |                          |                           |  clear: otpHash,         |
 |                          |                           |         otpExpiry        |
 |                          |                           |  (single-use!)           |
 |                          |                           |------------------------->|
 |                          |                           |                          |
 |                          |                           |  createToken(user) → JWT |
 |                          |                           |  Set-Cookie: token=JWT;  |
 |                          |                           |  HttpOnly; Secure        |
 |                          |  { user: {...} }          |                          |
 |                          |  + httpOnly cookie        |                          |
 |                          |<--------------------------|                          |
 |   redirect to dashboard  |                           |                          |
 |<-------------------------|                           |                          |
```

---

## שלב 1 — ה-Database Schema

**קובץ:** `server/src/models/user.model.js`

הוספנו 3 שדות חדשים לכל משתמש:

```javascript
otpHash:     { type: String },             // הקוד לאחר bcrypt.hash
otpExpiry:   { type: Date },               // מתי פג תוקף הקוד (5 דקות)
otpAttempts: { type: Number, default: 0 }, // מספר נסיונות כושלים
```

**למה nullable ולא required?**
כי השדות הם "temporary state" — הם קיימים רק כשיש OTP פעיל. כשאין OTP, הם ריקים. אין צורך ב-collection נפרד.

---

## שלב 2 — ה-Login Controller

**קובץ:** `server/src/controllers/auth.controller.js`

```javascript
export const login = async (req, res, next) => {
  // 1. מוצאים את המשתמש לפי אימייל
  const user = await findUserByEmailWithPassword(email);

  // 2. בודקים שהמשתמש קיים ו-ACTIVE (לא PENDING - לא אימת אימייל)
  if (!user || user.status !== USER_STATUS.ACTIVE) {
    throw new AppError('Invalid credentials', 401);
  }

  // 3. מאמתים את הסיסמה עם bcrypt
  const isValidPassword = await validatePassword(password, user.password);
  if (!isValidPassword) throw new AppError('Invalid credentials', 401);

  // 4. מייצרים קוד 6 ספרות (100000 עד 999999)
  const otp = String(Math.floor(100000 + Math.random() * 900000));

  // 5. שומרים OTP מוצפן ב-DB
  await saveOtp(user.id, otp);

  // 6. שולחים אימייל (async - לא מחכים לתשובה)
  sendOtpEmailAsync(user.email, otp);

  // 7. מחזירים לפרונט - אל תנפיק JWT עדיין!
  return response.ok(res, { otpRequired: true });
};
```

**הנקודה הכי חשובה:** ה-JWT **לא** מונפק בשלב הזה. המשתמש לא מחובר עדיין.

---

## שלב 3 — שמירת ה-OTP בצורה מאובטחת

**קובץ:** `server/src/services/user.service.js`

```javascript
const OTP_EXPIRY_MS = 5 * 60 * 1000; // 5 דקות
const MAX_OTP_ATTEMPTS = 5;

export const saveOtp = async (userId, otp) => {
  const otpHash = await bcrypt.hash(otp, SALT_ROUNDS); // מצפינים!
  await User.findOneAndUpdate(
    { id: userId },
    {
      otpHash,                                          // שומרים hash - לא plain text
      otpExpiry: new Date(Date.now() + OTP_EXPIRY_MS), // 5 דקות מעכשיו
      otpAttempts: 0,                                   // מאפסים נסיונות (login חדש)
    }
  );
};
```

**למה bcrypt על OTP?** ← שאלת ראיון!
> "כי אם מישהו פרץ למסד הנתונים, הוא לא יכול לגנוב OTPs פעילים. כמו שלא שומרים סיסמאות ב-plain text, לא שומרים OTPs ב-plain text."

---

## שלב 4 — אימות ה-OTP

**קובץ:** `server/src/services/user.service.js`

```javascript
export const verifyOtp = async (user, otp) => {
  // בדיקה 1: האם המשתמש חסום (יותר מ-5 נסיונות)?
  if (user.otpAttempts >= MAX_OTP_ATTEMPTS) {
    throw new AppError('Too many failed attempts. Please request a new OTP.', 429);
  }

  // בדיקה 2: האם ה-OTP קיים ועדיין בתוקף?
  if (!user.otpHash || !user.otpExpiry || user.otpExpiry < new Date()) {
    throw new AppError('OTP has expired. Please request a new one.', 400);
  }

  // בדיקה 3: האם הקוד נכון?
  const isValid = await bcrypt.compare(otp, user.otpHash);

  if (!isValid) {
    // מגדילים מונה נסיונות כושלים
    await User.findOneAndUpdate({ id: user.id }, { $inc: { otpAttempts: 1 } });
    throw new AppError('Invalid OTP.', 400);
  }

  // הצלחה! מוחקים את ה-OTP (single-use!)
  await User.findOneAndUpdate(
    { id: user.id },
    { $unset: { otpHash: '', otpExpiry: '' }, $set: { otpAttempts: 0 } }
  );
};
```

**3 הבדיקות לזכור בראיון:**
1. מספר נסיונות (brute-force protection)
2. תפוגה (replay attack protection)
3. bcrypt.compare (נכונות הקוד)

---

## שלב 5 — הנפקת JWT לאחר אימות מוצלח

**קובץ:** `server/src/controllers/auth.controller.js`

```javascript
export const verifyOtp = async (req, res, next) => {
  const user = await findUserByEmail(email);

  // מריץ את כל 3 הבדיקות
  await verifyOtpService(user, otp);

  // רק עכשיו - מנפיקים JWT!
  const token = createToken(user);
  res.cookie(config.cookie.tokenName, token, {
    httpOnly: true,  // JavaScript לא יכול לקרוא את ה-cookie
    secure: true,    // רק HTTPS (ב-production)
    sameSite: 'lax', // הגנה מ-CSRF
    maxAge: 3600000, // שעה אחת
    path: '/',
  });

  return response.ok(res, { user: { id, firstName, lastName, email } });
};
```

---

## שלב 6 — ה-Frontend (useLogin.ts)

**קובץ:** `client/src/screens/login-signup/useLogin.ts`

```typescript
// 3 instances של useAsyncOperation:
const { loading, error, execute } = useAsyncOperation();   // login
const resendAsync = useAsyncOperation();                    // resend verification
const otpAsync = useAsyncOperation();                       // OTP verification ← חדש

// State חדש
const [otpRequired, setOtpRequired] = useState(false);
const [otp, setOtp] = useState('');
const [resendCooldown, setResendCooldown] = useState(0);

// Countdown timer - useEffect + setTimeout
useEffect(() => {
  if (resendCooldown <= 0) return;
  const timer = setTimeout(() => setResendCooldown((c) => c - 1), 1000);
  return () => clearTimeout(timer); // cleanup!
}, [resendCooldown]);

// Login - מסתיים ב-otpRequired: true במקום navigation
const handleSubmit = async (e) => {
  await execute(
    () => authService.login(email, password),
    (data) => {
      if (data.otpRequired) {
        setOtpRequired(true); // מציגים מסך OTP
      } else {
        navigate(ROUTES.DASHBOARD); // fallback אם אי פעם OTP מושבת
      }
    }
  );
};

// OTP verification - רק עכשיו navigate
const handleVerifyOtp = async (e) => {
  e.preventDefault();
  await otpAsync.execute(
    () => authService.verifyOtp(email, otp),
    (data) => {
      authStorage.setUser(data.user!);
      navigate(ROUTES.DASHBOARD);
    }
  );
};

// Resend - תמיד מוגבל ל-60 שניות
const handleResendOtp = async () => {
  await authService.resendOtp(email).catch(() => {});
  setOtp('');
  setResendCooldown(60);
};
```

---

## שלב 7 — ה-UI (LoginForm.tsx)

**קובץ:** `client/src/screens/login-signup/LoginForm.tsx`

```tsx
// Early return - אם otpRequired, מציגים מסך שונה לגמרי
if (otpRequired) {
  return (
    <form onSubmit={onVerifyOtp}>
      <TextField
        label="Verification Code"
        inputProps={{ maxLength: 6 }}
        autoComplete="one-time-code"  // iOS autofill!
      />

      <Button
        type="submit"
        disabled={otpLoading || otp.length !== 6}  // disabled עד 6 ספרות
      >
        Verify Code
      </Button>

      <Button
        onClick={onResendOtp}
        disabled={resendCooldown > 0}
      >
        {resendCooldown > 0 ? `Resend code in ${resendCooldown}s` : 'Resend code'}
      </Button>
    </form>
  );
}

// אחרת - form רגיל של email+password
return ( ... );
```

**עיצוב נקי:** אין route חדש, אין component חדש — רק `early return` בתוך LoginForm הקיים.

---

## ה-Resend OTP — User Enumeration Protection

**קובץ:** `server/src/controllers/auth.controller.js`

```javascript
export const resendOtp = async (req, res, next) => {
  const user = await findUserByEmail(email);

  if (user && user.status === USER_STATUS.ACTIVE) {
    // שולחים רק אם המשתמש קיים - אבל...
    const otp = String(Math.floor(100000 + Math.random() * 900000));
    await saveOtp(user.id, otp);
    sendOtpEmailAsync(user.email, otp);
  }

  // ...תמיד מחזירים אותה תשובה, גם אם המשתמש לא קיים!
  return response.ok(res, { message: 'If the account exists, a new OTP has been sent.' });
};
```

**למה "If the account exists"?** ← שאלת ראיון!
> "כדי למנוע User Enumeration. אם היינו מחזירים 'user not found', תוקף יוכל לגלות אילו כתובות אימייל רשומות במערכת שלנו. Generic response מונע את זה."

---

## ה-Routes + Rate Limiting

**קובץ:** `server/src/routes/auth.routes.js`

```javascript
router.post('/verify-otp', authLimiter, verifyOtp);
router.post('/resend-otp',  authLimiter, resendOtp);
```

`authLimiter` כבר היה קיים על `/login` — השתמשנו בו גם כאן.
**5 נסיונות / 15 דקות** (ב-production).

**למה rate limiting על OTP?**
> "קוד OTP של 6 ספרות יש מיליון אפשרויות. בלי rate limiting תוקף יכול לנסות 1,000,000 combinations. עם 5 נסיונות לפני lockout + rate limiter, זה בלתי אפשרי."

---

## סיכום החלטות האבטחה — לראיון

| החלטה | למה |
|--------|-----|
| `bcrypt.hash(otp)` | DB leak לא חושף OTPs פעילים |
| 5 דקות תפוגה | OTP גנוב = חסר ערך אחרי 5 דק' |
| מקסימום 5 נסיונות | מונע brute force על 1M combinations |
| מוחקים OTP אחרי שימוש | Single-use — מונע replay attacks |
| Generic message ב-resend | מונע user enumeration |
| אין JWT עד OTP מאומת | Factor 1 לא מספיק לגישה |
| שדות nullable ב-User | פשטות — אין צורך ב-collection נפרד |
| שימוש ב-authLimiter הקיים | אין קוד חדש, אותה הגנה |

---

## שאלות ותשובות לראיון

---

**"Walk me through your authentication flow."**

> "We implemented two-factor authentication. When a user logs in with email and password, if credentials are valid, we don't issue a token yet — instead we generate a 6-digit OTP, hash it with bcrypt, store it in the user's document with a 5-minute expiry, and send it to their email. The frontend switches to an OTP input screen. Once the user enters the correct code, the backend verifies it against the stored hash, clears the OTP from the database, and only then issues a JWT stored in an httpOnly cookie. This means both the password AND email access are required to authenticate."

---

**"Why do you hash the OTP with bcrypt?"**

> "Same reason we hash passwords — if the database is compromised, an attacker shouldn't be able to extract active login codes. Storing a plain OTP means a database leak instantly gives away all pending 2FA codes."

---

**"Why a 5-minute expiry?"**

> "It's a balance between security and usability. Short enough that a stolen OTP becomes worthless quickly, long enough that a user can realistically check their email and complete login. Banks typically use 3–10 minutes."

---

**"What happens if someone tries to brute-force the OTP?"**

> "Two layers of protection: First, after 5 failed attempts the OTP is locked — the user must request a new one. Second, the rate limiter on the endpoint allows only 5 requests per 15 minutes in production. A 6-digit code has 1,000,000 combinations — with these limits, brute-forcing is computationally infeasible."

---

**"Why clear the OTP after successful verification?"**

> "OTPs must be single-use. If we didn't delete it, the same code could be replayed to authenticate again within the 5-minute window, which completely defeats the purpose of 2FA."

---

**"Why does the resend endpoint return a generic message?"**

> "To prevent user enumeration. If we returned 'user not found', an attacker could probe which email addresses are registered in our system and target those accounts specifically. The generic message is the same whether the user exists or not."

---

**"Where do you store the JWT after login?"**

> "In an httpOnly cookie set by the server. httpOnly means JavaScript cannot access it — so XSS attacks can't steal the token. It's sent automatically by the browser on every request, but scripts can't read it. This is safer than localStorage."

---

**"How does the frontend know to show the OTP screen?"**

> "The login endpoint returns `{ otpRequired: true }` instead of user data when credentials are valid. The `useLogin` hook checks for this flag and sets local state `otpRequired = true`. The `LoginForm` component has an early return — if `otpRequired` is true, it renders the OTP form instead of the email/password form. No new routes or pages needed."

---

**"What would you improve if you had more time?"**

> "I'd add SMS as a second OTP delivery channel — email is convenient but not instant. I'd also implement a 'remember this device for 30 days' feature using a separate long-lived device token, so trusted users aren't prompted for OTP every login. And I'd add a proper audit log of all authentication attempts."

---

## המבנה הכולל — כל הקבצים שנגעת בהם

```
server/
├── models/
│   └── user.model.js          ← הוספת otpHash, otpExpiry, otpAttempts
├── services/
│   └── user.service.js        ← הוספת saveOtp(), verifyOtp()
├── utils/
│   └── email.util.js          ← הוספת buildOtpEmailTemplate(), sendOtpEmailAsync()
├── controllers/
│   └── auth.controller.js     ← שינוי login(), הוספת verifyOtp(), resendOtp()
└── routes/
    └── auth.routes.js         ← הוספת POST /verify-otp, POST /resend-otp

client/src/
├── types/
│   └── index.ts               ← LoginResponse: user? optional, otpRequired?
├── api/
│   └── auth.service.ts        ← הוספת verifyOtp(), resendOtp()
└── screens/login-signup/
    ├── useLogin.ts             ← state חדש, handlers חדשים, countdown
    ├── LoginPage.tsx           ← העברת כל ה-props החדשים
    └── LoginForm.tsx           ← OTP form כ-early return
```

**סה"כ: 10 קבצים, 0 dependencies חדשות** — הכל נבנה על bcrypt ו-rate limiter שכבר היו בפרויקט.
