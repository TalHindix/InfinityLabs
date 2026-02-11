// Auth persistence: user in localStorage only; JWT lives in httpOnly cookie (server-set).
import type { User } from '../types';

export const AUTH_CHANGE_EVENT = 'auth-state-change';

export const authStorage = {
  getUser(): User | null {
    const user = localStorage.getItem('user');
    return user ? JSON.parse(user) : null;
  },

  setUser(user: User) {
    localStorage.setItem('user', JSON.stringify(user));
    window.dispatchEvent(new Event(AUTH_CHANGE_EVENT));
  },

  clearAuth() {
    localStorage.removeItem('user');
    window.dispatchEvent(new Event(AUTH_CHANGE_EVENT));
  },

  logout(): void {
    this.clearAuth();
  },

  isAuthenticated() {
    return !!this.getUser();
  },
};
