// Auth API: login, signup, resend verification, logout.
import { httpClient } from './http.client';
import type { LoginResponse, SignupData, SignupResponse , ResendVerificationResponse} from '../types';


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

  async verifyOtp(email: string, otp: string): Promise<LoginResponse> {
    const res = await httpClient.post('/auth/verify-otp', { email, otp });
    return res.data;
  },

  async resendOtp(email: string): Promise<{ message: string }> {
    const res = await httpClient.post('/auth/resend-otp', { email });
    return res.data;
  },

  async logout(): Promise<void> {
    await httpClient.post('/auth/logout');
  },
};
