import mongoose from 'mongoose';
import Transaction, { getNextTransactionId } from '../models/transaction.model.js';
import User from '../models/user.model.js';
import { DEFAULT_PAGE_SIZE } from '../constants/index.js';
import { AppError } from '../utils/error.util.js';

const userTransactionQuery = (userEmail) => ({
  $or: [{ fromEmail: userEmail }, { toEmail: userEmail }],
});

export const findTransactionsByUserEmail = async (userEmail, page = 1, pageSize = DEFAULT_PAGE_SIZE) => {
  const firstItemIndex = (page - 1) * pageSize;
  const query = userTransactionQuery(userEmail);

  const [transactions, total] = await Promise.all([
    Transaction.find(query).sort({ createdAt: -1 }).skip(firstItemIndex).limit(pageSize),
    Transaction.countDocuments(query),
  ]);
  return {
    transactions,
    total,
    totalPages: Math.ceil(total / pageSize),
    currentPage: page,
  };
};

export const findRecentTransactions = async (userEmail, pageSize = 10) => {
  return Transaction.find(userTransactionQuery(userEmail))
    .sort({ createdAt: -1 })
    .limit(pageSize);
};

export const findTransactionById = async (transactionId, userEmail) => {
  const transaction = await Transaction.findOne({
    id: Number(transactionId),
    ...userTransactionQuery(userEmail),
  });
  return transaction ? { status: 'SUCCESS', data: transaction } : { status: 'NOT_FOUND', data: null };
};


/**
 * Deducts amount from sender's balance
 * @param {string} senderEmail - Email of the sender
 * @param {number} amount - Amount to deduct
 * @param {mongoose.ClientSession} session - MongoDB session for transaction
 * @returns {Promise<User>} Updated sender user document
 * @throws {AppError} If sender not found or insufficient funds
 */
const deductSenderBalance = async (senderEmail, amount, session) => {
  const sender = await User.findOneAndUpdate(
    { email: senderEmail, balance: { $gte: amount } },
    { $inc: { balance: -amount } },
    { session, new: true },
  );
  if (!sender) throw new AppError('Insufficient funds', 400);
  return sender;
};

/**
 * Adds amount to receiver's balance
 * @param {string} receiverEmail - Email of the receiver
 * @param {number} amount - Amount to add
 * @param {mongoose.ClientSession} session - MongoDB session for transaction
 * @returns {Promise<User>} Updated receiver user document
 * @throws {AppError} If receiver not found
 */
const addReceiverBalance = async (receiverEmail, amount, session) => {
  const receiver = await User.findOneAndUpdate(
    { email: receiverEmail },
    { $inc: { balance: amount } },
    { session, new: true },
  );
  if (!receiver) throw new AppError('Receiver not found', 404);
  return receiver;
};

/**
 * Creates transaction record in database
 * @param {string} senderEmail - Email of the sender
 * @param {string} receiverEmail - Email of the receiver
 * @param {number} amount - Transfer amount
 * @param {string} description - Optional transaction description
 * @param {mongoose.ClientSession} session - MongoDB session for transaction
 * @returns {Promise<Transaction>} Created transaction document
 */
const createTransactionRecord = async (senderEmail, receiverEmail, amount, description, session) => {
  const id = await getNextTransactionId(session);
  const [transaction] = await Transaction.create(
    [{ id, fromEmail: senderEmail, toEmail: receiverEmail, amount, description }],
    { session },
  );
  return transaction;
};

/**
 * Executes a money transfer between two users
 * Uses MongoDB transactions to ensure atomicity
 * 
 * @param {string} senderEmail - Email of the user sending money
 * @param {string} receiverEmail - Email of the user receiving money
 * @param {number} amount - Amount to transfer (validated before this function)
 * @param {string} description - Optional transaction description
 * @returns {Promise<Transaction>} Created transaction record
 * @throws {AppError} For validation or business logic errors
 */
export const executeTransfer = async (senderEmail, receiverEmail, amount, description) => {
  const session = await mongoose.startSession();
  session.startTransaction();
  try {
    if (receiverEmail.toLowerCase() === senderEmail.toLowerCase()) {
      throw new AppError('Cannot transfer to yourself', 400);
    }
    await deductSenderBalance(senderEmail, amount, session);
    await addReceiverBalance(receiverEmail, amount, session);
    const transaction = await createTransactionRecord(
      senderEmail,
      receiverEmail,
      amount,
      description,
      session,
    );
    await session.commitTransaction();
    return transaction;
  } catch (error) {
    await session.abortTransaction();
    throw error;
  } finally {
    session.endSession();
  }
};
