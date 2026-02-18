# Repository Pattern — תמיכה ב-MongoDB וגם ב-SQL

## הבעיה

היום כל ה-Services מדברים ישירות עם Mongoose (MongoDB). למשל:

```javascript
// user.service.js — היום
const user = await User.findOne({ email });
await user.save();

// transaction.service.js — היום
const session = await mongoose.startSession();
await User.findOneAndUpdate({ email }, { $inc: { balance: -amount } });
```

אם נרצה מחר לעבוד עם SQL (PostgreSQL למשל), נצטרך לשנות **כל שורה** בכל Service. או גרוע מזה — לשים `if` בכל מקום.

## הפתרון: Repository Pattern

הרעיון פשוט — **פולימורפיזם**. אותו interface, שתי implementations שונות.

```
Service (הלוגיקה העסקית)
    │
    ▼
Repository Interface (חוזה — מה המתודות)
    │
    ├── MongoRepository (מימוש עם Mongoose)
    └── SQLRepository   (מימוש עם Sequelize)
```

ה-Service לא יודע ולא צריך לדעת איזה DB רץ מאחורה. הוא קורא ל-`repository.findByEmail(email)` — וזהו.

## איך זה עובד ב-JavaScript

ב-JS אין `interface` כמו ב-Java, אז אנחנו משתמשים ב-**class בסיס** שזורק שגיאה אם לא מימשו אותו:

```javascript
// repository interface — החוזה
class UserRepository {
  async create(data)        { throw new Error('Not implemented'); }
  async findByEmail(email)  { throw new Error('Not implemented'); }
  async findById(id)        { throw new Error('Not implemented'); }
  async updateBalance(email, amount) { throw new Error('Not implemented'); }
}
```

אחרי זה, כל מימוש **יורש** ודורס את המתודות:

```javascript
// mongo/user.repository.js
class MongoUserRepository extends UserRepository {
  async findByEmail(email) {
    return User.findOne({ email });  // Mongoose
  }
}

// sql/user.repository.js
class SQLUserRepository extends UserRepository {
  async findByEmail(email) {
    return User.findOne({ where: { email } });  // Sequelize
  }
}
```

## ה-`if` היחיד — ב-Factory

כל הפרויקט מכיל **`if` אחד בלבד** שקובע איזה DB לטעון. הוא רץ פעם אחת, בעלייה:

```javascript
// repositories/index.js — ה-Factory
import config from '../config/index.js';

let userRepository;

if (config.dbType === 'sql') {
  userRepository = new SQLUserRepository();
} else {
  userRepository = new MongoUserRepository();
}

export { userRepository };
```

## מה משתנה ב-Service — כמעט כלום

לפני:

```javascript
// user.service.js — קשור ל-Mongoose
import User from '../models/user.model.js';

const user = await User.findOne({ email });
```

אחרי:

```javascript
// user.service.js — לא קשור לשום DB
import { userRepository } from '../repositories/index.js';

const user = await userRepository.findByEmail(email);
```

ה-Service נשאר אותו דבר. רק ה-import השתנה.

## הדוגמה החזקה — העברת כסף (Transaction)

זה המקום שהכי מראה את הערך של ה-Pattern. היום יש לנו:

```javascript
// ב-Mongo: session + startTransaction + commit/abort
const session = await mongoose.startSession();
session.startTransaction();
// ... עדכון יתרות ...
session.commitTransaction();
```

ב-SQL המנגנון שונה לגמרי:

```javascript
// ב-SQL: BEGIN + COMMIT / ROLLBACK
const t = await sequelize.transaction();
// ... עדכון יתרות ...
await t.commit();
```

אבל **ה-Service לא יודע מזה**. הוא פשוט קורא:

```javascript
await transactionRepository.executeTransfer({
  fromEmail, toEmail, amount, description
});
```

וכל מימוש (Mongo / SQL) מטפל ב-Transaction בדרך שלו. ה-Service לא משתנה.

## למה זה חשוב

**בלי Pattern:**
- הוספת DB חדש = שינוי בכל Service
- עשרות `if` פזורים בקוד
- בדיקות דורשות DB אמיתי

**עם Repository Pattern:**
- הוספת DB חדש = מימוש Repository חדש בלבד
- `if` אחד בלבד (ב-Factory)
- אפשר לבדוק עם Mock Repository
- עומד בעקרון Open/Closed Principle

## סיכום בשורה אחת

**Repository Pattern = פולימורפיזם ל-Data Access. ה-Service מכיר את ה-interface, לא את ה-DB.**
