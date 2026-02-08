# Production Deployment — Vercel (client) + Render (API) + MongoDB Atlas

מסמך זה מתאר את הגדרות ה-production: איפה כל חלק רץ, אילו משתני סביבה להגדיר ב-Vercel וב-Render, והתאמת cookies ו-CORS ל-cross-origin.

---

## ארכיטקטורת Production

| רכיב | פלטפורמה | כתובת |
|------|-----------|--------|
| **Client (React)** | Vercel | https://dubai-bank.vercel.app |
| **API (Node + Express)** | Render (Docker) | https://dubai-bank-api.onrender.com |
| **MongoDB** | Atlas | connection string ב-Render בלבד (לא בקוד) |

- הדפדפן טוען את האתר מ-Vercel ושולח בקשות API ו-Socket.IO ל-Render.
- השרת ב-Render מתחבר ל-MongoDB Atlas עם `MONGO_URI` שמוגדר רק ב-Render (משתנה סביבה).
- CORS בשרת מאפשר רק את `CLIENT_URL` (Vercel). Cookies הם cross-origin ולכן דורשים `SameSite=None; Secure` (ראו להלן).

---

## 1. Vercel (Client)

ה-build ב-Vercel קורא את משתני `VITE_*` **בזמן build** ומקפיא אותם ב-bundle. יש להגדיר אותם ב-**Vercel Dashboard → Project → Settings → Environment Variables**.

| משתנה | ערך (Production) | הערה |
|--------|-------------------|------|
| `VITE_API_URL` | `https://dubai-bank-api.onrender.com/api/v1` | baseURL ל-Axios (כתובת ה-API). |
| `VITE_SOCKET_URL` | `https://dubai-bank-api.onrender.com` | בסיס ל-Socket.IO ב-`ChatAssistant` (נוסף `/chat` בקוד). |

אם יש לך גם Preview/Development — אפשר להגדיר שם ערכים אחרים (למשל `http://localhost:3000/api/v1` ל-preview מקומי).

**בקוד:**  
- [client/src/api/http-client.ts](../client/src/api/http-client.ts) — `baseURL: import.meta.env.VITE_API_URL`.  
- [client/src/components/ChatAssistant.tsx](../client/src/components/ChatAssistant.tsx) — `VITE_SOCKET_URL` ל-`io(\`${SOCKET_URL}/chat\`)`.

---

## 2. Render (API / Server)

השרת רץ ב-Render כ-**Web Service** עם Docker (ה-Dockerfile מתוך `server/`). כל הסודות וההגדרות מגיעים **רק** ממשתני הסביבה ב-Render (לא מקובץ ב-repo).

### משתני חובה

| משתנה | ערך (Production) | הערה |
|--------|-------------------|------|
| `NODE_ENV` | `production` | מפעיל `cookie.secure: true` ב-[server/src/config/index.js](../server/src/config/index.js). |
| `PORT` | `3000` | Render מזין פורט משלו; בדרך כלל לא חייבים לדרוס. |
| `MONGO_URI` | *( connection string מ-Atlas)* | **להגדיר רק ב-Render Dashboard.** לא להכניס ל-repo או לשלוח בצ'אט. |
| `JWT_SECRET` | *(מחרוזת אקראית חזקה)* | חובה. ליצירה: `node -e "console.log(require('crypto').randomBytes(64).toString('hex'))"`. |
| `CLIENT_URL` | `https://dubai-bank.vercel.app` | Origin מותר ב-CORS ([server/src/app.js](../server/src/app.js)) ובקישורים במייל ([server/src/utils/email.util.js](../server/src/utils/email.util.js)). |
| `SERVER_URL` | `https://dubai-bank-api.onrender.com` | בסיס ללינקים במייל (אימות וכו'). |
| `COOKIE_SAME_SITE` | `none` | חובה כש-client ו-API על דומיינים שונים; מאפשר שליחת cookie cross-origin. |

### משתנים אופציונליים (לפי שימוש)

| משתנה | שימוש |
|--------|--------|
| `JWT_EXPIRES_IN` | תוקף JWT (ברירת מחדל `1h`). |
| `BREVO_API_KEY` | שליחת מייל אימות (Brevo). |
| `EMAIL_FROM` | כתובת השולח במיילים. |
| `OPENAI_API_KEY` | צ'אטבוט (fallback ל-intent). |

**בקוד:**  
- [server/src/config/index.js](../server/src/config/index.js) — קורא את כל המשתנים האלה.  
- CORS: `origin: config.clientUrl`.  
- Cookie: `sameSite: process.env.COOKIE_SAME_SITE || 'lax'`, `secure: process.env.NODE_ENV === 'production'`.

---

## 3. Cookies ו-CORS ב-Production (Cross-Origin)

- **מקום:** Client על `https://dubai-bank.vercel.app`, API על `https://dubai-bank-api.onrender.com` — מקור שונה.  
- **CORS:** השרת כבר מוגדר ל-`credentials: true` ו-`origin: config.clientUrl`. מספיק ש-`CLIENT_URL` יהיה `https://dubai-bank.vercel.app`.  
- **Cookies:**  
  - בדומיין אחר הדפדפן שולח cookies רק אם `SameSite=None` ו-`Secure`.  
  - ב-`NODE_ENV=production` הקוד כבר מגדיר `secure: true`.  
  - לכן **חובה** ב-Render: `COOKIE_SAME_SITE=none`.  
  - אחרת ה-cookie של ה-JWT לא יישלח עם הבקשות מ-Vercel ל-Render והמשתמש ייראה כלא מחובר.

---

## 4. MongoDB Atlas

- ה-DB ב-production הוא Atlas; אין צורך ב-container ל-MongoDB.  
- ב-Render מגדירים רק את **MONGO_URI** (כולל user + password) ב-Environment.  
- **אבטחה:** אל תכניסו את ה-connection string ל-repo או לקבצי תיעוד. אם הוא נחשף (למשל בצ'אט) — עדכנו סיסמה ב-Atlas (Database Access → Edit user → Reset password) והחליפו את ה-`MONGO_URI` ב-Render.

---

## 5. סיכום בדיקה

| בדיקה | איפה |
|--------|------|
| `VITE_API_URL` / `VITE_SOCKET_URL` | Vercel → Environment Variables |
| `CLIENT_URL` = `https://dubai-bank.vercel.app` | Render → Environment |
| `SERVER_URL` = `https://dubai-bank-api.onrender.com` | Render → Environment |
| `COOKIE_SAME_SITE` = `none` | Render → Environment |
| `MONGO_URI`, `JWT_SECRET` | Render → Environment בלבד (לא בקוד) |
| `NODE_ENV` = `production` | Render (לרוב כבר מוגדר) |

אם משהו לא עובד (למשל התחברות נכשלת אחרי לוגין), לבדוק:  
- ב-Render שיש `COOKIE_SAME_SITE=none` ו-`CLIENT_URL` עם `https://`.  
- ב-Vercel ש-`VITE_API_URL` מסתיים ב-`/api/v1` ו-`VITE_SOCKET_URL` בלי path.
