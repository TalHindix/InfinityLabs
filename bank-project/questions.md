# Fullstack Interview Questions

---

## HTML

**Q1: What is the difference between `div` and `span`?**
A: `div` is a block-level element (takes full width, starts on a new line). `span` is an inline element (takes only the space it needs, stays in the same line).

**Q2: What is semantic HTML?**
A: Using HTML tags that describe their meaning, like `<header>`, `<nav>`, `<main>`, `<article>`, `<footer>` instead of generic `<div>` everywhere. It improves accessibility, SEO, and code readability.

**Q3: What is the difference between `localStorage`, `sessionStorage`, and cookies?**
A: `localStorage` persists until manually cleared. `sessionStorage` is cleared when the tab closes. Cookies are sent with every HTTP request and can have an expiry date. Cookies have a 4KB limit, while storage APIs allow ~5MB.

**Q4: What does `DOCTYPE` do?**
A: It tells the browser which version of HTML the page is written in. `<!DOCTYPE html>` tells the browser to use HTML5 standards mode.

**Q5: What is the difference between `id` and `class`?**
A: `id` must be unique per page, `class` can be reused on multiple elements. `id` has higher CSS specificity than `class`.

---

## CSS

**Q6: What is the box model?**
A: Every HTML element is a box made of: content, padding (space inside the border), border, and margin (space outside the border). `box-sizing: border-box` makes width/height include padding and border.

**Q7: What is the difference between `flexbox` and `grid`?**
A: Flexbox is one-dimensional (row OR column). Grid is two-dimensional (rows AND columns at the same time). Use flexbox for navbars, card rows, simple layouts. Use grid for full page layouts, dashboards, complex arrangements.

**Q8: What does `position: relative` vs `position: absolute` mean?**
A: `relative` positions the element relative to where it normally would be. `absolute` positions it relative to the nearest positioned ancestor (an ancestor with position other than static). If none exists, it uses the document body.

**Q9: What is a media query?**
A: A CSS rule that applies styles only when certain conditions are met, like screen width. Example: `@media (max-width: 768px) { ... }` applies styles only on screens narrower than 768px. Used for responsive design.

**Q10: What is the difference between `em`, `rem`, `px`, and `%`?**
A: `px` is a fixed size. `em` is relative to the parent element's font size. `rem` is relative to the root (`<html>`) font size. `%` is relative to the parent element's size. `rem` is preferred for consistency.

---

## JavaScript

**Q11: What is the difference between `let`, `const`, and `var`?**
A: `var` is function-scoped and hoisted. `let` is block-scoped and can be reassigned. `const` is block-scoped and cannot be reassigned (but objects/arrays can still be mutated). Always prefer `const`, use `let` when reassignment is needed, avoid `var`.

**Q12: What is the difference between `==` and `===`?**
A: `==` compares with type coercion (`"5" == 5` is true). `===` compares without coercion, both value and type must match (`"5" === 5` is false). Always use `===`.

**Q13: What is a closure?**
A: A function that remembers the variables from the scope where it was created, even after that scope has finished executing.
```js
function counter() {
  let count = 0;
  return function () {
    count++;
    return count;
  };
}
const inc = counter();
inc(); // 1
inc(); // 2
```

**Q14: What is the difference between synchronous and asynchronous code?**
A: Synchronous code runs line by line, each line waits for the previous one. Asynchronous code (like `fetch`, `setTimeout`, database calls) doesn't block — it starts the operation and moves on, handling the result later via callbacks, promises, or async/await.

**Q15: What is a Promise?**
A: An object representing a value that will be available in the future. It has three states: pending, fulfilled (resolved), or rejected. You handle results with `.then()/.catch()` or `async/await`.
```js
fetch("/api/data")
  .then((res) => res.json())
  .then((data) => console.log(data))
  .catch((err) => console.error(err));
```

**Q16: What is the difference between `map`, `filter`, and `reduce`?**
A: `map` transforms each item and returns a new array of the same length. `filter` returns a new array with only items that pass a test. `reduce` combines all items into a single value (sum, object, etc.).

**Q17: What is the event loop?**
A: JavaScript is single-threaded. The event loop is the mechanism that handles async operations. It runs the call stack first, then checks the microtask queue (promises), then the macrotask queue (setTimeout, setInterval). This is why a `setTimeout(fn, 0)` still runs after the current code finishes.

**Q18: What is destructuring?**
A: A shorthand for extracting values from objects or arrays.
```js
const { name, age } = user; // object destructuring
const [first, second] = items; // array destructuring
```

**Q19: What is the spread operator (`...`)?**
A: It expands an array or object into individual elements.
```js
const copy = [...array];
const merged = { ...obj1, ...obj2 };
const newArr = [...arr1, ...arr2];
```

**Q20: What is `this` in JavaScript?**
A: `this` depends on how a function is called. In a method, `this` is the object. In a regular function, `this` is `undefined` (strict mode) or `window`. Arrow functions don't have their own `this` — they inherit it from the surrounding scope.

---

## React

**Q21: What is the virtual DOM?**
A: A lightweight JavaScript copy of the real DOM. When state changes, React creates a new virtual DOM, compares it with the previous one (diffing), and only updates the parts of the real DOM that actually changed. This is faster than manipulating the real DOM directly.

**Q22: What is the difference between state and props?**
A: Props are passed from parent to child and are read-only. State is owned by the component and can be changed with `setState` or `useState`. When state changes, the component re-renders.

**Q23: What is `useState`?**
A: A React hook that adds state to a functional component. It returns a value and a setter function.
```jsx
const [count, setCount] = useState(0);
```
Calling `setCount(1)` updates the value and triggers a re-render.

**Q24: What is `useEffect`?**
A: A hook for running side effects (API calls, subscriptions, timers) after render. The dependency array controls when it runs:
```jsx
useEffect(() => { ... }, []);    // runs once on mount
useEffect(() => { ... }, [x]);   // runs when x changes
useEffect(() => { ... });        // runs on every render
```

**Q25: What is the difference between controlled and uncontrolled components?**
A: Controlled: React state drives the input value (`value={state}` + `onChange`). Uncontrolled: the DOM holds the value, accessed via `ref`. Controlled is preferred because React is the single source of truth.

**Q26: What is `useRef`?**
A: A hook that holds a mutable value that persists across renders without causing re-renders. Common uses: referencing DOM elements, storing previous values, keeping timers.
```jsx
const inputRef = useRef(null);
inputRef.current.focus();
```

**Q27: What are keys in React lists and why are they important?**
A: Keys help React identify which items changed, were added, or removed. Without keys (or using array index as key), React can mix up elements during re-renders, causing bugs and poor performance. Use unique, stable IDs as keys.

**Q28: What is lifting state up?**
A: Moving state from a child component to the nearest common parent so that multiple children can share and sync the same data. The parent passes the state down as props and a setter function for children to update it.

**Q29: What is the difference between `useMemo` and `useCallback`?**
A: `useMemo` caches a computed value. `useCallback` caches a function reference. Both prevent unnecessary recalculations or re-renders when dependencies haven't changed.
```jsx
const filtered = useMemo(() => items.filter(fn), [items]);
const handleClick = useCallback(() => { ... }, [dep]);
```

**Q30: What is React Router and why is it needed?**
A: React is a single-page app — the browser loads one HTML file. React Router handles URL changes on the client side, rendering different components for different paths without a full page reload. It gives the user a multi-page experience in a SPA.

---

## Node.js / Express

**Q31: What is Node.js?**
A: A JavaScript runtime built on Chrome's V8 engine that lets you run JavaScript outside the browser. It's designed for building server-side applications with non-blocking, event-driven I/O, making it good for handling many concurrent connections.

**Q32: What is Express?**
A: A minimal web framework for Node.js that provides routing, middleware support, and HTTP utilities. It handles incoming requests, routes them to the right handler, and sends responses. It's the most popular Node.js backend framework.

**Q33: What is middleware in Express?**
A: A function that runs between receiving a request and sending a response. It can modify the request/response, end the request, or pass control to the next middleware with `next()`. Examples: authentication, logging, CORS, body parsing, error handling.

**Q34: What is the difference between `req.params`, `req.query`, and `req.body`?**
A: `req.params` comes from URL path parameters (`/users/:id` → `req.params.id`). `req.query` comes from the query string (`/users?page=2` → `req.query.page`). `req.body` comes from the request body (POST/PUT data, needs a body parser).

**Q35: What is CORS and why does it matter?**
A: Cross-Origin Resource Sharing. Browsers block requests from one domain to a different domain by default (security). CORS headers tell the browser which origins are allowed. Without configuring CORS on the server, your frontend on `localhost:5173` can't call your API on `localhost:3000`.

**Q36: What is the difference between SQL and NoSQL databases?**
A: SQL (PostgreSQL, MySQL) uses tables with fixed schemas, relationships, and joins. NoSQL (MongoDB) uses flexible documents (JSON-like), no fixed schema, and is easier to scale horizontally. SQL is better for complex relationships. NoSQL is better for flexible, rapidly changing data.

**Q37: What is an ORM/ODM?**
A: A library that lets you interact with the database using JavaScript objects instead of raw queries. ORM (Object-Relational Mapping) is for SQL databases (Sequelize, Prisma). ODM (Object-Document Mapping) is for document databases (Mongoose for MongoDB). They provide schema validation, type safety, and cleaner code.

**Q38: What is a REST API?**
A: An architectural style for APIs that uses HTTP methods to perform operations on resources. GET = read, POST = create, PUT/PATCH = update, DELETE = remove. Resources are identified by URLs (`/api/users/123`). It's stateless — each request contains all info needed to process it.

**Q39: What are HTTP status codes you should know?**
A: `200` OK, `201` Created, `204` No Content, `301` Redirect (permanent), `400` Bad Request, `401` Unauthorized (not logged in), `403` Forbidden (logged in but not allowed), `404` Not Found, `409` Conflict (duplicate), `429` Too Many Requests, `500` Server Error.

**Q40: What is environment variables and why use them?**
A: Variables stored outside your code (in `.env` files or system config) that hold sensitive or environment-specific values like database URLs, API keys, and secrets. They keep secrets out of the codebase and let the same code run in different environments (dev, staging, production) with different configs.

---

## Database / MongoDB

**Q41: What is a document in MongoDB?**
A: A JSON-like object stored in a collection. It's the equivalent of a row in SQL. Documents can have nested objects, arrays, and flexible schemas — different documents in the same collection can have different fields.

**Q42: What is an index and why is it important?**
A: A data structure that makes queries faster by avoiding full collection scans. Like a book's index — instead of reading every page, you jump directly to the right one. Without indexes, finding a user by email scans every document. With an index on `email`, it's nearly instant. Trade-off: indexes slow down writes and use extra storage.

**Q43: What is the difference between `find` and `findOne` in MongoDB?**
A: `find` returns an array of all matching documents (can be empty). `findOne` returns the first matching document or `null`. Use `findOne` when you expect a single result (like looking up a user by email).

**Q44: What is a MongoDB schema in Mongoose?**
A: A blueprint that defines the structure, types, validation rules, and defaults for documents in a collection. Even though MongoDB is "schemaless," Mongoose enforces a schema at the application level for data consistency.

**Q45: What is the difference between embedding and referencing in MongoDB?**
A: Embedding stores related data inside the same document (e.g., addresses inside a user document). Referencing stores an ID that points to a separate document (like a foreign key). Embed when data is always accessed together. Reference when data is large, shared, or independently accessed.

---

## Authentication & Security

**Q46: What is the difference between authentication and authorization?**
A: Authentication = verifying who you are (login with email/password). Authorization = verifying what you're allowed to do (can this user access this resource?). Authentication comes first, then authorization.

**Q47: What is JWT and how does it work?**
A: JSON Web Token. A self-contained token with three parts: header (algorithm), payload (user data like id and email), and signature (proves it wasn't tampered with). The server creates it on login, the client sends it with each request, and the server verifies the signature without needing a database lookup.

**Q48: What is hashing and why do we hash passwords?**
A: Hashing is a one-way transformation — you can turn a password into a hash but can't turn the hash back into a password. If the database is breached, attackers see hashes, not passwords. Bcrypt adds a random salt to each hash so identical passwords produce different hashes.

**Q49: What is XSS?**
A: Cross-Site Scripting. An attacker injects malicious JavaScript into your page (through user inputs, comments, etc.). The script runs in other users' browsers and can steal tokens, cookies, or data. Prevention: sanitize user input, escape output, use httpOnly cookies, and Content Security Policy headers.

**Q50: What is CSRF?**
A: Cross-Site Request Forgery. An attacker tricks a logged-in user's browser into making unwanted requests to your server (the browser automatically sends cookies). Prevention: CSRF tokens, SameSite cookie attribute, checking the Origin header.

---

## Git

**Q51: What is the difference between `git merge` and `git rebase`?**
A: `merge` creates a new merge commit combining two branches (preserves full history). `rebase` moves your commits on top of the target branch (creates a linear history). Merge is safer for shared branches. Rebase makes history cleaner but rewrites commits.

**Q52: What is `git stash`?**
A: Temporarily saves your uncommitted changes and reverts to a clean working directory. Useful when you need to switch branches but aren't ready to commit. `git stash` saves, `git stash pop` restores.

**Q53: What is the difference between `git pull` and `git fetch`?**
A: `git fetch` downloads changes from the remote but doesn't merge them into your branch. `git pull` does `fetch` + `merge` in one step. Fetch is safer because you can review changes before merging.

**Q54: What is a pull request?**
A: A request to merge your branch into another branch (usually main). It allows team members to review your code, leave comments, suggest changes, and approve before merging. It's the main code review mechanism in team workflows.

**Q55: How do you undo the last commit?**
A: `git reset --soft HEAD~1` undoes the commit but keeps changes staged. `git reset --mixed HEAD~1` undoes the commit and unstages changes. `git reset --hard HEAD~1` undoes everything (destructive). For already-pushed commits, use `git revert` instead — it creates a new commit that reverses the changes.

---

## General / Concepts

**Q56: What is an API?**
A: Application Programming Interface. A set of rules for how software components communicate. In web development, it usually means HTTP endpoints that accept requests and return data (usually JSON). The frontend calls the API to get/send data to the backend.

**Q57: What is the difference between client-side rendering and server-side rendering?**
A: CSR (React SPA): the browser downloads JavaScript, which builds the page. First load is slow, but navigation after that is fast. SSR (Next.js): the server sends a fully built HTML page. First load is fast, better for SEO. Hybrid approaches exist (SSR for first load, CSR for navigation).

**Q58: What is a WebSocket?**
A: A protocol that keeps a persistent, two-way connection between client and server. Unlike HTTP (request-response), either side can send data at any time. Used for real-time features: chat, live notifications, collaborative editing, stock tickers.

**Q59: What is Docker?**
A: A tool that packages your application and its dependencies into a container — a lightweight, isolated environment that runs the same everywhere. "It works on my machine" is solved because the container IS the machine. Docker Compose runs multiple containers together (app + database + reverse proxy).

**Q60: What is CI/CD?**
A: Continuous Integration: automatically running tests when code is pushed. Continuous Deployment: automatically deploying code to production after tests pass. Together, they automate the path from code commit to production, catching bugs early and shipping faster. Tools: GitHub Actions, Jenkins, GitLab CI.

---

## TypeScript

**Q61: What is TypeScript and why use it?**
A: A superset of JavaScript that adds static types. It catches errors at compile time instead of runtime. For example, calling `user.nmae` instead of `user.name` is caught before the code runs. It also enables better IDE autocompletion and refactoring. It compiles down to regular JavaScript.

**Q62: What is the difference between `interface` and `type` in TypeScript?**
A: Both define the shape of an object. `interface` can be extended and merged. `type` can represent unions, intersections, and primitives. Use `interface` for object shapes (especially in libraries). Use `type` for unions (`type Status = 'active' | 'pending'`) and complex types.

**Q63: What is the difference between `any` and `unknown`?**
A: `any` disables type checking entirely — you can do anything with it. `unknown` is the safe version — you must check the type before using it. Prefer `unknown` over `any` because it forces you to handle types properly.

**Q64: What are generics?**
A: Types that take parameters, making functions and classes work with multiple types while staying type-safe.
```ts
function first<T>(arr: T[]): T {
  return arr[0];
}
first<number>([1, 2, 3]); // returns number
first<string>(["a", "b"]); // returns string
```

**Q65: What is `Optional Chaining` (`?.`) and `Nullish Coalescing` (`??`)?**
A: `?.` safely accesses nested properties without crashing if something is null/undefined: `user?.address?.city` returns `undefined` instead of throwing. `??` provides a default only for `null`/`undefined` (not for `0` or `""`): `value ?? "default"`.
