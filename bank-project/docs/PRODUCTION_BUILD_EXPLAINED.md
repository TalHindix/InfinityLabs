# מה קורה מאחורי הקלעים ב-Production Build

מסמך זה מסביר **מה בדיוק קורה** כשמריצים את הבנייה ב-production (למשל `docker compose up --build`), ואיך להסביר את זה לאחרים.

---

## הפקודה שאתה מריץ

```bash
docker compose up --build
```

`--build` אומר ל-Docker Compose: בנה מחדש את ה-images לפני ההרצה (אם יש שינויים בקוד או ב-Dockerfile).

---

## סדר הביצוע והשלבים

Docker Compose קורא את `docker-compose.yml` ומפעיל שלושה שירותים: **mongo**, **server**, **client**. התלות היא: client תלוי ב-server, server תלוי ב-mongo — אז הסדר הלוגי של בנייה/הרצה הוא mongo, אחריו server, אחריו client.

### 1. MongoDB (mongo)

- **אין build:** משתמשים ב-image מוכן `mongo:7` מ-Docker Hub.
- **מה קורה:** Container עולה עם MongoDB על פורט 27017, ונתונים נשמרים ב-volume `mongo_data` (כך שהמידע לא נמחק כשעוצרים את ה-containers).

### 2. Server (API)

**Build (מה קורה מאחורי הקלעים):**

| שלב | מה קורה |
|-----|---------|
| **FROM** | לוקחים image בסיסי `node:20-alpine` — גרסת Node קבועה, תמונה קטנה. |
| **WORKDIR** | מגדירים תיקיית עבודה `/app` בתוך ה-container. |
| **COPY** | מעתיקים רק `package.json` ו-`package-lock.json` (לא את כל הקוד). |
| **RUN npm ci --omit=dev** | מתקינים תלויות **רק production** — בלי devDependencies. תוצאה: image קטן יותר ובטוח יותר. |
| **COPY . .** | מעתיקים את שאר קוד השרת (לפי `.dockerignore` — בלי `node_modules`, בלי `.env`, בלי tests). |
| **EXPOSE 3000** | מצהירים שה-container מאזין על פורט 3000 (תיעוד, לא פותח פורט בחוץ לבד). |
| **CMD** |container רץ — `node server.js`. |

**Runtime:** ה-container מקבל מ-`docker-compose` את משתני הסביבה (למשל `NODE_ENV=production`, `MONGO_URI=mongodb://mongo:27017/Dubai-Bank`) — הסודות לא "אפויים" בתוך ה-image, רק מוזרקים בהרצה.

### 3. Client (פרונט)

ה-client נבנה ב-**multi-stage build** — שני שלבים בתמונה אחת.

**Stage 1 — Builder:**

| שלב | מה קורה |
|-----|---------|
| **FROM node:20-alpine AS builder** | שלב בנייה נפרד, עם Node. |
| **COPY package.json, package-lock.json** | מעתיקים קבצי ניהול תלויות. |
| **RUN npm ci** | מתקינים **כל** התלויות (כולל dev — צריך ל-Vite ו-TypeScript לבנייה). |
| **COPY . .** | מעתיקים את קוד הפרונט (לפי `.dockerignore`). |
| **ARG VITE_API_URL** | מקבלים מ-`docker-compose` את כתובת ה-API (למשל `http://localhost:3000/api/v1`) כ-build-time. |
| **RUN npm run build** | מריצים `tsc -b && vite build` — קומפילציית TypeScript ובניית bundle אופטימלי (minify, tree-shake) ל-`dist/`. |

**Stage 2 — Serve:**

| שלב | מה קורה |
|-----|---------|
| **FROM nginx:alpine** | image נפרד, קל — רק nginx. **לא** Node. |
| **COPY --from=builder /app/dist** | מעתיקים רק את תוצאת ה-build (`dist/`) משלב ה-builder לתוך ה-image הסופי. |
| **COPY nginx.conf** | מגדירים ל-nginx להגיש את הקבצים הסטטיים ו-`try_files` ל-SPA (כל route מחזיר `index.html`). |
| **CMD nginx** | ה-container הסופי רץ רק nginx — משרת HTML/JS/CSS על פורט 80. |

**למה שני שלבים:** ה-image הסופי לא מכיל Node, לא את קוד המקור, לא את ה-devDependencies — רק nginx + קבצי `dist/`. התוצאה: image קטן ובטוח יותר.

---

## סיכום זרימה (להסבר בעל פה)

1. **docker compose up --build** קורא את `docker-compose.yml`.
2. **mongo** — עולה מ-image מוכן, עם volume לנתונים.
3. **server** — נבנה מ-`server/Dockerfile`: Node 20, `npm ci --omit=dev`, העתקת קוד, הרצה עם `node server.js`; env מ-compose (לא ב-image).
4. **client** — נבנה מ-`client/Dockerfile`: שלב 1 — Node, `npm ci`, `npm run build` עם `VITE_API_URL`; שלב 2 — רק nginx + `dist/` מהשלב הראשון.
5. **רשת:** client על 80, server על 3000, mongo פנימי; client שולח בקשות ל-API לפי `VITE_API_URL` שהוגדר ב-build.

---

## למה זה טוב (נקודות להסבר)

| נושא | הסבר קצר |
|------|-----------|
| **עקביות** | אותה גרסת Node (20) ואותן תלויות (לפי lockfile) בכל מקום — מקומי ו-production (למשל Render). אין "עובד אצלי". |
| **בידוד** | כל שירות ב-container משלו; אין תלות במה מותקן על המחשב מלבד Docker. |
| **אבטחה** | `.env` לא נכנס ל-image (`.dockerignore` + env ב-runtime); רק production dependencies בשרת; ה-client רק קבצים סטטיים. |
| **גודל ותחזוקה** | Server בלי devDependencies; client ב-multi-stage בלי Node ב-image הסופי — images קטנים וברורים. |
| **התאמה ל-production** | ב-Render (או שרת אחר) מריצים container באותו אופן — בדיקה מקומית עם `docker compose up --build` משקפת את מה שיעלה שם. |
| **תיעוד** | ה-Dockerfile וה-compose מתעדים איך בונים ומריצים — כל מפתח יכול להריץ באותה צורה. |

---

## טבלת פקודות שימושיות

| מטרה | פקודה |
|------|--------|
| בנייה והרצה (production-like) | `docker compose up --build` |
| הרצה ברקע | `docker compose up --build -d` |
| עצירה | `docker compose down` |
| עצירה + מחיקת נתוני DB | `docker compose down -v` |
| לוגים | `docker compose logs -f` (או `logs -f server` / `client` / `mongo`) |

---

אם תצטרך להרחיב (למשל רק build בלי להריץ, או הסבר על Render), אפשר להפנות ל-`DOCKER_WHY_AND_MANUAL.md` ו-`FROM_LOCAL_TO_PRODUCTION.md`.
