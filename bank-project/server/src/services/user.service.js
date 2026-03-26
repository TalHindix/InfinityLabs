import bcrypt from 'bcrypt';
import User from '../models/user.model.js';
import { SALT_ROUNDS, USER_STATUS } from '../constants/index.js';
import { generateVerificationToken, hashToken } from '../utils/generate.util.js';
import { AppError } from '../utils/error.util.js';

export const findAndVerifyUserByToken = async (token) => {
  const user = await User.findOneAndUpdate(
    { verificationToken: hashToken(token), status: USER_STATUS.PENDING },
    { status: USER_STATUS.ACTIVE, verificationToken: undefined },
    { new: true }
  );
  return user;
};

export const findUserByEmailWithPassword = async (email) => {
  return User.findOne({ email: email.toLowerCase() }).select('+password');
};

export const findUserById = async (id) => {
  return User.findOne({ id });
};

export const findActiveUserById = async (id) => {
  const user = await User.findOne({ id }).select('id email status');
  return user?.status === USER_STATUS.ACTIVE ? user : null;
};

export const getAccountSummary = async (userId) => {
  const user = await User.findOne({ id: userId }).select('id balance');
  if (!user) throw new AppError('User not found', 404);
  return { userId: user.id, balance: user.balance };
};

export const createUser = async (userData) => {
  const { firstName, lastName, email, phone, password } = userData;
  const hashedPassword = await bcrypt.hash(password, SALT_ROUNDS);
  const verificationToken = generateVerificationToken();

  const user = await User.create({
    firstName,
    lastName,
    email: email.toLowerCase(),
    phone,
    password: hashedPassword,
    status: USER_STATUS.PENDING,
    verificationToken: hashToken(verificationToken),
  });
  return { user, verificationToken };
};

export const regenerateVerificationToken = async (email) => {
  const user = await User.findOne({ email: email.toLowerCase(), status: USER_STATUS.PENDING });
  if (!user) return null;
  const verificationToken = generateVerificationToken();
  user.verificationToken = hashToken(verificationToken);
  await user.save();
  return { verificationToken };
};

export const validatePassword = async (inputPassword, hashedPassword) => {
  return bcrypt.compare(inputPassword, hashedPassword);
};

export const findUserByEmail = async (email) => {
  return User.findOne({ email: email.toLowerCase() });
};

const OTP_EXPIRY_MS = 5 * 60 * 1000;
const MAX_OTP_ATTEMPTS = 5;

export const saveOtp = async (userId, otp) => {
  const otpHash = await bcrypt.hash(otp, SALT_ROUNDS);
  await User.findOneAndUpdate(
    { id: userId },
    { otpHash, otpExpiry: new Date(Date.now() + OTP_EXPIRY_MS), otpAttempts: 0 }
  );
};

export const verifyOtp = async (user, otp) => {
  if (user.otpAttempts >= MAX_OTP_ATTEMPTS) {
    throw new AppError('Too many failed attempts. Please request a new OTP.', 429);
  }

  if (!user.otpHash || !user.otpExpiry || user.otpExpiry < new Date()) {
    throw new AppError('OTP has expired. Please request a new one.', 400);
  }

  const isValid = await bcrypt.compare(otp, user.otpHash);

  if (!isValid) {
    await User.findOneAndUpdate({ id: user.id }, { $inc: { otpAttempts: 1 } });
    throw new AppError('Invalid OTP.', 400);
  }

  await User.findOneAndUpdate(
    { id: user.id },
    { $unset: { otpHash: '', otpExpiry: '' }, $set: { otpAttempts: 0 } }
  );
};