# תיעוד — Docker, Deploy ופיתוח

כל קבצי ההסבר על Docker, Production וסביבת פיתוח מרוכזים כאן. קרא לפי הצורך.

---

## מפת קבצים

| קובץ | מתי לקרוא | תוכן בקצרה |
|------|------------|-------------|
| **[DEV_WORKFLOW.md](DEV_WORKFLOW.md)** | פיתוח יומי | איך להריץ **Dev** (שרת + קליינט מקומית, בלי Docker). מתי להשתמש ב-Docker. |
| **[DOCKER.md](DOCKER.md)** | Docker מקומי (compose) | הסבר על כל קבצי Docker (Dockerfile, .dockerignore, nginx, docker-compose). איך להריץ `docker compose up`. |
| **[LOCAL_DOCKER_TEST.md](LOCAL_DOCKER_TEST.md)** | לפני deploy ל-Render | בדיקת image השרת מקומית: סקריפט או פקודות ידניות, health + signup. |
| **[FROM_LOCAL_TO_PRODUCTION.md](FROM_LOCAL_TO_PRODUCTION.md)** | מעבר ל-production | מה לעשות אחרי שהבדיקה המקומית עברה; הסבר על image, סקריפט ו-Render. |
| **[DEPLOY.md](DEPLOY.md)** | הגדרות production | משתני סביבה ב-Vercel וב-Render, CORS ו-cookies, Atlas. |
| **[DOCKER_RENDER_CHECKLIST.md](DOCKER_RENDER_CHECKLIST.md)** | וידוא לפני deploy | רשימת בדיקות: Dockerfile, .dockerignore, /health, env, CORS, start script. |
| **[DOCKER_WHY_AND_MANUAL.md](DOCKER_WHY_AND_MANUAL.md)** | למה Docker ואיך בלי סקריפטים | תרומות Docker (עקביות, בידוד, production); פקודות ידניות — build, run, health, signup, compose, ניקוי. |

---

## Dev מול Prod (תזכורת)

- **Dev:** שרת ו-client על המחשב (`npm run dev`), MongoDB מקומי או Atlas. **ללא Docker** — ראה [DEV_WORKFLOW.md](DEV_WORKFLOW.md).
- **Prod:** Client ב-Vercel, שרת ב-Render (**Docker**), DB ב-Atlas. ראה [DEPLOY.md](DEPLOY.md).

קבצי ה-Docker (בתיקיית הפרויקט) משמשים: (1) בדיקה מקומית של השרת — [LOCAL_DOCKER_TEST.md](LOCAL_DOCKER_TEST.md); (2) הרצה מקומית "כמו production" עם compose — [DOCKER.md](DOCKER.md); (3) build ב-Render — [DEPLOY.md](DEPLOY.md), [FROM_LOCAL_TO_PRODUCTION.md](FROM_LOCAL_TO_PRODUCTION.md).
