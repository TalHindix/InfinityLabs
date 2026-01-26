import api from './api';
import type { User, SignupData, LoginResponse, SignupResponse } from '../types';
import { getCookie, setCookie, deleteCookie } from '../utils/cookies';

export const authService = {
  login: async (email: string, password: string): Promise<LoginResponse> => {
    const response = await api.post<LoginResponse>('/auth/login', { email, password });
    return response.data;
  },

  signup: async (data: SignupData): Promise<SignupResponse> => {
    const response = await api.post<SignupResponse>('/auth/signup', data);
    return response.data;
  },

  logout: (): void => {
    deleteCookie('token');
    localStorage.removeItem('user');
  },

  getUser: (): User | null => {
    const user = localStorage.getItem('user');
    return user ? JSON.parse(user) : null;
  },

  getToken: (): string | null => {
    return getCookie('token');
  },

  setToken: (token: string): void => {
    setCookie('token', token);
  },

  isAuthenticated: (): boolean => {
    return !!getCookie('token');
  },
};
