// Signup form state and submit; manages stepper and success step.
import { useState } from 'react';
import { authService } from '../../api/auth.service';
import { useAsyncOperation } from '../../shared/useAsyncOperation';
import type { SignupFormData } from '../../types';

export const useSignup = () => {
  const [activeStep, setActiveStep] = useState(0);
  const { loading, error, execute } = useAsyncOperation();

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
    await execute(
      () => authService.signup(formData),
      () => setActiveStep(1)
    );
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
