import mongoose from 'mongoose';
import config from './index.js';
import logger from '../utils/logger.util.js';

const RETRIES = parseInt(process.env.MONGO_CONNECT_RETRIES, 10) || 3;
const DELAY_MS = parseInt(process.env.MONGO_CONNECT_RETRY_DELAY_MS, 10) || 2000;

const sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));

const connectDB = async () => {
  let lastError;
  for (let attempt = 1; attempt <= RETRIES; attempt++) {
    try {
      const conn = await mongoose.connect(config.mongoUri);
      logger.info(`MongoDB Connected: ${conn.connection.host}/${conn.connection.name}`);
      return;
    } catch (error) {
      lastError = error;
      logger.warn(`MongoDB connection attempt ${attempt}/${RETRIES} failed: ${error.message}`);
      if (attempt < RETRIES) {
        await sleep(DELAY_MS);
      }
    }
  }
  logger.error(`MongoDB connection failed after ${RETRIES} attempts: ${lastError.message}`);
  process.exit(1);
};

export default connectDB;
