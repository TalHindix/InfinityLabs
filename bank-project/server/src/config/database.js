import mongoose from 'mongoose';
import config from './index.js';
import logger from '../utils/logger.util.js';

const MAX_RETRIES = parseInt(process.env.MONGO_CONNECT_RETRIES, 10) || 5;
const RETRY_DELAY_MS = parseInt(process.env.MONGO_CONNECT_RETRY_DELAY_MS, 10) || 2000;

/**
 * Waits for the given number of milliseconds.
 */
const sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));

/**
 * Connects to MongoDB using config.mongoUri.
 * Retries with exponential backoff on failure (configurable via MONGO_CONNECT_RETRIES and MONGO_CONNECT_RETRY_DELAY_MS).
 * On final failure, logs the error and exits the process with code 1.
 */
const connectDB = async () => {
  let lastError;
  for (let attempt = 1; attempt <= MAX_RETRIES; attempt++) {
    try {
      const conn = await mongoose.connect(config.mongoUri);
      logger.info(`MongoDB Connected: ${conn.connection.host}/${conn.connection.name}`);
      return;
    } catch (error) {
      lastError = error;
      logger.warn(
        `MongoDB connection attempt ${attempt}/${MAX_RETRIES} failed: ${error.message}`,
        { error }
      );
      if (attempt < MAX_RETRIES) {
        const delay = RETRY_DELAY_MS * Math.pow(2, attempt - 1);
        logger.info(`Retrying in ${delay}ms...`);
        await sleep(delay);
      }
    }
  }
  logger.error(
    `MongoDB Connection Error after ${MAX_RETRIES} attempts: ${lastError.message}`,
    { error: lastError }
  );
  process.exit(1);
};

export default connectDB;