// Auth persistence: user in localStorage, token in memory (for mobile cookie fallback).
import type { User, StoredUser } from '../types';

export const AUTH_CHANGE_EVENT = 'auth-state-change';

let inMemoryToken: string | null = null;

export const authStorage = {
  getUser(): StoredUser | null {
    const user = localStorage.getItem('user');
    if (!user) return null;
    try {
      return JSON.parse(user);
    } catch {
      return null;
    }
  },

  setUser(user: User) {
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

  setToken(token: string) {
    inMemoryToken = token;
  },

  clearAuth() {
    localStorage.removeItem('user');
    inMemoryToken = null;
    window.dispatchEvent(new Event(AUTH_CHANGE_EVENT));
  },

  isAuthenticated() {
    return !!this.getUser();
  },
};
