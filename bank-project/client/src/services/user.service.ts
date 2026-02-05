// User API: get current user (e.g. /me).
import { httpClient } from './httpClient';
import type { UserResponse } from '../types';

export const userService = {
  async getMe(): Promise<UserResponse> {
    const res = await httpClient.get('/me');
    return res.data;
  },
};
