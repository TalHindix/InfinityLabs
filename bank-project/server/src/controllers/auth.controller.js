import { USER_STATUS } from '../constants/index.js';
import { createToken } from '../utils/jwt.util.js';
import {
  findUserByEmailWithPassword,
  createUser,
  validatePassword,
  findAndVerifyUserByToken
} from '../services/user.service.js';
import { sendVerificationEmailAsync } from '../utils/email.util.js';
import * as response from '../utils/response.util.js';
import { AppError } from '../middleware/error.middleware.js';

export const signup = async (req, res, next) => {
  try {
    const { firstName, lastName, email, phone, password } = req.body;
    const { user, verificationToken } = await createUser({ firstName, lastName, email, phone, password });
    sendVerificationEmailAsync(user.email, verificationToken);
    return response.created(res, null);
  } catch (error) {
    next(error); // Pass to error handler
  }
};

export const verifyEmail = async (req, res, next) => {
  try {
    const { token } = req.query;
    if (!token) {
      throw new AppError('Verification token is required', 400);
    }
    const user = await findAndVerifyUserByToken(token);
    if (!user) {
      throw new AppError('Invalid or expired verification token', 400);
    }
    return res.redirect('http://localhost:5173/login?verified=true');
  } catch (error) {
    next(error);
  }
};

export const login = async (req, res, next) => {
  try {
    const { email, password } = req.body;

    if (!email || !password) {
      throw new AppError('Email and password are required', 400);
    }

    const user = await findUserByEmailWithPassword(email);

    if (!user || user.status !== USER_STATUS.ACTIVE) {
      throw new AppError('Invalid credentials', 401);
    }

    const isValidPassword = await validatePassword(password, user.password);
    if (!isValidPassword) {
      throw new AppError('Invalid credentials', 401);
    }

    return response.ok(res, {
      token: createToken(user),
      user: {
        id: user.id,
        firstName: user.firstName,
        lastName: user.lastName,
        email: user.email
      }
    });
  } catch (error) {
    console.log("REQ:", req.method, req.url);
    next(error);
  }
};

export const logout = (req, res) => response.ok(res, null);
