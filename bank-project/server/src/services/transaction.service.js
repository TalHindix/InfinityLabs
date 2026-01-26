import mongoose from 'mongoose';
import Transaction, { getNextTransactionId } from '../models/transaction.model.js';
import User from '../models/user.model.js';
import { DEFAULT_PAGE_SIZE, TRANSACTION_STATUS } from '../constants/index.js';

const userTransactionQuery = (userEmail) => ({
  $or: [{ fromEmail: userEmail }, { toEmail: userEmail }]
});

export const findTransactionsByUserEmail = async (userEmail, page = 1, pageSize = DEFAULT_PAGE_SIZE) => {
  const firstItemIndex = (page - 1) * pageSize;
  const query = userTransactionQuery(userEmail);

  const [transactions, total] = await Promise.all([
    Transaction.find(query)
      .sort({ createdAt: -1 })
      .skip(firstItemIndex)
      .limit(pageSize),
    Transaction.countDocuments(query)
  ]);

  return {
    transactions,
    total,
    totalPages: Math.ceil(total / pageSize),
    currentPage: page
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
    ...userTransactionQuery(userEmail)
  });

  return transaction
    ? { status: 'SUCCESS', data: transaction }
    : { status: 'NOT_FOUND', data: null };
};

export const executeTransfer = async (senderEmail, receiverEmail, amount, description) => {
  const session = await mongoose.startSession();
  session.startTransaction();

  try {
    if (receiverEmail.toLowerCase() === senderEmail.toLowerCase()) {
      const error = new Error('Cannot transfer to yourself');
      error.status = 400;
      throw error;
    }

    const sender = await User.findOneAndUpdate(
      { email: senderEmail, balance: { $gte: amount } },
      { $inc: { balance: -amount } },
      { session, new: true }
    );

    if (!sender) {
      const error = new Error('Insufficient funds');
      error.status = 400;
      throw error;
    }

    const receiver = await User.findOneAndUpdate(
      { email: receiverEmail },
      { $inc: { balance: amount } },
      { session }
    );

    if (!receiver) {
      const error = new Error('Receiver not found');
      error.status = 404;
      throw error;
    }

    const transactionId = await getNextTransactionId(session);

    const [transaction] = await Transaction.create([{
      id: transactionId,
      fromEmail: senderEmail,
      toEmail: receiverEmail,
      amount,
      status: TRANSACTION_STATUS.COMPLETED,
      description
    }], { session });

    await session.commitTransaction();
    return transaction;
  } catch (error) {
    await session.abortTransaction();
    throw error;
  } finally {
    session.endSession();
  }
};
