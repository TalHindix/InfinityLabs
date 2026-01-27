export function getIntelligentErrorMessage(errorMessage: string): string {
  const message = errorMessage.toLowerCase();

  if (
    message.includes('invalid') ||
    message.includes('incorrect') ||
    message.includes('wrong')
  ) {
    return 'Credentials could not be verified. Please check and try again.';
  }

  if (
    message.includes('not found') ||
    message.includes('no user')
  ) {
    return 'Account not recognized. Please verify your email address.';
  }

  if (
    message.includes('locked') ||
    message.includes('blocked') ||
    message.includes('suspended')
  ) {
    return 'Access temporarily restricted — unusual activity detected.';
  }

  if (
    message.includes('too many') ||
    message.includes('rate limit')
  ) {
    return 'Sign-in paused — multiple attempts detected. Please wait.';
  }

  if (
    message.includes('network') ||
    message.includes('connection')
  ) {
    return 'Secure connection interrupted. Please try again.';
  }

  return 'Authentication could not be completed. Please try again.';
}
