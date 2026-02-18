import mongoose from 'mongoose';
import crypto from 'crypto';
import { USER_STATUS, DEFAULT_BALANCE } from '../constants/index.js';

const userSchema = new mongoose.Schema(
  {
    id: {
      type: String,
      required: true,
      unique: true,
      default: () => crypto.randomUUID(),
    },
    firstName: {
      type: String,
      required: [true, 'First name is required'],
      trim: true,
      minlength: [2, 'First name must be at least 2 characters'],
      maxlength: [50, 'First name cannot exceed 50 characters'],
    },
    lastName: {
      type: String,
      required: [true, 'Last name is required'],
      trim: true,
      minlength: [2, 'Last name must be at least 2 characters'],
      maxlength: [50, 'Last name cannot exceed 50 characters'],
    },
    email: {
      type: String,
      required: [true, 'Email is required'],
      unique: true,
      lowercase: true,
      trim: true,
      match: [/^[^\s@]+@[^\s@]+\.[^\s@]+$/, 'Please provide a valid email address'],
    },
    password: {
      type: String,
      required: [true, 'Password is required'],
      minlength: [8, 'Password must be at least 8 characters'],
      select: false,
    },
    phone: {
      type: String,
      required: [true, 'Phone number is required'],
      trim: true,
      match: [/^\+?[1-9]\d{7,14}$/, 'Please provide a valid phone number'],
    },
    status: {
      type: String,
      enum: {
        values: Object.values(USER_STATUS),
        message: 'Invalid user status',
      },
      default: USER_STATUS.PENDING,
    },
    verificationToken: { type: String },
    balance: {
      type: Number,
      default: DEFAULT_BALANCE,
      min: [0, 'Balance cannot be negative'],
    },
  },
  {
    timestamps: { createdAt: true, updatedAt: false },
  },
);

const User = mongoose.model('User', userSchema);

export default User;