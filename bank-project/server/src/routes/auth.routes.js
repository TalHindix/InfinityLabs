import { Router } from 'express';
import {
  signup,
  verifyEmail,
  login,
  logout,
  resendVerification,
} from '../controllers/auth.controller.js';
import { authLimiter, verifyLimiter } from '../middleware/rateLimit.middleware.js';

const router = Router();

router.post('/signup', authLimiter, signup);
router.get('/verify', verifyLimiter, verifyEmail);
router.post('/resend-verification', authLimiter, resendVerification);
router.post('/login', authLimiter, login);
router.post('/logout', logout);

export default router;