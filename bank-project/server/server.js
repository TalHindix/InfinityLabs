import mongoose from 'mongoose';
import app from './src/app.js';
import config from './src/config/index.js';
import connectDB from './src/config/database.js';
import logger from './src/utils/logger.util.js';
import { Server } from 'socket.io';
import { initChatbotSocket } from './src/socket/socket.handler.js';

let httpServer = null;

const startServer = async () => {
  await connectDB();

  httpServer = app.listen(config.port, () => {
    logger.info(`Listening on ${config.port}`);
  });

  const io = new Server(httpServer, {
    cors: {
      origin: config.clientUrl || 'http://localhost:5173',
      methods: ['GET', 'POST'],
      credentials: true,
    },
  });

  initChatbotSocket(io);
  logger.info('Socket.IO chatbot initialized');
};

const shutdown = () => {
  logger.info('Shutting down gracefully');
  if (httpServer) {
    httpServer.close(() => {
      mongoose.connection.close(false)
        .then(() => process.exit(0))
        .catch(() => process.exit(1));
    });
  } else {
    process.exit(0);
  }
};

process.on('SIGTERM', shutdown);
process.on('SIGINT', shutdown);

try {
  await startServer();
} catch (err) {
  logger.error('Failed to start server', err);
  process.exit(1);
}