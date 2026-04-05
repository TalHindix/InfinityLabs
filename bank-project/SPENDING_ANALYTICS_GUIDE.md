# Spending Analytics Chart — Planning, Implementation & Interview Guide

## How to Verify Everything Works

### Step 1: Backend API

Start the server and test the endpoint directly:

```bash
cd server && npm start
```

Use curl or Postman to hit:

```
GET /api/v1/transactions/analytics/spending?months=6
Authorization: Bearer <your-jwt-token>
```

Expected response:

```json
{
  "success": true,
  "data": {
    "monthlySpending": [
      { "year": 2026, "month": 2, "totalSpent": 350.00, "transactionCount": 3 },
      { "year": 2026, "month": 3, "totalSpent": 120.50, "transactionCount": 1 }
    ],
    "topRecipients": [
      { "email": "john@example.com", "totalSent": 250.00, "transactionCount": 2 },
      { "email": "sara@example.com", "totalSent": 100.00, "transactionCount": 1 }
    ]
  }
}
```

**What to check:**
- Returns 401 if no token provided
- Returns empty arrays if user has no outgoing transactions
- `months` query param works (try `?months=1` vs `?months=12`)
- Amounts are rounded to 2 decimals
- Monthly data is sorted chronologically (oldest first)
- Top recipients are sorted by amount (highest first)

### Step 2: Frontend Chart

```bash
cd client && npm run dev
```

**What to check:**
- Log in and go to the dashboard
- Chart appears between the Balance Card and Recent Transactions
- Loading spinner shows while data is fetching
- If no transactions exist: shows "No spending data available" message
- Bar chart displays gold bars with month labels on X-axis
- Hovering a bar shows tooltip with amount in AED
- Top Recipients card appears below the chart (only if recipients exist)
- Dark mode: toggle theme — chart grid lines, tooltip, and axis labels adapt

### Step 3: Real-Time Refresh

- Open the dashboard
- Open the AI chatbot and make a transfer through it
- The chart should auto-refresh without page reload (via DASHBOARD_REFRESH_EVENT)
- Alternatively, make a transfer from the transfer page, go back to dashboard, and verify the chart reflects the new data

### Step 4: Edge Cases

- New user with zero transactions: empty state message, no errors
- User who only receives money (never sends): empty chart (spending = money sent only)
- Mobile view: chart is responsive and scrollable horizontally if needed

### Step 5: Existing Tests

Make sure nothing broke:

```bash
cd server && npm test
cd client && npm test
```

---

## How It Was Planned

### Problem

The dashboard only showed a balance card and a list of recent transactions. For an interview demo, it lacked a visual "wow factor" and didn't demonstrate data aggregation skills.

### Decision Process

| Decision | Choice | Why |
|----------|--------|-----|
| Where to aggregate data? | Server-side (MongoDB) | Shows aggregation pipeline knowledge; O(1) client memory regardless of transaction volume |
| New route file or existing? | Added to transaction routes | Analytics is derived from transactions, not a new domain — keeps modules cohesive |
| Charting library? | Recharts | Lightweight (~45kB), React-native, works well with MUI, zero config needed |
| Separate hook or extend existing? | New `useSpendingAnalytics` hook | Separation of concerns — doesn't break existing dashboard hook or its tests |
| Route placement? | Before `/:transactionId` | Express matches top-down — `/analytics/spending` must not be caught by the `:transactionId` param |

### Architecture Flow

```
MongoDB ──$match──$group──$sort──$project──> Service Layer
                                                  │
                                            Controller (Promise.all)
                                                  │
                                         GET /analytics/spending
                                                  │
                                         transactionService.getSpendingAnalytics()
                                                  │
                                         useSpendingAnalytics hook
                                                  │
                                         SpendingChart component
                                                  │
                                         DashboardPage
```

### Files Changed

| File | What | Why |
|------|------|-----|
| `server/src/services/transaction.service.js` | 2 new aggregation functions | Core business logic — keeps controllers thin |
| `server/src/controllers/transaction.controller.js` | 1 new controller | Handles HTTP, delegates to service, runs both queries in parallel |
| `server/src/routes/transaction.routes.js` | 1 new route | Endpoint registration, placed before param route |
| `client/src/types/index.ts` | 3 new interfaces | Type safety for API response |
| `client/src/api/transaction.service.ts` | 1 new method | API layer, same pattern as existing methods |
| `client/src/screens/dashboard/useSpendingAnalytics.ts` | New hook | Data fetching + auto-refresh via custom event |
| `client/src/screens/dashboard/SpendingChart.tsx` | New component | Chart + top recipients, theme-aware |
| `client/src/screens/dashboard/SpendingChart.styles.ts` | New styles | MUI SxProps pattern matching existing style files |
| `client/src/screens/dashboard/DashboardPage.tsx` | 2 lines added | Import + render, zero props needed |

---

## How to Explain in an Interview

### The 30-Second Pitch

> "I built a spending analytics feature that shows users their monthly spending as a bar chart and their top recipients. On the backend, I used a MongoDB aggregation pipeline that groups transactions by month, sums the amounts, and sorts chronologically. It leverages an existing compound index on `fromEmail` and `createdAt` so it's efficient even with large datasets. On the frontend, I used Recharts inside a self-contained React component with its own data-fetching hook. The chart auto-refreshes in real-time when a new transfer happens through the AI chatbot, using a custom event system."

### Key Technical Points to Highlight

**1. MongoDB Aggregation Pipeline**

> "Instead of fetching all transactions and computing totals on the client, I used a server-side aggregation pipeline. The `$match` stage filters by user email and date range — this hits a compound index so it's an indexed scan, not a collection scan. Then `$group` aggregates by year and month, `$sum` totals the amounts, and `$count` gives transaction counts. Finally `$round` avoids floating-point display issues."

If asked "why not compute on the client?":

> "Client-side computation downloads all transactions to the browser. If a user has 10,000 transactions, that's wasted bandwidth and memory. The aggregation pipeline processes everything in MongoDB and returns just 6 objects — one per month."

**2. Route Ordering (Express Gotcha)**

> "I placed `/analytics/spending` before `/:transactionId` in the routes. In Express, if the param route came first, the word 'analytics' would be matched as a transaction ID and return a 404. This is a common pitfall that shows understanding of how Express routing works."

**3. Parallel Execution**

> "The controller uses `Promise.all` to run `getMonthlySpending` and `getTopRecipients` simultaneously. Since they're independent queries, there's no reason to wait for one before starting the other. This cuts the endpoint response time roughly in half."

**4. Self-Contained Component Architecture**

> "The `SpendingChart` is fully self-contained — it has its own hook, its own loading/error states, and requires zero props. I can drop `<SpendingChart />` into any page with a single line. This means the existing dashboard code didn't need any changes to its data flow — I just added one import and one JSX line."

**5. Real-Time Updates**

> "When a user makes a transfer through the AI chatbot, a custom browser event (`DASHBOARD_REFRESH_EVENT`) is dispatched. Both the dashboard data hook and the analytics hook listen to this event and silently re-fetch without showing a loading spinner. So the chart updates instantly after a transfer without the user needing to refresh the page."

**6. Theme Awareness**

> "The chart adapts to dark and light mode by reading the theme context. The grid lines, tooltip background, and axis label colors all switch based on the current theme. This isn't something many candidates think about, but it shows attention to a consistent user experience."

### Anticipate These Questions

**Q: Why Recharts and not Chart.js or D3?**
> "Recharts is built specifically for React — it uses React components instead of imperative DOM manipulation. Chart.js needs a wrapper library, and D3 is overkill for a bar chart. Recharts gave me the best developer experience with the smallest bundle impact."

**Q: How would you handle this at scale with millions of transactions?**
> "The aggregation already uses the compound index on `{fromEmail, createdAt}`, so the `$match` stage is efficient. For true scale, I'd add a materialized view or a pre-computed summary collection that updates on each transaction — essentially trading write-time cost for read-time performance. Or use MongoDB's `$merge` stage to write aggregation results to a cache collection."

**Q: Why not add a date range picker?**
> "The endpoint already supports a `months` query parameter (1-12). Adding a UI picker is straightforward — pass the value to the hook, which passes it to the API call. I kept the UI simple for the demo, but the backend is already flexible."

**Q: What about testing?**
> "The component is easy to test because it's self-contained. I'd mock the `transactionService.getSpendingAnalytics` call and verify: loading state renders a spinner, empty data shows the empty message, valid data renders the chart with correct labels. For the backend, I'd use integration tests with a real MongoDB instance to verify the aggregation pipeline returns correct totals."
