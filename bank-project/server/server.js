import app from './src/app.js';
import config from './src/config/index.js';
import connectDB from './src/config/database.js';
import logger from './src/utils/logger.util.js';

const startServer = async () => {
  await connectDB();
  
  app.listen(config.port, () => {
    logger.info(`Server running on http://localhost:${config.port}`);
  });
};

startServer();