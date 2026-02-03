import app from './src/app.js';
import config from './src/config/index.js';
import connectDB from './src/config/database.js';
import logger from './src/utils/logger.util.js';
import { Server } from 'socket.io';
import { initChatbotSocket } from './src/socket/socket.handler.js';

const startServer = async () => {
  await connectDB();
  
  const server = app.listen(config.port, () => {
    logger.info(`Listening on ${config.port}`);
  });

  const io = new Server(server, {
    cors: {
      origin: config.clientUrl || 'http://localhost:5173',
      methods: ['GET', 'POST']
    }
  });

  initChatbotSocket(io);
};

startServer();