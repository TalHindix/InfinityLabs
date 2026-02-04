import mongoose from 'mongoose';
import config from './index.js';
import logger from '../utils/logger.util.js';

/**
 * Connects to MongoDB using config.mongoUri.
 * On failure, logs the error (including stack via logger metadata) and exits the process with code 1.
 */
const connectDB = async () => {
  try {
    const conn = await mongoose.connect(config.mongoUri);
    logger.info(`MongoDB Connected: ${conn.connection.host}/${conn.connection.name}`);
  } catch (error) {
    logger.error(`MongoDB Connection Error: ${error.message}`, { error });
    process.exit(1);
  }
};

export default connectDB;