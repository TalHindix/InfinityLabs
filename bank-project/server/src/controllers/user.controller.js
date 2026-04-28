import { RECENT_TRANSACTIONS_LIMIT } from '../constants/index.js';
import { findRecentTransactions } from '../services/transaction.service.js';
import * as response from '../utils/response.util.js';

export const getCurrentUser = async (req, res, next) => {
  try {
    const user = req.user;
    const recentTransactions = await findRecentTransactions(user.email, RECENT_TRANSACTIONS_LIMIT);
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
    next(error);
  }
};
