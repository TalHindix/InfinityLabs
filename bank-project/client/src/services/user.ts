import api from './api';
import type { UserResponse } from '../types';

export const userService = {
  getMe: async (): Promise<UserResponse> => {
    const response = await api.get<UserResponse>('/me');
    return response.data;
  },
};
