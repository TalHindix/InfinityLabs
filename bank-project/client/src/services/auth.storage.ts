// auth.storage.ts
import type { User } from '../types';
import { getCookie, setCookie, deleteCookie } from '../utils/cookies';

const TOKEN_COOKIE_NAME = 'token';
/** Token cookie expiry in days (1 hour ≈ 1/24) to align with typical JWT expiry */
const TOKEN_COOKIE_DAYS = 1 / 24;

export const authStorage = {
  getUser(): User | null {
    const user = localStorage.getItem('user');
    return user ? JSON.parse(user) : null;
  },

  setUser(user: User) {
    localStorage.setItem('user', JSON.stringify(user));
  },

  getToken(): string | null {
    return getCookie(TOKEN_COOKIE_NAME);
  },

  setToken(token: string) {
    setCookie(TOKEN_COOKIE_NAME, token, TOKEN_COOKIE_DAYS);
  },

  clearAuth() {
    localStorage.removeItem('user');
    deleteCookie(TOKEN_COOKIE_NAME);
  },

  logout(): void {
    this.clearAuth();
  },

  isAuthenticated() {
    return !!this.getUser();
  },
};
