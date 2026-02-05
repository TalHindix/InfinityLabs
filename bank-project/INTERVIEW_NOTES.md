# הצגת מודלים - ראיון עבודה

## מודל User - החלטות עיצוב מרכזיות

### שדות עיקריים ותרומתם:

1. **`id` (UUID)** - מזהה ייחודי נפרד מ-`_id` של MongoDB
   - משמש ב-JWT tokens (אבטחה - לא חושף ObjectId פנימי)
   - יציב ולא משתנה

2. **`email` (unique, lowercase)** - זיהוי ייחודי + אימות
   - משמש גם לזיהוי שולח/מקבל בהעברות (`fromEmail`, `toEmail`)
   - Indexed לביצועים טובים ב-queries

3. **`password` (select: false)** - אבטחה
   - לא נשלף אוטומטית - רק כשנדרש במפורש (login)

4. **`status` (PENDING/ACTIVE)** - ניהול אימות אימייל
   - PENDING = ממתין לאימות
   - ACTIVE = מאומת, יכול לבצע פעולות
   - מונע גישה לפני אימות

5. **`balance`** - יתרת המשתמש
   - מעודכן atomically בתוך MongoDB transaction בעת העברות
   - מוצג ב-Dashboard

6. **`verificationToken`** - token לאימות אימייל
   - השדה מוגדר ל-`undefined` לאחר אימות מוצלח (הרשומה לא נמחקת, רק השדה)

---

## מודל Transaction - החלטות עיצוב מרכזיות

### שדות עיקריים ותרומתם:

1. **`id` (Number, unique)** - מזהה מספרי ייחודי
   - לא ObjectId של MongoDB - מספר ידידותי למשתמש (1, 2, 3...)
   - נוצר ע"י Counter pattern (sequential, thread-safe)
   - משמש ב-URLs: `/transactions/:transactionId`

2. **`fromEmail` / `toEmail` (indexed)** - זיהוי שולח/מקבל
   - Index: `{ fromEmail: 1, createdAt: -1 }` ו-`{ toEmail: 1, createdAt: -1 }`
   - מאפשר queries מהירים של כל ההעברות של משתמש
   - Query: `$or: [{ fromEmail }, { toEmail }]` - מציג גם העברות יוצאות וגם נכנסות

3. **`amount`** - עם validation מחמיר
   - Min: 0.01, Max: 1,000,000
   - Validator נוסף: חייב להיות מספר חיובי וסופי

4. **`description` (optional)** - הערה אופציונלית
   - Max length: 500 תווים

5. **`createdAt`** - תאריך ושעה
   - מיון default: descending (החדש ביותר קודם)

---

## Counter Pattern - החלטה טכנית חשובה

### למה Counter?
- **בעיה**: MongoDB ObjectIds לא ידידותיים למשתמשים (למשל: `507f1f77bcf86cd799439011`)
- **פתרון**: Counter pattern ליצירת מספרים עוקבים (1, 2, 3...)

### איך זה עובד:
```javascript
// Counter schema
{
  _id: 'transactions',  // מזהה קבוע
  seq: 0                 // המספר הנוכחי
}

// פונקציה thread-safe
getNextTransactionId(session) {
  // Atomic increment - מונע duplicates גם ב-concurrent requests
  Counter.findByIdAndUpdate('transactions', { $inc: { seq: 1 } })
}
```

### יתרונות:
- ✅ **Thread-safe**: Atomic increment מונע race conditions
- ✅ **תמיכה ב-MongoDB Transactions**: הפונקציה מקבלת `session` לעבודה בתוך transaction
- ✅ **User-friendly**: מספרים במקום ObjectIds
- ✅ **Sequential**: קל לזכור ולצטט

### למה Counter בקובץ Transaction?
- Counter משמש **רק** ל-Transaction IDs
- הקשר לוגי חזק - Counter ו-Transaction קשורים זה לזה
- `getNextTransactionId` מיוצא ונמצא קרוב לשימוש
- אם בעתיד נוסיף counters נוספים → נזיז לקובץ נפרד

---

## נקודות להדגשה בראיון

### 1. אבטחה
- `password` עם `select: false` - לא נשלף אוטומטית
- `id` (UUID) ב-JWT במקום `_id` של MongoDB
- `status` מונע גישה לפני אימות אימייל

### 2. ביצועים
- **Indexes** על `fromEmail` ו-`toEmail` עם `createdAt` - queries מהירים
- **MongoDB Transactions** - atomicity בעת העברות (balance + transaction record)

### 3. User Experience
- Transaction IDs מספריים במקום ObjectIds
- Email normalization (lowercase, trim) - אחידות בנתונים

### 4. תכנון נכון
- Counter pattern ל-sequential IDs
- Validation מחמיר על כל השדות
- Separation of concerns - כל שדה עם מטרה ברורה

---

## שאלות אפשריות בראיון - תשובות

**Q: למה לא להשתמש ב-`_id` של MongoDB ל-transactions?**  
A: ObjectIds לא ידידותיים למשתמשים. Counter pattern נותן מספרים עוקבים (1, 2, 3) שקל יותר לזכור ולצטט.

**Q: איך מונעים race conditions ב-Counter?**  
A: `findByIdAndUpdate` עם `$inc` הוא atomic operation ב-MongoDB. גם אם יש concurrent requests, MongoDB מבטיח שהתוספת תהיה atomic.

**Q: למה `password` עם `select: false`?**  
A: אבטחה - הסיסמה לא נשלפת אוטומטית בכל query. רק כשנדרש במפורש (login) עם `.select('+password')`.

**Q: למה Indexes על `fromEmail` ו-`toEmail`?**  
A: Queries תכופים - כל משתמש צריך לראות את כל ההעברות שלו (יוצאות ונכנסות). Indexes משפרים ביצועים משמעותית.

---

## הסבר מפורט: למה Indexes משפרים ביצועים?

### מה זה Index? (אנלוגיה פשוטה)
דמיינו ספר טלפונים:
- **בלי Index**: צריך לעבור על כל העמודים מההתחלה עד שמצאנו את השם (Full Collection Scan)
- **עם Index**: יש לנו אינדקס בסוף הספר - רשימה מסודרת לפי שם עם מספר עמוד. קופצים ישר לעמוד הנכון!

### מה קורה ב-MongoDB בלי Index?

**דוגמה מהפרויקט:**
```javascript
// Query: מצא כל ההעברות של משתמש מסוים
Transaction.find({ 
  $or: [
    { fromEmail: 'user@example.com' },
    { toEmail: 'user@example.com' }
  ]
})
```

**בלי Index:**
- MongoDB צריך לעבור על **כל המסמכים** בקולקציה (Full Collection Scan)
- אם יש 1,000,000 transactions → MongoDB בודק את כולם אחד אחד
- זמן: O(n) - ליניארי, גדל עם כמות הנתונים
- **איטי מאוד!** 🐌

**עם Index:**
- MongoDB משתמש ב-Index (מעין "טבלת תוכן מסודרת")
- קופץ ישר למסמכים הרלוונטיים
- זמן: O(log n) - לוגריתמי, הרבה יותר מהיר
- **מהיר פי 100-1000!** ⚡

### איך Index עובד טכנית?

**Index הוא מבנה נתונים מסודר:**
```
Index על fromEmail:
{
  'user1@example.com': [doc1_id, doc5_id, doc12_id],  // מצביעים למסמכים
  'user2@example.com': [doc2_id, doc8_id],
  'user3@example.com': [doc3_id, doc9_id, doc15_id]
}
```

כשמחפשים `fromEmail: 'user1@example.com'`:
1. MongoDB מחפש ב-Index (מהיר מאוד - B-tree structure)
2. מקבל רשימת IDs של המסמכים הרלוונטיים
3. קופץ ישר למסמכים האלה (במקום לסרוק הכל)

### דוגמה קונקרטית מהפרויקט

**האינדקסים שלנו:**
```javascript
transactionSchema.index({ fromEmail: 1, createdAt: -1 });
transactionSchema.index({ toEmail: 1, createdAt: -1 });
```

**למה גם `createdAt` באינדקס?**
- Query שלנו גם ממיין לפי `createdAt` (descending)
- Index על שני השדות יחד = MongoDB יכול לעשות גם חיפוש וגם מיון בלי sort נפרד
- עוד יותר מהיר! 🚀

**תוצאה:**
- Query של משתמש עם 10,000 transactions: **מ-2 שניות ל-0.01 שניות**
- ככל שיש יותר נתונים → ההבדל גדל!

### מתי Index לא עוזר?

1. **Queries שלא משתמשים בשדה עם Index** - אז עדיין Full Scan
2. **עדכונים/הוספות** - צריך לעדכן גם את ה-Index (עלות קטנה)
3. **Index על שדה שלא משתמשים בו** - בזבוז מקום וזמן עדכון

### סיכום - למה Index חשוב בפרויקט שלנו?

✅ **Queries תכופים** - כל משתמש רואה את ההעברות שלו (Dashboard, Transactions Page)
✅ **כמות נתונים גדולה** - ככל שהפרויקט גדל, ההבדל נהיה קריטי
✅ **User Experience** - משתמשים לא רוצים לחכות 2 שניות לכל query
✅ **עלות נמוכה** - Index תופס מקום קטן יחסית, אבל חוסך המון זמן

**בלי Index:** Full Collection Scan = איטי, לא scalable
**עם Index:** Direct lookup = מהיר, scalable, production-ready

**Q: איך מבטיחים atomicity בעת העברה?**  
A: MongoDB Transactions - כל הפעולות (deduct balance, add balance, create transaction record, increment counter) רצות בתוך session אחד. אם משהו נכשל - הכל מתבטל.
