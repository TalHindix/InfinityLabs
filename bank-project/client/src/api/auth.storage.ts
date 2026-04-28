// Auth persistence: user in localStorage, token in memory (guards against XSS token theft).
import type { User, StoredUser } from '../types';

export const AUTH_CHANGE_EVENT = 'auth-state-change';

// Token lives only in memory — intentionally lost on page refresh and re-fetched via cookie.
let inMemoryToken: string | null = null;

function parseStoredUser(raw: string): StoredUser | null {
  try {
    const parsed = JSON.parse(raw);
    if (!parsed?.id || !parsed?.email) return null;
    return parsed as StoredUser;
  } catch {
    return null;
  }
}

export const authStorage = {
  getUser(): StoredUser | null {
    const raw = localStorage.getItem('user');
    if (!raw) return null;

    const user = parseStoredUser(raw);
    if (!user) localStorage.removeItem('user');
    return user;
  },

  setUser(user: User): void {
    const stored: StoredUser = {
      id: user.id,
      firstName: user.firstName,
      lastName: user.lastName,
      email: user.email,
    };
    localStorage.setItem('user', JSON.stringify(stored));
    window.dispatchEvent(new Event(AUTH_CHANGE_EVENT));
  },

  getToken(): string | null {
    return inMemoryToken;
  },

  setToken(token: string): void {
    inMemoryToken = token;
  },

  removeToken(): void {
    inMemoryToken = null;
  },

  clearAuth(): void {
    try {
      localStorage.removeItem('user');
    } catch {
      // localStorage unavailable (e.g. private mode storage quota hit)
    }
    inMemoryToken = null;
    window.dispatchEvent(new Event(AUTH_CHANGE_EVENT));
  },

  isAuthenticated(): boolean {
    return !!authStorage.getUser();
  },
};
