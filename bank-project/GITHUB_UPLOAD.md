# העלאת הפרויקט ל-GitHub (עם גיבוי למה שרץ עכשיו)

## מה אנחנו עושים

1. **גיבוי** – שומרים עותק של מה שרץ כרגע ב-GitHub (למקרה שתצטרך לחזור).
2. **העלאה** – מעלים את הפרויקט הנוכחי (bank-project) לאותו repo במקום.

---

## שלב 1: גיבוי – שמירת מה שרץ עכשיו ב-GitHub

פתח טרמינל והרץ (החלף את `YOUR_GITHUB_USERNAME` ו-`YOUR_REPO_NAME` בכתובת האמיתית של ה-repo):

```bash
cd ~/Desktop/projects
git clone https://github.com/YOUR_GITHUB_USERNAME/YOUR_REPO_NAME.git bank-backup-$(date +%Y%m%d)
```

**דוגמה:** אם ה-repo שלך הוא `https://github.com/talhindi/dubai-bank`:

```bash
git clone https://github.com/talhindi/dubai-bank.git bank-backup-20250204
```

עכשיו יש לך תיקייה `bank-backup-20250204` (או עם התאריך של היום) עם **כל הקוד שרץ כרגע ב-GitHub**. אל תמחק אותה – זה הגיבוי.

---

## שלב 2: העלאת הפרויקט הנוכחי (bank-project) לאותו repo

### 2.1 כניסה לפרויקט ואתחול Git (פעם אחת)

```bash
cd /home/hindi/Desktop/projects/bank-project
git init
git branch -M main
git remote add origin https://github.com/YOUR_GITHUB_USERNAME/YOUR_REPO_NAME.git
```

(שוב – החלף ב-URL האמיתי של ה-repo.)

### 2.2 וידוא ש-.env לא יעלה

הקבצים `.env` כבר ממוקמים ב-`.gitignore` בתוך `server/` ו-`client/`. אם תרצה גם בשורש:

```bash
echo ".env" >> .gitignore
echo "*.local" >> .gitignore
```

### 2.3 קומיט והעלאה (דורס את מה שב-GitHub)

```bash
git add .
git status   # בדיקה no .env / node_modules
git commit -m "Full project: server + client refactor, README"
git push -u origin main --force
```

**חשוב:** `--force` מחליף את ההיסטוריה ב-GitHub במה שיש אצלך עכשיו. **בגלל שעשית גיבוי בשלב 1**, אם תצטרך את הקוד הישן – הוא בתיקיית הגיבוי.

---

## סיכום

| שלב | פעולה |
|-----|--------|
| 1 | `git clone <repo> bank-backup-<date>` – גיבוי של מה שרץ עכשיו |
| 2 | `cd bank-project` → `git init` → `remote add origin` → `add` → `commit` → `push --force` |

אחרי ה-push, ב-GitHub יהיה **רק** הפרויקט הנוכחי (bank-project). הגיבוי נשאר אצלך בתיקייה המקומית.

---

## אם ה-repo כבר מחובר (כבר יש .git ב-bank-project)

אם מתישהו הפרויקט כבר היה repo עם אותו origin:

```bash
cd /home/hindi/Desktop/projects/bank-project
git add .
git status
git commit -m "Full project: server + client refactor, README"
git push origin main --force
```

גם כאן – קודם עשה גיבוי עם `git clone` כמו בשלב 1.
