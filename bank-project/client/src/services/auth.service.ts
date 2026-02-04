// auth.service.ts
import { httpClient } from './httpClient';
import type { LoginResponse, SignupData, SignupResponse } from '../types';

interface ResendVerificationResponse {
  message: string;
}

export const authService = {
  async login(email: string, password: string): Promise<LoginResponse> {
    const res = await httpClient.post('/auth/login', { email, password });
    return res.data;
  },

  async signup(data: SignupData): Promise<SignupResponse> {
    const res = await httpClient.post('/auth/signup', data);
    return res.data;
  },

  async resendVerification(email: string): Promise<ResendVerificationResponse> {
    const res = await httpClient.post('/auth/resend-verification', { email });
    return res.data;
  },

  async logout(): Promise<void> {
    await httpClient.post('/auth/logout');
  },
};
