import { describe, it, expect } from 'vitest';
import { mergeMonthlyData } from '../../screens/dashboard/chartUtils';

describe('mergeMonthlyData', () => {
  it('sorts months in calendar order within a year', () => {
    const spending = [
      { year: 2024, month: 11, totalSpent: 100 },
      { year: 2024, month: 3,  totalSpent: 200 },
      { year: 2024, month: 12, totalSpent: 300 },
    ];
    const result = mergeMonthlyData(spending, []);
    expect(result.map((r) => r.label)).toEqual(['Mar 2024', 'Nov 2024', 'Dec 2024']);
  });

  it('sorts across years', () => {
    const spending = [
      { year: 2025, month: 1, totalSpent: 50 },
      { year: 2024, month: 12, totalSpent: 80 },
    ];
    const result = mergeMonthlyData(spending, []);
    expect(result.map((r) => r.label)).toEqual(['Dec 2024', 'Jan 2025']);
  });

  it('merges spending and received for the same month', () => {
    const spending  = [{ year: 2024, month: 5, totalSpent: 400 }];
    const received  = [{ year: 2024, month: 5, totalReceived: 150 }];
    const result = mergeMonthlyData(spending, received);
    expect(result).toEqual([{ label: 'May 2024', spent: 400, received: 150 }]);
  });

  it('fills spent=0 for months that only have received', () => {
    const result = mergeMonthlyData([], [{ year: 2024, month: 7, totalReceived: 300 }]);
    expect(result[0]).toEqual({ label: 'Jul 2024', spent: 0, received: 300 });
  });

  it('returns empty array when both inputs are empty', () => {
    expect(mergeMonthlyData([], [])).toEqual([]);
  });
});
