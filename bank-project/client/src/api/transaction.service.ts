import { httpClient } from './http.client';
import type { TransactionsResponse, TransactionResponse, TransferData, SpendingAnalyticsResponse } from '../types';

export const transactionService = {
  async getAll(page = 1, limit = 10): Promise<TransactionsResponse> {
    const res = await httpClient.get(`/transactions?page=${page}&limit=${limit}`);
    return res.data;
  },

  async getById(id: string): Promise<TransactionResponse> {
    const res = await httpClient.get(`/transactions/${id}`);
    return res.data;
  },

  async create(data: TransferData): Promise<TransactionResponse> {
    const res = await httpClient.post('/transactions', data);
    return res.data;
  },

  async sendNotification(transactionId: string): Promise<{ message: string; roomName: string }> {
    const res = await httpClient.post(`/transactions/${transactionId}/send-notification`);
    return res.data;
  },

  async getSpendingAnalytics(months = 6): Promise<SpendingAnalyticsResponse> {
    const res = await httpClient.get(`/transactions/analytics?months=${months}`);
    return res.data;
  },
};
