import { DEFAULT_PAGE_SIZE, MAX_PAGE_SIZE, MAX_ANALYTICS_MONTHS, DEFAULT_ANALYTICS_MONTHS } from '../constants/index.js';
import {
  findTransactionsByUserEmail,
  findTransactionById,
  executeTransfer,
  notifyTransferRecipient,
  getMonthlySpending,
  getMonthlyReceived,
  getTopRecipients,
} from '../services/transaction.service.js';
import * as response from '../utils/response.util.js';
import { AppError } from '../utils/error.util.js';

export const getTransactions = async (req, res, next) => {
  try {
    const page = Math.max(1, Number(req.query.page) || 1);
    const limit = Math.min(
      MAX_PAGE_SIZE,
      Math.max(1, Number(req.query.limit) || DEFAULT_PAGE_SIZE)
    );
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

    const transaction = await findTransactionById(transactionId, userEmail);
    if (!transaction) throw new AppError('Transaction not found', 404);

    return response.ok(res, { transaction });
  } catch (error) {
    next(error);
  }
};

export const createTransaction = async (req, res, next) => {
  try {
    const { receiverEmail, amount, description } = req.body;
    const senderEmail = req.user.email;

    const transaction = await executeTransfer(senderEmail, receiverEmail, amount, description);

    return response.created(res, { transaction });
  } catch (error) {
    next(error);
  }
};

export const getSpendingAnalytics = async (req, res, next) => {
  try {
    const userEmail = req.user.email;
    const months = Math.min(MAX_ANALYTICS_MONTHS, Math.max(1, Number(req.query.months) || DEFAULT_ANALYTICS_MONTHS));

    const [monthlySpending, monthlyReceived, topRecipients] = await Promise.all([
      getMonthlySpending(userEmail, months),
      getMonthlyReceived(userEmail, months),
      getTopRecipients(userEmail, months),
    ]);

    const totalSpent = monthlySpending.reduce((sum, m) => sum + m.totalSpent, 0);
    const totalReceived = monthlyReceived.reduce((sum, m) => sum + m.totalReceived, 0);

    return response.ok(res, {
      monthlySpending,
      monthlyReceived,
      topRecipients,
      totalSpent: Math.round(totalSpent * 100) / 100,
      totalReceived: Math.round(totalReceived * 100) / 100,
    });
  } catch (error) {
    next(error);
  }
};

export const sendTransferNotification = async (req, res, next) => {
  try {
    const { transactionId } = req.params;
    const userEmail = req.user.email;

    const result = await notifyTransferRecipient(transactionId, userEmail);

    return response.ok(res, {
      message: 'Notification email sent successfully',
      roomName: result.roomName,
    });
  } catch (error) {
    next(error);
  }
};
