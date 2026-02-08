# סביבת פיתוח (Dev) מול Production

---

## Dev מול Prod — מתי מה

| | **Development (פיתוח יומי)** | **Production** |
|---|-----------------------------|----------------|
| **Client** | Vite dev server מקומי (port 5173), hot reload | Vercel — build סטטי |
| **Server** | Node מקומי (`npm run dev`), hot reload | Render — **Docker** (image מ-Dockerfile) |
| **MongoDB** | מקומי (localhost) או Atlas (cluster נפרד ל-dev) | Atlas |
| **Docker** | לא חובה; משמש רק לבדיקה "כמו production" | רק השרת ב-Render רץ כ-container |

**בפיתוח** רצים בלי Docker: שרת וקליינט על המחשב עם hot reload. **ב-production** רק השרת עולה כ-Docker ב-Render; הקליינט ב-Vercel וה-DB ב-Atlas.

---

## איך להריץ סביבת Dev (מקומי)

### דרישות

- Node.js מותקן
- MongoDB רץ מקומית (פורט 27017) או connection string של Atlas ל-dev

### 1. שרת (API)

```bash
cd server
cp .env.example .env
# ערוך .env: JWT_SECRET (חובה), MONGO_URI (localhost או Atlas)
npm install
npm run dev
```

השרת ירוץ על `http://localhost:3000` עם `--watch` (ריענון אוטומטי בשינוי קוד).

### 2. קליינט (React)

```bash
cd client
npm install
npm run dev
```

Vite ירוץ על `http://localhost:5173`.  
ב-dev הקליינט משתמש ב-`VITE_API_URL` מ-`client/.env.development` (למשל `/api/v1`) — אז הבקשות הולכות ל-`/api` ו-Vite מפרוקס ל-`http://localhost:3000` (ראה `client/vite.config.ts`).

### 3. גישה מהדפדפן

פתח `http://localhost:5173`. האפליקציה תדבר עם השרת המקומי; אין צורך ב-Docker.

---

## מתי כן להשתמש ב-Docker בפיתוח

- **לבדוק שה-image של השרת עובד** לפני דחיפה ל-Render — השתמש בסקריפט:  
  `./scripts/test-docker-server.sh`  
  (ראה [LOCAL_DOCKER_TEST.md](LOCAL_DOCKER_TEST.md)).
- **להריץ את כל המערכת "כמו production" מקומית** (client build + nginx, server, mongo):  
  `docker compose up --build`  
  (ראה [DOCKER.md](DOCKER.md)).

סיכום: **Dev = הרצה מקומית בלי Docker; Docker = לבדיקות ו-production.**
