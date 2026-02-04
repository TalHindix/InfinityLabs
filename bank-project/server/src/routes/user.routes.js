import { Router } from 'express';
import { getCurrentUser } from '../controllers/user.controller.js';
import { protect } from '../middleware/auth.middleware.js';

/**
 * User routes (mounted at /api/v1): GET /me returns the current authenticated user.
 * All routes require a valid JWT (protect middleware).
 */
const router = Router();

router.use(protect);
router.get('/me', getCurrentUser);

export default router;