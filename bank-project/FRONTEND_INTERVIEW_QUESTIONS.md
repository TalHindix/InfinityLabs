# שאלות ראיון - Frontend (React/TypeScript)

קובץ זה מכיל שאלות שמראיין שלא מכיר את הקוד יכול לשאול לגבי ה-frontend, במיוחד לגבי hooks ודפוסים אחרים.

**הערה למראיין:** השאלות מבוססות על מה שניתן לראות במבנה הקבצים והקוד. כל שאלה כוללת הקשר קצר על מה שנראה בקוד.

---

## Custom Hooks - שאלות כלליות

### 1. Custom Hooks Pattern

**הקשר:** רואים במערכת מספר custom hooks כמו `useLogin`, `useSignup`, `useTransfer`, `useAsyncOperation`.

**שאלה:** איך אתה מחליט מתי ליצור custom hook? מה ההבדל בין custom hook לפונקציה רגילה?

**מה לבדוק בתשובה:**
- הבנה של React hooks rules
- הבנה של separation of concerns
- דוגמאות לשימוש ב-hooks במערכת

**שאלות המשך אפשריות:**
- מתי לא ליצור custom hook?
- מה ה-advantages של custom hooks?
- איך אתה בודק אם hook עובד נכון?

---

### 2. useAsyncOperation Hook

**הקשר:** רואים hook בשם `useAsyncOperation` ב-`client/src/shared/useAsyncOperation.ts` שמשמש hooks אחרים.

**שאלה:** אני רואה hook בשם `useAsyncOperation` שמשמש hooks אחרים. מה לדעתך הוא עושה? למה ליצור hook כזה?

**מה לבדוק בתשובה:**
- הבנה של abstraction ו-reusability
- הבנה של async operations management
- יכולת לקרוא קוד ולהבין את ה-purpose

**שאלות המשך אפשריות:**
- איך היית משפר את ה-hook הזה?
- מה ה-advantages של גישה כזו?
- מתי לא להשתמש ב-hook כזה?

---

### 3. Form State Management

**הקשר:** רואים hooks כמו `useLogin`, `useSignup`, `useTransfer` שמנהלים form state.

**שאלה:** איך אתה מנהל state של forms? אני רואה hooks שמנהלים form fields - מה הגישה שלך?

**מה לבדוק בתשובה:**
- הבנה של controlled components
- הבנה של state management ב-forms
- דוגמאות מהקוד (אם הוא מסתכל)

**שאלות המשך אפשריות:**
- למה לא להשתמש ב-Formik או React Hook Form?
- איך אתה מטפל ב-validation?
- מה קורה אם ה-form גדול מאוד?

---

### 4. Async Operations in Hooks

**הקשר:** רואים hooks שמבצעים API calls, כמו `useDashboardData`, `useTransactions`.

**שאלה:** איך אתה מטפל ב-async operations ב-hooks? מה הגישה שלך לניהול loading ו-error states?

**מה לבדוק בתשובה:**
- הבנה של useEffect עם async functions
- הבנה של error handling
- הבנה של loading states
- דוגמאות מהקוד

**שאלות המשך אפשריות:**
- מה קורה אם component unmount לפני שה-request מסתיים?
- איך אתה מונע memory leaks?
- מה ההבדל בין גישות שונות?

---

### 5. Data Fetching Patterns

**הקשר:** רואים hooks כמו `useDashboardData` ו-`useTransactions` שטוענים נתונים.

**שאלה:** אני רואה hooks שטוענים נתונים. מה הגישה שלך ל-data fetching? מתי להשתמש ב-useEffect vs אחר?

**מה לבדוק בתשובה:**
- הבנה של data fetching patterns
- הבנה של useEffect dependencies
- הבנה של cleanup
- דוגמאות מהקוד

**שאלות המשך אפשריות:**
- למה להשתמש ב-`cancelled` flag?
- מה ההבדל בין גישות שונות?
- איך היית משפר את ה-data fetching?

---

## Built-in Hooks - שאלות כלליות

### 6. useState Usage

**הקשר:** רואים שימוש נרחב ב-`useState` ב-hooks שונים.

**שאלה:** מתי אתה משתמש ב-`useState`? מה ה-best practices שלך?

**מה לבדוק בתשובה:**
- הבנה של useState
- מתי להשתמש ב-useState vs alternatives
- דוגמאות מהקוד

**שאלות המשך אפשריות:**
- מה ההבדל בין `useState` ל-`useRef`?
- מתי להשתמש ב-`useReducer` במקום?
- איך אתה מחליט כמה state variables ליצור?

---

### 7. useEffect Patterns

**הקשר:** רואים שימוש ב-`useEffect` ב-hooks שונים, עם cleanup functions.

**שאלה:** איך אתה משתמש ב-`useEffect`? מתי צריך cleanup function?

**מה לבדוק בתשובה:**
- הבנה של useEffect lifecycle
- הבנה של dependencies array
- הבנה של cleanup
- דוגמאות מהקוד

**שאלות המשך אפשריות:**
- מה קורה אם לא נקה resources?
- מתי להשתמש ב-empty dependencies array?
- איך אתה מונע infinite loops?

---

### 8. useCallback and useMemo

**הקשר:** רואים שימוש ב-`useCallback` ב-`useAsyncOperation`.

**שאלה:** מתי אתה משתמש ב-`useCallback` או `useMemo`? מה ה-benefits?

**מה לבדוק בתשובה:**
- הבנה של performance optimization
- הבנה של referential equality
- מתי זה באמת עוזר

**שאלות המשך אפשריות:**
- מתי `useCallback` לא עוזר?
- מה ההבדל בין `useCallback` ל-`useMemo`?
- איך אתה יודע אם צריך optimization?

---

### 9. useRef Usage

**הקשר:** רואים שימוש ב-`useRef` ב-`ChatAssistant` (socket) וב-`useTransactions` (loadingRef).

**שאלה:** מתי אתה משתמש ב-`useRef`? מה ההבדל בינו לבין `useState`?

**מה לבדוק בתשובה:**
- הבנה של useRef
- מתי להשתמש ב-useRef
- דוגמאות מהקוד

**שאלות המשך אפשריות:**
- למה להשתמש ב-`useRef` ל-socket?
- מתי צריך `useRef` ל-DOM elements?
- מה ההבדל בין useRef ל-regular variable?

---

### 10. useContext and Context API

**הקשר:** רואים `ThemeContext` ו-`ThemeContextProvider` במערכת.

**שאלה:** איך אתה משתמש ב-Context API? מתי להשתמש בו?

**מה לבדוק בתשובה:**
- הבנה של Context API
- מתי להשתמש ב-Context
- הבנה של provider pattern

**שאלות המשך אפשריות:**
- מה ה-disadvantages של Context?
- מתי לא להשתמש ב-Context?
- איך אתה מונע re-renders מ-Context?

---

## State Management

### 11. Local vs Global State

**הקשר:** רואים state management ב-hooks (local) וב-Context (global).

**שאלה:** איך אתה מחליט מתי להשתמש ב-local state vs global state?

**מה לבדוק בתשובה:**
- הבנה של state management patterns
- מתי להשתמש בכל גישה
- דוגמאות מהקוד

**שאלות המשך אפשריות:**
- מתי להשתמש ב-Redux או Zustand?
- מה ה-advantages של כל גישה?
- איך אתה מונע prop drilling?

---

### 12. Auth State Management

**הקשר:** רואים `authStorage` שמשתמש ב-localStorage.

**שאלה:** איך אתה מנהל auth state? למה localStorage ולא Context?

**מה לבדוק בתשובה:**
- הבנה של auth state management
- הבנה של persistence
- הבנה של security considerations

**שאלות המשך אפשריות:**
- למה לא לשמור JWT ב-localStorage?
- איך components יודעים על שינוי ב-auth?
- מה קורה אם localStorage נמחק?

---

### 13. URL State Management

**הקשר:** רואים שימוש ב-`useSearchParams` ב-`useTransactions` ו-`useLogin`.

**שאלה:** מתי אתה מנהל state ב-URL? מה ה-advantages?

**מה לבדוק בתשובה:**
- הבנה של URL state management
- מתי להשתמש ב-URL state
- הבנה של React Router

**שאלות המשך אפשריות:**
- מה ההבדל בין URL state ל-component state?
- מתי לא להשתמש ב-URL state?
- איך אתה מטפל ב-invalid URL params?

---

## API & Data Fetching

### 14. HTTP Client Setup

**הקשר:** רואים `httpClient` ב-`client/src/api/http-client.ts` עם interceptors.

**שאלה:** איך אתה מגדיר HTTP client? מה ה-interceptors עושים?

**מה לבדוק בתשובה:**
- הבנה של axios setup
- הבנה של interceptors
- הבנה של error handling

**שאלות המשך אפשריות:**
- למה להשתמש ב-interceptor?
- איך אתה מטפל ב-CORS?
- מה קורה אם ה-API URL לא מוגדר?

---

### 15. Error Handling Strategy

**הקשר:** רואים error handling ב-`useAsyncOperation` וב-services.

**שאלה:** איך אתה מטפל ב-errors ב-API calls? מה הגישה שלך?

**מה לבדוק בתשובה:**
- הבנה של error handling patterns
- הבנה של user experience
- דוגמאות מהקוד

**שאלות המשך אפשריות:**
- איך אתה מבדיל בין סוגי errors?
- מה קורה אם ה-network נכשל?
- איך אתה מציג errors למשתמש?

---

### 16. Loading States

**הקשר:** רואים loading states ב-hooks שונים.

**שאלה:** איך אתה מנהל loading states? מה הגישה שלך?

**מה לבדוק בתשובה:**
- הבנה של loading state management
- הבנה של user experience
- דוגמאות מהקוד

**שאלות המשך אפשריות:**
- איך אתה מונע multiple loading indicators?
- מה ההבדל בין loading states שונים?
- איך אתה מטפל ב-skeleton screens?

---

## Routing

### 17. Protected Routes

**הקשר:** רואים `RequireAuth` component ב-`App.tsx` שמגן על routes.

**שאלה:** איך אתה מגן על routes? מה הגישה שלך?

**מה לבדוק בתשובה:**
- הבנה של route protection
- הבנה של authentication checks
- הבנה של React Router

**שאלות המשך אפשריות:**
- מה קורה אם המשתמש לא authenticated?
- איך אתה מטפל ב-redirects?
- מה ההבדל בין `Navigate` ל-`redirect`?

---

### 18. Route Structure

**הקשר:** רואים routes שונים ב-`App.tsx` - login, signup, dashboard, transfer, transactions.

**שאלה:** איך אתה מארגן routes? מה המבנה שלך?

**מה לבדוק בתשובה:**
- הבנה של routing structure
- הבנה של nested routes
- הבנה של code organization

**שאלות המשך אפשריות:**
- איך אתה מטפל ב-404?
- מתי להשתמש ב-nested routes?
- איך אתה מארגן route guards?

---

## Components & Patterns

### 19. Component Structure

**הקשר:** רואים מבנה של `components/`, `screens/`, `hooks/`.

**שאלה:** איך אתה מארגן components? מה המבנה שלך?

**מה לבדוק בתשובה:**
- הבנה של component organization
- הבנה של separation of concerns
- הבנה של folder structure

**שאלות המשך אפשריות:**
- מתי ליצור component חדש?
- מה ההבדל בין component ל-screen?
- איך אתה מחליט מה לשים ב-hook?

---

### 20. Reusable Components

**הקשר:** רואים components כמו `ChatAssistant`, `TransferSuccessDialog`.

**שאלה:** איך אתה מחליט מה לעשות reusable? מה הגישה שלך?

**מה לבדוק בתשובה:**
- הבנה של reusability
- הבנה של component design
- דוגמאות מהקוד

**שאלות המשך אפשריות:**
- מתי לא לעשות component reusable?
- איך אתה מטפל ב-variations?
- מה ההבדל בין component ל-utility function?

---

### 21. Form Components

**הקשר:** רואים form components כמו `LoginForm`, `SignupForm`, `TransferForm`.

**שאלה:** איך אתה בונה form components? מה הגישה שלך?

**מה לבדוק בתשובה:**
- הבנה של form handling
- הבנה של controlled components
- הבנה של validation

**שאלות המשך אפשריות:**
- למה לא להשתמש ב-form library?
- איך אתה מטפל ב-validation?
- מה קורה אם ה-form מורכב מאוד?

---

## Performance

### 22. Optimization Techniques

**הקשר:** רואים שימוש ב-`useCallback`, `useRef`, cleanup functions.

**שאלה:** אילו optimization techniques אתה משתמש? מה הגישה שלך?

**מה לבדוק בתשובה:**
- הבנה של React optimization
- הבנה של performance patterns
- דוגמאות מהקוד

**שאלות המשך אפשריות:**
- מתי להשתמש ב-`React.memo`?
- מה ההבדל בין `useMemo` ל-`useCallback`?
- איך אתה מודד performance?

---

### 23. Memory Leaks Prevention

**הקשר:** רואים cleanup functions ב-`useEffect` hooks, `cancelled` flags.

**שאלה:** איך אתה מונע memory leaks? מה הגישה שלך?

**מה לבדוק בתשובה:**
- הבנה של memory leaks
- הבנה של cleanup patterns
- דוגמאות מהקוד

**שאלות המשך אפשריות:**
- מה קורה אם לא נקה resources?
- מה ההבדל בין `cancelled` flag ל-`AbortController`?
- איך אתה מזהה memory leaks?

---

### 24. Re-render Optimization

**הקשר:** רואים שימוש ב-`useCallback`, `useRef` למניעת re-renders.

**שאלה:** איך אתה מונע unnecessary re-renders? מה הגישה שלך?

**מה לבדוק בתשובה:**
- הבנה של React rendering
- הבנה של optimization techniques
- דוגמאות מהקוד

**שאלות המשך אפשריות:**
- מתי re-render הוא בעיה?
- איך אתה יודע אם יש בעיית performance?
- מה ה-tools שלך ל-debugging?

---

## Advanced Topics

### 25. Socket.IO Integration

**הקשר:** רואים `ChatAssistant` שמשתמש ב-Socket.IO.

**שאלה:** איך אתה מטפל ב-Socket.IO connections? מה הגישה שלך?

**מה לבדוק בתשובה:**
- הבנה של Socket.IO
- הבנה של connection management
- הבנה של cleanup

**שאלות המשך אפשריות:**
- למה להשתמש ב-`useRef` ל-socket?
- איך אתה מטפל ב-reconnection?
- מה קורה אם ה-connection נכשל?

---

### 26. Event-Driven Architecture

**הקשר:** רואים `AUTH_CHANGE_EVENT` ב-`authStorage` ו-`ChatAssistant`.

**שאלה:** מתי אתה משתמש ב-custom events? מה ה-advantages?

**מה לבדוק בתשובה:**
- הבנה של event-driven patterns
- הבנה של decoupling
- דוגמאות מהקוד

**שאלות המשך אפשריות:**
- למה לא להשתמש ב-Context?
- מה ה-disadvantages של events?
- מתי להשתמש ב-events vs state?

---

### 27. TypeScript Usage

**הקשר:** רואים שימוש נרחב ב-TypeScript במערכת.

**שאלה:** איך אתה משתמש ב-TypeScript? מה ה-benefits?

**מה לבדוק בתשובה:**
- הבנה של TypeScript
- הבנה של type safety
- דוגמאות מהקוד

**שאלות המשך אפשריות:**
- מתי להשתמש ב-`interface` vs `type`?
- איך אתה מטפל ב-unknown types?
- מה ה-limitations של TypeScript?

---

## Testing

### 28. Testing Hooks

**הקשר:** רואים test files ב-`__tests__/hooks/`.

**שאלה:** איך אתה בודק hooks? מה הגישה שלך?

**מה לבדוק בתשובה:**
- הבנה של testing hooks
- הבנה של testing libraries
- הבנה של test patterns

**שאלות המשך אפשריות:**
- איך אתה mock API calls?
- מה אתה בודק ב-unit tests?
- מתי לכתוב integration tests?

---

### 29. Testing Components

**הקשר:** רואים test files ל-components שונים.

**שאלה:** איך אתה בודק components? מה הגישה שלך?

**מה לבדוק בתשובה:**
- הבנה של component testing
- הבנה של testing libraries
- הבנה של user-centric testing

**שאלות המשך אפשריות:**
- מה ההבדל בין unit tests ל-integration tests?
- איך אתה בודק user interactions?
- מה ה-coverage שאתה שואף אליו?

---

## Debugging & Problem Solving

### 30. Debugging Hooks

**שאלה:** איך אתה debug בעיה ב-hook? תן דוגמה.

**מה לבדוק בתשובה:**
- הבנה של debugging tools
- הבנה של React DevTools
- יכולת לפתור בעיות

**שאלות המשך אפשריות:**
- מה אתה עושה אם hook לא עובד?
- איך אתה מוצא memory leak?
- מה ה-tools שלך ל-debugging?

---

### 31. Code Review

**שאלה:** אם היית צריך לבדוק את הקוד הזה, מה היית מחפש?

**מה לבדוק בתשובה:**
- הבנה של code quality
- הבנה של best practices
- יכולת לזהות בעיות

**שאלות המשך אפשריות:**
- מה ה-red flags שאתה מחפש?
- איך אתה בודק performance?
- מה ה-security concerns?

---

### 32. Refactoring

**שאלה:** אם היית צריך לשפר את הקוד הזה, מה היית משנה?

**מה לבדוק בתשובה:**
- יכולת לזהות improvement opportunities
- הבנה של refactoring patterns
- יכולת להציע פתרונות

**שאלות המשך אפשריות:**
- מה ה-bottlenecks שאתה רואה?
- איך היית משפר את ה-performance?
- מה ה-features שהיית מוסיף?

---

## Architecture & Design

### 33. Scalability

**שאלה:** איך הקוד הזה יתמודד עם גדילה? מה היית משנה?

**מה לבדוק בתשובה:**
- הבנה של scalability
- הבנה של architecture patterns
- יכולת לחשוב על עתיד

**שאלות המשך אפשריות:**
- מה ה-limitations שאתה רואה?
- איך היית מארגן את הקוד אם היה גדול יותר?
- מה ה-architecture decisions שהיית משנה?

---

### 34. Code Organization

**שאלה:** מה הגישה שלך לארגון קוד? מה העקרונות?

**מה לבדוק בתשובה:**
- הבנה של code organization
- הבנה של design principles
- יכולת להסביר החלטות

**שאלות המשך אפשריות:**
- מתי ליצור file חדש?
- איך אתה מחליט איפה לשים function?
- מה ה-conventions שלך ל-naming?

---

### 35. Best Practices

**שאלה:** מה ה-best practices שלך ל-React development?

**מה לבדוק בתשובה:**
- הבנה של React best practices
- הבנה של industry standards
- יכולת להסביר החלטות

**שאלות המשך אפשריות:**
- מה ה-common mistakes שאתה רואה?
- איך אתה שומר על code quality?
- מה ה-resources שאתה משתמש בהם?

---

## שאלות פתוחות / סיטואציות

### 36. Code Walkthrough

**שאלה:** בוא נסתכל על hook מסוים. תסביר מה הוא עושה ואיך הוא עובד.

**מה לבדוק בתשובה:**
- יכולת לקרוא קוד
- הבנה של React patterns
- יכולת להסביר קוד

**דוגמאות לקבצים לבדיקה:**
- `client/src/shared/useAsyncOperation.ts`
- `client/src/screens/login-signup/useLogin.ts`
- `client/src/screens/transfer-money/useTransfer.ts`
- `client/src/components/ChatAssistant.tsx`

---

### 37. Live Coding

**שאלה:** איך היית מוסיף feature חדש? למשל, hook חדש ל-feature מסוים.

**מה לבדוק בתשובה:**
- יכולת לכתוב קוד
- הבנה של patterns
- יכולת לחשוב על edge cases

**דוגמאות לנושאים:**
- הוספת hook חדש
- שיפור hook קיים
- תיקון bug

---

### 38. Problem Solving

**שאלה:** יש בעיה ב-production - hook לא עובד כמו שצריך. איך אתה מתמודד?

**מה לבדוק בתשובה:**
- יכולת לפתור בעיות
- הבנה של debugging
- יכולת לחשוב על solutions

**שאלות המשך אפשריות:**
- איך אתה מזהה את הבעיה?
- מה ה-steps שלך לפתרון?
- איך אתה מונע את זה בעתיד?

---

## סיכום

קובץ זה מכיל שאלות שמראיין יכול לשאול על:
- Custom hooks ו-built-in hooks
- State management
- API calls ו-error handling
- Routing
- Components patterns
- Performance
- Best practices
- Advanced topics
- Debugging ו-problem solving

**הערות למראיין:**
- כל שאלה כוללת הקשר קצר על מה שנראה בקוד
- השאלות פתוחות ומאפשרות למועמד להסביר את הגישה שלו
- אפשר להתאים את השאלות לרמה הנדרשת
- מומלץ לשלב שאלות פתוחות עם שאלות טכניות ספציפיות
- אפשר לבקש מהמועמד להסתכל על קוד ספציפי ולהסביר אותו

**טיפים לראיון:**
- התחל בשאלות כלליות ותעבור לספציפיות
- בקש דוגמאות מהקוד
- שאל על trade-offs והחלטות
- בדוק הבנה של best practices
- תן למועמד להסביר את החשיבה שלו
