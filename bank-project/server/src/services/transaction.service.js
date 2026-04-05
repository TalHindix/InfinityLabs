import crypto from 'node:crypto';
import mongoose from 'mongoose';
import Transaction, { getNextTransactionId } from '../models/transaction.model.js';
import User from '../models/user.model.js';
import { DEFAULT_PAGE_SIZE } from '../constants/index.js';
import { AppError } from '../utils/error.util.js';
import { sendTransferNotificationEmailAsync } from '../utils/email.util.js';
import config from '../config/index.js';

function transactionsForUserQuery(userEmail) {
  return { $or: [{ fromEmail: userEmail }, { toEmail: userEmail }] };
}

export async function findTransactionsByUserEmail(userEmail, page = 1, pageSize = DEFAULT_PAGE_SIZE) {
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
}

export async function findRecentTransactions(userEmail, pageSize = 10) {
  const query = transactionsForUserQuery(userEmail);
  return Transaction.find(query).sort({ createdAt: -1 }).limit(pageSize);
}

export async function findTransactionById(transactionId, userEmail) {
  const query = { id: Number(transactionId), ...transactionsForUserQuery(userEmail) };
  return Transaction.findOne(query);
}

async function deductSenderBalance(senderEmail, amount, session) {
  const sender = await User.findOneAndUpdate(
    { email: senderEmail, balance: { $gte: amount } },
    { $inc: { balance: -amount } },
    { session, new: true }
  );
  if (!sender) throw new AppError('Insufficient funds', 400);
  return sender;
}

async function addReceiverBalance(receiverEmail, amount, session) {
  const receiver = await User.findOneAndUpdate(
    { email: receiverEmail },
    { $inc: { balance: amount } },
    { session, new: true }
  );
  if (!receiver) throw new AppError('Receiver not found', 404);
  return receiver;
}

async function createTransactionRecord(senderEmail, receiverEmail, amount, description, session) {
  const nextId = await getNextTransactionId(session);
  const [transaction] = await Transaction.create(
    [{ id: nextId, fromEmail: senderEmail, toEmail: receiverEmail, amount, description }],
    { session }
  );
  return transaction;
}

export async function executeTransfer(senderEmail, receiverEmail, amount, description) {
  if (receiverEmail.toLowerCase() === senderEmail.toLowerCase()) {
    throw new AppError('Cannot transfer to yourself', 400);
  }

  const session = await mongoose.startSession();
  session.startTransaction();

  try {
    await deductSenderBalance(senderEmail, amount, session);
    await addReceiverBalance(receiverEmail, amount, session);
    const transaction = await createTransactionRecord(
      senderEmail,
      receiverEmail,
      amount,
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
}

export function generateVideoCallRoomName(firstEmail, secondEmail) {
  const emails = [firstEmail.toLowerCase(), secondEmail.toLowerCase()].sort();
  const pair = emails.join('|');
  const hash = crypto.createHash('sha256').update(pair).digest('hex');
  return hash.slice(0, 16);
}

export async function getMonthlySpending(userEmail, months = 6) {
  const startDate = new Date();
  startDate.setMonth(startDate.getMonth() - months);
  startDate.setDate(1);
  startDate.setHours(0, 0, 0, 0);

  return Transaction.aggregate([
    { $match: { fromEmail: userEmail, createdAt: { $gte: startDate } } },
    {
      $group: {
        _id: { year: { $year: '$createdAt' }, month: { $month: '$createdAt' } },
        totalSpent: { $sum: '$amount' },
        transactionCount: { $count: {} },
      },
    },
    { $sort: { '_id.year': 1, '_id.month': 1 } },
    {
      $project: {
        _id: 0,
        year: '$_id.year',
        month: '$_id.month',
        totalSpent: { $round: ['$totalSpent', 2] },
        transactionCount: 1,
      },
    },
  ]);
}

export async function getTopRecipients(userEmail, months = 6, limit = 5) {
  const startDate = new Date();
  startDate.setMonth(startDate.getMonth() - months);
  startDate.setDate(1);
  startDate.setHours(0, 0, 0, 0);

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
}

export async function sendTransferEmailNotification(transaction, sender, receiver) {
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
}
