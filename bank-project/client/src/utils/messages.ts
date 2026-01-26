export const getIntelligentErrorMessage = (originalError: string): string => {
  const lowerError = originalError.toLowerCase();

  if (lowerError.includes('invalid') || lowerError.includes('incorrect') || lowerError.includes('wrong')) {
    return 'Credentials could not be verified. Please check and try again.';
  }
  if (lowerError.includes('not found') || lowerError.includes('no user')) {
    return 'Account not recognized. Please verify your email address.';
  }
  if (lowerError.includes('locked') || lowerError.includes('blocked') || lowerError.includes('suspended')) {
    return 'Access temporarily restricted — unusual activity detected.';
  }
  if (lowerError.includes('too many') || lowerError.includes('rate limit')) {
    return 'Sign-in paused — multiple attempts detected. Please wait.';
  }
  if (lowerError.includes('network') || lowerError.includes('connection')) {
    return 'Secure connection interrupted. Please try again.';
  }

  return 'Authentication could not be completed. Please try again.';
};
