/** Minimum allowed transfer amount. */
const MIN_AMOUNT = 0.01;

/** Maximum allowed transfer amount. */
const MAX_AMOUNT = 1_000_000;

/**
 * Validates transaction amount.
 * @param {*} amount - The amount to validate
 * @returns {{ isValid: boolean, error: string | null, sanitized: number | null }}
 */
export const validateTransactionAmount = (amount) => {
  const invalid = (error) => ({
    isValid: false,
    error,
    sanitized: null,
  });

  if (amount === null || amount === undefined) {
    return invalid('Amount is required');
  }

  if (typeof amount !== 'number') {
    return invalid('Amount must be a number');
  }

  if (!Number.isFinite(amount)) {
    return invalid('Amount must be a finite number');
  }

  if (Number.isNaN(amount)) {
    return invalid('Amount cannot be NaN');
  }

  if (amount < MIN_AMOUNT) {
    return invalid('Amount must be at least 0.01');
  }

  if (amount > MAX_AMOUNT) {
    return invalid(`Amount cannot exceed ${MAX_AMOUNT.toLocaleString()}`);
  }

  const decimalPlaces = (amount.toString().split('.')[1] || '').length;
  if (decimalPlaces > 2) {
    return invalid('Amount cannot have more than 2 decimal places');
  }

  return {
    isValid: true,
    error: null,
    sanitized: amount,
  };
};
