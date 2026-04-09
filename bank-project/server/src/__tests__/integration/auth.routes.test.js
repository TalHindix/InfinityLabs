import { describe, it, expect, beforeAll, afterAll, beforeEach } from '@jest/globals';
import request from 'supertest';
import app from '../../app.js';
import User from '../../models/user.model.js';
import { connectDB, closeDB, clearDB } from '../helpers/db.helper.js';

describe('POST /api/v1/auth/signup - Integration Test', () => {
  
  beforeAll(async () => {
    await connectDB();
  });

  afterAll(async () => {
    await closeDB();
  });

  beforeEach(async () => {
    await clearDB();
  });

  it('should create a new user with PENDING status', async () => {
    const firstName = 'John';
    const lastName = 'Doe';
    const email = 'john@example.com';
    const phone = '+1234567890';
    const password = 'Password123';

    const userData = {
      firstName: firstName,
      lastName: lastName,
      email: email,
      phone: phone,
      password: password,
    };

    const requestPath = '/api/v1/auth/signup';
    const postRequest = request(app)
      .post(requestPath)
      .send(userData);

    const response = await postRequest.expect(201);

    const responseSuccess = response.body.success;
    const responseData = response.body.data;
    const responseMessage = responseData.message;

    const expectedMessageContains = 'check your email';

    expect(responseSuccess).toBe(true);
    expect(responseMessage).toContain(expectedMessageContains);

    const userQuery = { email: email };
    const selectPassword = '+password';
    const user = await User.findOne(userQuery).select(selectPassword);

    const expectedFirstName = 'John';
    const expectedLastName = 'Doe';
    const expectedStatus = 'PENDING';
    const originalPassword = 'Password123';
    const minimumPasswordLength = 20;

    expect(user).toBeTruthy();
    expect(user.firstName).toBe(expectedFirstName);
    expect(user.lastName).toBe(expectedLastName);
    expect(user.status).toBe(expectedStatus);
    expect(user.verificationToken).toBeDefined();
    expect(user.password).not.toBe(originalPassword);
    expect(user.password.length).toBeGreaterThan(minimumPasswordLength);
  });

  it('should return same response for duplicate email (prevent enumeration)', async () => {
    const firstName = 'John';
    const lastName = 'Doe';
    const email = 'john@example.com';
    const phone = '+1234567890';
    const password = 'Password123';

    const userData = {
      firstName: firstName,
      lastName: lastName,
      email: email,
      phone: phone,
      password: password,
    };

    const requestPath = '/api/v1/auth/signup';
    const firstPostRequest = request(app)
      .post(requestPath)
      .send(userData);
    await firstPostRequest;

    const secondPostRequest = request(app)
      .post(requestPath)
      .send(userData);

    const response = await secondPostRequest.expect(201);

    const responseSuccess = response.body.success;
    const responseMessage = response.body.data.message;

    expect(responseSuccess).toBe(true);
    expect(responseMessage).toContain('check your email');
  });

  it('should reject signup with missing email', async () => {
    const firstName = 'John';
    const lastName = 'Doe';
    const phone = '+1234567890';
    const password = 'Password123';

    const userData = {
      firstName: firstName,
      lastName: lastName,
      phone: phone,
      password: password,
    };

    const requestPath = '/api/v1/auth/signup';
    const postRequest = request(app)
      .post(requestPath)
      .send(userData);

    const response = await postRequest.expect(400);

    const responseSuccess = response.body.success;

    expect(responseSuccess).toBe(false);
  });

  it('should reject invalid email format', async () => {
    const firstName = 'John';
    const lastName = 'Doe';
    const invalidEmail = 'not-an-email';
    const phone = '+1234567890';
    const password = 'Password123';

    const userData = {
      firstName: firstName,
      lastName: lastName,
      email: invalidEmail,
      phone: phone,
      password: password,
    };

    const requestPath = '/api/v1/auth/signup';
    const postRequest = request(app)
      .post(requestPath)
      .send(userData);

    const response = await postRequest.expect(400);

    const responseSuccess = response.body.success;

    expect(responseSuccess).toBe(false);
  });
});
