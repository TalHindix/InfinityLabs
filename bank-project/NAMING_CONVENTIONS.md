# קונבנציית שמות קבצים – Server & Client
**תאריך:** פברואר 2026

---

## סיכום ביצוע

בוצעו התאמות לשמות קבצים כדי שיהיו **ברורים ועקביים**:

| מיקום | קובץ ישן | קובץ חדש | סיבה |
|--------|----------|----------|------|
| Server | `openai-intent.service.js` | `openaiIntent.service.js` | קונבנציה: camelCase (לא kebab-case) כמו שאר הקבצים |
| Server | `socket-auth.middleware.js` | `socketAuth.middleware.js` | אותה סיבה |
| Client | `TransactionPage.tsx` | `TransferPage.tsx` | העמוד הוא Transfer (העברה), לא רשימת Transactions |
| Client | `TransactionPage.styles.ts` | `TransferPage.styles.ts` | תואם ל-TransferPage |

---

## Server – קונבנציה נוכחית

- **פורמט:** `name.type.js` (camelCase + סיומת לפי תפקיד).
- **תיקיות:** `config`, `constants`, `controllers`, `middleware`, `models`, `routes`, `services`, `socket`, `utils`.

| סוג | דוגמאות | הערות |
|-----|---------|--------|
| Controllers | `auth.controller.js`, `transaction.controller.js`, `user.controller.js` | ✅ עקבי |
| Routes | `auth.routes.js`, `transaction.routes.js`, `user.routes.js` | ✅ עקבי |
| Models | `transaction.model.js`, `user.model.js` | ✅ עקבי |
| Middleware | `auth.middleware.js`, `error.middleware.js`, `logger.middleware.js`, `rateLimit.middleware.js`, `socketAuth.middleware.js` | ✅ כולם camelCase |
| Services | `chatbot.service.js`, `openaiIntent.service.js`, `transaction.service.js`, `user.service.js` | ✅ כולם camelCase |
| Utils | `email.util.js`, `generate.util.js`, `jwt.util.js`, `logger.util.js`, `response.util.js`, `validation.util.js` | ✅ עקבי |
| Config | `database.js`, `index.js` | ✅ |

**שורש הפרויקט:** `server.js`, `app.js` – שמות מקובלים.

---

## Client – קונבנציה נוכחית

- **קומפוננטות:** `PascalCase.tsx` + `PascalCase.styles.ts`.
- **עמודים:** `PascalCasePage.tsx` + `PascalCasePage.styles.ts`.
- **Hooks:** `useCamelCase.ts`.
- **Services:** `camelCase.service.ts` או `domain.action.ts` (למשל `auth.service.ts`, `auth.storage.ts`).
- **Utils/Types:** `camelCase.ts` או `index.ts`.

| סוג | דוגמאות | הערות |
|-----|---------|--------|
| Components | `AppHeader.tsx`, `BalanceCard.tsx`, `ChatWidget.tsx`, `TransferForm.tsx` | ✅ PascalCase |
| Styles | `AppHeader.styles.ts`, `TransferPage.styles.ts` | ✅ תואם לשם הקומפוננטה/עמוד |
| Pages | `DashboardPage.tsx`, `LoginPage.tsx`, `SignupPage.tsx`, `TransferPage.tsx`, `TransactionsPage.tsx` | ✅ שם + Page |
| Hooks | `useDashboardData.ts`, `useLogin.ts`, `useTransfer.ts` | ✅ use + camelCase |
| Services | `auth.service.ts`, `auth.storage.ts`, `httpClient.ts`, `transactions.service.ts`, `user.service.ts` | ✅ |
| Context | `ThemeContext.tsx`, `ThemeContextProvider.tsx` | ✅ PascalCase |
| Router | `ProtectedRoute.tsx` | ✅ PascalCase |
| Utils | `cookies.ts`, `formatters.ts`, `greetings.ts`, `theme.ts`, `ui.ts` | ✅ camelCase |
| Styles (shared) | `styles/authForm.styles.ts` | ✅ camelCase |

---

## הערות אופציונליות (לא בוצעו)

1. **Server – `validation-test.js`**  
   נמצא בשורש הפרויקט. אם זה קובץ טסט, מקובל להעביר ל-`tests/` או ל-`__tests__/` ולשם כמו `validation.util.test.js` (או לפי קונבנציית הפרויקט).

2. **Client – `auth.service.ts` vs `authStorage.ts`**  
   כרגע: `auth.service.ts`, `auth.storage.ts` (דומיין + סיומת). אם תרצה עקביות מלאה עם camelCase כמו `user.service.ts`: `authService.ts`, `authStorage.ts`. דורש עדכון ייבואים.

---

## סיכום

- **Server:** כל קבצי ה-source עכשיו ב-**camelCase** (ללא מקפים).
- **Client:** שמות עמודים תואמים לתוכן – **TransferPage** לעמוד העברה, **TransactionsPage** לרשימת תנועות.
- **Build:** השרת והקליינט עוברים build בהצלחה לאחר השינויים.
