import config from '../config/index.js';

const cookieOptions = () => ({
  httpOnly: true,
  secure: config.cookie.secure,
  sameSite: config.cookie.sameSite,
  path: '/',
});

export const setAuthCookie = (res, token) =>
  res.cookie(config.cookie.tokenName, token, {
    ...cookieOptions(),
    maxAge: config.cookie.maxAgeSeconds * 1000,
  });

export const clearAuthCookie = (res) =>
  res.clearCookie(config.cookie.tokenName, cookieOptions());
