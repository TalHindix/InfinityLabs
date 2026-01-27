import axios, { AxiosError } from 'axios';
import { getCookie, deleteCookie } from '../utils/cookies';

const api = axios.create({
  baseURL: import.meta.env.VITE_API_URL,
  headers: {
    'Content-Type': 'application/json',
  },
});

// before every request. 
api.interceptors.request.use((config) => {
  const token = getCookie('token');
  if (token) {
    config.headers.Authorization = `Bearer ${token}`;
  }
  return config;
});

// after every response.
api.interceptors.response.use(
  (response) => response,
  (error: AxiosError) => {
    const { status, config } = error.response || {};
    const url = config?.url ?? '';

    const authPaths = ['/auth/login', '/auth/signup'];
    const isAuthEndpoint = authPaths.some(path => url.includes(path));

    const shouldRedirect = status === 401 && !isAuthEndpoint && getCookie('token');

    if (shouldRedirect) {
      handleUnauthorized();
    }

    return Promise.reject(error);
  }
);

function handleUnauthorized() {
  deleteCookie('token');
  localStorage.removeItem('user');

  if (window.location.pathname !== '/login') {
    window.location.replace('/login');
  }
}

export default api;
