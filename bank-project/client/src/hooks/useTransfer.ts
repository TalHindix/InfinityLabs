import { useState } from 'react';
import { transactionService } from '../services/transactions.service';
import { getErrorMessage } from '../types';

export const useTransfer = () => {
  const [receiverEmail, setReceiverEmail] = useState('');
  const [amount, setAmount] = useState('');
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');
  const [success, setSuccess] = useState(false);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setError('');
    setSuccess(false);
    setLoading(true);

    try {
      await transactionService.create({
        receiverEmail,
        amount: Number(amount),
      });
      setSuccess(true);
      setReceiverEmail('');
      setAmount('');
    } catch (err: unknown) {
      setError(getErrorMessage(err));
    } finally {
      setLoading(false);
    }
  };

  return {
    receiverEmail,
    amount,
    loading,
    error,
    success,
    setReceiverEmail,
    setAmount,
    handleSubmit
  };
};
