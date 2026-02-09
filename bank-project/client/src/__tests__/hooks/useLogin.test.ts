import { describe, it, expect, beforeEach, vi } from 'vitest';
import { renderHook, act, waitFor } from '@testing-library/react';
import { useLogin } from '../../screens/login-signup/useLogin';
import { authService } from '../../api/auth.service';
import { authStorage } from '../../api/auth.storage';

vi.mock('../../api/auth.service', () => ({
  authService: {
    login: vi.fn(),
    resendVerification: vi.fn(),
  },
}));

vi.mock('../../api/auth.storage', () => ({
  authStorage: {
    setUser: vi.fn(),
  },
}));

vi.mock('react-router-dom', () => ({
  useNavigate: () => vi.fn(),
  useSearchParams: () => [new URLSearchParams()],
}));

describe('useLogin', () => {
  beforeEach(() => {
    vi.clearAllMocks();
  });

  it('should initialize with empty values', () => {
    const { result } = renderHook(() => useLogin());

    expect(result.current.email).toBe('');
    expect(result.current.password).toBe('');
    expect(result.current.loading).toBe(false);
    expect(result.current.error).toBe('');
    expect(result.current.showVerifiedMsg).toBe(false);
    expect(result.current.showResendOption).toBe(false);
  });

  it('should update email when handleFieldChange is called', () => {
    const { result } = renderHook(() => useLogin());

    act(() => {
      result.current.handleFieldChange('email', 'test@example.com');
    });

    expect(result.current.email).toBe('test@example.com');
  });

  it('should update password when handleFieldChange is called', () => {
    const { result } = renderHook(() => useLogin());

    act(() => {
      result.current.handleFieldChange('password', 'password123');
    });

    expect(result.current.password).toBe('password123');
  });

  it('should reset resend option when email changes', () => {
    const { result } = renderHook(() => useLogin());

    act(() => {
      result.current.handleFieldChange('email', 'test@example.com');
    });

    expect(result.current.showResendOption).toBe(false);
  });

  it('should call authService.login on submit', async () => {
    const mockUser = { _id: '1', id: '1', email: 'test@example.com', firstName: 'Test', lastName: 'User', phone: '+1234567890', balance: 1000 };
    vi.mocked(authService.login).mockResolvedValue({ user: mockUser });

    const { result } = renderHook(() => useLogin());

    act(() => {
      result.current.handleFieldChange('email', 'test@example.com');
      result.current.handleFieldChange('password', 'password123');
    });

    await act(async () => {
      await result.current.handleSubmit({
        preventDefault: vi.fn(),
      } as any);
    });

    expect(authService.login).toHaveBeenCalledWith('test@example.com', 'password123');
  });

  it('should save user to storage on successful login', async () => {
    const mockUser = { _id: '1', id: '1', email: 'test@example.com', firstName: 'Test', lastName: 'User', phone: '+1234567890', balance: 1000 };
    vi.mocked(authService.login).mockResolvedValue({ user: mockUser });

    const { result } = renderHook(() => useLogin());

    act(() => {
      result.current.handleFieldChange('email', 'test@example.com');
      result.current.handleFieldChange('password', 'password123');
    });

    await act(async () => {
      await result.current.handleSubmit({
        preventDefault: vi.fn(),
      } as any);
    });

    await waitFor(() => {
      expect(authStorage.setUser).toHaveBeenCalledWith(mockUser);
    });
  });

  it('should show resend option on login error', async () => {
    vi.mocked(authService.login).mockRejectedValue(new Error('Invalid credentials'));

    const { result } = renderHook(() => useLogin());

    act(() => {
      result.current.handleFieldChange('email', 'test@example.com');
      result.current.handleFieldChange('password', 'wrongpassword');
    });

    await act(async () => {
      await result.current.handleSubmit({
        preventDefault: vi.fn(),
      } as any);
    });

    await waitFor(() => {
      expect(result.current.showResendOption).toBe(true);
      expect(result.current.password).toBe('');
    });
  });

  it('should call resendVerification when handleResendVerification is called', async () => {
    vi.mocked(authService.resendVerification).mockResolvedValue({ message: 'Email sent' });

    const { result } = renderHook(() => useLogin());

    act(() => {
      result.current.handleFieldChange('email', 'test@example.com');
    });

    await act(async () => {
      await result.current.handleResendVerification();
    });

    expect(authService.resendVerification).toHaveBeenCalledWith('test@example.com');
  });

  it('should show error if resendVerification called without email', async () => {
    const { result } = renderHook(() => useLogin());

    await act(async () => {
      await result.current.handleResendVerification();
    });

    expect(result.current.error).toContain('email');
    expect(authService.resendVerification).not.toHaveBeenCalled();
  });

  it('should show success message after resendVerification succeeds', async () => {
    vi.mocked(authService.resendVerification).mockResolvedValue({ message: 'Email sent' });

    const { result } = renderHook(() => useLogin());

    act(() => {
      result.current.handleFieldChange('email', 'test@example.com');
    });

    await act(async () => {
      await result.current.handleResendVerification();
    });

    await waitFor(() => {
      expect(result.current.resendSuccess).toBe(true);
    });
  });
});
