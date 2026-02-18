# תשובות מפורטות לשאלות ראיון - Frontend (React/TypeScript)

קובץ זה מכיל תשובות מפורטות ומסודרות לכל השאלות, מניח שאין ידע מוקדם.

---

## Custom Hooks - שאלות כלליות

### 1. Custom Hooks Pattern

**שאלה:** איך אתה מחליט מתי ליצור custom hook? מה ההבדל בין custom hook לפונקציה רגילה?

**תשובה מפורטת:**

#### מה זה Custom Hook?

Custom hook הוא פונקציה ב-React שמתחילה ב-`use` ויכולה להשתמש ב-built-in hooks של React (כמו `useState`, `useEffect`).

#### ההבדל בין Custom Hook לפונקציה רגילה:

1. **Custom Hook:**
   - מתחיל ב-`use` (convention של React)
   - יכול להשתמש ב-hooks אחרים (`useState`, `useEffect`, וכו')
   - יכול לנהל state ו-side effects
   - משתף לוגיקה בין components

2. **פונקציה רגילה:**
   - לא יכולה להשתמש ב-hooks
   - לא מנהלת state
   - רק מבצעת חישובים או פעולות

#### מתי ליצור Custom Hook?

**ליצור Custom Hook כאשר:**
- יש לוגיקה שחוזרת על עצמה ב-components שונים
- צריך לנהל state מורכב
- צריך לשלב מספר hooks יחד
- רוצים להפריד בין לוגיקה ל-UI

**דוגמה מהקוד:**
```typescript
// useLogin.ts - Custom hook שמנהל את כל הלוגיקה של login
export const useLogin = () => {
  const [email, setEmail] = useState('');
  const [password, setPassword] = useState('');
  // ... עוד לוגיקה
  
  return { email, password, handleSubmit, ... };
};

// LoginForm.tsx - Component שמשתמש ב-hook
const LoginForm = () => {
  const { email, password, handleSubmit } = useLogin();
  // רק UI, לא לוגיקה
};
```

**מתי לא ליצור Custom Hook:**
- אם הלוגיקה פשוטה מאוד (רק חישוב אחד)
- אם הלוגיקה משתמשת רק ב-component אחד
- אם זה רק utility function (לא צריך hooks)

#### ה-Advantages של Custom Hooks:

1. **Reusability** - אפשר להשתמש שוב ושוב
2. **Separation of Concerns** - לוגיקה נפרדת מ-UI
3. **Testability** - קל יותר לבדוק hooks בנפרד
4. **Readability** - הקוד יותר נקי וקריא

---

### 2. useAsyncOperation Hook

**שאלה:** אני רואה hook בשם `useAsyncOperation` שמשמש hooks אחרים. מה לדעתך הוא עושה? למה ליצור hook כזה?

**תשובה מפורטת:**

#### מה ה-Hook עושה?

`useAsyncOperation` הוא hook כללי שמטפל ב-**async operations** (פעולות אסינכרוניות) כמו קריאות API.

#### מה הוא מספק:

1. **Loading State** - מצב שמציין אם הפעולה רצה
2. **Error State** - מצב שמציין אם יש שגיאה
3. **Execute Function** - פונקציה שמבצעת את הפעולה
4. **SetError Function** - פונקציה להגדרת שגיאה ידנית

#### הקוד המלא:

```typescript
export const useAsyncOperation = (initialLoading = false) => {
  // State לניהול loading
  const [loading, setLoading] = useState(initialLoading);
  
  // State לניהול errors
  const [error, setError] = useState('');

  // הפונקציה שמבצעת את הפעולה
  const execute = useCallback(
    async (
      operation: () => Promise<any>,  // הפעולה האסינכרונית
      onSuccess?: (result: any) => void  // callback להצלחה
    ) => {
      setError('');  // מנקה שגיאות קודמות
      setLoading(true);  // מתחיל loading

      try {
        const result = await operation();  // מבצע את הפעולה
        
        // אם יש callback להצלחה, קורא לו
        if (onSuccess !== undefined) {
          onSuccess(result);
        }

        return { result: result, error: '' };
      } catch (err: unknown) {
        // אם יש שגיאה, שומר אותה
        const errorMessage = getErrorMessage(err);
        setError(errorMessage);
        return { result: undefined, error: errorMessage };
      } finally {
        setLoading(false);  // מסיים loading תמיד
      }
    },
    []
  );

  return {
    loading,    // האם הפעולה רצה
    error,      // הודעת שגיאה
    execute,    // פונקציה לביצוע פעולה
    setError,   // פונקציה להגדרת שגיאה ידנית
  };
};
```

#### למה ליצור Hook כזה?

**הבעיה שהוא פותר:**

בלי hook כזה, כל פעם שצריך לעשות API call, צריך לכתוב:
```typescript
const [loading, setLoading] = useState(false);
const [error, setError] = useState('');

const handleSubmit = async () => {
  setLoading(true);
  setError('');
  try {
    const result = await api.call();
    // עושה משהו עם התוצאה
  } catch (err) {
    setError('שגיאה');
  } finally {
    setLoading(false);
  }
};
```

זה חוזר על עצמו בכל מקום!

**עם `useAsyncOperation`:**
```typescript
const { loading, error, execute } = useAsyncOperation();

const handleSubmit = async () => {
  await execute(
    () => api.call(),
    (result) => {
      // עושה משהו עם התוצאה
    }
  );
};
```

**ה-Advantages:**
1. **DRY (Don't Repeat Yourself)** - לא חוזר על אותו קוד
2. **Consistency** - אותו pattern בכל מקום
3. **Maintainability** - אם צריך לשנות משהו, משנים במקום אחד
4. **Readability** - הקוד יותר נקי

#### דוגמה מהקוד:

```typescript
// useLogin.ts
const { loading, error, execute } = useAsyncOperation();

const handleSubmit = async (e: React.FormEvent) => {
  e.preventDefault();
  
  await execute(
    () => authService.login(email, password),  // הפעולה
    (data) => {  // מה לעשות בהצלחה
      authStorage.setUser(data.user);
      navigate(ROUTES.DASHBOARD);
    }
  );
};
```

#### למה `useCallback`?

`useCallback` שומר על אותה reference של הפונקציה בין renders. זה חשוב כי:
- מונע re-creation של הפונקציה בכל render
- אם הפונקציה עוברת כ-prop, לא גורמת ל-re-render מיותר
- ה-dependencies array ריק `[]` כי הפונקציה לא תלויה בשום דבר

---

### 3. Form State Management

**שאלה:** איך אתה מנהל state של forms? אני רואה hooks שמנהלים form fields - מה הגישה שלך?

**תשובה מפורטת:**

#### מה זה Form State Management?

זה איך אנחנו שומרים ומנהלים את הערכים שהמשתמש מזין בטופס (email, password, וכו').

#### הגישה במערכת: Controlled Components

**מה זה Controlled Component?**

זה component שהערך שלו נשלט על ידי React state. כל שינוי בערך עובר דרך state.

**דוגמה פשוטה:**
```typescript
const [email, setEmail] = useState('');

<input 
  value={email}           // הערך נשלט על ידי state
  onChange={(e) => setEmail(e.target.value)}  // כל שינוי מעדכן את ה-state
/>
```

#### איך זה עובד במערכת:

**1. ה-Hook מנהל את ה-State:**

```typescript
// useLogin.ts
export const useLogin = () => {
  const [email, setEmail] = useState('');
  const [password, setPassword] = useState('');

  const handleFieldChange = (field: 'email' | 'password', value: string) => {
    if (field === 'email') {
      setEmail(value);
      // מנקה הודעות קשורות כשמשנים email
      setShowResendOption(false);
      setResendSuccess(false);
    } else {
      setPassword(value);
    }
  };

  return {
    email,
    password,
    handleFieldChange,
    // ...
  };
};
```

**2. ה-Component משתמש ב-Hook:**

```typescript
// LoginForm.tsx
const LoginForm = () => {
  const { email, password, handleFieldChange, handleSubmit } = useLogin();

  return (
    <form onSubmit={handleSubmit}>
      <input
        value={email}
        onChange={(e) => handleFieldChange('email', e.target.value)}
      />
      <input
        type="password"
        value={password}
        onChange={(e) => handleFieldChange('password', e.target.value)}
      />
      <button type="submit">Login</button>
    </form>
  );
};
```

#### למה לא להשתמש ב-Formik או React Hook Form?

**ה-Advantages של הגישה הנוכחית:**
1. **פחות dependencies** - לא צריך ספריות נוספות
2. **יותר control** - שולטים בכל דבר
3. **קל יותר להבין** - פחות magic
4. **קטן יותר** - פחות bundle size

**מתי כן להשתמש ב-Formik/React Hook Form:**
- אם יש forms מאוד מורכבים
- אם צריך validation מתקדמת
- אם יש הרבה forms במערכת

#### איך מטפלים ב-Validation?

**במערכת הנוכחית:**
- חלק מה-validation ב-client (בדיקות בסיסיות)
- רוב ה-validation ב-server (יותר בטוח)

**דוגמה:**
```typescript
const handleSubmit = async (e: React.FormEvent) => {
  e.preventDefault();
  
  // בדיקה בסיסית ב-client
  if (!email || !password) {
    setError('נא למלא את כל השדות');
    return;
  }
  
  // ה-validation העיקרי ב-server
  await execute(() => authService.login(email, password));
};
```

#### מה קורה אם ה-Form גדול מאוד?

אם יש הרבה שדות, אפשר:
1. **לחלק ל-sub-forms** - מספר hooks קטנים
2. **לשמור object במקום variables נפרדות:**
```typescript
const [formData, setFormData] = useState({
  email: '',
  password: '',
  // ... עוד שדות
});

const handleFieldChange = (field: string, value: string) => {
  setFormData(prev => ({ ...prev, [field]: value }));
};
```

---

### 4. Async Operations in Hooks

**שאלה:** איך אתה מטפל ב-async operations ב-hooks? מה הגישה שלך לניהול loading ו-error states?

**תשובה מפורטת:**

#### מה זה Async Operation?

זה פעולה שלוקחת זמן, כמו קריאת API, טעינת קבצים, וכו'. בזמן שהפעולה רצה, אנחנו לא יודעים מתי היא תסתיים.

#### הגישה במערכת:

יש שתי גישות עיקריות:

**1. שימוש ב-`useAsyncOperation` (לפעולות ידניות):**

```typescript
// useLogin.ts
const { loading, error, execute } = useAsyncOperation();

const handleSubmit = async () => {
  await execute(
    () => authService.login(email, password),
    (data) => {
      // מה לעשות בהצלחה
    }
  );
};
```

**2. שימוש ב-`useEffect` (לטעינה אוטומטית):**

```typescript
// useDashboardData.ts
useEffect(() => {
  let cancelled = false;

  const loadData = async () => {
    setLoading(true);
    setError('');

    try {
      const data = await userService.getMe();
      if (!cancelled) {  // בודק אם component עדיין mounted
        setUser(data.user);
      }
    } catch (err) {
      if (!cancelled) {
        setError(getErrorMessage(err));
      }
    } finally {
      if (!cancelled) setLoading(false);
    }
  };

  loadData();
  
  return () => {
    cancelled = true;  // Cleanup - מונע state updates אם component unmounted
  };
}, []);
```

#### למה צריך `cancelled` Flag?

**הבעיה:**

אם component unmount (נמחק מהמסך) לפני שה-API call מסתיים:
- ה-API call עדיין רצה
- כשהוא מסתיים, הוא מנסה לעדכן state
- אבל ה-component כבר לא קיים!
- זה גורם ל-warning ב-React: "Can't perform a React state update on an unmounted component"

**הפתרון:**

```typescript
let cancelled = false;

const loadData = async () => {
  // ...
  if (!cancelled) {  // בודק לפני כל state update
    setUser(data.user);
  }
};

return () => {
  cancelled = true;  // כשה�component unmount, מסמן כ-cancelled
};
```

#### איך מונעים Memory Leaks?

**Memory Leak** זה כשהזיכרון לא משוחרר כשהוא לא צריך יותר.

**דרכים למניעה:**

1. **Cleanup functions ב-`useEffect`:**
```typescript
useEffect(() => {
  const timer = setTimeout(() => {}, 1000);
  
  return () => {
    clearTimeout(timer);  // מנקה את ה-timer
  };
}, []);
```

2. **`cancelled` flags:**
```typescript
let cancelled = false;
// ... בודק לפני state updates
return () => { cancelled = true; };
```

3. **ניקוי event listeners:**
```typescript
useEffect(() => {
  const handler = () => {};
  window.addEventListener('event', handler);
  
  return () => {
    window.removeEventListener('event', handler);  // מנקה את ה-listener
  };
}, []);
```

#### ההבדל בין הגישות:

| גישה | מתי להשתמש | דוגמה |
|------|------------|-------|
| `useAsyncOperation` | פעולות שהמשתמש מפעיל (כפתור, submit) | Login, Transfer |
| `useEffect` | טעינה אוטומטית כשהדף נטען | Dashboard data, Transactions list |

---

### 5. Data Fetching Patterns

**שאלה:** אני רואה hooks שטוענים נתונים. מה הגישה שלך ל-data fetching? מתי להשתמש ב-useEffect vs אחר?

**תשובה מפורטת:**

#### מה זה Data Fetching?

זה תהליך של טעינת נתונים מ-server (API) להצגה ב-frontend.

#### הגישות במערכת:

**1. `useEffect` לטעינה אוטומטית:**

```typescript
// useDashboardData.ts
useEffect(() => {
  const loadData = async () => {
    // טוען נתונים
    const [userData, transactionsData] = await Promise.all([
      userService.getMe(),
      transactionService.getAll(),
    ]);
    setUser(userData.user);
    setTransactions(transactionsData.transactions);
  };

  loadData();
}, []);  // [] = רץ רק פעם אחת כשהדף נטען
```

**מתי להשתמש:**
- כשצריך לטעון נתונים כשהדף נטען
- נתונים שצריכים להיות זמינים מיד

**2. `useEffect` עם Dependencies לטעינה דינמית:**

```typescript
// useTransactions.ts
useEffect(() => {
  const loadTransactions = async () => {
    const data = await transactionService.getAll(currentPage, pageSize);
    setTransactions(data.transactions);
  };

  loadTransactions();
}, [currentPage, pageSize]);  // רץ מחדש כשהדף או הגודל משתנים
```

**מתי להשתמש:**
- כשצריך לטעון נתונים בהתאם ל-state או props
- Pagination, filtering, sorting

**3. Manual Trigger עם `useAsyncOperation`:**

```typescript
// useTransfer.ts
const { loading, error, execute } = useAsyncOperation();

const handleSubmit = async () => {
  await execute(() => transactionService.create(data));
};
```

**מתי להשתמש:**
- כשהמשתמש מפעיל את הפעולה (כפתור, submit)
- לא טעינה אוטומטית

#### למה להשתמש ב-`cancelled` Flag?

**הבעיה:**

```typescript
useEffect(() => {
  const loadData = async () => {
    const data = await api.getData();  // לוקח 2 שניות
    setData(data);  // מנסה לעדכן state
  };
  loadData();
}, []);

// אם component unmount אחרי שנייה אחת:
// - ה-API call עדיין רצה
// - כשהוא מסתיים, מנסה לעדכן state של component שלא קיים
// - React זורק warning
```

**הפתרון:**

```typescript
useEffect(() => {
  let cancelled = false;

  const loadData = async () => {
    const data = await api.getData();
    if (!cancelled) {  // בודק לפני state update
      setData(data);
    }
  };
  
  loadData();
  
  return () => {
    cancelled = true;  // כשהדף נסגר, מסמן כ-cancelled
  };
}, []);
```

#### Promise.all vs Sequential Calls:

**Promise.all (מקבילי):**
```typescript
const [user, transactions] = await Promise.all([
  userService.getMe(),
  transactionService.getAll(),
]);
// שני ה-calls רצים במקביל - יותר מהיר!
```

**Sequential (רצף):**
```typescript
const user = await userService.getMe();
const transactions = await transactionService.getAll();
// השני מחכה לראשון - יותר איטי
```

**מתי להשתמש בכל אחד:**
- `Promise.all` - כשהנתונים לא תלויים זה בזה
- Sequential - כשהשני צריך את התוצאה של הראשון

#### איך היית משפר את ה-Data Fetching?

**שיפורים אפשריים:**

1. **Caching** - לשמור נתונים שכבר נטענו
2. **Optimistic Updates** - לעדכן UI לפני שה-API מסתיים
3. **Error Retry** - לנסות שוב אם נכשל
4. **Loading Skeletons** - להציג placeholder בזמן טעינה

---

## Built-in Hooks - שאלות כלליות

### 6. useState Usage

**שאלה:** מתי אתה משתמש ב-`useState`? מה ה-best practices שלך?

**תשובה מפורטת:**

#### מה זה `useState`?

`useState` הוא hook שמאפשר לשמור state (נתונים) ב-component. כשהערך משתנה, React מעדכן את ה-UI.

#### איך זה עובד:

```typescript
const [value, setValue] = useState(initialValue);
```

- `value` - הערך הנוכחי
- `setValue` - פונקציה לעדכון הערך
- `initialValue` - הערך ההתחלתי

**דוגמה:**
```typescript
const [count, setCount] = useState(0);

<button onClick={() => setCount(count + 1)}>
  Count: {count}
</button>
```

#### מתי להשתמש ב-`useState`:

**1. Form Fields:**
```typescript
const [email, setEmail] = useState('');
const [password, setPassword] = useState('');
```

**2. UI State:**
```typescript
const [isOpen, setIsOpen] = useState(false);
const [loading, setLoading] = useState(false);
```

**3. Data from API:**
```typescript
const [user, setUser] = useState<User | null>(null);
const [transactions, setTransactions] = useState<Transaction[]>([]);
```

#### Best Practices:

**1. כמה State Variables ליצור?**

**טוב:**
```typescript
const [email, setEmail] = useState('');
const [password, setPassword] = useState('');
// כל אחד עם מטרה ברורה
```

**לא טוב:**
```typescript
const [formData, setFormData] = useState({ email: '', password: '' });
// אם רק email משתנה, password גם re-render
```

**אבל אם יש הרבה שדות:**
```typescript
const [formData, setFormData] = useState({
  email: '',
  password: '',
  firstName: '',
  lastName: '',
  // ... עוד הרבה שדות
});
// זה בסדר - יותר נוח לנהל
```

**2. Initial Value:**

```typescript
// טוב - ערך ברור
const [user, setUser] = useState<User | null>(null);
const [loading, setLoading] = useState(false);

// לא טוב - לא ברור מה זה
const [data, setData] = useState(null);
```

**3. Functional Updates:**

```typescript
// אם העדכון תלוי בערך הקודם:
setCount(count + 1);  // יכול להיות stale

setCount(prev => prev + 1);  // תמיד מעודכן
```

#### מה ההבדל בין `useState` ל-`useRef`?

| `useState` | `useRef` |
|------------|----------|
| גורם ל-re-render כשמשתנה | לא גורם ל-re-render |
| מתאים ל-UI state | מתאים לערכים שלא משפיעים על UI |
| ערך נשמר בין renders | ערך נשמר בין renders |

**דוגמה:**
```typescript
// useState - משפיע על UI
const [count, setCount] = useState(0);  // UI מתעדכן

// useRef - לא משפיע על UI
const timerRef = useRef<NodeJS.Timeout | null>(null);  // רק לשמירה
```

#### מתי להשתמש ב-`useReducer` במקום?

`useReducer` מתאים כשיש state מורכב עם הרבה לוגיקה.

**דוגמה:**
```typescript
// עם useState - מורכב
const [state, setState] = useState({
  loading: false,
  error: null,
  data: null,
  step: 'idle',
});

// עם useReducer - יותר מסודר
const [state, dispatch] = useReducer(reducer, initialState);
```

**מתי להשתמש:**
- State מורכב עם הרבה actions
- לוגיקה מורכבת של עדכונים
- אם יש הרבה `setState` calls

---

### 7. useEffect Patterns

**שאלה:** איך אתה משתמש ב-`useEffect`? מתי צריך cleanup function?

**תשובה מפורטת:**

#### מה זה `useEffect`?

`useEffect` הוא hook שמאפשר לבצע side effects (פעולות צד) ב-components. זה כמו lifecycle methods ב-class components.

#### איך זה עובד:

```typescript
useEffect(() => {
  // הקוד שרץ כאן
  doSomething();
  
  return () => {
    // Cleanup function - רץ לפני שהדף נסגר או לפני ה-effect הבא
    cleanup();
  };
}, [dependencies]);  // dependencies - מתי לרוץ מחדש
```

#### מתי `useEffect` רץ?

1. **אחרי כל render** - אם אין dependencies array
2. **רק פעם אחת** - אם dependencies array ריק `[]`
3. **כשדependency משתנה** - אם יש dependencies `[value]`

#### דוגמאות מהקוד:

**1. רץ פעם אחת (ב-mount):**
```typescript
// useDashboardData.ts
useEffect(() => {
  const loadData = async () => {
    const data = await userService.getMe();
    setUser(data.user);
  };
  loadData();
}, []);  // [] = רץ רק פעם אחת
```

**2. רץ כשדependency משתנה:**
```typescript
// useTransactions.ts
useEffect(() => {
  const loadTransactions = async () => {
    const data = await transactionService.getAll(currentPage, pageSize);
    setTransactions(data.transactions);
  };
  loadTransactions();
}, [currentPage, pageSize]);  // רץ מחדש כשהדף או הגודל משתנים
```

**3. עם cleanup:**
```typescript
// ChatAssistant.tsx
useEffect(() => {
  window.addEventListener(AUTH_CHANGE_EVENT, handleAuthChange);
  
  return () => {
    // Cleanup - מנקה את ה-event listener
    window.removeEventListener(AUTH_CHANGE_EVENT, handleAuthChange);
  };
}, []);
```

#### מתי צריך Cleanup Function?

**צריך cleanup כאשר:**
1. **Event Listeners** - צריך להסיר אותם
2. **Timers** - צריך לנקות אותם
3. **API Calls** - צריך לבטל אותם (עם `cancelled` flag)
4. **Subscriptions** - צריך לבטל אותם
5. **DOM Manipulation** - צריך לנקות

**דוגמה - Event Listener:**
```typescript
useEffect(() => {
  const handleClick = () => console.log('clicked');
  window.addEventListener('click', handleClick);
  
  return () => {
    // חשוב! אם לא ננקה, ה-listener נשאר גם אחרי שה-component נמחק
    window.removeEventListener('click', handleClick);
  };
}, []);
```

**דוגמה - Timer:**
```typescript
useEffect(() => {
  const timer = setTimeout(() => {
    console.log('Timer finished');
  }, 1000);
  
  return () => {
    // מנקה את ה-timer אם component unmount לפני שהוא מסתיים
    clearTimeout(timer);
  };
}, []);
```

**דוגמה - API Call עם cancelled flag:**
```typescript
useEffect(() => {
  let cancelled = false;
  
  const loadData = async () => {
    const data = await api.getData();
    if (!cancelled) {  // בודק לפני state update
      setData(data);
    }
  };
  
  loadData();
  
  return () => {
    cancelled = true;  // מסמן כ-cancelled
  };
}, []);
```

#### מה קורה אם לא נקה Resources?

**בעיות אפשריות:**

1. **Memory Leaks** - הזיכרון לא משוחרר
2. **Warnings ב-React** - "Can't perform a React state update on an unmounted component"
3. **Performance Issues** - listeners/timers ממשיכים לרוץ
4. **Bugs** - קוד רץ על components שלא קיימים

**דוגמה לבעיה:**
```typescript
// ללא cleanup - בעיה!
useEffect(() => {
  const timer = setInterval(() => {
    console.log('Running...');
  }, 1000);
  // אם component unmount, ה-timer ממשיך לרוץ לנצח!
}, []);
```

#### מתי להשתמש ב-Empty Dependencies Array `[]`?

**משתמשים ב-`[]` כאשר:**
- רוצים שהקוד ירוץ רק פעם אחת (ב-mount)
- לא תלוי ב-props או state
- Setup code (event listeners, timers, וכו')

**דוגמה:**
```typescript
useEffect(() => {
  // Setup - רץ רק פעם אחת
  window.addEventListener('resize', handleResize);
  
  return () => {
    window.removeEventListener('resize', handleResize);
  };
}, []);  // [] = רק פעם אחת
```

#### איך מונעים Infinite Loops?

**הבעיה:**
```typescript
const [count, setCount] = useState(0);

useEffect(() => {
  setCount(count + 1);  // משנה את count
}, [count]);  // count משתנה → effect רץ מחדש → משנה count → loop!
```

**הפתרונות:**

1. **לא לכלול ב-dependencies אם לא צריך:**
```typescript
useEffect(() => {
  // רק setup, לא תלוי ב-count
  console.log('Component mounted');
}, []);  // [] = רק פעם אחת
```

2. **להשתמש ב-functional update:**
```typescript
useEffect(() => {
  setCount(prev => prev + 1);  // לא תלוי ב-count הנוכחי
}, []);  // [] = רק פעם אחת
```

3. **להשתמש ב-`useRef` אם צריך ערך שלא גורם ל-re-render:**
```typescript
const countRef = useRef(0);

useEffect(() => {
  countRef.current = countRef.current + 1;  // לא גורם ל-re-render
}, []);  // [] = רק פעם אחת
```

---

### 8. useCallback and useMemo

**שאלה:** מתי אתה משתמש ב-`useCallback` או `useMemo`? מה ה-benefits?

**תשובה מפורטת:**

#### מה זה `useCallback`?

`useCallback` שומר על אותה reference של פונקציה בין renders. זה מונע re-creation של הפונקציה בכל render.

#### איך זה עובד:

```typescript
const memoizedCallback = useCallback(
  () => {
    doSomething(a, b);
  },
  [a, b]  // dependencies - הפונקציה נוצרת מחדש רק אם a או b משתנים
);
```

#### למה צריך את זה?

**הבעיה:**
```typescript
const Parent = () => {
  const [count, setCount] = useState(0);
  
  const handleClick = () => {
    console.log('clicked');
  };
  
  return <Child onClick={handleClick} />;
};

// כל פעם ש-Parent re-render, handleClick נוצר מחדש
// זה גורם ל-Child ל-re-render גם אם הוא wrapped ב-React.memo
```

**הפתרון:**
```typescript
const Parent = () => {
  const [count, setCount] = useState(0);
  
  const handleClick = useCallback(() => {
    console.log('clicked');
  }, []);  // [] = הפונקציה נוצרת רק פעם אחת
  
  return <Child onClick={handleClick} />;
};
```

#### דוגמה מהקוד:

```typescript
// useAsyncOperation.ts
const execute = useCallback(
  async (operation: () => Promise<any>, onSuccess?: (result: any) => void) => {
    // ...
  },
  []  // [] = הפונקציה נוצרת רק פעם אחת
);
```

**למה `[]` כאן?**
- הפונקציה לא תלויה ב-props או state
- היא רק wrapper ל-operation
- ה-operation עצמו יכול להיות closure עם dependencies

#### מתי `useCallback` לא עוזר?

**לא צריך `useCallback` כאשר:**
1. הפונקציה לא עוברת כ-prop ל-child component
2. ה-child לא wrapped ב-`React.memo`
3. הפונקציה פשוטה מאוד (לא משפיע על performance)

**דוגמה - לא צריך:**
```typescript
const Component = () => {
  const handleClick = useCallback(() => {
    console.log('clicked');
  }, []);
  
  return <button onClick={handleClick}>Click</button>;
  // לא צריך - הפונקציה לא עוברת ל-child memoized
};
```

#### מה זה `useMemo`?

`useMemo` שומר על תוצאה של חישוב יקר בין renders.

#### איך זה עובד:

```typescript
const expensiveValue = useMemo(
  () => {
    return expensiveCalculation(a, b);
  },
  [a, b]  // החישוב רץ מחדש רק אם a או b משתנים
);
```

#### דוגמה:

```typescript
const Component = ({ items }) => {
  // חישוב יקר - רץ רק אם items משתנה
  const sortedItems = useMemo(() => {
    return items.sort((a, b) => a.value - b.value);
  }, [items]);
  
  return <div>{sortedItems.map(...)}</div>;
};
```

#### ההבדל בין `useCallback` ל-`useMemo`:

| `useCallback` | `useMemo` |
|---------------|-----------|
| שומר על פונקציה | שומר על תוצאה של חישוב |
| מחזיר פונקציה | מחזיר ערך |
| `useCallback(fn, deps)` = `useMemo(() => fn, deps)` | `useMemo(() => value, deps)` |

**דוגמה:**
```typescript
// useCallback - שומר על הפונקציה
const handleClick = useCallback(() => {
  console.log('clicked');
}, []);

// useMemo - שומר על התוצאה
const sortedItems = useMemo(() => {
  return items.sort(...);
}, [items]);
```

#### איך יודעים אם צריך Optimization?

**סימנים שצריך optimization:**
1. ה-UI איטי או laggy
2. יש הרבה re-renders מיותרים
3. יש חישובים יקרים שצריך למנוע

**כלים לבדיקה:**
1. **React DevTools Profiler** - רואה כמה re-renders יש
2. **Console.log** - לבדוק כמה פעמים component render
3. **Performance tab** - לבדוק bottlenecks

**כלל אצבע:**
- לא להתחיל עם optimization
- למדוד לפני
- להוסיף רק אם יש בעיה אמיתית

---

### 9. useRef Usage

**שאלה:** מתי אתה משתמש ב-`useRef`? מה ההבדל בינו לבין `useState`?

**תשובה מפורטת:**

#### מה זה `useRef`?

`useRef` הוא hook שמחזיר object עם property `current` שנשמר בין renders ולא גורם ל-re-render כשמשתנה.

#### איך זה עובד:

```typescript
const ref = useRef(initialValue);

// גישה לערך
ref.current = newValue;  // עדכון
const value = ref.current;  // קריאה
```

#### ההבדל בין `useRef` ל-`useState`:

| `useState` | `useRef` |
|------------|----------|
| גורם ל-re-render כשמשתנה | לא גורם ל-re-render |
| מתאים ל-UI state | מתאים לערכים שלא משפיעים על UI |
| ערך נשמר בין renders | ערך נשמר בין renders |
| יש setter function | עדכון ישיר דרך `.current` |

#### מתי להשתמש ב-`useRef`?

**1. DOM References:**
```typescript
// ChatAssistant.tsx
const messagesEndRef = useRef<HTMLDivElement | null>(null);

useEffect(() => {
  if (messagesEndRef.current) {
    messagesEndRef.current.scrollIntoView({ behavior: 'smooth' });
  }
}, [messages]);

return <div ref={messagesEndRef} />;
```

**2. לשמור ערכים שלא גורמים ל-re-render:**
```typescript
// useTransactions.ts
const loadingRef = useRef(false);

useEffect(() => {
  if (!loadingRef.current) {
    loadingRef.current = true;
    setLoading(true);
  }
  // ...
}, []);
```

**3. לשמור instances של libraries:**
```typescript
// ChatAssistant.tsx
const socketRef = useRef<Socket | null>(null);

useEffect(() => {
  socketRef.current = io(SOCKET_URL);
  
  return () => {
    socketRef.current?.disconnect();
  };
}, []);
```

#### למה להשתמש ב-`useRef` ל-Socket?

**הבעיה עם `useState`:**
```typescript
const [socket, setSocket] = useState<Socket | null>(null);

useEffect(() => {
  setSocket(io(SOCKET_URL));  // גורם ל-re-render
}, []);

// כל פעם שמשנים socket, component re-render
// זה לא נחוץ - socket לא משפיע על UI
```

**הפתרון עם `useRef`:**
```typescript
const socketRef = useRef<Socket | null>(null);

useEffect(() => {
  socketRef.current = io(SOCKET_URL);  // לא גורם ל-re-render
}, []);

// socket נשמר אבל לא גורם ל-re-renders מיותרים
```

#### מתי צריך `useRef` ל-DOM Elements?

**כשצריך לגשת ל-DOM element ישירות:**
- Scroll to element
- Focus on input
- Measure element size
- Animate element

**דוגמה:**
```typescript
const inputRef = useRef<HTMLInputElement>(null);

const handleFocus = () => {
  inputRef.current?.focus();  // מכניס focus ל-input
};

return <input ref={inputRef} />;
```

#### מה ההבדל בין `useRef` ל-Regular Variable?

**Regular Variable:**
```typescript
let count = 0;  // מאפס בכל render!

const Component = () => {
  count = count + 1;  // לא נשמר בין renders
  return <div>{count}</div>;
};
```

**useRef:**
```typescript
const countRef = useRef(0);  // נשמר בין renders!

const Component = () => {
  countRef.current = countRef.current + 1;  // נשמר בין renders
  return <div>{countRef.current}</div>;
};
```

**ההבדל:**
- Regular variable - מאפס בכל render
- `useRef` - נשמר בין renders

---

### 10. useContext and Context API

**שאלה:** איך אתה משתמש ב-Context API? מתי להשתמש בו?

**תשובה מפורטת:**

#### מה זה Context API?

Context API הוא דרך ב-React להעביר data דרך component tree בלי לעבור דרך כל ה-components באמצע (prop drilling).

#### איך זה עובד:

**1. יוצרים Context:**
```typescript
// ThemeContext.ts
export const ThemeContext = createContext<ThemeContextValue | null>(null);
```

**2. יוצרים Provider:**
```typescript
// ThemeContextProvider.tsx
export function ThemeContextProvider({ children }: Props) {
  const [mode, setMode] = useState<ThemeMode>('dark');
  
  return (
    <ThemeContext.Provider value={{ mode, isDark: mode === 'dark', toggleTheme }}>
      {children}
    </ThemeContext.Provider>
  );
}
```

**3. משתמשים ב-Context:**
```typescript
// Component
const { mode, toggleTheme } = useThemeContext();
```

#### דוגמה מהקוד:

**1. יצירת Context:**
```typescript
// ThemeContext.ts
export const ThemeContext = createContext<ThemeContextValue | null>(null);

export function useThemeContext(): ThemeContextValue {
  const value = useContext(ThemeContext);
  
  if (value === null) {
    throw new Error('useThemeContext must be used inside ThemeContextProvider');
  }
  
  return value;
}
```

**2. Provider:**
```typescript
// ThemeContextProvider.tsx
export function ThemeContextProvider({ children }: Props) {
  const [mode, setMode] = useState<ThemeMode>(getInitialThemeFromStorage);
  
  useEffect(() => {
    localStorage.setItem('theme', mode);  // שומר ב-localStorage
  }, [mode]);
  
  function toggleTheme() {
    setMode((m) => (m === 'light' ? 'dark' : 'light'));
  }
  
  return (
    <ThemeContext.Provider value={{ mode, isDark: mode === 'dark', toggleTheme }}>
      <MuiThemeProvider theme={isDark ? darkTheme : lightTheme}>
        {children}
      </MuiThemeProvider>
    </ThemeContext.Provider>
  );
}
```

**3. שימוש:**
```typescript
// Component
const MyComponent = () => {
  const { mode, toggleTheme } = useThemeContext();
  
  return (
    <button onClick={toggleTheme}>
      Current theme: {mode}
    </button>
  );
};
```

#### למה ליצור Custom Hook `useThemeContext`?

**הבעיה:**
```typescript
const value = useContext(ThemeContext);
// value יכול להיות null אם לא בתוך Provider
// צריך לבדוק בכל מקום
```

**הפתרון:**
```typescript
export function useThemeContext(): ThemeContextValue {
  const value = useContext(ThemeContext);
  
  if (value === null) {
    throw new Error('useThemeContext must be used inside ThemeContextProvider');
  }
  
  return value;  // TypeScript יודע שזה לא null
}
```

**ה-Advantages:**
1. **Type Safety** - TypeScript יודע שהערך לא null
2. **Error Handling** - error ברור אם משתמשים מחוץ ל-Provider
3. **Reusability** - לא צריך לבדוק null בכל מקום

#### מתי להשתמש ב-Context?

**משתמשים ב-Context כאשר:**
- יש data שצריך להיות זמין ב-components רבים
- רוצים להימנע מ-prop drilling
- Data שלא משתנה לעתים קרובות

**דוגמאות טובות:**
- Theme (light/dark mode)
- Language/i18n
- User authentication (אבל במערכת זו משתמשים ב-localStorage)
- Global settings

#### מה ה-Disadvantages של Context?

**1. Re-renders:**
```typescript
// כל component שמשתמש ב-Context re-render כשהערך משתנה
// גם אם הוא לא צריך את הערך שהשתנה
```

**2. לא מתאים ל-Data שמשתנה לעתים קרובות:**
```typescript
// לא טוב - משתנה כל הזמן
const [mousePosition, setMousePosition] = useState({ x: 0, y: 0 });

// טוב - משתנה רק כשצריך
const [theme, setTheme] = useState('dark');
```

**3. קשה ל-Debug:**
- לא ברור מאיפה הערך מגיע
- קשה לעקוב אחרי data flow

#### איך מונעים Re-renders מ-Context?

**1. לחלק ל-multiple contexts:**
```typescript
// במקום context אחד גדול
const ThemeContext = createContext({ mode, user, settings });

// לחלק למספר contexts קטנים
const ThemeContext = createContext({ mode });
const UserContext = createContext({ user });
const SettingsContext = createContext({ settings });
```

**2. להשתמש ב-`useMemo` ל-value:**
```typescript
const value = useMemo(
  () => ({ mode, isDark, toggleTheme }),
  [mode]  // רק אם mode משתנה
);
```

**3. לא להשתמש ב-Context ל-Data שמשתנה לעתים קרובות**

---

## State Management

### 11. Local vs Global State

**שאלה:** איך אתה מחליט מתי להשתמש ב-local state vs global state?

**תשובה מפורטת:**

#### מה זה Local State?

Local state הוא state ששייך ל-component ספציפי ונשמר רק בו.

**דוגמה:**
```typescript
const LoginForm = () => {
  const [email, setEmail] = useState('');  // Local state
  const [password, setPassword] = useState('');  // Local state
};
```

#### מה זה Global State?

Global state הוא state שזמין ב-components רבים.

**דוגמה:**
```typescript
// ThemeContext - Global state
const { mode, toggleTheme } = useThemeContext();
```

#### מתי להשתמש בכל אחד?

**Local State כאשר:**
- State ששייך רק ל-component אחד
- Form fields
- UI state (dialog open/close)
- Selected item

**Global State כאשר:**
- State שצריך להיות זמין ב-components רבים
- Theme
- User authentication
- Language settings

#### דוגמאות מהקוד:

**Local State:**
```typescript
// useLogin.ts
const [email, setEmail] = useState('');  // רק ב-LoginForm
const [password, setPassword] = useState('');
```

**Global State:**
```typescript
// ThemeContext - זמין בכל האפליקציה
const { mode, toggleTheme } = useThemeContext();
```

#### מתי להשתמש ב-Redux או Zustand?

**משתמשים ב-Redux/Zustand כאשר:**
- יש הרבה global state
- צריך time-travel debugging
- יש state מורכב עם הרבה actions
- צוות גדול שצריך consistency

**במערכת הנוכחית:**
- לא צריך - יש מעט global state
- Context מספיק ל-theme
- localStorage מספיק ל-auth

---

### 12. Auth State Management

**שאלה:** איך אתה מנהל auth state? למה localStorage ולא Context?

**תשובה מפורטת:**

#### הגישה במערכת:

```typescript
// auth.storage.ts
export const authStorage = {
  getUser(): User | null {
    const user = localStorage.getItem('user');
    return user ? JSON.parse(user) : null;
  },

  setUser(user: User) {
    localStorage.setItem('user', JSON.stringify(user));
    window.dispatchEvent(new Event(AUTH_CHANGE_EVENT));
  },

  clearAuth() {
    localStorage.removeItem('user');
    window.dispatchEvent(new Event(AUTH_CHANGE_EVENT));
  },

  isAuthenticated() {
    return !!this.getUser();
  },
};
```

#### למה localStorage ולא Context?

**1. Persistence:**
- localStorage נשמר גם אחרי refresh
- Context מאפס ב-refresh

**2. JWT ב-httpOnly Cookie:**
- JWT נשמר ב-cookie מאובטח (server-side)
- לא נגיש מ-JavaScript (יותר בטוח)
- localStorage נגיש מ-JavaScript (פחות בטוח)

**3. Event-Driven Updates:**
- משתמשים ב-custom events לעדכון components
- `AUTH_CHANGE_EVENT` - כל component יכול להאזין

#### איך Components יודעים על שינוי ב-Auth?

**דוגמה:**
```typescript
// ChatAssistant.tsx
useEffect(() => {
  const handleAuthChange = () => {
    setIsAuthenticated(authStorage.isAuthenticated());
  };

  window.addEventListener(AUTH_CHANGE_EVENT, handleAuthChange);
  
  return () => {
    window.removeEventListener(AUTH_CHANGE_EVENT, handleAuthChange);
  };
}, []);
```

#### למה לא לשמור JWT ב-localStorage?

**בעיות אבטחה:**
1. **XSS Attacks** - JavaScript יכול לגשת ל-localStorage
2. **לא מאובטח** - נגיש לכל script בדף
3. **httpOnly Cookie** - לא נגיש מ-JavaScript, רק מ-server

**הגישה הנכונה:**
- JWT ב-httpOnly cookie (server-side)
- User data ב-localStorage (רק לצורך UI)
- עםCredentials: true ב-axios (שולח cookies)

---

### 13. URL State Management

**שאלה:** מתי אתה מנהל state ב-URL? מה ה-advantages?

**תשובה מפורטת:**

#### מתי להשתמש ב-URL State?

**משתמשים ב-URL state כאשר:**
- רוצים שהמשתמש יוכל לשתף את ה-URL
- רוצים שהמשתמש יוכל לחזור אחורה (back button)
- Pagination, filtering, sorting
- Deep linking

#### דוגמה מהקוד:

```typescript
// useTransactions.ts
const [searchParams, setSearchParams] = useSearchParams();
const currentPage = Number(searchParams.get('page')) || 1;

const handlePageChange = (page: number) => {
  const next = new URLSearchParams(searchParams);
  next.set('page', String(page));
  setSearchParams(next);  // מעדכן את ה-URL
};
```

#### ה-Advantages:

1. **Shareable** - אפשר לשתף URL עם state
2. **Bookmarkable** - אפשר לשמור ב-bookmarks
3. **Browser History** - back/forward buttons עובדים
4. **Deep Linking** - אפשר לגשת ישירות למצב מסוים

#### מה ההבדל בין URL State ל-Component State?

| URL State | Component State |
|-----------|-----------------|
| נשמר ב-URL | נשמר רק ב-component |
| נגיש מ-URL | לא נגיש מ-URL |
| נשמר ב-refresh | מאפס ב-refresh |
| מתאים ל-pagination, filters | מתאים ל-UI state |

---

## API & Data Fetching

### 14. HTTP Client Setup

**שאלה:** איך אתה מגדיר HTTP client? מה ה-interceptors עושים?

**תשובה מפורטת:**

#### הקוד:

```typescript
// http-client.ts
export const httpClient = axios.create({
  baseURL: import.meta.env.VITE_API_URL,
  headers: { 'Content-Type': 'application/json' },
  withCredentials: true,  // שולח cookies
});

httpClient.interceptors.response.use(
  (res) => res,  // אם הצליח, מחזיר את ה-response
  (err: AxiosError) => {
    if (err.response?.status === 401) {
      authStorage.clearAuth();  // אם 401, מנקה auth
    } else if (err.request && !err.response) {
      console.error('Network error: No response from server', err.message);
    }
    return Promise.reject(err);
  }
);
```

#### מה ה-Interceptors עושים?

**Response Interceptor:**
- בודק כל response מה-server
- אם 401 (Unauthorized) - מנקה auth ומנתק את המשתמש
- אם network error - לוג error

**למה זה שימושי?**
- לא צריך לבדוק 401 בכל API call
- טיפול מרכזי ב-errors
- Consistency בכל האפליקציה

#### למה `withCredentials: true`?

- שולח cookies עם כל request
- חשוב ל-JWT שנשמר ב-httpOnly cookie
- מאפשר authentication דרך cookies

---

### 15. Error Handling Strategy

**שאלה:** איך אתה מטפל ב-errors ב-API calls?

**תשובה מפורטת:**

#### הגישה במערכת:

**1. `useAsyncOperation` מטפל ב-errors:**
```typescript
const { error, execute } = useAsyncOperation();

await execute(() => api.call());
// אם יש error, הוא נשמר ב-error state
```

**2. `getErrorMessage` utility:**
```typescript
export function getErrorMessage(err: unknown): string {
  const error = err as ApiError;
  return error.response?.data?.error || error.message || 'An error occurred';
}
```

**3. הצגה למשתמש:**
```typescript
{error && <div className="error">{error}</div>}
```

#### איך מבדילים בין סוגי Errors?

**דוגמה:**
```typescript
if (err.response?.status === 401) {
  // Unauthorized - redirect to login
} else if (err.response?.status === 404) {
  // Not found - show message
} else if (err.request && !err.response) {
  // Network error - show network message
}
```

---

## Routing

### 17. Protected Routes

**שאלה:** איך אתה מגן על routes?

**תשובה מפורטת:**

#### הקוד:

```typescript
// App.tsx
function RequireAuth({ children }: { children: ReactNode }) {
  if (!authStorage.isAuthenticated()) {
    return <Navigate to={ROUTES.LOGIN} replace />;
  }
  return (
    <>
      <ChatAssistant />
      {children}
    </>
  );
}

<Route path={ROUTES.DASHBOARD} element={<RequireAuth><DashboardPage /></RequireAuth>} />
```

#### איך זה עובד?

1. `RequireAuth` בודק אם המשתמש authenticated
2. אם לא - redirect ל-login
3. אם כן - מציג את ה-component

#### מה קורה אם המשתמש לא Authenticated?

- `Navigate` עם `replace` - מחליף את ה-URL
- לא נשאר ב-history (לא יכול לחזור עם back button)
- מועבר ל-login page

---

## Performance

### 22. Optimization Techniques

**שאלה:** אילו optimization techniques אתה משתמש?

**תשובה מפורטת:**

#### הטכניקות במערכת:

1. **`useCallback`** - למניעת re-creation של functions
2. **`useRef`** - למניעת re-renders מ-unnecessary state
3. **`cancelled` flags** - למניעת memory leaks
4. **Cleanup functions** - לניקוי resources

#### מתי להשתמש ב-`React.memo`?

**משתמשים ב-`React.memo` כאשר:**
- Component render לעתים קרובות
- Props לא משתנים לעתים קרובות
- Component כבד (expensive rendering)

**דוגמה:**
```typescript
const ExpensiveComponent = React.memo(({ data }) => {
  // רק re-render אם data משתנה
  return <div>{expensiveCalculation(data)}</div>;
});
```

---

### 23. Memory Leaks Prevention

**שאלה:** איך אתה מונע memory leaks?

**תשובה מפורטת:**

#### הטכניקות:

1. **Cleanup functions ב-`useEffect`:**
```typescript
useEffect(() => {
  const timer = setTimeout(() => {}, 1000);
  return () => clearTimeout(timer);
}, []);
```

2. **`cancelled` flags:**
```typescript
let cancelled = false;
// ...
if (!cancelled) setData(data);
return () => { cancelled = true; };
```

3. **Event listeners cleanup:**
```typescript
window.addEventListener('event', handler);
return () => window.removeEventListener('event', handler);
```

---

## Advanced Topics

### 25. Socket.IO Integration

**שאלה:** איך אתה מטפל ב-Socket.IO connections?

**תשובה מפורטת:**

#### הקוד:

```typescript
// ChatAssistant.tsx
const socketRef = useRef<Socket | null>(null);

useEffect(() => {
  if (!isAuthenticated) {
    if (socketRef.current) {
      socketRef.current.disconnect();
    }
    return;
  }

  socketRef.current = io(`${SOCKET_URL}/chat`, {
    withCredentials: true,
  });

  socketRef.current.on('bot-message', (data) => {
    setMessages(prev => [...prev, { type: 'bot', text: data.response }]);
  });

  return () => {
    socketRef.current?.disconnect();
  };
}, [isAuthenticated]);
```

#### למה `useRef` ל-Socket?

- Socket לא משפיע על UI
- לא צריך re-render כשמשתנה
- צריך לשמור את ה-instance בין renders

---

## סיכום

קובץ זה מכיל תשובות מפורטות לשאלות ראיון על:
- Custom hooks ו-built-in hooks
- State management
- API calls ו-error handling
- Routing
- Performance
- Advanced topics

כל תשובה כוללת:
- הסבר מפורט מהבסיס
- דוגמאות מהקוד
- Best practices
- Trade-offs והחלטות

**טיפים למועמד:**
- קרא את הקוד לפני הראיון
- הבן את ה-patterns וההחלטות
- תרגל הסבר של הקוד בקול
- הכין דוגמאות מהקוד שלך
