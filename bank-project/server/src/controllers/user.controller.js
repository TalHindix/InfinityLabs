import { findUserById } from '../services/user.service.js';
import { findRecentTransactions } from '../services/transaction.service.js';
import * as response from '../utils/response.util.js';
import { mapErrorToResponse } from '../utils/error.util.js';
import { AppError } from '../utils/error.util.js';

/**
 * Returns the current authenticated user (from req.user) and their 10 most recent transactions.
 */
export const getCurrentUser = async (req, res) => {
  try {
    const userId = req.user.id;
    const user = await findUserById(userId);
    if (!user) throw new AppError('User not found', 404);

    const recentTransactions = await findRecentTransactions(user.email, 10);
    return response.ok(res, {
      user: {
        id: user.id,
        firstName: user.firstName,
        lastName: user.lastName,
        email: user.email,
        phone: user.phone,
        balance: user.balance,
        status: user.status,
      },
      recentTransactions,
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
