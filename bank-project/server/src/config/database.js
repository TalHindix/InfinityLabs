import mongoose from 'mongoose';
import config from './index.js';
import logger from '../utils/logger.util.js';

const connectDB = async () => {
  try {
    const conn = await mongoose.connect(config.mongoUri);
    logger.info(`MongoDB Connected: ${conn.connection.host}/${conn.connection.name}`);
  } catch (error) {
    logger.error(`MongoDB Connection Error: ${error.message}`);
    process.exit(1);
  }
};

export default connectDB;