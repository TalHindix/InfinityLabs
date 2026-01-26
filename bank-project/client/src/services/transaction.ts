import api from './api';
import type {
  Transaction,
  TransferData,
  TransactionsResponse,
  TransactionResponse,
} from '../types';

export type { Transaction };

export const transactionService = {
  getAll: async (page = 1, limit = 10): Promise<TransactionsResponse> => {
    const response = await api.get<TransactionsResponse>(`/transactions?page=${page}&limit=${limit}`);
    return response.data;
  },

  getById: async (id: string): Promise<TransactionResponse> => {
    const response = await api.get<TransactionResponse>(`/transactions/${id}`);
    return response.data;
  },

  create: async (data: TransferData): Promise<TransactionResponse> => {
    const response = await api.post<TransactionResponse>('/transactions', data);
    return response.data;
  },
};
