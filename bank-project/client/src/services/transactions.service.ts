// transactions.service.ts
import { httpClient } from './httpClient';
import type { TransactionsResponse, TransactionResponse, TransferData } from '../types';

export const transactionsService = {
  async getAll(page = 1, limit = 10): Promise<TransactionsResponse> {
    const res = await httpClient.get(`/transactions?page=${page}&limit=${limit}`);
    return res.data;
  },

  async create(data: TransferData): Promise<TransactionResponse> {
    const res = await httpClient.post('/transactions', data);
    return res.data;
  },
};
