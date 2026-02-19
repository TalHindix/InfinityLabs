import { Router } from 'express';
import {
  getTransactions,
  createTransaction,
  getTransactionById,
  sendTransferNotification,
} from '../controllers/transaction.controller.js';
import { protect } from '../middleware/auth.middleware.js';
import { transactionLimiter } from '../middleware/ratelimit.middleware.js';

const router = Router();

router.use(protect);

router
  .route('/')
  .get(getTransactions)
  .post(transactionLimiter, createTransaction);
router.route('/:transactionId').get(getTransactionById);
router.route('/:transactionId/send-notification').post(sendTransferNotification);

export default router;