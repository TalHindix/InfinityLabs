import { useState } from 'react';
import { authService } from '../services/auth';
import { getErrorMessage } from '../types';

interface SignupFormData {
  firstName: string;
  lastName: string;
  email: string;
  password: string;
  phone: string;
}

export const useSignup = () => {
  const [activeStep, setActiveStep] = useState(0);
  const [error, setError] = useState('');
  const [loading, setLoading] = useState(false);

  const [formData, setFormData] = useState<SignupFormData>({
    firstName: '',
    lastName: '',
    email: '',
    password: '',
    phone: '',
  });

  const handleFieldChange = (field: keyof SignupFormData, value: string) => {
    setFormData(prev => ({ ...prev, [field]: value }));
  };

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    setError('');
    setLoading(true);

    try {
      await authService.signup(formData);
      setActiveStep(1);
    } catch (err: unknown) {
      setError(getErrorMessage(err));
    } finally {
      setLoading(false);
    }
  };

  const goBack = () => {
    setActiveStep(0);
  };

  return {
    formData,
    activeStep,
    loading,
    error,
    handleFieldChange,
    handleSubmit,
    goBack
  };
};
