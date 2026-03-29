// Auth persistence: user in localStorage, token in memory (for mobile cookie fallback).
import type { User } from '../types';

export const AUTH_CHANGE_EVENT = 'auth-state-change';

let inMemoryToken: string | null = null;

export const authStorage = {
  getUser(): User | null {
    const user = localStorage.getItem('user');
    return user ? JSON.parse(user) : null;
  },

  setUser(user: User) {
    localStorage.setItem('user', JSON.stringify(user));
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
