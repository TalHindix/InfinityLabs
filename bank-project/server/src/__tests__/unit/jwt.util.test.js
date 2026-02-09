import { describe, it, expect } from '@jest/globals';
import { createToken, verifyToken } from '../../utils/jwt.util.js';

describe('JWT Utils', () => {
  it('should create a token for a user with id and email', () => {
    const userId = '123';
    const userEmail = 'test@example.com';
    const user = { id: userId, email: userEmail };
    
    const token = createToken(user);
    
    const tokenType = typeof token;
    const minimumTokenLength = 0;
    
    expect(tokenType).toBe('string');
    expect(token.length).toBeGreaterThan(minimumTokenLength);
  });

  it('should throw error if user missing id', () => {
    const userEmail = 'test@example.com';
    const user = { email: userEmail };
    
    const expectedErrorMessage = 'User must have id and email to create token';
    
    expect(() => {
      createToken(user);
    }).toThrow(expectedErrorMessage);
  });

  it('should throw error if user missing email', () => {
    const userId = '123';
    const user = { id: userId };
    
    const expectedErrorMessage = 'User must have id and email to create token';
    
    expect(() => {
      createToken(user);
    }).toThrow(expectedErrorMessage);
  });

  it('should verify and decode a valid token', () => {
    const userId = '123';
    const userEmail = 'test@example.com';
    const user = { id: userId, email: userEmail };
    
    const token = createToken(user);
    const decoded = verifyToken(token);
    
    const decodedId = decoded.id;
    const decodedEmail = decoded.email;
    
    const expectedId = '123';
    const expectedEmail = 'test@example.com';
    
    expect(decodedId).toBe(expectedId);
    expect(decodedEmail).toBe(expectedEmail);
  });

  it('should throw error for invalid token', () => {
    const invalidToken = 'not.a.valid.token';
    
    expect(() => {
      verifyToken(invalidToken);
    }).toThrow();
  });
});
