// Transfer form state and submit; handles success and reset.
import { useState } from 'react';
import { transactionService } from '../../api/transaction.service';
import { useAsyncOperation } from '../../shared/useAsyncOperation';

export const useTransfer = () => {
  const [receiverEmail, setReceiverEmail] = useState('');
  const [amount, setAmount] = useState('');
  const [description, setDescription] = useState('');
  const [success, setSuccess] = useState(false);
  const { loading, error, execute } = useAsyncOperation();

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setSuccess(false);
    if (!description.trim()) return;

    await execute(
      () => transactionService.create({
        receiverEmail,
        amount: Number(amount),
        description: description.trim(),
      }),
      () => {
        setSuccess(true);
        setReceiverEmail('');
        setAmount('');
        setDescription('');
      }
    );
  };

  return {
    receiverEmail,
    amount,
    description,
    loading,
    error,
    success,
    setReceiverEmail,
    setAmount,
    setDescription,
    handleSubmit
  };
};
