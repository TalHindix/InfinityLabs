import config from '../config/index.js';
import { USER_STATUS } from '../constants/index.js';
import { createToken } from '../utils/jwt.util.js';
import {
  findUserByEmailWithPassword,
  createUser,
  validatePassword,
  findAndVerifyUserByToken,
  regenerateVerificationToken,
} from '../services/user.service.js';
import {
  sendVerificationEmailAsync,
  buildVerificationResultPage,
} from '../utils/email.util.js';
import * as response from '../utils/response.util.js';
import { mapErrorToResponse } from '../utils/error.util.js';
import { AppError } from '../utils/error.util.js';
import { disconnectUser } from '../socket/socket.handler.js';

export const signup = async (req, res) => {
  try {
    const { firstName, lastName, email, phone, password } = req.body;
    if (!firstName || !lastName || !email || !phone || !password) {
      throw new AppError('All fields are required', 400);
    }
    
    const { user, verificationToken } = await createUser({ firstName, lastName, email, phone, password });
    sendVerificationEmailAsync(user.email, verificationToken);
    return response.created(res, { message: 'Please check your email to verify your account.' });
  } catch (error) {
    const { statusCode, message } = mapErrorToResponse(error);

    if (process.env.NODE_ENV === 'development') {
      console.error(`[${req.method}] ${req.originalUrl}:`, error);
    }

    return res.status(statusCode).json({
      success: false,
      error: message,
    });
  }
};

/** Verifies email via query token; returns HTML success or failure page. */
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
  } catch {
    return res.status(500).send(buildVerificationResultPage(false, 'Something went wrong. Please try again later.'));
  }
};

/** Resends verification email if user exists and is PENDING; always returns same message (no user enumeration). */
export const resendVerification = async (req, res) => {
  try {
    const { email } = req.body;
    if (!email) throw new AppError('Email is required', 400);

    const result = await regenerateVerificationToken(email);
    const successMessage = 'If this email exists and is not verified, a new verification link has been sent.';
    if (result) sendVerificationEmailAsync(email, result.verificationToken);

    return response.ok(res, { message: successMessage });
  } catch (error) {
    const { statusCode, message } = mapErrorToResponse(error);

    if (process.env.NODE_ENV === 'development') {
      console.error(`[${req.method}] ${req.originalUrl}:`, error);
    }

    return res.status(statusCode).json({
      success: false,
      error: message,
    });
  }
};

/** Logs in with email/password, sets HTTP-only cookie with JWT, returns user summary. Fails with same message for wrong email, wrong password, or unverified account. */
export const login = async (req, res) => {
  try {
    const { email, password } = req.body;
    if (!email || !password) throw new AppError('Email and password are required', 400);

    const user = await findUserByEmailWithPassword(email);
    if (!user || user.status !== USER_STATUS.ACTIVE) {
      throw new AppError('Invalid credentials', 401);
    }

    const isValidPassword = await validatePassword(password, user.password);
    if (!isValidPassword) throw new AppError('Invalid credentials', 401);

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
    });
  } catch (error) {
    const { statusCode, message } = mapErrorToResponse(error);

    if (process.env.NODE_ENV === 'development') {
      console.error(`[${req.method}] ${req.originalUrl}:`, error);
    }

    return res.status(statusCode).json({
      success: false,
      error: message,
    });
  }
};

/** Clears the auth cookie, disconnects active sockets, and returns 200. */
export const logout = (req, res) => {
  // Disconnect sockets if user is authenticated (optionalProtect sets req.user if token exists)
  if (req.user?.id) {
    disconnectUser(req.user.id);
  }

  res.clearCookie(config.cookie.tokenName, {
    path: '/',
    httpOnly: true,
    secure: config.cookie.secure,
    sameSite: config.cookie.sameSite,
  });
  return response.ok(res, null);
};