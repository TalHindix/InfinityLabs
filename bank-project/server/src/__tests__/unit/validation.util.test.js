import { describe, it, expect } from '@jest/globals';
import { validateTransactionAmount } from '../../utils/validation.util.js';

describe('validateTransactionAmount - Unit Test', () => {
  
  it('should accept a valid amount (100.50)', () => {
    const validAmount = 100.50;
    
    const result = validateTransactionAmount(validAmount);
    
    const isValid = result.isValid;
    const error = result.error;
    const sanitized = result.sanitized;
    
    const expectedIsValid = true;
    const expectedError = null;
    const expectedSanitized = 100.50;
    
    expect(isValid).toBe(expectedIsValid);
    expect(error).toBe(expectedError);
    expect(sanitized).toBe(expectedSanitized);
  });

  it('should reject null amount', () => {
    const nullAmount = null;
    
    const result = validateTransactionAmount(nullAmount);
    
    const isValid = result.isValid;
    const error = result.error;
    
    const expectedIsValid = false;
    const expectedError = 'Amount is required';
    
    expect(isValid).toBe(expectedIsValid);
    expect(error).toBe(expectedError);
  });

  it('should reject undefined amount', () => {
    const undefinedAmount = undefined;
    
    const result = validateTransactionAmount(undefinedAmount);
    
    const isValid = result.isValid;
    const error = result.error;
    
    const expectedIsValid = false;
    const expectedError = 'Amount is required';
    
    expect(isValid).toBe(expectedIsValid);
    expect(error).toBe(expectedError);
  });

  it('should reject amount below minimum (0.01)', () => {
    const amountBelowMinimum = 0.005;
    
    const result = validateTransactionAmount(amountBelowMinimum);
    
    const isValid = result.isValid;
    const error = result.error;
    
    const expectedIsValid = false;
    const expectedError = 'Amount must be at least 0.01';
    
    expect(isValid).toBe(expectedIsValid);
    expect(error).toBe(expectedError);
  });

  it('should reject amount above maximum (1,000,000)', () => {
    const amountAboveMaximum = 2000000;
    
    const result = validateTransactionAmount(amountAboveMaximum);
    
    const isValid = result.isValid;
    const error = result.error;
    
    const expectedIsValid = false;
    const expectedErrorContains = 'cannot exceed';
    
    expect(isValid).toBe(expectedIsValid);
    expect(error).toContain(expectedErrorContains);
  });

  it('should reject more than 2 decimal places', () => {
    const amountWithTooManyDecimals = 100.999;
    
    const result = validateTransactionAmount(amountWithTooManyDecimals);
    
    const isValid = result.isValid;
    const error = result.error;
    
    const expectedIsValid = false;
    const expectedError = 'Amount cannot have more than 2 decimal places';
    
    expect(isValid).toBe(expectedIsValid);
    expect(error).toBe(expectedError);
  });

  it('should accept exactly minimum amount (0.01)', () => {
    const exactMinimumAmount = 0.01;
    
    const result = validateTransactionAmount(exactMinimumAmount);
    
    const isValid = result.isValid;
    
    const expectedIsValid = true;
    
    expect(isValid).toBe(expectedIsValid);
  });

  it('should accept exactly maximum amount (1,000,000)', () => {
    const exactMaximumAmount = 1000000;
    
    const result = validateTransactionAmount(exactMaximumAmount);
    
    const isValid = result.isValid;
    
    const expectedIsValid = true;
    
    expect(isValid).toBe(expectedIsValid);
  });

  it('should reject NaN (Not a Number)', () => {
    const nanAmount = NaN;
    
    const result = validateTransactionAmount(nanAmount);
    
    const isValid = result.isValid;
    const error = result.error;
    
    const expectedIsValid = false;
    const expectedError = 'Amount cannot be NaN';
    
    expect(isValid).toBe(expectedIsValid);
    expect(error).toBe(expectedError);
  });

  it('should reject Infinity', () => {
    const infinityAmount = Infinity;
    
    const result = validateTransactionAmount(infinityAmount);
    
    const isValid = result.isValid;
    const error = result.error;
    
    const expectedIsValid = false;
    const expectedError = 'Amount must be a finite number';
    
    expect(isValid).toBe(expectedIsValid);
    expect(error).toBe(expectedError);
  });
});
