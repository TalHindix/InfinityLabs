import mongoose from 'mongoose';

/** Bounds for transaction amount (must match validation.util.js). */
const MIN_AMOUNT = 0.01;
const MAX_AMOUNT = 1_000_000;

const counterSchema = new mongoose.Schema({
  _id: { type: String, required: true },
  seq: { type: Number, default: 0 },
});

const Counter = mongoose.model('Counter', counterSchema);

/**
 * Returns the next sequential transaction id. Optionally runs inside a MongoDB transaction session.
 * @param {mongoose.ClientSession | null} session - Optional session for transactional use
 * @returns {Promise<number>} Next transaction id
 */
export const getNextTransactionId = async (session = null) => {
  const options = { new: true, upsert: true };
  if (session) {
    options.session = session;
  }
  const counter = await Counter.findByIdAndUpdate(
    'transactions',
    { $inc: { seq: 1 } },
    options,
  );
  return counter.seq;
};

/** Transfer record: sender, receiver, amount, optional description. */
const transactionSchema = new mongoose.Schema(
  {
    id: {
      type: Number,
      required: true,
      unique: true,
    },
    fromEmail: {
      type: String,
      required: [true, 'Sender email is required'],
      lowercase: true,
      trim: true,
      match: [/^[^\s@]+@[^\s@]+\.[^\s@]+$/, 'Invalid sender email format'],
    },
    toEmail: {
      type: String,
      required: [true, 'Receiver email is required'],
      lowercase: true,
      trim: true,
      match: [/^[^\s@]+@[^\s@]+\.[^\s@]+$/, 'Invalid receiver email format'],
    },
    amount: {
      type: Number,
      required: [true, 'Amount is required'],
      min: [MIN_AMOUNT, 'Amount must be at least 0.01'],
      max: [MAX_AMOUNT, 'Amount cannot exceed 1,000,000'],
      validate: {
        validator: function (v) {
          return Number.isFinite(v) && v > 0;
        },
        message: 'Amount must be a positive number',
      },
    },
    description: {
      type: String,
      trim: true,
      maxlength: [500, 'Description cannot exceed 500 characters'],
    },
  },
  {
    timestamps: { createdAt: true, updatedAt: false },
  },
);

transactionSchema.index({ fromEmail: 1, createdAt: -1 });
transactionSchema.index({ toEmail: 1, createdAt: -1 });

const Transaction = mongoose.model('Transaction', transactionSchema);

export default Transaction;