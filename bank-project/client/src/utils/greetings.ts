
export function getTimeBasedGreeting(): string {
  const currentHour = new Date().getHours();

  if (currentHour >= 5 && currentHour < 12) {
    return 'Good morning';
  }

  if (currentHour >= 12 && currentHour < 17) {
    return 'Good afternoon';
  }

  if (currentHour >= 17 && currentHour < 21) {
    return 'Good evening';
  }

  return 'Welcome back';
}
