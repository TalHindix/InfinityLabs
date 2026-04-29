import axios from 'axios';
import config from '../config/index.js';
import logger from './logger.util.js';
import { AppError } from './error.util.js';

const brevo = axios.create({
  baseURL: 'https://api.brevo.com/v3',
  timeout: 15_000,
  headers: {
    'Content-Type': 'application/json',
    'api-key': config.email.brevoApiKey,
  },
});

export async function sendEmail({ to, toName, subject, html }) {
  if (!config.email.brevoApiKey) throw new AppError('Missing BREVO_API_KEY', 500);
  if (!config.email.from) throw new AppError('Missing EMAIL_FROM', 500);

  const payload = {
    sender: { name: 'Dubai-Bank', email: config.email.from },
    to: [{ email: to, ...(toName && { name: toName }) }],
    subject,
    htmlContent: html,
  };

  const res = await brevo.post('/smtp/email', payload);
  logger.info(`Email sent to ${to} (brevoMessageId: ${res.data?.messageId ?? 'n/a'})`);
}

export const fireAndForget = (label, guardObj, promiseFn) => {
  const missing = Object.keys(guardObj).filter((k) => !guardObj[k]);
  if (missing.length) {
    logger.error(`fireAndForget ${label}: ${missing.join(', ')} are required`);
    return;
  }
  promiseFn().catch((err) => {
    const status = err?.response?.status;
    const data = err?.response?.data;
    logger.error(`Async ${label} email failed: status=${status} data=${JSON.stringify(data)}`, { error: err });
  });
};
