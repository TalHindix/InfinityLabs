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

app.set('trust proxy', 1);

app.use(cors({
  origin: config.clientUrl,
  credentials: true,
}));

app.use(cookieParser());
app.use(express.json({ limit: '10kb' }));
app.use(requestLogger);

app.get(['/', '/health'], (req, res) => {
  res.json({ status: 'ok', timestamp: new Date().toISOString() });
});

app.use('/api/v1/auth', authRoutes);
app.use('/api/v1', userRoutes);
app.use('/api/v1/transactions', transactionRoutes);

app.use(notFoundHandler);
app.use(errorHandler);

export default app;