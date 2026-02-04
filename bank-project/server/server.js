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

  httpServer.on('error', (err) => {
    logger.error('HTTP server error', err);
    process.exit(1);
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

const shutdown = (signal) => {
  logger.info(`${signal} received, shutting down gracefully`);
  if (httpServer) {
    httpServer.close(() => {
      logger.info('HTTP server closed');
      mongoose.connection.close(false).then(() => {
        logger.info('MongoDB connection closed');
        process.exit(0);
      }).catch((err) => {
        logger.error('Error closing MongoDB connection', err);
        process.exit(1);
      });
    });
  } else {
    process.exit(0);
  }
};

process.on('SIGTERM', () => shutdown('SIGTERM'));
process.on('SIGINT', () => shutdown('SIGINT'));

startServer().catch((err) => {
  logger.error('Failed to start server', err);
  process.exit(1);
});
