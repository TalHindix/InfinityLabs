import { Router } from 'express';
import {
  getTransactions,
  createTransaction,
  getTransactionById,
} from '../controllers/transaction.controller.js';
import { protect } from '../middleware/auth.middleware.js';
import { transactionLimiter } from '../middleware/rateLimit.middleware.js';

/**
 * Transaction routes (mounted at /api/v1/transactions):
 * GET / = list current user's transactions (paginated)
 * POST / = create a transfer (rate-limited)
 * GET /:transactionId = get one transaction by id (must belong to user)
 * All routes require a valid JWT (protect middleware).
 */
const router = Router();

router.use(protect);

router
  .route('/')
  .get(getTransactions)
  .post(transactionLimiter, createTransaction);
router.route('/:transactionId').get(getTransactionById);

export default router;