# Frontend

React 19 TypeScript SPA built with Vite and Material-UI.

## Directory Structure

```
client/src/
  main.tsx                Entry point
  App.tsx                 Router setup + route definitions
  api/
    http-client.ts        Axios instance (CORS, credentials, 401 handler)
    auth.storage.ts       localStorage persistence + auth state events
    auth.service.ts       Login, signup, verify, logout API calls
    user.service.ts       User profile fetching
    transaction.service.ts  Transfer and transaction API calls
  components/
    ChatAssistant.tsx     Floating chat panel (FAB + expandable)
    useChatSocket.ts      Socket.IO hook for chat
    DubaiBankLogo.tsx     Logo component
    TransferSuccessDialog.tsx  Post-transfer confirmation dialog
    VideoCall.tsx         Jitsi Meet wrapper
  screens/
    login-signup/
      LoginPage.tsx       Login form
      LoginForm.tsx       Login form component
      SignupPage.tsx       Two-step signup with MUI Stepper
      SignupForm.tsx       Signup form component
      useLogin.ts         Login form state + verification redirect
      useSignup.ts        Stepper state management
    dashboard/
      DashboardPage.tsx   Main dashboard view
      useDashboardData.ts Parallel fetch of user + transactions
      BalanceCard.tsx     Account balance display
      RecentTransactions.tsx  Last 10 transactions
    transfer-money/
      TransferPage.tsx    Transfer form page
      useTransfer.ts      Transfer logic hook
      TransferForm.tsx    Transfer form component
    transaction-history/
      TransactionsPage.tsx     Paginated transaction list
      TransactionTable.tsx     Table component
      TransactionRow.tsx       Table row component
      TransactionDetailPanel.tsx  Slide-out detail view
      useTransactions.ts       Paginated, URL-synced queries
      useTransactionDetail.ts  Single transaction fetch
    video-call/
      VideoCallPage.tsx   Jitsi Meet full-screen page
  shared/
    useAuth.ts            Auth state listener (event-based)
    useAsyncOperation.ts  Generic async state wrapper (loading/error/data)
    muiTheme.ts           Light/dark themes (navy #0D1B2A, gold #C9A227)
    muiExports.ts         Centralized MUI re-exports
    displayFormatters.ts  Currency + date formatting
    timeBasedGreeting.ts  Time-of-day greeting text
    authErrorMessages.ts  Auth error message constants
    authFormStyles.ts     Shared auth form styling
  context/
    ThemeContextProvider.tsx  Dark/light mode with localStorage
  layout/
    AppHeader.tsx         Navigation bar
    PageFooter.tsx        Page footer component
    PageFooterCaption.tsx Footer caption text
  types/
    index.ts              TypeScript interfaces
  constants/
    routePaths.ts         Route path constants
```

## Routing

Defined in `App.tsx` using React Router 7:

| Path | Component | Auth Required |
|------|-----------|:---:|
| `/login` | LoginPage | No |
| `/signup` | SignupPage | No |
| `/dashboard` | DashboardPage | Yes |
| `/transfer` | TransferPage | Yes |
| `/transactions` | TransactionsPage | Yes |
| `/video-call` | VideoCallPage | Yes |

Protected routes are wrapped in a `RequireAuth` component that checks localStorage for a stored user.

## State Management

No external state library (no Redux/Zustand). State is managed through:

- **React hooks** -- `useState`, `useEffect`, custom hooks per screen
- **Context** -- `ThemeContextProvider` for dark/light mode
- **localStorage** -- User object persistence, theme preference
- **Pub/Sub DOM events** -- `auth-state-change` and `dashboard:refresh` for cross-component communication

### `useAsyncOperation` Pattern
A generic hook that wraps any async function, providing `{data, loading, error, execute}`. Used by all data-fetching hooks for consistent loading/error state.

### Auth State Flow
1. On login, user object stored in localStorage
2. `auth-state-change` event dispatched
3. `useAuth` hook listens and updates components
4. On 401 response, Axios interceptor clears storage and dispatches event

## HTTP Client (`http-client.ts`)

Axios instance configured with:
- Base URL from `VITE_API_URL`
- `withCredentials: true` (sends httpOnly cookie)
- 401 response interceptor: clears auth storage, dispatches `auth-state-change`

## Real-Time Chat (`useChatSocket.ts`)

1. Connects to Socket.IO `/chat` namespace with credentials
2. Sends `user-message` events with message text and chat history
3. Listens for `bot-message` responses
4. Listens for `transfer-completed` events
5. On transfer complete, dispatches `dashboard:refresh` custom DOM event

## Theming

MUI theme with two modes:
- **Light** -- White background, navy (#0D1B2A) primary
- **Dark** -- Dark background, gold (#C9A227) accents

Theme preference persisted in localStorage, toggled via `ThemeContextProvider`.

## Testing

Vitest + React Testing Library. Tests cover API services, custom hooks, and utility functions.
