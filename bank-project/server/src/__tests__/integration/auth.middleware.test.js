import { describe, it, expect, beforeAll, afterAll, beforeEach } from '@jest/globals';
import request from 'supertest';
import app from '../../app.js';
import User from '../../models/user.model.js';
import { USER_STATUS } from '../../constants/index.js';
import { connectDB, closeDB, clearDB } from '../helpers/db.helper.js';
import { createToken } from '../../utils/jwt.util.js';
import bcrypt from 'bcrypt';

describe('protect middleware', () => {
  let user;
  let validToken;

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

    validToken = createToken(user);
  });

  it('should allow access with valid token in cookie', async () => {
    const requestPath = '/api/v1/me';
    const cookieValue = `token=${validToken}`;

    const getRequest = request(app)
      .get(requestPath)
      .set('Cookie', cookieValue);

    const response = await getRequest.expect(200);

    const responseSuccess = response.body.success;
    const responseData = response.body.data;
    const responseUser = responseData.user;
    const responseUserEmail = responseUser.email;

    const expectedEmail = 'test@example.com';

    expect(responseSuccess).toBe(true);
    expect(responseUserEmail).toBe(expectedEmail);
  });

  it('should allow access with valid token in Authorization header', async () => {
    const requestPath = '/api/v1/me';
    const authorizationValue = `Bearer ${validToken}`;

    const getRequest = request(app)
      .get(requestPath)
      .set('Authorization', authorizationValue);

    const response = await getRequest.expect(200);

    const responseSuccess = response.body.success;
    const responseData = response.body.data;
    const responseUser = responseData.user;
    const responseUserEmail = responseUser.email;

    const expectedEmail = 'test@example.com';

    expect(responseSuccess).toBe(true);
    expect(responseUserEmail).toBe(expectedEmail);
  });

  it('should reject request without token', async () => {
    const requestPath = '/api/v1/me';

    const getRequest = request(app)
      .get(requestPath);

    const response = await getRequest.expect(401);

    const responseSuccess = response.body.success;
    const responseError = response.body.error;

    const expectedError = 'Authentication failed';

    expect(responseSuccess).toBe(false);
    expect(responseError).toBe(expectedError);
  });

  it('should reject request with invalid token', async () => {
    const requestPath = '/api/v1/me';
    const invalidToken = 'invalid.token.here';
    const cookieValue = `token=${invalidToken}`;

    const getRequest = request(app)
      .get(requestPath)
      .set('Cookie', cookieValue);

    const response = await getRequest.expect(401);

    const responseSuccess = response.body.success;
    const responseError = response.body.error;

    const expectedError = 'Authentication failed';

    expect(responseSuccess).toBe(false);
    expect(responseError).toBe(expectedError);
  });

  it('should reject request with token for non-existent user', async () => {
    const fakeUserId = 'fake-id';
    const fakeUserEmail = 'fake@example.com';
    const fakeUser = { id: fakeUserId, email: fakeUserEmail };
    const fakeToken = createToken(fakeUser);

    const requestPath = '/api/v1/me';
    const cookieValue = `token=${fakeToken}`;

    const getRequest = request(app)
      .get(requestPath)
      .set('Cookie', cookieValue);

    const response = await getRequest.expect(401);

    const responseSuccess = response.body.success;
    const responseError = response.body.error;

    const expectedError = 'Authentication failed';

    expect(responseSuccess).toBe(false);
    expect(responseError).toBe(expectedError);
  });

  it('should reject request with token for inactive user', async () => {
    const password = 'password123';
    const saltRounds = 10;
    const hashedPassword = await bcrypt.hash(password, saltRounds);

    const inactiveUserData = {
      firstName: 'Inactive',
      lastName: 'User',
      email: 'inactive@example.com',
      phone: '+1111111111',
      password: hashedPassword,
      status: USER_STATUS.PENDING,
      balance: 0,
    };
    const inactiveUser = await User.create(inactiveUserData);

    const inactiveToken = createToken(inactiveUser);

    const requestPath = '/api/v1/me';
    const cookieValue = `token=${inactiveToken}`;

    const getRequest = request(app)
      .get(requestPath)
      .set('Cookie', cookieValue);

    const response = await getRequest.expect(401);

    const responseSuccess = response.body.success;
    const responseError = response.body.error;

    const expectedError = 'Authentication failed';

    expect(responseSuccess).toBe(false);
    expect(responseError).toBe(expectedError);
  });

  it('should prefer cookie token over Authorization header', async () => {
    const password = 'password123';
    const saltRounds = 10;
    const hashedPassword = await bcrypt.hash(password, saltRounds);

    const otherUserData = {
      firstName: 'Other',
      lastName: 'User',
      email: 'other@example.com',
      phone: '+2222222222',
      password: hashedPassword,
      status: USER_STATUS.ACTIVE,
      balance: 500,
    };
    const otherUser = await User.create(otherUserData);

    const otherToken = createToken(otherUser);

    const requestPath = '/api/v1/me';
    const cookieValue = `token=${validToken}`;
    const authorizationValue = `Bearer ${otherToken}`;

    const getRequest = request(app)
      .get(requestPath)
      .set('Cookie', cookieValue)
      .set('Authorization', authorizationValue);

    const response = await getRequest.expect(200);

    const responseSuccess = response.body.success;
    const responseData = response.body.data;
    const responseUser = responseData.user;
    const responseUserEmail = responseUser.email;

    const expectedEmail = 'test@example.com';

    expect(responseSuccess).toBe(true);
    expect(responseUserEmail).toBe(expectedEmail);
  });
});
