import { describe, it, expect, beforeAll, afterAll, beforeEach } from '@jest/globals';
import request from 'supertest';
import app from '../../app.js';
import User from '../../models/user.model.js';
import { USER_STATUS } from '../../constants/index.js';
import { connectDB, closeDB, clearDB } from '../helpers/db.helper.js';
import bcrypt from 'bcrypt';

describe('POST /api/v1/auth/login - Integration Test', () => {
  
  beforeAll(async () => {
    await connectDB();
  });

  afterAll(async () => {
    await closeDB();
  });

  beforeEach(async () => {
    await clearDB();
  });

  it('should login and set cookie for valid user', async () => {
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
    };
    await User.create(userData);

    const loginEmail = 'john@example.com';
    const loginPassword = 'password123';
    const loginData = {
      email: loginEmail,
      password: loginPassword,
    };

    const requestPath = '/api/v1/auth/login';
    const postRequest = request(app)
      .post(requestPath)
      .send(loginData);

    const response = await postRequest.expect(200);

    const responseSuccess = response.body.success;
    const responseData = response.body.data;
    const responseUser = responseData.user;

    const expectedEmail = 'john@example.com';

    expect(responseSuccess).toBe(true);
    expect(responseUser).toBeDefined();
    expect(responseUser.email).toBe(expectedEmail);

    const responseHeaders = response.headers;
    const cookies = responseHeaders['set-cookie'];

    expect(cookies).toBeDefined();
    const hasTokenCookie = cookies.some((cookie) => cookie.includes('token='));
    expect(hasTokenCookie).toBe(true);
  });

  it('should reject login with wrong password', async () => {
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
    };
    await User.create(userData);

    const loginEmail = 'john@example.com';
    const wrongPassword = 'wrongpassword';
    const loginData = {
      email: loginEmail,
      password: wrongPassword,
    };

    const requestPath = '/api/v1/auth/login';
    const postRequest = request(app)
      .post(requestPath)
      .send(loginData);

    const response = await postRequest.expect(401);

    const responseSuccess = response.body.success;

    expect(responseSuccess).toBe(false);
  });

  it('should reject login for non-existent user', async () => {
    const nonExistentEmail = 'nonexistent@example.com';
    const loginPassword = 'password123';
    const loginData = {
      email: nonExistentEmail,
      password: loginPassword,
    };

    const requestPath = '/api/v1/auth/login';
    const postRequest = request(app)
      .post(requestPath)
      .send(loginData);

    const response = await postRequest.expect(401);

    const responseSuccess = response.body.success;

    expect(responseSuccess).toBe(false);
  });

  it('should reject login for unverified user', async () => {
    const password = 'password123';
    const saltRounds = 10;
    const hashedPassword = await bcrypt.hash(password, saltRounds);

    const userData = {
      firstName: 'John',
      lastName: 'Doe',
      email: 'john@example.com',
      phone: '+1234567890',
      password: hashedPassword,
      status: USER_STATUS.PENDING,
    };
    await User.create(userData);

    const loginEmail = 'john@example.com';
    const loginPassword = 'password123';
    const loginData = {
      email: loginEmail,
      password: loginPassword,
    };

    const requestPath = '/api/v1/auth/login';
    const postRequest = request(app)
      .post(requestPath)
      .send(loginData);

    const response = await postRequest.expect(401);

    const responseSuccess = response.body.success;

    expect(responseSuccess).toBe(false);
  });
});
