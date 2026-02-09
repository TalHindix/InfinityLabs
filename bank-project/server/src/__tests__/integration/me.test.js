import { describe, it, expect, beforeAll, afterAll, beforeEach } from '@jest/globals';
import request from 'supertest';
import app from '../../app.js';
import User from '../../models/user.model.js';
import Transaction from '../../models/transaction.model.js';
import { USER_STATUS } from '../../constants/index.js';
import { connectDB, closeDB, clearDB } from '../helpers/db.helper.js';
import { createToken } from '../../utils/jwt.util.js';
import bcrypt from 'bcrypt';

describe('GET /api/v1/me', () => {
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
      firstName: 'John',
      lastName: 'Doe',
      email: 'john@example.com',
      phone: '+1234567890',
      password: hashedPassword,
      status: USER_STATUS.ACTIVE,
      balance: 1000,
    };
    user = await User.create(userData);

    userToken = createToken(user);

    const userEmail = user.email;
    const firstTransactionData = {
      id: 1,
      fromEmail: userEmail,
      toEmail: 'other@example.com',
      amount: 100,
      description: 'First transaction',
    };

    const secondTransactionData = {
      id: 2,
      fromEmail: 'other@example.com',
      toEmail: userEmail,
      amount: 50,
      description: 'Second transaction',
    };

    const thirdTransactionData = {
      id: 3,
      fromEmail: userEmail,
      toEmail: 'another@example.com',
      amount: 200,
      description: 'Third transaction',
    };

    const fourthTransactionData = {
      id: 4,
      fromEmail: userEmail,
      toEmail: 'someone@example.com',
      amount: 75,
      description: 'Fourth transaction',
    };

    const fifthTransactionData = {
      id: 5,
      fromEmail: userEmail,
      toEmail: 'person@example.com',
      amount: 150,
      description: 'Fifth transaction',
    };

    const sixthTransactionData = {
      id: 6,
      fromEmail: userEmail,
      toEmail: 'user@example.com',
      amount: 300,
      description: 'Sixth transaction',
    };

    const seventhTransactionData = {
      id: 7,
      fromEmail: userEmail,
      toEmail: 'client@example.com',
      amount: 25,
      description: 'Seventh transaction',
    };

    const eighthTransactionData = {
      id: 8,
      fromEmail: userEmail,
      toEmail: 'customer@example.com',
      amount: 80,
      description: 'Eighth transaction',
    };

    const ninthTransactionData = {
      id: 9,
      fromEmail: userEmail,
      toEmail: 'member@example.com',
      amount: 120,
      description: 'Ninth transaction',
    };

    const tenthTransactionData = {
      id: 10,
      fromEmail: userEmail,
      toEmail: 'guest@example.com',
      amount: 90,
      description: 'Tenth transaction',
    };

    const eleventhTransactionData = {
      id: 11,
      fromEmail: userEmail,
      toEmail: 'visitor@example.com',
      amount: 60,
      description: 'Eleventh transaction',
    };

    await Transaction.create([
      firstTransactionData,
      secondTransactionData,
      thirdTransactionData,
      fourthTransactionData,
      fifthTransactionData,
      sixthTransactionData,
      seventhTransactionData,
      eighthTransactionData,
      ninthTransactionData,
      tenthTransactionData,
      eleventhTransactionData,
    ]);
  });

  it('should return current user information', async () => {
    const requestPath = '/api/v1/me';

    const getRequest = request(app)
      .get(requestPath)
      .set('Cookie', `token=${userToken}`);

    const response = await getRequest.expect(200);

    const responseSuccess = response.body.success;
    const responseData = response.body.data;
    const responseUser = responseData.user;

    const expectedFirstName = 'John';
    const expectedLastName = 'Doe';
    const expectedEmail = 'john@example.com';
    const expectedPhone = '+1234567890';
    const expectedBalance = 1000;
    const expectedStatus = 'ACTIVE';

    expect(responseSuccess).toBe(true);
    expect(responseUser).toBeDefined();
    expect(responseUser.id).toBe(user.id);
    expect(responseUser.firstName).toBe(expectedFirstName);
    expect(responseUser.lastName).toBe(expectedLastName);
    expect(responseUser.email).toBe(expectedEmail);
    expect(responseUser.phone).toBe(expectedPhone);
    expect(responseUser.balance).toBe(expectedBalance);
    expect(responseUser.status).toBe(expectedStatus);
  });

  it('should return recent transactions limited to 10', async () => {
    const requestPath = '/api/v1/me';

    const getRequest = request(app)
      .get(requestPath)
      .set('Cookie', `token=${userToken}`);

    const response = await getRequest.expect(200);

    const responseSuccess = response.body.success;
    const responseData = response.body.data;
    const recentTransactions = responseData.recentTransactions;

    const maximumAllowedTransactions = 10;

    expect(responseSuccess).toBe(true);
    expect(recentTransactions).toBeDefined();
    expect(recentTransactions.length).toBeLessThanOrEqual(maximumAllowedTransactions);
  });

  it('should reject request without authentication', async () => {
    const requestPath = '/api/v1/me';

    const getRequest = request(app)
      .get(requestPath);

    const response = await getRequest.expect(401);

    const responseSuccess = response.body.success;

    expect(responseSuccess).toBe(false);
  });

  it('should return transactions sorted by newest first', async () => {
    const requestPath = '/api/v1/me';

    const getRequest = request(app)
      .get(requestPath)
      .set('Cookie', `token=${userToken}`);

    const response = await getRequest.expect(200);

    const responseData = response.body.data;
    const transactions = responseData.recentTransactions;

    if (transactions.length > 1) {
      const firstTransaction = transactions[0];
      const secondTransaction = transactions[1];
      const firstTransactionCreatedAt = firstTransaction.createdAt;
      const secondTransactionCreatedAt = secondTransaction.createdAt;
      const firstDate = new Date(firstTransactionCreatedAt);
      const secondDate = new Date(secondTransactionCreatedAt);
      const firstTimestamp = firstDate.getTime();
      const secondTimestamp = secondDate.getTime();

      expect(firstTimestamp).toBeGreaterThanOrEqual(secondTimestamp);
    }
  });
});
