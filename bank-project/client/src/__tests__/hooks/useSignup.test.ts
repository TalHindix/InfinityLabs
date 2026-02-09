/**
 * CRITICAL TEST: Hook Testing - Form State and Stepper Logic
 * 
 * WHAT ARE WE TESTING?
 * The useSignup hook that manages:
 * - Form state (firstName, lastName, email, password, phone)
 * - Stepper navigation (activeStep: 0 = form, 1 = success)
 * - Form submission with authService.signup
 * - goBack function to return to form
 * 
 * WHY IS THIS CRITICAL FOR INTERVIEWS?
 * Interviewers ask: "How do you test form state management? How do you test stepper logic?"
 * This test demonstrates:
 * - Form state updates
 * - Stepper/step navigation
 * - Integration with useAsyncOperation
 * - Error handling in forms
 */

import { describe, it, expect, beforeEach, vi } from 'vitest';
import { renderHook, act, waitFor } from '@testing-library/react';
import { useSignup } from '../../screens/login-signup/useSignup';
import { authService } from '../../api/auth.service';

vi.mock('../../api/auth.service', () => ({
  authService: {
    signup: vi.fn(),
  },
}));

describe('useSignup - Critical Hook Tests', () => {
  beforeEach(() => {
    vi.clearAllMocks();
  });

  describe('Initial state', () => {
    it('should initialize with empty form data', () => {
      const { result } = renderHook(() => useSignup());

      expect(result.current.formData.firstName).toBe('');
      expect(result.current.formData.lastName).toBe('');
      expect(result.current.formData.email).toBe('');
      expect(result.current.formData.password).toBe('');
      expect(result.current.formData.phone).toBe('');
    });

    it('should initialize with step 0', () => {
      const { result } = renderHook(() => useSignup());

      expect(result.current.activeStep).toBe(0);
    });

    it('should initialize with loading false and no error', () => {
      const { result } = renderHook(() => useSignup());

      expect(result.current.loading).toBe(false);
      expect(result.current.error).toBe('');
    });
  });

  describe('Form field updates', () => {
    it('should update firstName when handleFieldChange is called', () => {
      const { result } = renderHook(() => useSignup());

      act(() => {
        result.current.handleFieldChange('firstName', 'John');
      });

      expect(result.current.formData.firstName).toBe('John');
    });

    it('should update lastName when handleFieldChange is called', () => {
      const { result } = renderHook(() => useSignup());

      act(() => {
        result.current.handleFieldChange('lastName', 'Doe');
      });

      expect(result.current.formData.lastName).toBe('Doe');
    });

    it('should update email when handleFieldChange is called', () => {
      const { result } = renderHook(() => useSignup());

      act(() => {
        result.current.handleFieldChange('email', 'john@example.com');
      });

      expect(result.current.formData.email).toBe('john@example.com');
    });

    it('should update password when handleFieldChange is called', () => {
      const { result } = renderHook(() => useSignup());

      act(() => {
        result.current.handleFieldChange('password', 'password123');
      });

      expect(result.current.formData.password).toBe('password123');
    });

    it('should update phone when handleFieldChange is called', () => {
      const { result } = renderHook(() => useSignup());

      act(() => {
        result.current.handleFieldChange('phone', '+972526006496');
      });

      expect(result.current.formData.phone).toBe('+972526006496');
    });

    it('should update multiple fields independently', () => {
      const { result } = renderHook(() => useSignup());

      act(() => {
        result.current.handleFieldChange('firstName', 'John');
        result.current.handleFieldChange('lastName', 'Doe');
        result.current.handleFieldChange('email', 'john@example.com');
      });

      expect(result.current.formData.firstName).toBe('John');
      expect(result.current.formData.lastName).toBe('Doe');
      expect(result.current.formData.email).toBe('john@example.com');
    });
  });

  describe('Form submission', () => {
    it('should call authService.signup with form data on submit', async () => {
      vi.mocked(authService.signup).mockResolvedValue({ message: 'User created' });

      const { result } = renderHook(() => useSignup());

      act(() => {
        result.current.handleFieldChange('firstName', 'John');
        result.current.handleFieldChange('lastName', 'Doe');
        result.current.handleFieldChange('email', 'john@example.com');
        result.current.handleFieldChange('password', 'password123');
        result.current.handleFieldChange('phone', '+972526006496');
      });

      await act(async () => {
        await result.current.handleSubmit({
          preventDefault: vi.fn(),
        } as any);
      });

      expect(authService.signup).toHaveBeenCalledWith({
        firstName: 'John',
        lastName: 'Doe',
        email: 'john@example.com',
        password: 'password123',
        phone: '+972526006496',
      });
    });

    it('should advance to step 1 on successful signup', async () => {
      vi.mocked(authService.signup).mockResolvedValue({ message: 'User created' });

      const { result } = renderHook(() => useSignup());

      act(() => {
        result.current.handleFieldChange('email', 'john@example.com');
        result.current.handleFieldChange('password', 'password123');
        result.current.handleFieldChange('firstName', 'John');
        result.current.handleFieldChange('lastName', 'Doe');
        result.current.handleFieldChange('phone', '+972526006496');
      });

      await act(async () => {
        await result.current.handleSubmit({
          preventDefault: vi.fn(),
        } as any);
      });

      await waitFor(() => {
        expect(result.current.activeStep).toBe(1);
      });
    });

    it('should set loading to true during submission', async () => {
      vi.mocked(authService.signup).mockImplementation(
        () => new Promise((resolve) => setTimeout(() => resolve({ message: 'Success' }), 100))
      );

      const { result } = renderHook(() => useSignup());

      act(() => {
        result.current.handleFieldChange('email', 'john@example.com');
        result.current.handleFieldChange('password', 'password123');
        result.current.handleFieldChange('firstName', 'John');
        result.current.handleFieldChange('lastName', 'Doe');
        result.current.handleFieldChange('phone', '+972526006496');
      });

      act(() => {
        result.current.handleSubmit({
          preventDefault: vi.fn(),
        } as any);
      });

      expect(result.current.loading).toBe(true);

      await waitFor(() => {
        expect(result.current.loading).toBe(false);
      });
    });

    it('should handle signup errors', async () => {
      const errorMessage = 'Email already exists';
      vi.mocked(authService.signup).mockRejectedValue(new Error(errorMessage));

      const { result } = renderHook(() => useSignup());

      act(() => {
        result.current.handleFieldChange('email', 'existing@example.com');
        result.current.handleFieldChange('password', 'password123');
        result.current.handleFieldChange('firstName', 'John');
        result.current.handleFieldChange('lastName', 'Doe');
        result.current.handleFieldChange('phone', '+972526006496');
      });

      await act(async () => {
        await result.current.handleSubmit({
          preventDefault: vi.fn(),
        } as any);
      });

      await waitFor(() => {
        expect(result.current.error).toBeTruthy();
        expect(result.current.activeStep).toBe(0); // Should stay on form step
      });
    });
  });

  describe('Stepper navigation', () => {
    it('should start at step 0', () => {
      const { result } = renderHook(() => useSignup());

      expect(result.current.activeStep).toBe(0);
    });

    it('should advance to step 1 after successful signup', async () => {
      vi.mocked(authService.signup).mockResolvedValue({ message: 'User created' });

      const { result } = renderHook(() => useSignup());

      act(() => {
        result.current.handleFieldChange('email', 'john@example.com');
        result.current.handleFieldChange('password', 'password123');
        result.current.handleFieldChange('firstName', 'John');
        result.current.handleFieldChange('lastName', 'Doe');
        result.current.handleFieldChange('phone', '+972526006496');
      });

      await act(async () => {
        await result.current.handleSubmit({
          preventDefault: vi.fn(),
        } as any);
      });

      await waitFor(() => {
        expect(result.current.activeStep).toBe(1);
      });
    });

    it('should go back to step 0 when goBack is called', async () => {
      vi.mocked(authService.signup).mockResolvedValue({ message: 'User created' });

      const { result } = renderHook(() => useSignup());

      // Submit form to advance to step 1
      act(() => {
        result.current.handleFieldChange('email', 'john@example.com');
        result.current.handleFieldChange('password', 'password123');
        result.current.handleFieldChange('firstName', 'John');
        result.current.handleFieldChange('lastName', 'Doe');
        result.current.handleFieldChange('phone', '+972526006496');
      });

      await act(async () => {
        await result.current.handleSubmit({
          preventDefault: vi.fn(),
        } as any);
      });

      await waitFor(() => {
        expect(result.current.activeStep).toBe(1);
      });

      // Go back to step 0
      act(() => {
        result.current.goBack();
      });

      expect(result.current.activeStep).toBe(0);
    });
  });
});
