import mongoose from 'mongoose';
import { MongoMemoryReplSet } from 'mongodb-memory-server';

let mongoReplSet = null;

const DISCONNECTED_STATE = 0;
const REPLICA_SET_COUNT = 1;
const STORAGE_ENGINE = 'wiredTiger';

export const connectDB = async () => {
  if (mongoose.connection.readyState !== DISCONNECTED_STATE) {
    return;
  }

  if (mongoReplSet) {
    const existingUri = mongoReplSet.getUri();
    await mongoose.connect(existingUri);
    return;
  }

  try {
    const replSetConfig = {
      replSet: {
        count: REPLICA_SET_COUNT,
        storageEngine: STORAGE_ENGINE,
      },
    };
    mongoReplSet = await MongoMemoryReplSet.create(replSetConfig);

    const mongoUri = mongoReplSet.getUri();
    await mongoose.connect(mongoUri);
  } catch (error) {
    if (mongoReplSet) {
      await mongoReplSet.stop();
      mongoReplSet = null;
    }
    throw error;
  }
};

export const closeDB = async () => {
  const isConnected = mongoose.connection.readyState !== DISCONNECTED_STATE;

  if (isConnected) {
    try {
      await mongoose.connection.dropDatabase();
    } catch (error) {
    }

    try {
      await mongoose.connection.close();
    } catch (error) {
    }
  }

  if (mongoReplSet) {
    try {
      await mongoReplSet.stop();
    } catch (error) {
    }
    mongoReplSet = null;
  }
};

export const clearDB = async () => {
  const isConnected = mongoose.connection.readyState !== DISCONNECTED_STATE;
  if (!isConnected) {
    return;
  }

  const collections = mongoose.connection.collections;
  const collectionNames = Object.keys(collections);

  for (const collectionName of collectionNames) {
    const collection = collections[collectionName];
    try {
      await collection.deleteMany({});
    } catch (error) {
    }
  }
};
