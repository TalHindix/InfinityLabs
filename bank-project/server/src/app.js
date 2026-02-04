import express from 'express';
import cors from 'cors';
import cookieParser from 'cookie-parser';

import config from './config/index.js';
import authRoutes from './routes/auth.routes.js';
import userRoutes from './routes/user.routes.js';
import transactionRoutes from './routes/transaction.routes.js';
import { requestLogger } from './middleware/logger.middleware.js';
import { errorHandler, notFoundHandler } from './middleware/error.middleware.js';

const app = express();

// Trust proxy - required for Render/Vercel (fixes rate limiter X-Forwarded-For error)
app.set('trust proxy', 1);

// Middleware
app.use(cors({
  origin: config.clientUrl || 'http://localhost:5173',
  credentials: true,
}));
app.use(cookieParser());
app.use(express.json({ limit: '10kb' }));
app.use(requestLogger);

// Health check (before API routes so GET / and GET /health are always available)
app.get(['/', '/health'], (req, res) => {
  res.json({ status: 'ok', timestamp: new Date().toISOString() });
});

// API routes
app.use('/api/v1/auth', authRoutes);
app.use('/api/v1', userRoutes);
app.use('/api/v1/transactions', transactionRoutes);

// Error handling (must be after all routes)
app.use(notFoundHandler);
app.use(errorHandler);

export default app;