// Transfer form state and submit; handles success and reset.
import { useState } from 'react';
import { transactionService } from '../../api/transaction.service';
import { useAsyncOperation } from '../../shared/useAsyncOperation';

export const useTransfer = () => {
  const [receiverEmail, setReceiverEmail] = useState('');
  const [amount, setAmount] = useState('');
  const [success, setSuccess] = useState(false);
  const { loading, error, execute } = useAsyncOperation();

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setSuccess(false);

    await execute(
      () => transactionService.create({
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
