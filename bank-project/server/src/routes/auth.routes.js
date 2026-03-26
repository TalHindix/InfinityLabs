import { Router } from 'express';
import {
  signup,
  verifyEmail,
  login,
  logout,
  resendVerification,
  verifyOtp,
  resendOtp,
} from '../controllers/auth.controller.js';
import { authLimiter, verifyLimiter } from '../middleware/ratelimit.middleware.js';

const router = Router();

router.post('/signup', authLimiter, signup);
router.get('/verify', verifyLimiter, verifyEmail);
router.post('/resend-verification', authLimiter, resendVerification);
router.post('/login', authLimiter, login);
router.post('/logout', logout);
router.post('/verify-otp', authLimiter, verifyOtp);
router.post('/resend-otp', authLimiter, resendOtp);

export default router;