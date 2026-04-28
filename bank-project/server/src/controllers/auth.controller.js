import config from '../config/index.js';
import { USER_STATUS } from '../constants/index.js';
import { createToken } from '../utils/jwt.util.js';
import {
  findUserByEmailWithPassword,
  findUserByEmail,
  createUser,
  findAndVerifyUserByToken,
  regenerateVerificationToken,
  issueOtp,
  verifyOtp as verifyOtpService,
} from '../services/user.service.js';
import {
  sendVerificationEmailAsync,
  buildVerificationResultPage,
} from '../utils/email.util.js';
import * as response from '../utils/response.util.js';
import { AppError } from '../utils/error.util.js';
import logger from '../utils/logger.util.js';
import { disconnectUser } from '../socket/socket.handler.js';
import { getAuthenticatedUser, validatePassword } from '../utils/auth.util.js';
import { PASSWORD_REGEX } from '../utils/validation.util.js';

export const signup = async (req, res, next) => {
  try {
    const { firstName, lastName, email, phone, password } = req.body;
    if (!firstName || !lastName || !email || !phone || !password) {
      throw new AppError('All fields are required', 400);
    }

    if (!PASSWORD_REGEX.test(password)) {
      throw new AppError('Password must be at least 8 characters with uppercase, lowercase, and number', 400);
    }

    const { user, verificationToken } = await createUser({ firstName, lastName, email, phone, password });
    sendVerificationEmailAsync(user.email, verificationToken);
    return response.created(res, { message: 'Please check your email to verify your account.' });
  } catch (error) {
    // Anti-enumeration: a duplicate-email insert returns the same success
    // response as a fresh signup so an attacker can't use this endpoint to
    // discover which emails already have an account.
    if (error.code === 11000) {
      return response.created(res, { message: 'Please check your email to verify your account.' });
    }
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
    logger.error('verifyEmail failed', { error });
    return res.status(500).send(buildVerificationResultPage(false, 'Something went wrong. Please try again later.'));
  }
};

export const resendVerification = async (req, res, next) => {
  try {
    const { email } = req.body;
    if (!email) throw new AppError('Email is required', 400);

    const result = await regenerateVerificationToken(email);
    const successMessage = 'If this email exists and is not verified, a new verification link has been sent.';
    if (result) sendVerificationEmailAsync(email, result.verificationToken);

    return response.ok(res, { message: successMessage });
  } catch (error) {
    next(error);
  }
};

// Login is step 1 of 2: password check only issues a one-time code by email.
// The JWT cookie is issued by verifyOtp once the code is confirmed.
export const login = async (req, res, next) => {
  try {
    const { email, password } = req.body;
    if (!email || !password) throw new AppError('Email and password are required', 400);

    const user = await findUserByEmailWithPassword(email);
    if (!user || user.status !== USER_STATUS.ACTIVE) {
      throw new AppError('Invalid credentials', 401);
    }

    const isValidPassword = await validatePassword(password, user.password);
    if (!isValidPassword) throw new AppError('Invalid credentials', 401);

    await issueOtp(user);

    return response.ok(res, { otpRequired: true });
  } catch (error) {
    next(error);
  }
};

export const verifyOtp = async (req, res, next) => {
  try {
    const { email, otp } = req.body;
    if (!email || !otp) throw new AppError('Email and OTP are required', 400);

    const user = await findUserByEmail(email);
    if (!user || user.status !== USER_STATUS.ACTIVE) {
      throw new AppError('Invalid request', 400);
    }

    await verifyOtpService(user, otp);

    const token = createToken(user);
    res.cookie(config.cookie.tokenName, token, {
      httpOnly: true,
      secure: config.cookie.secure,
      sameSite: config.cookie.sameSite,
      maxAge: config.cookie.maxAgeSeconds * 1000,
      path: '/',
    });

    return response.ok(res, {
      user: {
        id: user.id,
        firstName: user.firstName,
        lastName: user.lastName,
        email: user.email,
      },
      token,
    });
  } catch (error) {
    next(error);
  }
};

export const resendOtp = async (req, res, next) => {
  try {
    const { email } = req.body;
    if (!email) throw new AppError('Email is required', 400);

    const user = await findUserByEmail(email);
    if (user && user.status === USER_STATUS.ACTIVE) {
      await issueOtp(user);
    }

    return response.ok(res, { message: 'If the account exists, a new OTP has been sent.' });
  } catch (error) {
    next(error);
  }
};

export const logout = async (req, res) => {
  const user = await getAuthenticatedUser(req);
  if (user?.id) disconnectUser(user.id);

  res.clearCookie(config.cookie.tokenName, {
    path: '/',
    httpOnly: true,
    secure: config.cookie.secure,
    sameSite: config.cookie.sameSite,
  });
  return response.ok(res, null);
};