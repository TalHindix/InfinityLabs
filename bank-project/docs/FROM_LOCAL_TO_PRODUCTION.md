# מקומי ל-Production — מה עכשיו ומה עשינו

---

## חלק 1: מה עכשיו — מעבר ל-Production

אחרי שהסקריפט עבר בהצלחה, ה-image `dubai-bank-server:test` מוכן. ב-Production (Render) לא בונים על המחשב שלך — Render בונה את **אותו** Dockerfile מהרפו.

### צעדים מעשיים

1. **לדחוף קוד ל-Git**  
   וודא ש-`server/Dockerfile`, `server/.dockerignore` והקוד ב-`server/` נמצאים ב-branch ש-Render עוקב אחריו (למשל `main`).

2. **ב-Render Dashboard**  
   - **Build:** Web Service מוגדר ל-**Docker** (לא "Native Environment"). Render יריץ `docker build` מתוך `server/` (או root עם Dockerfile path אם הגדרת כך).  
   - **Environment:** וודא שכל המשתנים מ-[DEPLOY.md](DEPLOY.md) מוגדרים:
     - חובה: `NODE_ENV=production`, `MONGO_URI` (מחרוזת Atlas), `JWT_SECRET`, `CLIENT_URL=https://dubai-bank.vercel.app`, `SERVER_URL=https://dubai-bank-api.onrender.com`, `COOKIE_SAME_SITE=none`
     - אופציונלי: `JWT_EXPIRES_IN`, `BREVO_API_KEY`, `EMAIL_FROM`, `OPENAI_API_KEY`
   - **Health Check Path:** ` /health` (כמו שבדקנו מקומית).

3. **Deploy**  
   אחרי push — Render יבנה מחדש ויעלה את ה-container. אין צורך להעלות את ה-image המקומי; Render בונה image משלו מהקוד ב-repo.

4. **בדיקה ב-Production**  
   - `https://dubai-bank-api.onrender.com/health` — מחזיר `{"status":"ok",...}`  
   - להיכנס ל-`https://dubai-bank.vercel.app`, להתחבר / להירשם — לוודא שהכל עובד (כולל cookies בין Vercel ל-Render).

**אם משהו נכשל ב-Render:**  
בדוק ב-Render את ה-Logs (Build + Runtime). שגיאות נפוצות: חסר `JWT_SECRET` או `MONGO_URI`, או `COOKIE_SAME_SITE` לא `none` (אז ההתחברות מהאתר תכשל).

---

## חלק 2: להבין לעומק — מה עשינו כאן

### המטרה של מה שבנינו

- **להריץ את ה-API באותה צורה** במקום (Docker) וב-Production (Render).  
- **לבדוק מקומית** שה-image עובד (חיבור ל-DB, health, signup) לפני דחיפה ל-Render.

---

### מה זה Docker Image ומה יש בו

- **Image** = תמונה סטטית: Node 20, התלויות (`npm ci --omit=dev`), והקוד (`server.js`, `src/`).  
- **אין בו** `.env` או `node_modules` מהמחשב — הם נשארים בחוץ (`.dockerignore` ו-env בזמן הרצה).  
- **ה-Dockerfile** מתאר איך לבנות את התמונה: איזה בסיס, אילו פקודות להריץ, ובסוף `CMD ["node", "server.js"]` — כך ש-`docker run` מפעיל בדיוק את אותו תהליך ש-Render יריץ.

**למה זה חשוב:**  
ב-Render אין "התקנת Node והרצת npm install" — יש רק "הרצת container מתמונה שנבנתה מה-Dockerfile". אותו image (בתיאור) = אותה סביבה = פחות "עובד אצלי ולא בשרת".

---

### מה עשינו במקומי (הסקריפט)

| שלב | מה קרה | למה |
|-----|--------|-----|
| **Build** | `docker build -t dubai-bank-server:test ./server` | בונה image מהקוד ב-`server/` לפי ה-Dockerfile. התוצאה שמורה במחשב שלך תחת השם `dubai-bank-server:test`. |
| **Run (Linux + MongoDB מקומי)** | `docker run -d --network host --env-file server/.env -e MONGO_URI=mongodb://localhost:27017/Dubai-Bank ...` | ה-container רץ עם **רשת המארח** (`--network host`). לכן `localhost` בתוך ה-container = המחשב שלך — וכך הוא מגיע ל-MongoDB שרץ על המחשב (פורט 27017) בלי צורך ב-`host.docker.internal`. |
| **Run (אם היה Atlas)** | `docker run -d -p 3000:3000 --env-file ...` (בלי דריסת MONGO_URI) | ה-container עם רשת משלו; פורט 3000 ממופה החוצה. ה-`MONGO_URI` מ-.env (Atlas) עובד כי Atlas נגיש מהאינטרנט. |
| **Health** | `curl http://localhost:3000/health` | בודקים שה-Express עונה — כמו ש-Render עושה עם Health Check Path. |
| **Signup** | `curl -X POST .../auth/signup` עם JSON | בודקים ששכבת האפליקציה (routes, DB, validation) עובדת **בתוך** ה-container. |
| **Cleanup** | `docker rm -f dubai-bank-server-test` | מסירים את ה-container; ה-image נשאר וניתן להריץ שוב. |

**למה `--network host` רק ב-Linux:**  
ב-Mac/Windows Docker רץ ב-VM; "localhost" בתוך ה-container הוא ה-VM, לא המחשב. שם משתמשים ב-`host.docker.internal` כדי להגיע ל-MongoDB על המחשב. ב-Linux ה-container יכול לשתף את רשת המארח, אז `localhost` = המחשב — וזה מה שהרצה הידנית שלך השתמשה בו.

---

### הקשר ל-Production (Render)

- **ב-Render אין `--network host`.**  
  ה-container ב-Render רץ ברשת פנימית; MongoDB הוא **Atlas** (בחוץ), נגיש דרך האינטרנט. לכן `MONGO_URI` ב-Render הוא connection string של Atlas.

- **אותו קוד, אותו Dockerfile.**  
  Render מריץ `docker build` על ה-repo (בדרך כלל מתוך `server/` או עם context/path שהגדרת) ומקבל image זהה במהותו לזה שבנית מקומית. ההבדל הוא רק **משתני הסביבה**: ב-Render מגדירים ב-Dashboard (MONGO_URI של Atlas, CLIENT_URL, COOKIE_SAME_SITE=none וכו') — כמו שבדיקה מקומית השתמשה ב-`--env-file` ו-`-e`.

- **הבדיקה המקומית** הוכיחה ש:  
  (1) ה-image עולה ומגיב ב-/health,  
  (2) האפליקציה מתחברת ל-DB ומבצעת signup.  
  ב-Production רק מחליפים את מקור ה-env (מ-.env ל-Dashboard) ואת ה-DB (מ-localhost ל-Atlas).

---

### סיכום בשורה אחת

**מה עשינו:** בנינו image אחד ל-API (Dockerfile), הרצנו אותו מקומית עם env נכון (כולל `--network host` ב-Linux ל-MongoDB מקומי), וידאנו health + signup.  
**מה עכשיו:** לדחוף ל-Git, לוודא ב-Render שה-build מ-Docker ושכל ה-env מ-DEPLOY.md מוגדרים, ואז לבדוק את האתר וה-API ב-production.
