// auth.storage.ts
import { getCookie, setCookie, deleteCookie } from '../utils/cookies';
import type { User } from '../types';

export const authStorage = {
  getToken() {
    return getCookie('token');
  },

  setToken(token: string) {
    setCookie('token', token);
  },

  getUser(): User | null {
    const user = localStorage.getItem('user');
    return user ? JSON.parse(user) : null;
  },

  setUser(user: User) {
    localStorage.setItem('user', JSON.stringify(user));
  },

  clearAuth() {
    deleteCookie('token');
    localStorage.removeItem('user');
  },

  logout(): void {
    this.clearAuth();
  },

  isAuthenticated() {
    return !!this.getToken();
  },
};
