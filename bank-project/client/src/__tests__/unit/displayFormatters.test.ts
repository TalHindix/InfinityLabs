/**
 * FRONTEND UNIT TEST EXAMPLE: Testing displayFormatters functions
 * 
 * WHAT IS A UNIT TEST?
 * - Tests ONE function in isolation
 * - No React components, no API calls, no side effects
 * - Just: input → output
 * 
 * WHAT ARE WE TESTING?
 * Functions that format numbers and dates for display:
 * - formatAmount(): Converts 1234.5 → "1,234.50"
 * - formatDate(): Converts ISO date → "27 Jan 2026, 12:14"
 * - formatShortDate(): Converts ISO date → "27 Jan"
 * 
 * WHY TEST THIS?
 * - These functions are used everywhere (displaying money, dates)
 * - Bugs here = wrong numbers/dates shown to users
 * - Easy to test (input number/string → output formatted string)
 */

import { describe, it, expect } from 'vitest';
import { formatAmount, formatDate, formatShortDate } from '../../shared/displayFormatters';

describe('displayFormatters - Unit Test', () => {
  
  describe('formatAmount', () => {
    it('should format amount with 2 decimal places', () => {
      // Test: 1234.5 should become "1,234.50"
      expect(formatAmount(1234.5)).toBe('1,234.50');
      
      // Test: 100 should become "100.00"
      expect(formatAmount(100)).toBe('100.00');
    });

    it('should handle zero', () => {
      expect(formatAmount(0)).toBe('0.00');
    });

    it('should handle large amounts with commas', () => {
      expect(formatAmount(1000000)).toBe('1,000,000.00');
    });

    it('should handle small amounts', () => {
      expect(formatAmount(0.01)).toBe('0.01');
    });
  });

  describe('formatDate', () => {
    it('should format ISO date string to readable format', () => {
      const dateStr = '2026-01-27T10:14:33Z';
      const formatted = formatDate(dateStr);
      
      // Should contain year, month, and time
      expect(formatted).toContain('2026');
      expect(formatted).toContain('Jan');
      expect(formatted).toContain('14'); // minute
    });
  });

  describe('formatShortDate', () => {
    it('should format to day and month only', () => {
      const dateStr = '2026-01-27T10:14:33Z';
      const formatted = formatShortDate(dateStr);
      
      // Should be "27 Jan" format
      expect(formatted).toBe('27 Jan');
    });
  });
});
