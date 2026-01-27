const DAY_IN_MS = 24 * 60 * 60 * 1000;

export function getCookie(name: string): string | null {
  const allCookies = document.cookie.split('; ');

  for (const cookie of allCookies) {
    const [key, value] = cookie.split('=');

    if (key === name) {
      return decodeURIComponent(value);
    }
  }

  return null;
}

export function setCookie(
  name: string,
  value: string,
  days: number = 7
): void {
  const expiresAt = new Date(Date.now() + days * DAY_IN_MS);

  document.cookie = [
    `${name}=${encodeURIComponent(value)}`,
    `Expires=${expiresAt.toUTCString()}`,
    'Path=/',
    'SameSite=Strict',
    'Secure',
  ].join('; ');
}

export function deleteCookie(name: string): void {
  document.cookie = [
    `${name}=`,
    'Expires=Thu, 01 Jan 1970 00:00:00 GMT',
    'Path=/',
    'SameSite=Strict',
    'Secure',
  ].join('; ');
}
