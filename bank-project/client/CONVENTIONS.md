# Client naming and structure conventions

## File names (clear and consistent)

- **shared/**
  - `muiExports.ts` – Re-exports of MUI components (single import source).
  - `muiTheme.ts` – MUI light/dark theme definitions.
  - `displayFormatters.ts` – Amount and date formatting for UI (e.g. `formatAmount`, `formatDate`).
  - `authErrorMessages.ts` – Maps API/network errors to user-friendly login messages.
  - `timeBasedGreeting.ts` – Greeting by time of day (morning/afternoon/evening).
  - `useAsyncOperation.ts` – Hook for async operations with loading/error state.
  - `authFormStyles.ts` – Shared styles for login/signup form fields.
- **constants/**
  - `routePaths.ts` – App route paths; exports `ROUTES`.
- **api/**
  - `*.service.ts` – API clients (e.g. `auth.service.ts`, `transaction.service.ts`).
  - `auth.storage.ts` – Auth state in localStorage.
  - `http-client.ts` – Axios instance and base config.
- **screens/** (per feature)
  - `*Page.tsx` – Full page component (default export for router).
  - `*Form.tsx`, `*Table.tsx`, etc. – Sub-components (named export).
  - `*.styles.ts` – MUI `sx` styles for the component with the same base name.
  - `use*.ts` – Custom hook for that screen (e.g. `useTransactions`, `useTransfer`).
- **components/** – Reusable UI; same pattern: `ComponentName.tsx` + `ComponentName.styles.ts` when needed.
- **layout/** – App shell (header, footer). Components: `ComponentName.tsx`; styles: camelCase `componentNameStyles.ts` (e.g. `appHeaderStyles.ts`, `pageFooterStyles.ts`, `pageFooterCaptionStyles.ts`).
- **context/** – React context: `ThemeContext.ts` (types + context object), `ThemeContextProvider.tsx` (provider component).

## Exports

- Route (page) components: `export default ...` for use in `<Route element={...} />`.
- Other components and hooks: named exports (e.g. `export const TransactionDetailPanel`, `export const useTransactions`).
- Services: named export object (e.g. `export const authService`, `export const transactionService`).

## Imports

- Use the named file for shared code: `../../shared/muiExports`, `../../shared/displayFormatters`, `../../constants/routePaths`.
- Types from `../../types` or `../types` as appropriate.
