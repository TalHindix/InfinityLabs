import crypto from 'node:crypto';
import mongoose from 'mongoose';
import Transaction, { getNextTransactionId } from '../models/transaction.model.js';
import User from '../models/user.model.js';
import { DEFAULT_PAGE_SIZE } from '../constants/index.js';
import { AppError } from '../utils/error.util.js';
import { sendTransferNotificationEmailAsync } from '../utils/email.util.js';
import { validateTransactionAmount } from '../utils/validation.util.js';
import { findUserByEmail } from './user.service.js';
import config from '../config/index.js';

const transactionsForUserQuery = (userEmail) => ({
  $or: [{ fromEmail: userEmail }, { toEmail: userEmail }],
});

const startOfMonthNMonthsAgo = (months) => {
  const date = new Date();
  date.setMonth(date.getMonth() - months);
  date.setDate(1);
  date.setHours(0, 0, 0, 0);
  return date;
};

export const findTransactionsByUserEmail = async (userEmail, page = 1, pageSize = DEFAULT_PAGE_SIZE) => {
  const firstItemIndex = (page - 1) * pageSize;
  const query = transactionsForUserQuery(userEmail);

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

export const findRecentTransactions = async (userEmail, pageSize = DEFAULT_PAGE_SIZE) => {
  const query = transactionsForUserQuery(userEmail);
  return Transaction.find(query).sort({ createdAt: -1 }).limit(pageSize);
};

export const findTransactionById = async (transactionId, userEmail) => {
  const query = { id: Number(transactionId), ...transactionsForUserQuery(userEmail) };
  return Transaction.findOne(query);
};

// Atomic check-and-decrement: the `balance: { $gte: amount }` filter and the
// `$inc: -amount` update happen in a single Mongo operation, so two concurrent
// transfers can never both "see enough" and each withdraw — one will miss the
// filter and get null.
const deductSenderBalance = async (senderEmail, amount, session) => {
  const sender = await User.findOneAndUpdate(
    { email: senderEmail, balance: { $gte: amount } },
    { $inc: { balance: -amount } },
    { session, new: true }
  );
  if (!sender) throw new AppError('Insufficient funds', 400);
  return sender;
};

const addReceiverBalance = async (receiverEmail, amount, session) => {
  const receiver = await User.findOneAndUpdate(
    { email: receiverEmail },
    { $inc: { balance: amount } },
    { session, new: true }
  );
  if (!receiver) throw new AppError('Receiver not found', 404);
  return receiver;
};

const createTransactionRecord = async (senderEmail, receiverEmail, amount, description, session) => {
  const nextId = await getNextTransactionId(session);
  const [transaction] = await Transaction.create(
    [{ id: nextId, fromEmail: senderEmail, toEmail: receiverEmail, amount, description }],
    { session }
  );
  return transaction;
};

// Money movement runs in a Mongo multi-document transaction: deduct, credit,
// and insert the ledger row either all commit or all roll back. Requires the
// database to be a replica set (single-node rs0 is fine for dev) — standalone
// mongod will throw on startSession.
export const executeTransfer = async (senderEmail, receiverEmail, amount, description) => {
  if (!receiverEmail) {
    throw new AppError('Receiver email is required', 400);
  }

  const amountValidation = validateTransactionAmount(amount);
  if (!amountValidation.isValid) {
    throw new AppError(amountValidation.error, 400);
  }

  if (receiverEmail.toLowerCase() === senderEmail.toLowerCase()) {
    throw new AppError('Cannot transfer to yourself', 400);
  }

  const session = await mongoose.startSession();
  session.startTransaction();

  try {
    await deductSenderBalance(senderEmail, amountValidation.sanitized, session);
    await addReceiverBalance(receiverEmail, amountValidation.sanitized, session);
    const transaction = await createTransactionRecord(
      senderEmail,
      receiverEmail,
      amountValidation.sanitized,
      description,
      session
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

// Deterministic, order-independent room id for a pair of users:
// sorting makes (A,B) and (B,A) hash to the same room; hashing keeps raw
// emails out of the URL; 16 hex chars (64 bits) is long enough that a
// stranger can't guess a real call while staying short enough for a URL.
export const generateVideoCallRoomName = (firstEmail, secondEmail) => {
  const emails = [firstEmail.toLowerCase(), secondEmail.toLowerCase()].sort();
  const pair = emails.join('|');
  const hash = crypto.createHash('sha256').update(pair).digest('hex');
  return hash.slice(0, 16);
};

const aggregateMonthlyByDirection = async (emailField, totalKey, userEmail, months) => {
  const startDate = startOfMonthNMonthsAgo(months);
  return Transaction.aggregate([
    { $match: { [emailField]: userEmail, createdAt: { $gte: startDate } } },
    {
      $group: {
        _id: { year: { $year: '$createdAt' }, month: { $month: '$createdAt' } },
        total: { $sum: '$amount' },
        transactionCount: { $count: {} },
      },
    },
    { $sort: { '_id.year': 1, '_id.month': 1 } },
    {
      $project: {
        _id: 0,
        year: '$_id.year',
        month: '$_id.month',
        [totalKey]: { $round: ['$total', 2] },
        transactionCount: 1,
      },
    },
  ]);
};

export const getMonthlySpending = (userEmail, months = 6) =>
  aggregateMonthlyByDirection('fromEmail', 'totalSpent', userEmail, months);

export const getMonthlyReceived = (userEmail, months = 6) =>
  aggregateMonthlyByDirection('toEmail', 'totalReceived', userEmail, months);

export const getTopRecipients = async (userEmail, months = 6, limit = 5) => {
  const startDate = startOfMonthNMonthsAgo(months);

  return Transaction.aggregate([
    { $match: { fromEmail: userEmail, createdAt: { $gte: startDate } } },
    {
      $group: {
        _id: '$toEmail',
        totalSent: { $sum: '$amount' },
        transactionCount: { $count: {} },
      },
    },
    { $sort: { totalSent: -1 } },
    { $limit: limit },
    {
      $project: {
        _id: 0,
        email: '$_id',
        totalSent: { $round: ['$totalSent', 2] },
        transactionCount: 1,
      },
    },
  ]);
};

export const sendTransferEmailNotification = async (transaction, sender, receiver) => {
  const roomName = generateVideoCallRoomName(sender.email, receiver.email);
  const videoCallUrl = `${config.clientUrl}/video-call/${roomName}`;
  const receiverName = `${receiver.firstName} ${receiver.lastName}`;
  const senderName = `${sender.firstName} ${sender.lastName}`;

  sendTransferNotificationEmailAsync({
    receiverEmail: receiver.email,
    receiverName,
    senderName,
    senderEmail: sender.email,
    amount: transaction.amount,
    description: transaction.description,
    transactionId: transaction.id,
    videoCallUrl,
  });
};

export const notifyTransferRecipient = async (transactionId, actingUserEmail) => {
  const transaction = await findTransactionById(transactionId, actingUserEmail);
  if (!transaction) throw new AppError('Transaction not found', 404);

  const isSender = transaction.fromEmail.toLowerCase() === actingUserEmail.toLowerCase();
  if (!isSender) {
    throw new AppError('You can only send notifications for your own transfers', 403);
  }

  const [sender, receiver] = await Promise.all([
    findUserByEmail(transaction.fromEmail),
    findUserByEmail(transaction.toEmail),
  ]);

  if (!sender) throw new AppError('Sender not found', 404);
  if (!receiver) throw new AppError('Receiver not found', 404);

  await sendTransferEmailNotification(transaction, sender, receiver);

  const roomName = generateVideoCallRoomName(sender.email, receiver.email);

  return { roomName };
};
