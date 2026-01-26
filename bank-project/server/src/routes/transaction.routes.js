import { Router } from 'express';
import { getTransactions, createTransaction, getTransactionById } from '../controllers/transaction.controller.js';
import { protect } from '../middleware/auth.middleware.js';
import { transactionLimiter } from '../middleware/rateLimit.middleware.js';

const router = Router();

router.use(protect);

router.route('/')
  .get(getTransactions)
  .post(transactionLimiter, createTransaction);

router.route('/:transactionId')
  .get(getTransactionById);

export default router;