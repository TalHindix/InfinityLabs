# Dubai Bank Project — Full Session Summary

## Project Overview

**Dubai Bank** is a full-stack banking application built as an interview portfolio project.

**Tech Stack:**
- **Frontend:** React 19, TypeScript, Vite, Material-UI 7
- **Backend:** Node.js, Express 5, MongoDB/Mongoose
- **AI:** OpenAI GPT-4o-mini with function calling (chatbot)
- **Real-time:** Socket.IO (chat + live dashboard refresh)
- **Video:** Jitsi Meet integration
- **Auth:** JWT + httpOnly cookies, bcrypt, OTP via email

**Existing Features Before This Session:**
- Signup with email verification
- Two-factor authentication (OTP)
- Money transfers with ACID transactions (MongoDB sessions)
- Paginated transaction history with detail panel
- AI chatbot that can check balance, view transactions, and execute transfers via function calling
- Video calling after transfers
- Dark/light theme toggle
- Rate limiting, request logging, error handling

---

## What We Built in This Session

### Phase 1: Spending Analytics Chart (Initial Version)

**Goal:** Add a visual analytics feature to the dashboard to impress in an interview.

**What was added:**

#### Backend — MongoDB Aggregation Pipeline

**File:** `server/src/services/transaction.service.js`

Added `getMonthlySpending(userEmail, months)`:
- `$match` stage filters by `fromEmail` and date range — leverages the existing compound index `{ fromEmail: 1, createdAt: -1 }` for efficient querying
- `$group` by `{ year, month }` to aggregate spending per calendar month
- `$sum` totals the amounts, `$count` gives transaction count per month
- `$sort` ensures chronological order
- `$round` to 2 decimals avoids floating-point display issues
- Returns: `[{ year, month, totalSpent, transactionCount }]`

Added `getTopRecipients(userEmail, months, limit)`:
- Same `$match` stage
- `$group` by `toEmail` (recipient)
- `$sort` by totalSent descending, `$limit` to top 5
- Returns: `[{ email, totalSent, transactionCount }]`

**File:** `server/src/controllers/transaction.controller.js`

Added `getSpendingAnalytics` controller:
- Extracts `req.user.email` from JWT (set by auth middleware)
- Clamps `months` query param between 1-12, defaults to 6
- Runs both aggregations in parallel with `Promise.all` for performance
- Returns via the existing `response.ok()` utility

**File:** `server/src/routes/transaction.routes.js`

Added route: `GET /api/v1/transactions/analytics/spending?months=6`
- Placed **before** `/:transactionId` — critical because Express matches routes top-down, and "analytics" would otherwise be caught as a transaction ID parameter
- Protected by existing `router.use(protect)` middleware — no additional auth needed

#### Frontend — Chart Component

**File:** `client/src/types/index.ts`
- Added `MonthlySpending`, `TopRecipient`, `SpendingAnalyticsResponse` TypeScript interfaces

**File:** `client/src/api/transaction.service.ts`
- Added `getSpendingAnalytics(months)` method following the same pattern as existing API methods

**File:** `client/src/screens/dashboard/useSpendingAnalytics.ts` (new)
- Custom hook mirroring the pattern of `useDashboardData.ts`
- Fetches analytics on mount
- Listens to `DASHBOARD_REFRESH_EVENT` for auto-refresh when transfers happen (via chatbot or transfer page)
- Returns `{ monthlySpending, topRecipients, loading, error }`

**File:** `client/src/screens/dashboard/SpendingChart.tsx` (new)
- Self-contained component with its own hook, loading, and error states
- Used Recharts library (`npm install recharts`) for the bar chart
- Integrated into dashboard between BalanceCard and RecentTransactions with zero props needed

**File:** `client/src/screens/dashboard/DashboardPage.tsx`
- Added just 2 lines: one import, one `<SpendingChart />` JSX element

---

### Phase 2: Full Analytics Dashboard Redesign

**Goal:** Upgrade from a simple bar chart to a polished analytics dashboard matching the app's design language.

**What changed:**

#### Backend Expansion

**New aggregation:** `getMonthlyReceived(userEmail, months)` — same pipeline structure as `getMonthlySpending` but queries `toEmail` instead of `fromEmail`, returns `totalReceived` per month.

**Updated controller:** Now runs 3 aggregations in parallel (`Promise.all`), and computes `totalSpent` and `totalReceived` summary totals server-side.

**Updated API response shape:**
```json
{
  "monthlySpending": [{ "year": 2026, "month": 3, "totalSpent": 350, "transactionCount": 3 }],
  "monthlyReceived": [{ "year": 2026, "month": 3, "totalReceived": 200, "transactionCount": 2 }],
  "topRecipients": [{ "email": "user@example.com", "totalSent": 250, "transactionCount": 2 }],
  "totalSpent": 1250.00,
  "totalReceived": 800.00
}
```

#### Frontend — Complete Component Rewrite

**New design features:**

1. **Dark navy background** (`#0d1526`) — self-contained section matching the app's premium banking theme

2. **3 Summary Cards:**
   - Total Spent — red accent border and text (`#ef4444`)
   - Total Received — green accent border and text (`#22c55e`)
   - Net Balance — dynamically colored (green if positive, red if negative)

3. **Grouped Bar Chart:**
   - Red bars = money sent, Green bars = money received
   - Custom tooltip styled with navy background and gold border
   - Horizontal grid lines only (cleaner look)
   - Axis labels in muted white for dark background

4. **Month Filter Buttons:** 3M / 6M / 12M
   - Gold active state (`#c9a227`), transparent inactive
   - Clicking re-fetches data from the API with the new `months` parameter
   - Hook state drives the filter — `setMonths` triggers `useEffect` re-fetch

5. **Top Recipients List:**
   - Rank number (muted)
   - Avatar circle with initials extracted from email (splits on `@`, then on `.` / `_` / `-`)
   - Email address (truncated on mobile)
   - Transfer count (e.g., "3 transfers")
   - Mini progress bar — width proportional to amount relative to the #1 recipient
   - Amount in AED with red color

6. **`mergeMonthlyData()` helper function:**
   - Merges spending and received arrays into a unified chart dataset
   - Handles months where only spending or only receiving occurred
   - Sorts by date key for chronological display

**Updated hook:** `useSpendingAnalytics` now exposes `months` and `setMonths` for the filter buttons, plus `totalSpent`, `totalReceived`, and `monthlyReceived` state.

---

### Phase 3: Design Consistency Audit & Normalization

**Goal:** Make the entire app feel like one product, not a collection of separate pages.

**Audit methodology:** Read every `.styles.ts` file and component across all screens. Compared exact values for colors, border-radius, typography, spacing, button styles, loading states, error states, and empty states.

#### Inconsistencies Found and Fixed:

**1. Primary Button Styles (5 files fixed)**

| File | Before | After |
|------|--------|-------|
| `TransferForm.styles.ts` | No shadow, no transition, no active/disabled | Full shadow, transition, active press, disabled state |
| `TransactionsPage.styles.ts` | No shadow, no transition | Added shadow, transition, active press |
| `BalanceCard.styles.ts` | No shadow, no transition, fontWeight missing | Added all, plus fontWeight: 700 |
| `VerificationSuccess.styles.ts` | Missing fontSize, letterSpacing, shadow, transition | Fully normalized to match auth buttons |
| `TransferForm.styles.ts` (dialog confirm) | fontWeight: 600, no shadow | fontWeight: 700, shadow, transition |

The "gold standard" was the auth page buttons in `LoginForm.styles.ts` — they had the complete set:
```
py: 1.45, fontSize: '1rem', fontWeight: 700, letterSpacing: '0.01em',
boxShadow: '0 12px 30px rgba(201,162,39,0.22)', transition: 'all 200ms ease',
hover shadow: '0 14px 34px rgba(201,162,39,0.28)',
active: translateY(1px), disabled: 0.65 opacity gradient
```

**2. Loading Spinner Colors (3 files fixed)**

| File | Before | After |
|------|--------|-------|
| `DashboardPage.styles.ts` | `color: 'primary.main'` (navy) | `color: 'secondary.main'` (gold) |
| `TransactionDetailPanel.styles.ts` | `color: '#C9A227'` (hardcoded) | `color: 'secondary.main'` (theme token) |
| `TransactionEmptyState.styles.ts` | `color: 'primary.main'` (navy) | `color: 'secondary.main'` (gold) |

Why `secondary.main`: It maps to `#C9A227` (gold) in both light and dark themes, which is the brand accent color.

**3. Empty State Patterns (2 files fixed)**

| File | Before | After |
|------|--------|-------|
| `TransactionEmptyState.styles.ts` | `bgcolor: 'action.hover'` | `bgcolor: 'rgba(13, 27, 42, 0.05)'` to match RecentTransactions |
| `TransactionDetailPanel.tsx` | Emoji `📋` as icon | Replaced with `$` in a proper 80px circle, matching all other empty states |

The canonical pattern (from `RecentTransactions`): 80x80px circle with `rgba(13, 27, 42, 0.05)` background, centered `$` icon at `opacity: 0.3`, title text, optional subtitle.

**4. Dark Mode Color Fixes (3 files fixed)**

| File | Before | After |
|------|--------|-------|
| `TransferForm.styles.ts` (dialogTitle) | `color: '#0D1B2A'` (invisible on dark paper) | `color: 'text.primary'` |
| `TransferSuccessDialog.styles.ts` (dialogTitle) | `color: '#0D1B2A'` | `color: 'text.primary'` |
| `TransferSuccessDialog.tsx` (check icon) | `color: '#4CAF50'` (Material green, wrong palette) | `color: 'success.main'` (`#10B981`) |
| `TransferSuccessDialog.tsx` (amount) | `color: '#C9A227'` (hardcoded) | `color: 'secondary.main'` |

**5. Container Padding (1 file fixed)**

| File | Before | After |
|------|--------|-------|
| `TransactionsPage.styles.ts` | `py: 4` (no responsive, no px) | `py: { xs: 2, sm: 4 }, px: { xs: 2, sm: 3 }` matching Dashboard |

**6. Border-Radius (1 file fixed)**

| File | Before | After |
|------|--------|-------|
| `Pagination.styles.ts` (page indicator) | `borderRadius: 1` | `borderRadius: 2` matching other small components |

**7. Inline Styles Extraction (1 component fixed)**

| File | Before | After |
|------|--------|-------|
| `TransferSuccessDialog.tsx` | 8-line inline `sx` on video call button | Extracted to `videoCallButtonSx` in styles file, with full button treatment |

---

## Git Commits (Chronological)

| Commit | Message | Files |
|--------|---------|-------|
| `99eb0b5` | `feat: add spending analytics chart to dashboard` | 11 files (3 new) |
| `b25ae9c` | `fix: resolve TS errors in SpendingChart` | 1 file |
| `99dd15c` | `feat: redesign spending analytics with grouped bar chart, summary cards & top recipients` | 7 files |
| `b3914a9` | `style: normalize design consistency across all screens` | 11 files |

---

## Technical Decisions and Their Reasoning

### Why MongoDB Aggregation Pipeline Instead of Client-Side Computation?
Server-side aggregation is O(1) memory on the client regardless of transaction volume. The `$match` stage leverages the existing compound index `{ fromEmail: 1, createdAt: -1 }`. If a user has 10,000 transactions, only 6 summary objects are sent to the browser — not 10,000 transaction documents.

### Why Add to Transaction Routes Instead of a New Route File?
Analytics is a derived view of transaction data, not a new domain entity. The project has 3 route files (auth, user, transaction) corresponding to 3 domains. Adding a 4th route file for one GET endpoint would be over-engineering.

### Why a Separate Hook Instead of Extending useDashboardData?
Separation of concerns — the analytics data has different loading characteristics and a different API call. Merging it would mean the entire dashboard waits for analytics to load before showing the balance card. Keeping it separate means the balance card renders immediately while the chart loads independently.

### Why Recharts Over Chart.js or D3?
Recharts is built for React — it uses JSX components, not imperative DOM manipulation. Chart.js requires a wrapper library. D3 is overkill for bar charts. Recharts is ~45kB gzipped with zero peer dependencies beyond React.

### Why Route Ordering Matters
`/analytics/spending` is placed before `/:transactionId`. In Express, routes are matched top-down. If the param route came first, "analytics" would be captured as a transactionId and return a 404.

### Why Promise.all in the Controller?
The controller runs `getMonthlySpending`, `getMonthlyReceived`, and `getTopRecipients` in parallel. These are independent queries — running them sequentially would triple the response time for no reason.

### Why secondary.main Instead of Hardcoded #C9A227?
`secondary.main` resolves to `#C9A227` in both light and dark themes via the MUI theme system. Hardcoded hex values bypass the theme and can break when switching modes. Using theme tokens means one source of truth.

### Why text.primary Instead of #0D1B2A for Dialog Titles?
`#0D1B2A` is the navy primary color — it's nearly invisible on the dark theme's paper background (`#121A22`). `text.primary` resolves to `#0D1B2A` in light mode and `#F0F4F8` in dark mode, making dialog titles readable in both themes.

---

## Architecture Diagram

```
┌─────────────────────────────────────────────────────┐
│                    Frontend (React)                  │
│                                                     │
│  DashboardPage                                      │
│  ├── BalanceCard                                    │
│  ├── SpendingChart (self-contained)                 │
│  │   ├── useSpendingAnalytics hook                  │
│  │   │   ├── transactionService.getSpendingAnalytics│
│  │   │   └── listens to DASHBOARD_REFRESH_EVENT     │
│  │   ├── Summary Cards (Spent/Received/Net)         │
│  │   ├── Filter Buttons (3M/6M/12M)                │
│  │   ├── Grouped BarChart (Recharts)                │
│  │   └── Top Recipients List                        │
│  └── RecentTransactions                             │
└─────────────────┬───────────────────────────────────┘
                  │ GET /api/v1/transactions/analytics/spending?months=6
                  │ Authorization: Bearer <jwt>
┌─────────────────▼───────────────────────────────────┐
│                    Backend (Express)                  │
│                                                      │
│  transaction.routes.js                               │
│  └── /analytics/spending (before /:transactionId)    │
│      └── transaction.controller.js                   │
│          └── Promise.all([                            │
│                getMonthlySpending(email, months),     │
│                getMonthlyReceived(email, months),     │
│                getTopRecipients(email, months)        │
│              ])                                       │
│              └── transaction.service.js               │
│                  └── Transaction.aggregate([...])     │
└─────────────────┬───────────────────────────────────┘
                  │ Aggregation Pipeline
┌─────────────────▼───────────────────────────────────┐
│               MongoDB                                │
│                                                      │
│  transactions collection                             │
│  Indexes:                                            │
│  ├── { fromEmail: 1, createdAt: -1 }  ← used by    │
│  │     $match for spending queries                   │
│  └── { toEmail: 1, createdAt: -1 }    ← used by    │
│        $match for received queries                   │
└─────────────────────────────────────────────────────┘
```

---

## File Map — Every File Touched in This Session

### New Files Created
| File | Purpose |
|------|---------|
| `client/src/screens/dashboard/useSpendingAnalytics.ts` | Hook for fetching analytics data with month filter and auto-refresh |
| `client/src/screens/dashboard/SpendingChart.tsx` | Full analytics dashboard component |
| `client/src/screens/dashboard/SpendingChart.styles.ts` | Styles for the analytics dashboard |

### Modified Files
| File | What Changed |
|------|-------------|
| `server/src/services/transaction.service.js` | Added 3 aggregation functions |
| `server/src/controllers/transaction.controller.js` | Added analytics controller with parallel queries |
| `server/src/routes/transaction.routes.js` | Added analytics route before param route |
| `client/src/types/index.ts` | Added analytics TypeScript interfaces |
| `client/src/api/transaction.service.ts` | Added getSpendingAnalytics API method |
| `client/src/screens/dashboard/DashboardPage.tsx` | Added SpendingChart to dashboard |
| `client/src/screens/dashboard/BalanceCard.styles.ts` | Normalized button styles |
| `client/src/screens/dashboard/DashboardPage.styles.ts` | Normalized spinner color |
| `client/src/screens/transfer-money/TransferForm.styles.ts` | Normalized button + dialog styles, fixed dark mode |
| `client/src/screens/transaction-history/TransactionsPage.styles.ts` | Normalized button + container padding |
| `client/src/screens/transaction-history/TransactionDetailPanel.styles.ts` | Normalized spinner + empty state |
| `client/src/screens/transaction-history/TransactionDetailPanel.tsx` | Replaced emoji with consistent empty state |
| `client/src/screens/transaction-history/TransactionEmptyState.styles.ts` | Normalized empty state + spinner |
| `client/src/screens/transaction-history/Pagination.styles.ts` | Normalized border-radius |
| `client/src/screens/login-signup/VerificationSuccess.styles.ts` | Normalized button styles |
| `client/src/components/TransferSuccessDialog.tsx` | Fixed dark mode colors, extracted inline styles |
| `client/src/components/TransferSuccessDialog.styles.ts` | Fixed dark mode, added videoCallButtonSx |
| `client/package.json` | Added recharts dependency |
