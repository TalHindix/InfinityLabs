/**
 * FRONTEND UNIT TEST EXAMPLE: Testing timeBasedGreeting function
 * 
 * WHAT ARE WE TESTING?
 * The getTimeBasedGreeting() function that returns different greetings
 * based on the time of day:
 * - 5 AM - 12 PM: "Good morning"
 * - 12 PM - 5 PM: "Good afternoon"
 * - 5 PM - 9 PM: "Good evening"
 * - Other times: "Welcome back"
 * 
 * WHY TEST THIS?
 * - Time-based logic needs to be tested at different hours
 * - We need to mock Date to test different times
 * - Shows how to test functions that depend on system time
 */

import { describe, it, expect, beforeEach, afterEach, vi } from 'vitest';
import { getTimeBasedGreeting } from '../../shared/timeBasedGreeting';

describe('getTimeBasedGreeting - Unit Test', () => {
  // Save original Date constructor
  let originalDate: DateConstructor;

  beforeEach(() => {
    originalDate = global.Date;
  });

  afterEach(() => {
    // Restore original Date after each test
    global.Date = originalDate;
  });

  it('should return "Good morning" between 5 AM and 12 PM', () => {
    // Mock Date to return 10 AM
    global.Date = class extends originalDate {
      getHours() {
        return 10; // 10 AM
      }
    } as DateConstructor;

    expect(getTimeBasedGreeting()).toBe('Good morning');
  });

  it('should return "Good afternoon" between 12 PM and 5 PM', () => {
    // Mock Date to return 2 PM
    global.Date = class extends originalDate {
      getHours() {
        return 14; // 2 PM
      }
    } as DateConstructor;

    expect(getTimeBasedGreeting()).toBe('Good afternoon');
  });

  it('should return "Good evening" between 5 PM and 9 PM', () => {
    // Mock Date to return 6 PM
    global.Date = class extends originalDate {
      getHours() {
        return 18; // 6 PM
      }
    } as DateConstructor;

    expect(getTimeBasedGreeting()).toBe('Good evening');
  });

  it('should return "Welcome back" for late night/early morning', () => {
    // Mock Date to return 2 AM
    global.Date = class extends originalDate {
      getHours() {
        return 2; // 2 AM
      }
    } as DateConstructor;

    expect(getTimeBasedGreeting()).toBe('Welcome back');
  });

  it('should return "Welcome back" for very late night', () => {
    // Mock Date to return 11 PM
    global.Date = class extends originalDate {
      getHours() {
        return 23; // 11 PM
      }
    } as DateConstructor;

    expect(getTimeBasedGreeting()).toBe('Welcome back');
  });
});
