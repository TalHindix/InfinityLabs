import { USER_STATUS } from '../constants/index.js';
import { createToken } from '../utils/jwt.util.js';
import {
  findUserByEmailWithPassword,
  createUser,
  validatePassword,
  findAndVerifyUserByToken,
  regenerateVerificationToken
} from '../services/user.service.js';
import {
  sendVerificationEmailAsync,
  buildVerificationResultPage
} from '../utils/email.util.js';
import * as response from '../utils/response.util.js';
import { AppError } from '../middleware/error.middleware.js';

export const signup = async (req, res, next) => {
  try {
    const { firstName, lastName, email, phone, password } = req.body;
    const { user, verificationToken } = await createUser({ firstName, lastName, email, phone, password });
    sendVerificationEmailAsync(user.email, verificationToken);
    return response.created(res, { message: 'Please check your email to verify your account.' });
  } catch (error) {
    next(error);
  }
};

export const verifyEmail = async (req, res) => {
  try {
    const { token } = req.query;

    if (!token) {
      return res.status(400).send(buildVerificationResultPage(false, 'Verification token is missing.'));
    }

    const user = await findAndVerifyUserByToken(token);

    if (!user) {
      return res.status(400).send(buildVerificationResultPage(false, 'Invalid or expired verification token.'));
    }

    return res.send(buildVerificationResultPage(true));
  } catch (error) {
    return res.status(500).send(buildVerificationResultPage(false, 'Something went wrong. Please try again later.'));
  }
};

export const resendVerification = async (req, res, next) => {
  try {
    const { email } = req.body;

    if (!email) {
      throw new AppError('Email is required', 400);
    }

    const result = await regenerateVerificationToken(email);

    // Security: Always return the same message (don't reveal if email exists)
    const successMessage = 'If this email exists and is not verified, a new verification link has been sent.';

    if (result) {
      sendVerificationEmailAsync(email, result.verificationToken);
    }

    return response.ok(res, { message: successMessage });
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

    // Security: Same error message for all failure cases
    // - User not found
    // - Wrong password  
    // - Account not verified (PENDING status)
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
    next(error);
  }
};

export const logout = (req, res) => response.ok(res, null);