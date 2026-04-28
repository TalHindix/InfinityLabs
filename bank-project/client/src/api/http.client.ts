// Axios instance with base URL, credentials, and 401 → clearAuth handling.
import axios, { type AxiosError } from 'axios';
import { authStorage } from './auth.storage';

if (!import.meta.env.VITE_API_URL && import.meta.env.MODE !== 'test') {
  throw new Error('VITE_API_URL environment variable is not defined');
}

export const httpClient = axios.create({
  baseURL: import.meta.env.VITE_API_URL,
  headers: { 'Content-Type': 'application/json' },
  withCredentials: true,
});

// Before sending any request, attach the user's token to prove they are logged in.
httpClient.interceptors.request.use((reqConfig) => {
  const token = authStorage.getToken();
  if (token) {
    reqConfig.headers.Authorization = `Bearer ${token}`;
  }
  return reqConfig;
});

// After receiving a response, unwrap the server's envelope ({ data: ... }) so
// callers get the actual payload directly instead of going through res.data.data.
// Also handles 401 by logging the user out.
httpClient.interceptors.response.use(
  (res) => res.data,
  (err: AxiosError) => {
    const isUnauthorized = err.response?.status === 401;
    if (isUnauthorized) {
      authStorage.clearAuth();
    }
    return Promise.reject(err);
  }
);
