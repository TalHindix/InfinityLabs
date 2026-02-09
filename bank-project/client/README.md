# Dubai-Bank — Client

React SPA for the Dubai-Bank banking app: auth, dashboard, transfers, transaction history, and chat assistant.

## Stack

- **React 19** + **TypeScript**
- **Vite** (dev server, build)
- **MUI** + **Emotion**
- **React Router** · **Axios** · **Socket.IO** client

## Setup

```bash
npm install
```

Copy env if needed (see `.env.development` / `.env.production` for `VITE_API_URL`, `VITE_SOCKET_URL`).

## Scripts

| Command        | Description              |
|----------------|--------------------------|
| `npm run dev`  | Dev server (port 5173)   |
| `npm run build`| TypeScript + production build |
| `npm run preview` | Preview production build |
| `npm run lint` | ESLint                   |
| `npm run test` | Vitest (unit tests)      |

In development, `/api` is proxied to `http://localhost:3000`. Run the [server](../server) on port 3000 for full functionality.
