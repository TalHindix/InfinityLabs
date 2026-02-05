// Auth persistence: user in localStorage only; JWT lives in httpOnly cookie (server-set).
import type { User } from '../types';

export const authStorage = {
  getUser(): User | null {
    const user = localStorage.getItem('user');
    return user ? JSON.parse(user) : null;
  },

  setUser(user: User) {
    localStorage.setItem('user', JSON.stringify(user));
  },

  clearAuth() {
    localStorage.removeItem('user');
  },

  logout(): void {
    this.clearAuth();
  },

  isAuthenticated() {
    return !!this.getUser();
  },
};
