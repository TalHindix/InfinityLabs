import bcrypt from 'bcrypt';
import User from '../models/user.model.js';
import { SALT_ROUNDS, USER_STATUS } from '../constants/index.js';
import { generateVerificationToken, hashToken } from '../utils/generate.util.js';

export const findAndVerifyUserByToken = async (token) => {
  const hashedToken = hashToken(token);

  const user = await User.findOne({
    verificationToken: hashedToken,
    status: USER_STATUS.PENDING
  });

  if (!user) return null;

  user.status = USER_STATUS.ACTIVE;
  user.verificationToken = undefined;

  await user.save();
  return user;
};

export const findUserByEmailWithPassword = async (email) => {
  return User.findOne({ email: email.toLowerCase() }).select('+password');
};

export const findUserById = async (id) => {
  return User.findOne({ id });
};

/**
 * Get account summary for ChatBot
 * @param {string} userId - User ID
 * @returns {Promise<{userId: string, balance: number}>}
 */
export const getAccountSummary = async (userId) => {
  const user = await User.findOne({ id: userId }).select('id balance');

  if (!user) {
    throw new Error('User not found');
  }

  return {
    userId: user.id,
    balance: user.balance
  };
};

/**
 * Create a new user
 * @param {Object} userData - User data
 * @returns {Promise<{user: Object, verificationToken: string}>}
 */
export const createUser = async (userData) => {
  const { firstName, lastName, email, phone, password } = userData;

  const hashedPassword = await bcrypt.hash(password, SALT_ROUNDS);
  const verificationToken = generateVerificationToken();
  const hashedVerificationToken = hashToken(verificationToken);

  const user = await User.create({
    firstName,
    lastName,
    email: email.toLowerCase(),
    phone,
    password: hashedPassword,
    status: USER_STATUS.PENDING,
    verificationToken: hashedVerificationToken
  });

  return { user, verificationToken };
};

/**
 * Regenerate verification token for resend functionality
 * @param {string} email - User email
 * @returns {Promise<{verificationToken: string}|null>}
 */
export const regenerateVerificationToken = async (email) => {
  const user = await User.findOne({
    email: email.toLowerCase(),
    status: USER_STATUS.PENDING
  });

  if (!user) {
    return null;
  }

  const verificationToken = generateVerificationToken();
  const hashedVerificationToken = hashToken(verificationToken);

  user.verificationToken = hashedVerificationToken;
  await user.save();

  return { verificationToken };
};

export const validatePassword = async (inputPassword, hashedPassword) => {
  return bcrypt.compare(inputPassword, hashedPassword);
};