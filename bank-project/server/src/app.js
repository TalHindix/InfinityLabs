import express from 'express';
import cors from 'cors';

import config from './config/index.js';
import authRoutes from './routes/auth.routes.js';
import userRoutes from './routes/user.routes.js';
import transactionRoutes from './routes/transaction.routes.js';
import { requestLogger } from './middleware/logger.middleware.js';
import { errorHandler, notFoundHandler } from './middleware/error.middleware.js';

const app = express();

// Middleware
app.use(cors({
  origin: config.clientUrl || 'http://localhost:5173',
  credentials: true,
}));


app.use(express.json({ limit: '10kb' }));
app.use(requestLogger);


// Routes
app.use('/api/v1/auth', authRoutes);
app.use('/api/v1', userRoutes);
app.use('/api/v1/transactions', transactionRoutes);

// Health check
app.get('/health', (req, res) => {
  res.json({ status: 'ok', timestamp: new Date().toISOString() });
});

// Error handling (must be after all routes)
app.use(notFoundHandler);
app.use(errorHandler);

export default app;