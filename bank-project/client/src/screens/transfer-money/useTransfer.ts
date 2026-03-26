import { useState } from 'react';
import { transactionService } from '../../api/transaction.service';
import { useAsyncOperation } from '../../shared/useAsyncOperation';
import type { Transaction } from '../../types';

export const useTransfer = () => {
  const [receiverEmail, setReceiverEmail] = useState('');
  const [amount, setAmount] = useState('');
  const [description, setDescription] = useState('');
  const [success, setSuccess] = useState(false);
  const [transaction, setTransaction] = useState<Transaction | null>(null);
  const [videoRoomName, setVideoRoomName] = useState('');
  const { loading, error, execute } = useAsyncOperation();

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setSuccess(false);
    setTransaction(null);
    if (!description.trim()) return;

    await execute(
      async () => {
        const response = await transactionService.create({
          receiverEmail,
          amount: Number(amount),
          description: description.trim(),
        });
        return response.transaction;
      },
      (tx) => {
        setSuccess(true);
        setTransaction(tx);
        setReceiverEmail('');
        setAmount('');
        setDescription('');
      }
    );
  };

  const handleVideoCall = async (transactionId: string) => {
    const res = await transactionService.sendNotification(transactionId);
    setVideoRoomName(res.roomName);
  };

  const handleCloseDialog = () => {
    setSuccess(false);
    setTransaction(null);
    setVideoRoomName('');
  };

  return {
    receiverEmail,
    amount,
    description,
    loading,
    error,
    success,
    transaction,
    videoRoomName,
    setReceiverEmail,
    setAmount,
    setDescription,
    handleSubmit,
    handleVideoCall,
    handleCloseDialog,
  };
};
