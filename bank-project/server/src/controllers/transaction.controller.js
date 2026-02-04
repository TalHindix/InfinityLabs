import { DEFAULT_PAGE_SIZE } from '../constants/index.js';
import {
  findTransactionById,
  findTransactionsByUserEmail,
  executeTransfer,
} from '../services/transaction.service.js';
import * as response from '../utils/response.util.js';
import { AppError } from '../middleware/error.middleware.js';
import { validateTransactionAmount } from '../utils/validation.util.js';

/** Max allowed page size for list transactions (avoids huge responses). */
const MAX_PAGE_SIZE = 100;

/**
 * Returns paginated transactions for the current user. Query: page (default 1), limit (default from constants, max 100).
 */
export const getTransactions = async (req, res, next) => {
  try {
    const page = Math.max(1, Number(req.query.page) || 1);
    const limit = Math.min(MAX_PAGE_SIZE, Math.max(1, Number(req.query.limit) || DEFAULT_PAGE_SIZE));
    const userEmail = req.user.email;

    const result = await findTransactionsByUserEmail(userEmail, page, limit);
    return response.ok(res, result);
  } catch (error) {
    next(error);
  }
};

/**
 * Returns one transaction by id if it belongs to the current user; 404 if not found or not owned.
 */
export const getTransactionById = async (req, res, next) => {
  try {
    const { transactionId } = req.params;
    const userEmail = req.user.email;
    const result = await findTransactionById(transactionId, userEmail);

    if (result.status === 'NOT_FOUND') throw new AppError('Transaction not found', 404);
    return response.ok(res, { transaction: result.data });
  } catch (error) {
    next(error);
  }
};

/**
 * Creates a transfer from the current user to receiverEmail. Validates amount; returns 201 with the created transaction.
 */
export const createTransaction = async (req, res, next) => {
  try {
    const { receiverEmail, amount, description } = req.body;
    const senderEmail = req.user.email;
    if (!receiverEmail) throw new AppError('Receiver email is required', 400);

    const amountValidation = validateTransactionAmount(amount);
    if (!amountValidation.isValid) throw new AppError(amountValidation.error, 400);

    const transaction = await executeTransfer(
      senderEmail,
      receiverEmail,
      amountValidation.sanitized,
      description,
    );
    return response.created(res, { transaction });
  } catch (error) {
    next(error);
  }
};
