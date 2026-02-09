// Transactions API: list (paginated), get by id, create transfer.
import { httpClient } from './http-client';
import type { TransactionsResponse, TransactionResponse, TransferData } from '../types';

export const transactionService = {
  async getAll(page = 1, limit = 10): Promise<TransactionsResponse> {
    const res = await httpClient.get(`/transactions?page=${page}&limit=${limit}`);
    return res.data.data;
  },

  async getById(id: string): Promise<TransactionResponse> {
    const res = await httpClient.get(`/transactions/${id}`);
    return res.data.data;
  },

  async create(data: TransferData): Promise<TransactionResponse> {
    const res = await httpClient.post('/transactions', data);
    return res.data.data;
  },
};
