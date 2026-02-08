# למה Docker תורם ואיך להשתמש בלי סקריפטים

מסמך זה מסביר **למה** Docker משמש בפרויקט ונותן **פקודות ידניות** בלבד (בלי סקריפטים) — build, run, בדיקות וניקוי.

---

## חלק 1: למה Docker תורם

### עקביות סביבה

- **בלי Docker:** כל מחשב עם Node מותקן יכול להריץ גרסאות שונות (Node 18 vs 20, גרסאות שונות של חבילות). "עובד אצלי" לא תמיד עובד בשרת.
- **עם Docker:** ה-Dockerfile קובע בדיוק — Node 20 Alpine, `npm ci` לפי ה-lockfile. **אותה תמונה** מקומית וב-Render: אותה גרסת Node, אותן תלויות. פחות הפתעות ב-production.

### בידוד וניקיון

- ה-API רץ **בתוך container** — מערכת הקבצים והרשת מנותקות מהמחשב. אין תלות ב-"מה מותקן על המחשב" חוץ מ-Docker.
- משתני סביבה (JWT_SECRET, MONGO_URI) **לא נכנסים ל-image** — מועברים רק בהרצה. הסודות נשארים ב-.env או ב-Dashboard של Render.

### התאמה ל-production (Render)

- ב-Render השרת רץ כ-**Docker container**. כשאתה בונה ומריץ את **אותו** Dockerfile מקומית, אתה בודק בדיוק את מה שיעלה ב-Render — חוץ מ-env ו-DB.
- בדיקה מקומית (health, signup) מוכיחה שה-image תקין לפני push.

### סיכום תרומה

| תרומה | הסבר קצר |
|--------|-----------|
| **עקביות** | אותה גרסת Node ותלויות מקומית וב-Render. |
| **בידוד** | האפליקציה רצה במיכל נפרד, בלי לזהם את המחשב. |
| **תיעוד** | Dockerfile מתעד איך בונים ומריצים את השרת. |
| **התאמה ל-production** | Render מריץ container — בדיקה מקומית על אותו image מפחיתה תקלות. |

---

## חלק 2: שימוש ב-Docker בלי סקריפטים

כל השלבים עם **פקודות ידניות** בלבד. מריצים מהשורש של הפרויקט (איפה שנמצאים `server/` ו-`docker-compose.yml`).

---

### א. בדיקת image השרת לבד (server בלבד)

מתאים כשרוצים לוודא שה-image של השרת עולה ומתחבר ל-DB בלי להריץ את כל ה-compose.

**1. בניית ה-image**

```bash
docker build -t dubai-bank-server:test ./server
```

- `-t dubai-bank-server:test` — שם ותג ל-image.
- `./server` — תיקיית ה-build (ה-Dockerfile בתוך `server/`).

**2. הרצת container**

צריך קובץ `.env` (בשורש או `server/.env`) עם לפחות `JWT_SECRET` ו-`MONGO_URI`.

**MongoDB מקומי — Linux:**

```bash
docker run -d --name dubai-bank-server-test --network host \
  --env-file server/.env \
  -e MONGO_URI=mongodb://localhost:27017/Dubai-Bank \
  dubai-bank-server:test
```

**MongoDB מקומי — Mac/Windows:**

```bash
docker run -d --name dubai-bank-server-test -p 3000:3000 \
  --env-file server/.env \
  -e MONGO_URI=mongodb://host.docker.internal:27017/Dubai-Bank \
  dubai-bank-server:test
```

**MongoDB Atlas:**

```bash
docker run -d --name dubai-bank-server-test -p 3000:3000 \
  --env-file server/.env \
  dubai-bank-server:test
```

- `-d` — רץ ברקע.
- `--name dubai-bank-server-test` — שם ל-container (נוח ללוגים ולעצירה).
- `-p 3000:3000` — map פורט 3000 במחשב לפורט 3000 ב-container (לא צריך ב-`--network host`).

**3. בדיקת health**

```bash
curl -s http://localhost:3000/health
```

מצופה: `{"status":"ok","timestamp":"..."}`.

**4. בדיקת API (למשל signup)**

```bash
curl -s -X POST http://localhost:3000/api/v1/auth/signup \
  -H "Content-Type: application/json" \
  -d '{"firstName":"Test","lastName":"User","email":"test@example.com","phone":"+972500000000","password":"TestPass123!"}'
```

מצופה: תשובה 201.

**5. צפייה בלוגים**

```bash
docker logs dubai-bank-server-test
```

**6. עצירה ומחיקת ה-container**

```bash
docker stop dubai-bank-server-test
docker rm dubai-bank-server-test
```

ה-image `dubai-bank-server:test` נשאר; אפשר להריץ שוב עם `docker run` כמו למעלה.

---

### ב. הרצת כל המערכת (server + client + mongo) עם docker-compose

מתאים להרצה מקומית "כמו production" — קליינט build + nginx, שרת, MongoDB ב-container.

**1. קובץ `.env` בשורש**

לפחות `JWT_SECRET`. אפשר להעתיק מ-`server/.env.example`. ה-compose ידרוס `MONGO_URI` ל-`mongodb://mongo:27017/Dubai-Bank` ו-`CLIENT_URL`/`SERVER_URL` ל-localhost.

**2. הרצה**

```bash
docker compose up --build
```

- `--build` — בונה מחדש images אם צריך.
- בונה ומפעיל: mongo, server, client. לוגים במסך.

**3. גישה**

- פרונט: `http://localhost`
- API: `http://localhost:3000`

**4. עצירה**

`Ctrl+C` ואז:

```bash
docker compose down
```

נתוני MongoDB נשמרים ב-volume `mongo_data`. מחיקת volume (מחיקת DB):

```bash
docker compose down -v
```

---

### פקודות שימושיות (ללא סקריפטים)

| מטרה | פקודה |
|------|--------|
| בניית image השרת | `docker build -t dubai-bank-server:test ./server` |
| רשימת images | `docker images` |
| רשימת containers שרצים | `docker ps` |
| לוגים של container | `docker logs <container-name>` |
| עצירת container | `docker stop <container-name>` |
| מחיקת container | `docker rm <container-name>` |
| מחיקת image | `docker rmi dubai-bank-server:test` |
| הרצת כל המערכת | `docker compose up --build` |
| עצירת compose | `docker compose down` |

---

## סיכום

- **למה Docker:** עקביות, בידוד, תיעוד, והתאמה ל-Render (אותו אופן הרצה).
- **איך בלי סקריפטים:** build עם `docker build`, run עם `docker run` (או `docker compose up`), בדיקה עם `curl /health` ו-`curl .../signup`, ניקוי עם `docker stop` + `docker rm` או `docker compose down`.
