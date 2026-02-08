# תיעוד Docker — פרויקט הבנק

מסמך זה מסביר את כל קבצי ה-Docker בפרויקט: תפקיד כל קובץ, איך הם עובדים יחד, ואיך להריץ.

---

## סקירה כללית

הפרויקט מורכב משלושה שירותים שרצים ב-Docker:

| שירות   | תפקיד                    | פורט (במחשב שלך) |
|---------|---------------------------|-------------------|
| **mongo**  | מסד נתונים MongoDB        | 27017             |
| **server** | API (Node + Express)      | 3000              |
| **client** | פרונט (React) — build + nginx | 80                |

הדפדפן פונה ל-`http://localhost` (client), והקליינט שולח בקשות API ל-`http://localhost:3000/api/v1` (server). השרת מתחבר ל-MongoDB ברשת הפנימית של Docker (hostname: `mongo`).

---

## רשימת קבצים שנוספו

| קובץ | מיקום | תפקיד |
|------|--------|--------|
| `.dockerignore` | `server/` | מגדיר מה **לא** להעתיק לתוך image של השרת |
| `.dockerignore` | `client/` | מגדיר מה **לא** להעתיק לתוך image של הקליינט |
| `Dockerfile` | `server/` | מתאר איך לבנות image של ה-API |
| `Dockerfile` | `client/` | מתאר איך לבנות image של הפרונט (build + הגשה) |
| `nginx.conf` | `client/` | קונפיגורציה של nginx להגשת האפליקציה (SPA) |
| `docker-compose.yml` | שורש הפרויקט | מגדיר את שלושת השירותים, הרשת, והמשתנים |

---

## קובץ אחר קובץ

### 1. `server/.dockerignore`

**מטרה:** כשעושים `docker build` מתוך `server/`, Docker שולח את כל התיקייה (ה־"context") לדי-mon. ה-.dockerignore אומר ל-Docker **לא** לכלול קבצים/תיקיות מסוימים ב-context.

**למה זה חשוב:**
- **נפח:** `node_modules/` לא נשלח — נבנה מחדש בתוך ה-image עם `npm ci`. חוסך זמן וגדול context.
- **אבטחה:** `.env` לא נכנס ל-image, כך שסודות לא "נאפים" לתמונה. משתני הסביבה יועברו בהרצה (למשל דרך `docker-compose`).
- **ניקיון:** קבצי לוג, בדיקות, .git, IDE — לא רלוונטיים להרצת השרת ב-production.

**מה מתעלמים ממנו:** `node_modules/`, `.env*`, לוגים, coverage, קבצי test, `dist/`, `.git/`, תיקיות IDE, קבצי מערכת. `README.md` נשאר (שורה `!README.md`).

---

### 2. `client/.dockerignore`

**מטרה:** אותו רעיון כמו בשרת — לצמצם את ה-build context של הקליינט.

**הבדלים רלוונטיים:**
- `dist/`, `build/`, `.vite/` — ה-build נעשה **בתוך** ה-Dockerfile, לא מעתיקים build מקומי.
- קבצי test (`.test.ts`, `.spec.tsx` וכו') — לא נחוצים ל-`npm run build` ב-production.

**יתרונות:** build מהיר יותר, image ללא קבצים מיותרים.

---

### 3. `server/Dockerfile`

**מטרה:** להגדיר איך בונים **image** אחד שמכיל את ה-API (Node + הקוד) ומוכן להריץ `node server.js`.

**שורה אחר שורה:**

| שורה | הסבר |
|------|--------|
| `FROM node:20-alpine` | תמונת בסיס: Node 20 על Alpine (קלה וקטנה). |
| `WORKDIR /app` | תיקיית העבודה בתוך ה-container היא `/app`. |
| `COPY package.json package-lock.json ./` | מעתיק רק קבצי התלויות. |
| `RUN npm ci --omit=dev` | מתקין תלויות **production** בלבד (בלי devDependencies). בפרויקט הנוכחי אין ל-server devDependencies, אבל ההרגל נכון ל-production. |
| `COPY . .` | מעתיק את שאר הקוד (server.js, src/). אחרי ה-npm כדי ששינויי קוד לא יבטלו cache של שכבת ה-npm. |
| `EXPOSE 3000` | מסמן שה-container מאזין על פורט 3000 (תיעוד; הפתיחה בפועל נעשית ב-`docker run` / compose עם `ports`). |
| `CMD ["node", "server.js"]` | פקודת ההרצה — כמו `npm start`. השרת מוגדר ל-ES Modules ב-package.json ולכן `node server.js` תומך בזה. |

**תוצאה:** image שמכיל Node 20, תלויות, והקוד — מוכן להרצה עם משתני סביבה (MONGO_URI, JWT_SECRET וכו') שיועברו מ-docker-compose.

---

### 4. `client/Dockerfile` (multi-stage)

**מטרה:** לבנות את הפרונט (Vite build) ולהוציא image סופי **רק** עם קבצים סטטיים ו-nginx, בלי Node ב-image הסופי.

**שלב 1 — Builder (שורות 1–14):**

| שורה | הסבר |
|------|--------|
| `FROM node:20-alpine AS builder` | תמונה זמנית בשם "builder" עם Node (לבנייה). |
| `WORKDIR /app` | תיקיית עבודה. |
| `COPY package.json package-lock.json ./` + `RUN npm ci` | מתקין **כל** התלויות (כולל devDependencies כמו Vite ו-TypeScript) — נחוץ ל-build. |
| `COPY . .` | מעתיק את קוד המקור. |
| `ARG VITE_API_URL` + `ENV VITE_API_URL=$VITE_API_URL` | Vite קורא את `import.meta.env.VITE_API_URL` **בזמן build** ומקפיא אותו ב-bundle. לכן ה-URL חייב להיכנס כ-build-arg ולהיות זמין כ-ENV בזמן `npm run build`. |
| `RUN npm run build` | מריץ `tsc -b && vite build` ויוצר את `dist/`. |

**שלב 2 — Serve (שורות 16–25):**

| שורה | הסבר |
|------|--------|
| `FROM nginx:alpine` | תמונה **חדשה** וקלה — רק nginx, בלי Node. |
| `COPY --from=builder /app/dist /usr/share/nginx/html` | מעתיק את תוצאת ה-build מ-stage "builder" לתיקייה ש-nginx מגיש. |
| `COPY nginx.conf /etc/nginx/conf.d/default.conf` | מכניס את קונפיג ה-SPA (ראו להלן). |
| `EXPOSE 80` | nginx מאזין על פורט 80. |
| `CMD ["nginx", "-g", "daemon off;"]` | מריץ את nginx ב-foreground (סטנדרט ב-container). |

**תוצאה:** image קטן שמכיל רק nginx + קבצים סטטיים, עם ה-API URL שקבעת ב-build.

---

### 5. `client/nginx.conf`

**מטרה:** להגדיר איך nginx מגיש את האפליקציה. ב-React (SPA) כל הניווט הוא בצד הלקוח — יש קובץ `index.html` אחד, ו-React Router מטפל ב-URL.

**בלי הקונפיג:** בכתובת כמו `http://localhost/dashboard` nginx היה מחפש קובץ בשם `dashboard` ולא מוצא, ומחזיר 404.

**עם הקונפיג:**
- `listen 80` — האזנה על פורט 80.
- `root /usr/share/nginx/html` — שורש הקבצים (שם ה-`dist` הועתק).
- `index index.html` — ברירת מחדל לדף ראשי.
- `try_files $uri $uri/ /index.html` — אם יש קובץ/תיקייה מתאימים מגיש אותם; אחרת מגיש את `index.html`. כך כל route (למשל `/dashboard`, `/transactions`) מחזיר את ה-SPA וה-Router מטפל בנתיב.

**סיכום:** קובץ זה מאפשר לפרונט לעבוד כ-SPA תחת nginx.

---

### 6. `docker-compose.yml` (שורש הפרויקט)

**מטרה:** להגדיר את שלושת השירותים, לחבר ביניהם ברשת, להעביר משתני סביבה, ולפתוח פורטים — ולהריץ הכל בפקודה אחת.

**סעיף `mongo`:**
- `image: mongo:7` — תמונת MongoDB 7 רשמית.
- `ports: "27017:27017"` — פורט 27017 במחשב שלך ממופה ל-27017 ב-container (נוח אם רוצים לחבר כלי חיצוני ל-DB).
- `volumes: mongo_data:/data/db` — נתוני MongoDB נשמרים ב-volume בשם `mongo_data`, כך שהנתונים לא נמחקים בין `docker compose down` ל-`docker compose up`.

**סעיף `server`:**
- `build: context: ./server` — בונה image מתוך תיקיית `server/` עם ה-Dockerfile שבה.
- `ports: "3000:3000"` — גישה ל-API ב-`http://localhost:3000`.
- `env_file: .env` — טוען משתני סביבה מקובץ `.env` **בשורש הפרויקט** (אותו מקום כמו ה-docker-compose.yml).
- `environment:` — דורס/מוסיף משתנים ספציפיים ל-Docker:
  - `MONGO_URI=mongodb://mongo:27017/Dubai-Bank` — השרת מתחבר ל-MongoDB דרך **שם השירות** "mongo" (רשת פנימית של Compose).
  - `CLIENT_URL`, `SERVER_URL` — כתובות שהדפדפן רואה (localhost), חשוב ל-cookies ו-CORS.
- `depends_on: mongo` — ה-server יופעל אחרי ש-mongo מופעל (לא מחכה עד ש-MongoDB מוכן לקבל חיבורים, אבל הסדר נשמר; השרת מנסה להתחבר ויכול לפרוש אם חסר JWT_SECRET וכו').

**סעיף `client`:**
- `build: context: ./client`, `args: VITE_API_URL: http://localhost:3000/api/v1` — בונה את הפרונט ומעביר את כתובת ה-API **בזמן build**, כדי שה-bundle י״ידע״ לשלוח בקשות ל-`http://localhost:3000/api/v1`.
- `ports: "80:80"` — האתר זמין ב-`http://localhost`.
- `depends_on: server` — סדר הפעלה (ה-client לא חייב שהשרת כבר עלה, אבל ה-build יכול לרוץ אחרי שה-server image נבנה).

**סעיף `volumes`:**
- `mongo_data:` — מגדיר volume named; Docker שומר שם את נתוני MongoDB.

---

## דרישות להרצה

1. **Docker** (ו-Docker Compose) מותקנים על המחשב.
2. **קובץ `.env` בשורש הפרויקט** עם לפחות:
   - `JWT_SECRET` — חובה (השרת בודק ויפרוש בלי זה).
   - שאר המשתנים לפי `server/.env.example` (למשל `JWT_EXPIRES_IN`, `BREVO_API_KEY`, `OPENAI_API_KEY` וכו' אם אתה משתמש בהם).

אפשר להעתיק מ-`server/.env.example` לשורש:
```bash
cp server/.env.example .env
```
ואז לערוך `.env` ולהוסיף `JWT_SECRET` (ולמלא ערכים נוספים לפי הצורך).

---

## פקודות שימושיות

| פקודה | משמעות |
|--------|---------|
| `docker compose up --build` | בונה את כל ה-images (אם צריך) ומפעיל את שלושת השירותים. לוגים במסך. |
| `docker compose up -d --build` | כמו למעלה, ברקע (detached). |
| `docker compose down` | עוצר ומסיר את ה-containers. ה-volume `mongo_data` נשאר (הנתונים נשמרים). |
| `docker compose down -v` | עוצר ומסיר גם volumes (מחיקת נתוני MongoDB). |

אחרי `docker compose up --build`:
- **פרונט:** `http://localhost`
- **API:** `http://localhost:3000` (למשל `http://localhost:3000/api/v1/...`)

---

## וידוא שההגדרות תקינות

- **server/Dockerfile:** תואם ל-`package.json` — אין build step, רק `node server.js`; פורט 3000 תואם ל-`config.port` (או ל-`PORT` ב-env).
- **client/Dockerfile:** ה-build משתמש ב-`npm run build` (tsc + vite); `VITE_API_URL` מועבר כ-ARG/ENV בזמן build; ה-stage הסופי רק מגיש קבצים סטטיים עם nginx.
- **client/nginx.conf:** `root` ו-`try_files` תואמים למיקום ה-`dist` ולתצורת SPA.
- **docker-compose:** `MONGO_URI` משתמש ב-hostname `mongo`; `CLIENT_URL` ו-`SERVER_URL` מתאימים לגישה מהדפדפן (localhost); ה-client מקבל `VITE_API_URL` כ-build-arg; השרת מקבל משתנים מ-`.env` ו-`environment`.

אם משהו לא עובד (למשל 404 ב-routes של הפרונט, או השרת לא מתחבר ל-DB), אפשר לבדוק לוגים עם:
- `docker compose logs server`
- `docker compose logs mongo`
- `docker compose logs client`

---

סיום התיעוד.
