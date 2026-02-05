// Axios instance with base URL, credentials, and 401 → clearAuth handling.
import axios, { type AxiosError } from 'axios';
import { authStorage } from './auth.storage';

if (!import.meta.env.VITE_API_URL) {
  throw new Error('VITE_API_URL environment variable is not defined');
}

export const httpClient = axios.create({
  baseURL: import.meta.env.VITE_API_URL,
  headers: { 'Content-Type': 'application/json' },
  withCredentials: true,
});

httpClient.interceptors.response.use(
  (res) => res,
  (err: AxiosError) => {
    if (err.response?.status === 401) {
      authStorage.clearAuth();
    } else if (err.request && !err.response) {
      console.error('Network error: No response from server', err.message);
    }
    return Promise.reject(err);
  }
);
