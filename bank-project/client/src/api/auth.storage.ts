// Auth persistence: user in localStorage, token in memory (guards against XSS token theft).
import type { User, StoredUser } from '../types';

export const AUTH_CHANGE_EVENT = 'auth-state-change';

function parseStoredUser(raw: string): StoredUser | null {
  try {
    const parsed = JSON.parse(raw);
    if (!parsed?.id || !parsed?.email) return null;
    return parsed as StoredUser;
  } catch {
    return null;
  }
}

class AuthStorage {
  // Token lives only in memory — intentionally lost on page refresh and re-fetched via cookie.
  #token: string | null = null;
  readonly #storage: Storage;
  readonly #bus: EventTarget;

  constructor(storage: Storage = localStorage, bus: EventTarget = window) {
    this.#storage = storage;
    this.#bus = bus;
  }

  getUser(): StoredUser | null {
    const raw = this.#storage.getItem('user');
    if (!raw) return null;

    const user = parseStoredUser(raw);
    if (!user) this.#storage.removeItem('user');
    return user;
  }

  setUser(user: User): void {
    const stored: StoredUser = {
      id: user.id,
      firstName: user.firstName,
      lastName: user.lastName,
      email: user.email,
    };
    this.#storage.setItem('user', JSON.stringify(stored));
    this.#bus.dispatchEvent(new Event(AUTH_CHANGE_EVENT));
  }

  getToken(): string | null {
    return this.#token;
  }

  setToken(token: string): void {
    this.#token = token;
  }

  removeToken(): void {
    this.#token = null;
  }

  clearAuth(): void {
    try {
      this.#storage.removeItem('user');
    } catch {
      // localStorage unavailable (e.g. private mode storage quota hit)
    }
    this.#token = null;
    this.#bus.dispatchEvent(new Event(AUTH_CHANGE_EVENT));
  }

  isAuthenticated(): boolean {
    return !!this.getUser();
  }
}

export const authStorage = new AuthStorage();
export { AuthStorage };
