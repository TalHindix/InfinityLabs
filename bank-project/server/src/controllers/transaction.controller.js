import { DEFAULT_PAGE_SIZE } from '../constants/index.js';
import {
  findTransactionById,
  findTransactionsByUserEmail,
  executeTransfer,
  sendTransferEmailNotification,
  generateVideoCallRoomName,
  getMonthlySpending,
  getTopRecipients,
} from '../services/transaction.service.js';
import * as response from '../utils/response.util.js';
import { AppError } from '../utils/error.util.js';
import { validateTransactionAmount } from '../utils/validation.util.js';
import User from '../models/user.model.js';

const MAX_PAGE_SIZE = 100;

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

    if (!receiverEmail) {
      throw new AppError('Receiver email is required', 400);
    }

    const amountValidation = validateTransactionAmount(amount);
    if (!amountValidation.isValid) {
      throw new AppError(amountValidation.error, 400);
    }

    const transaction = await executeTransfer(
      senderEmail,
      receiverEmail,
      amountValidation.sanitized,
      description
    );

    return response.created(res, { transaction });
  } catch (error) {
    next(error);
  }
};

export const getSpendingAnalytics = async (req, res, next) => {
  try {
    const userEmail = req.user.email;
    const months = Math.min(12, Math.max(1, Number(req.query.months) || 6));

    const [monthlySpending, topRecipients] = await Promise.all([
      getMonthlySpending(userEmail, months),
      getTopRecipients(userEmail, months),
    ]);

    return response.ok(res, { monthlySpending, topRecipients });
  } catch (error) {
    next(error);
  }
};

export const sendTransferNotification = async (req, res, next) => {
  try {
    const { transactionId } = req.params;
    const userEmail = req.user.email;

    const transaction = await findTransactionById(transactionId, userEmail);
    if (!transaction) throw new AppError('Transaction not found', 404);

    const isSender =
      transaction.fromEmail.toLowerCase() === userEmail.toLowerCase();
    if (!isSender) {
      throw new AppError(
        'You can only send notifications for your own transfers',
        403
      );
    }

    const [sender, receiver] = await Promise.all([
      User.findOne({ email: transaction.fromEmail }),
      User.findOne({ email: transaction.toEmail }),
    ]);

    if (!sender) throw new AppError('Sender not found', 404);
    if (!receiver) throw new AppError('Receiver not found', 404);

    await sendTransferEmailNotification(transaction, sender, receiver);

    const roomName = generateVideoCallRoomName(sender.email, receiver.email);

    return response.ok(res, {
      message: 'Notification email sent successfully',
      roomName,
    });
  } catch (error) {
    next(error);
  }
};
