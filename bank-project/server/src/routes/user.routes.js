import { Router } from 'express';
import { getCurrentUser } from '../controllers/user.controller.js';
import { protect } from '../middleware/auth.middleware.js';

const router = Router();

router.use(protect);

router.get('/me', getCurrentUser);

export default router;