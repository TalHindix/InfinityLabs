// auth.service.ts
import { httpClient } from './httpClient';
import type { LoginResponse, SignupData, SignupResponse } from '../types';

export const authService = {
  async login(email: string, password: string): Promise<LoginResponse> {
    const res = await httpClient.post('/auth/login', { email, password });
    return res.data;
  },

  async signup(data: SignupData): Promise<SignupResponse> {
    const res = await httpClient.post('/auth/signup', data);
    return res.data;
  },
};
