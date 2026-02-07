// User type
export interface User {
  _id: string;
  id: string;
  firstName: string;
  lastName: string;
  email: string;
  phone: string;
  balance: number;
}

// Auth types
export interface SignupData {
  firstName: string;
  lastName: string;
  email: string;
  password: string;
  phone: string;
}

// Form data type (same as SignupData, but kept separate for clarity)
export type SignupFormData = SignupData;

export interface LoginResponse {
  user: User;
  /** JWT for Authorization header (used when cookie is not sent cross-origin) */
  token?: string;
}

export interface SignupResponse {
  message: string;
}

// User types
export interface UserResponse {
  user: User;
}

export interface ResendVerificationResponse {
  message: string;
}

// Transaction types
export interface Transaction {
  _id: string;
  id: number;
  fromEmail: string;
  toEmail: string;
  amount: number;
  createdAt: string;
  description?: string;
}

export interface TransferData {
  receiverEmail: string;
  amount: number;
  description: string;
}

export interface TransactionsResponse {
  transactions: Transaction[];
  total: number;
  totalPages: number;
  currentPage: number;
}

export interface TransactionResponse {
  transaction: Transaction;
}

// Error handling
export interface ApiError {
  response?: {
    data?: {
      error?: string;
    };
  };
  message?: string;
}

export function getErrorMessage(err: unknown): string {
  const error = err as ApiError;
  return error.response?.data?.error || error.message || 'An error occurred'; 
}
