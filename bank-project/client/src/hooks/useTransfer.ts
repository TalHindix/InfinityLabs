import { useState } from 'react';
import { transactionsService } from '../services/transactions.service';
import { useAsyncOperation } from './useAsyncOperation';

export const useTransfer = () => {
  const [receiverEmail, setReceiverEmail] = useState('');
  const [amount, setAmount] = useState('');
  const [success, setSuccess] = useState(false);
  const { loading, error, execute } = useAsyncOperation();

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setSuccess(false);
    
    await execute(
      () => transactionsService.create({
        receiverEmail,
        amount: Number(amount),
      }),
      () => {
        setSuccess(true);
        setReceiverEmail('');
        setAmount('');
      }
    );
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
