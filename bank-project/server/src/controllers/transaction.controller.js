import { DEFAULT_PAGE_SIZE } from '../constants/index.js';
import {
  findTransactionById,
  findTransactionsByUserEmail,
  executeTransfer,
  sendTransferEmailNotification,
  generateVideoCallRoomName,
} from '../services/transaction.service.js';
import * as response from '../utils/response.util.js';
import { mapErrorToResponse } from '../utils/error.util.js';
import { AppError } from '../utils/error.util.js';
import { validateTransactionAmount } from '../utils/validation.util.js';
import User from '../models/user.model.js';

/** Max allowed page size for list transactions (avoids huge responses). */
const MAX_PAGE_SIZE = 100;

/**
 * Returns paginated transactions for the current user. Query: page (default 1), limit (default from constants, max 100).
 */
export const getTransactions = async (req, res) => {
  try {
    const page = Math.max(1, Number(req.query.page) || 1);
    const limit = Math.min(MAX_PAGE_SIZE, Math.max(1, Number(req.query.limit) || DEFAULT_PAGE_SIZE));
    const userEmail = req.user.email;

    const result = await findTransactionsByUserEmail(userEmail, page, limit);
    return response.ok(res, result);
  } catch (error) {
    const { statusCode, message } = mapErrorToResponse(error);

    if (process.env.NODE_ENV === 'development') {
      console.error(`[${req.method}] ${req.originalUrl}:`, error);
    }

    return res.status(statusCode).json({
      success: false,
      error: message,
    });
  }
};

/**
 * Returns one transaction by id if it belongs to the current user; 404 if not found or not owned.
 */
export const getTransactionById = async (req, res) => {
  try {
    const { transactionId } = req.params;
    const userEmail = req.user.email;
    const result = await findTransactionById(transactionId, userEmail);

    if (result.status === 'NOT_FOUND') throw new AppError('Transaction not found', 404);
    return response.ok(res, { transaction: result.data });
  } catch (error) {
    const { statusCode, message } = mapErrorToResponse(error);

    if (process.env.NODE_ENV === 'development') {
      console.error(`[${req.method}] ${req.originalUrl}:`, error);
    }

    return res.status(statusCode).json({
      success: false,
      error: message,
    });
  }
};

/**
 * Creates a transfer from the current user to receiverEmail. Validates amount; returns 201 with the created transaction.
 */
export const createTransaction = async (req, res) => {
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
    const { statusCode, message } = mapErrorToResponse(error);

    if (process.env.NODE_ENV === 'development') {
      console.error(`[${req.method}] ${req.originalUrl}:`, error);
    }

    return res.status(statusCode).json({
      success: false,
      error: message,
    });
  }
};

/**
 * Sends transfer notification email to receiver.
 * Requires transaction to belong to current user (as sender).
 */
export const sendTransferNotification = async (req, res) => {
  try {
    const { transactionId } = req.params;
    const userEmail = req.user.email;
    
    const result = await findTransactionById(transactionId, userEmail);
    if (result.status === 'NOT_FOUND') {
      throw new AppError('Transaction not found', 404);
    }

    const transaction = result.data;
    
    // Verify that current user is the sender
    if (transaction.fromEmail.toLowerCase() !== userEmail.toLowerCase()) {
      throw new AppError('You can only send notifications for your own transfers', 403);
    }

    // Get sender and receiver user details
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
    const { statusCode, message } = mapErrorToResponse(error);

    if (process.env.NODE_ENV === 'development') {
      console.error(`[${req.method}] ${req.originalUrl}:`, error);
    }

    return res.status(statusCode).json({
      success: false,
      error: message,
    });
  }
};

