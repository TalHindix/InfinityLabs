import { DEFAULT_PAGE_SIZE } from '../constants/index.js';
import {
  findTransactionById,
  findTransactionsByUserEmail,
  executeTransfer
} from '../services/transaction.service.js';
import * as response from '../utils/response.util.js';
import { AppError } from '../middleware/error.middleware.js';

export const getTransactions = async (req, res, next) => {
  try {
    const MAX_LIMIT = 100;
    const page = Math.max(1, Number(req.query.page) || 1);
    const limit = Math.min(MAX_LIMIT, Math.max(1, Number(req.query.limit) || DEFAULT_PAGE_SIZE));
    const userEmail = req.user.email;

    const result = await findTransactionsByUserEmail(userEmail, page, limit);

    return response.ok(res, result);
  } catch (error) {
    next(error);
  }
};

export const getTransactionById = async (req, res, next) => {
  try {
    const { transactionId } = req.params;
    const userEmail = req.user.email;

    const result = await findTransactionById(transactionId, userEmail);

    if (result.status === 'NOT_FOUND') {
      throw new AppError('Transaction not found', 404);
    }

    return response.ok(res, { transaction: result.data });
  } catch (error) {
    next(error);
  }
};

export const createTransaction = async (req, res, next) => {
  try {
    const { receiverEmail, amount, description } = req.body;
    const senderEmail = req.user.email;

    if (!receiverEmail || !amount) {
      throw new AppError('Receiver email and amount are required', 400);
    }

    const transaction = await executeTransfer(senderEmail, receiverEmail, amount, description);

    return response.created(res, { transaction });
  } catch (error) {
    next(error);
  }
};
