import { describe, it, expect, beforeAll, afterAll, beforeEach } from '@jest/globals';
import request from 'supertest';
import app from '../../app.js';
import User from '../../models/user.model.js';
import Transaction from '../../models/transaction.model.js';
import { USER_STATUS } from '../../constants/index.js';
import { connectDB, closeDB, clearDB } from '../helpers/db.helper.js';
import { createToken } from '../../utils/jwt.util.js';
import bcrypt from 'bcrypt';

describe('POST /api/v1/transactions', () => {
  let sender;
  let receiver;
  let senderToken;

  beforeAll(async () => {
    await connectDB();
  });

  afterAll(async () => {
    await closeDB();
  });

  beforeEach(async () => {
    await clearDB();

    const password = 'password123';
    const saltRounds = 10;
    const hashedPassword = await bcrypt.hash(password, saltRounds);

    const senderData = {
      firstName: 'John',
      lastName: 'Doe',
      email: 'john@example.com',
      phone: '+972526006496',
      password: hashedPassword,
      status: USER_STATUS.ACTIVE,
      balance: 1000,
    };
    sender = await User.create(senderData);

    const receiverData = {
      firstName: 'Jane',
      lastName: 'Smith',
      email: 'jane@example.com',
      phone: '+972526006496',
      password: hashedPassword,
      status: USER_STATUS.ACTIVE,
      balance: 500,
    };
    receiver = await User.create(receiverData);

    senderToken = createToken(sender);
  });

  it('should transfer money from sender to receiver', async () => {
    const receiverEmail = receiver.email;
    const transferAmount = 100;
    const transferDescription = 'Test transfer';

    const transferData = {
      receiverEmail: receiverEmail,
      amount: transferAmount,
      description: transferDescription,
    };

    const postRequest = request(app)
      .post('/api/v1/transactions')
      .set('Cookie', `token=${senderToken}`)
      .send(transferData);

    const response = await postRequest.expect(201);

    const responseSuccess = response.body.success;
    const responseData = response.body.data;
    const responseTransaction = responseData.transaction;

    expect(responseSuccess).toBe(true);
    expect(responseTransaction).toBeDefined();
    expect(responseTransaction.amount).toBe(transferAmount);
    expect(responseTransaction.fromEmail).toBe(sender.email);
    expect(responseTransaction.toEmail).toBe(receiverEmail);

    const senderEmail = sender.email;
    const receiverEmailForQuery = receiver.email;
    const updatedSender = await User.findOne({ email: senderEmail });
    const updatedReceiver = await User.findOne({ email: receiverEmailForQuery });

    const expectedSenderBalance = 900;
    const expectedReceiverBalance = 600;

    expect(updatedSender.balance).toBe(expectedSenderBalance);
    expect(updatedReceiver.balance).toBe(expectedReceiverBalance);
  });

  it('should reject transfer without authentication', async () => {
    const receiverEmail = receiver.email;
    const transferAmount = 100;

    const transferData = {
      receiverEmail: receiverEmail,
      amount: transferAmount,
    };

    const postRequest = request(app)
      .post('/api/v1/transactions')
      .send(transferData);

    const response = await postRequest.expect(401);

    const responseSuccess = response.body.success;

    expect(responseSuccess).toBe(false);
  });

  it('should reject transfer with insufficient funds', async () => {
    const receiverEmail = receiver.email;
    const transferAmount = 2000;

    const transferData = {
      receiverEmail: receiverEmail,
      amount: transferAmount,
    };

    const postRequest = request(app)
      .post('/api/v1/transactions')
      .set('Cookie', `token=${senderToken}`)
      .send(transferData);

    const response = await postRequest.expect(400);

    const responseSuccess = response.body.success;
    const responseError = response.body.error;

    expect(responseSuccess).toBe(false);
    expect(responseError).toContain('Insufficient funds');
  });

  it('should reject transfer to yourself', async () => {
    const senderEmail = sender.email;
    const transferAmount = 100;

    const transferData = {
      receiverEmail: senderEmail,
      amount: transferAmount,
    };

    const postRequest = request(app)
      .post('/api/v1/transactions')
      .set('Cookie', `token=${senderToken}`)
      .send(transferData);

    const response = await postRequest.expect(400);

    const responseSuccess = response.body.success;
    const responseError = response.body.error;

    expect(responseSuccess).toBe(false);
    expect(responseError).toContain('yourself');
  });

  it('should reject transfer without receiver email', async () => {
    const transferAmount = 100;

    const transferData = {
      amount: transferAmount,
    };

    const postRequest = request(app)
      .post('/api/v1/transactions')
      .set('Cookie', `token=${senderToken}`)
      .send(transferData);

    const response = await postRequest.expect(400);

    const responseSuccess = response.body.success;

    expect(responseSuccess).toBe(false);
  });

  it('should reject transfer with invalid amount', async () => {
    const receiverEmail = receiver.email;
    const invalidAmount = -100;

    const transferData = {
      receiverEmail: receiverEmail,
      amount: invalidAmount,
    };

    const postRequest = request(app)
      .post('/api/v1/transactions')
      .set('Cookie', `token=${senderToken}`)
      .send(transferData);

    const response = await postRequest.expect(400);

    const responseSuccess = response.body.success;

    expect(responseSuccess).toBe(false);
  });

  it('should create transaction record in database', async () => {
    const receiverEmail = receiver.email;
    const transferAmount = 100;
    const transferDescription = 'Test transaction';

    const transferData = {
      receiverEmail: receiverEmail,
      amount: transferAmount,
      description: transferDescription,
    };

    const postRequest = request(app)
      .post('/api/v1/transactions')
      .set('Cookie', `token=${senderToken}`)
      .send(transferData);

    await postRequest.expect(201);

    const senderEmail = sender.email;
    const transactionQuery = {
      fromEmail: senderEmail,
      toEmail: receiverEmail,
    };
    const transaction = await Transaction.findOne(transactionQuery);

    const expectedAmount = 100;
    const expectedDescription = 'Test transaction';

    expect(transaction).toBeTruthy();
    expect(transaction.amount).toBe(expectedAmount);
    expect(transaction.description).toBe(expectedDescription);
  });
});

describe('GET /api/v1/transactions', () => {
  let user;
  let userToken;

  beforeAll(async () => {
    await connectDB();
  });

  afterAll(async () => {
    await closeDB();
  });

  beforeEach(async () => {
    await clearDB();

    const password = 'password123';
    const saltRounds = 10;
    const hashedPassword = await bcrypt.hash(password, saltRounds);

    const userData = {
      firstName: 'Test',
      lastName: 'User',
      email: 'test@example.com',
      phone: '+1234567890',
      password: hashedPassword,
      status: USER_STATUS.ACTIVE,
      balance: 1000,
    };
    user = await User.create(userData);

    userToken = createToken(user);

    const firstTransactionData = {
      id: 1,
      fromEmail: user.email,
      toEmail: 'other@example.com',
      amount: 100,
      description: 'First transaction',
    };

    const secondTransactionData = {
      id: 2,
      fromEmail: 'other@example.com',
      toEmail: user.email,
      amount: 50,
      description: 'Second transaction',
    };

    const thirdTransactionData = {
      id: 3,
      fromEmail: user.email,
      toEmail: 'another@example.com',
      amount: 200,
      description: 'Third transaction',
    };

    await Transaction.create([
      firstTransactionData,
      secondTransactionData,
      thirdTransactionData,
    ]);
  });

  it('should return all transactions for user', async () => {
    const getRequest = request(app)
      .get('/api/v1/transactions')
      .set('Cookie', `token=${userToken}`);

    const response = await getRequest.expect(200);

    const responseSuccess = response.body.success;
    const responseData = response.body.data;
    const responseTransactions = responseData.transactions;
    const responseTotal = responseData.total;

    const expectedTransactionCount = 3;

    expect(responseSuccess).toBe(true);
    expect(responseTransactions).toHaveLength(expectedTransactionCount);
    expect(responseTotal).toBe(expectedTransactionCount);
  });

  it('should return paginated transactions', async () => {
    const pageNumber = 1;
    const limitNumber = 2;
    const queryString = `?page=${pageNumber}&limit=${limitNumber}`;
    const requestPath = `/api/v1/transactions${queryString}`;

    const getRequest = request(app)
      .get(requestPath)
      .set('Cookie', `token=${userToken}`);

    const response = await getRequest.expect(200);

    const responseSuccess = response.body.success;
    const responseData = response.body.data;
    const responseTransactions = responseData.transactions;
    const responseCurrentPage = responseData.currentPage;
    const responseTotalPages = responseData.totalPages;

    const expectedTransactionCount = 2;
    const expectedCurrentPage = 1;
    const expectedTotalPages = 2;

    expect(responseSuccess).toBe(true);
    expect(responseTransactions).toHaveLength(expectedTransactionCount);
    expect(responseCurrentPage).toBe(expectedCurrentPage);
    expect(responseTotalPages).toBe(expectedTotalPages);
  });

  it('should reject request without authentication', async () => {
    const getRequest = request(app)
      .get('/api/v1/transactions');

    const response = await getRequest.expect(401);

    const responseSuccess = response.body.success;

    expect(responseSuccess).toBe(false);
  });

  it('should only return transactions for authenticated user', async () => {
    const password = 'password123';
    const saltRounds = 10;
    const hashedPassword = await bcrypt.hash(password, saltRounds);

    const otherUserData = {
      firstName: 'Other',
      lastName: 'User',
      email: 'other@example.com',
      phone: '+1111111111',
      password: hashedPassword,
      status: USER_STATUS.ACTIVE,
      balance: 500,
    };
    const otherUser = await User.create(otherUserData);

    const otherToken = createToken(otherUser);

    const getRequest = request(app)
      .get('/api/v1/transactions')
      .set('Cookie', `token=${otherToken}`);

    const response = await getRequest.expect(200);

    const responseData = response.body.data;
    const userTransactions = responseData.transactions;
    const otherUserEmail = otherUser.email;

    const allBelongToUser = userTransactions.every((transaction) => {
      const isFromUser = transaction.fromEmail === otherUserEmail;
      const isToUser = transaction.toEmail === otherUserEmail;
      return isFromUser || isToUser;
    });

    expect(allBelongToUser).toBe(true);
  });
});

describe('GET /api/v1/transactions/:transactionId', () => {
  let user;
  let userToken;
  let transaction;

  beforeAll(async () => {
    await connectDB();
  });

  afterAll(async () => {
    await closeDB();
  });

  beforeEach(async () => {
    await clearDB();

    const password = 'password123';
    const saltRounds = 10;
    const hashedPassword = await bcrypt.hash(password, saltRounds);

    const userData = {
      firstName: 'Test',
      lastName: 'User',
      email: 'test@example.com',
      phone: '+1234567890',
      password: hashedPassword,
      status: USER_STATUS.ACTIVE,
      balance: 1000,
    };
    user = await User.create(userData);

    userToken = createToken(user);

    const transactionData = {
      id: 1,
      fromEmail: user.email,
      toEmail: 'other@example.com',
      amount: 100,
      description: 'Test transaction',
    };
    transaction = await Transaction.create(transactionData);
  });

  it('should return transaction by id', async () => {
    const transactionId = transaction.id;
    const requestPath = `/api/v1/transactions/${transactionId}`;

    const getRequest = request(app)
      .get(requestPath)
      .set('Cookie', `token=${userToken}`);

    const response = await getRequest.expect(200);

    const responseSuccess = response.body.success;
    const responseData = response.body.data;
    const responseTransaction = responseData.transaction;

    const expectedTransactionId = transaction.id;
    const expectedAmount = 100;

    expect(responseSuccess).toBe(true);
    expect(responseTransaction.id).toBe(expectedTransactionId);
    expect(responseTransaction.amount).toBe(expectedAmount);
  });

  it('should reject request without authentication', async () => {
    const transactionId = transaction.id;
    const requestPath = `/api/v1/transactions/${transactionId}`;

    const getRequest = request(app)
      .get(requestPath);

    const response = await getRequest.expect(401);

    const responseSuccess = response.body.success;

    expect(responseSuccess).toBe(false);
  });

  it('should return 404 for non-existent transaction', async () => {
    const nonExistentTransactionId = 99999;
    const requestPath = `/api/v1/transactions/${nonExistentTransactionId}`;

    const getRequest = request(app)
      .get(requestPath)
      .set('Cookie', `token=${userToken}`);

    const response = await getRequest.expect(404);

    const responseSuccess = response.body.success;

    expect(responseSuccess).toBe(false);
  });

  it('should return 404 for transaction belonging to another user', async () => {
    const password = 'password123';
    const saltRounds = 10;
    const hashedPassword = await bcrypt.hash(password, saltRounds);

    const otherUserData = {
      firstName: 'Other',
      lastName: 'User',
      email: 'other@example.com',
      phone: '+1111111111',
      password: hashedPassword,
      status: USER_STATUS.ACTIVE,
      balance: 500,
    };
    const otherUser = await User.create(otherUserData);

    const otherTransactionData = {
      id: 2,
      fromEmail: otherUser.email,
      toEmail: 'another@example.com',
      amount: 200,
    };
    const otherTransaction = await Transaction.create(otherTransactionData);

    const otherTransactionId = otherTransaction.id;
    const requestPath = `/api/v1/transactions/${otherTransactionId}`;

    const getRequest = request(app)
      .get(requestPath)
      .set('Cookie', `token=${userToken}`);

    const response = await getRequest.expect(404);

    const responseSuccess = response.body.success;

    expect(responseSuccess).toBe(false);
  });
});
