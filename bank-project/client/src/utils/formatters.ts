const LOCALE = 'en-AE';

/**
 * Converts a number to a readable money format (2 decimal digits).
 * Example: 1234.5 → "1,234.50"
 */
export function formatAmount(amount: number): string {
  return amount.toLocaleString(LOCALE, {
    minimumFractionDigits: 2,
    maximumFractionDigits: 2,
  });
}

/**
 * Converts a date string to a full readable date and time.
 * Example: "2026-01-27T10:14:33Z" → "27 Jan 2026, 12:14"
 */
export function formatDate(dateString: string): string {
  const date = new Date(dateString);

  return date.toLocaleDateString(LOCALE, {
    day: 'numeric',
    month: 'short',
    year: 'numeric',
    hour: '2-digit',
    minute: '2-digit',
  });
}

/**
 * Converts a date string to a short readable date (day + month).
 * Example: "2026-01-27T10:14:33Z" → "27 Jan"
 */
export function formatShortDate(dateString: string): string {
  const date = new Date(dateString);

  return date.toLocaleDateString(LOCALE, {
    day: 'numeric',
    month: 'short',
  });
}
