import axios from 'axios';
import config from '../config/index.js';
import logger from './logger.util.js';

const brevo = axios.create({
  baseURL: 'https://api.brevo.com/v3',
  timeout: 15_000,
  headers: {
    'Content-Type': 'application/json',
    'api-key': config.email.brevoApiKey,
  },
});

const buildVerificationUrl = (token) =>
  `${config.serverUrl}/api/v1/auth/verify?token=${token}`;

const buildEmailTemplate = (token) => {
  const verificationUrl = buildVerificationUrl(token);
  const year = new Date().getFullYear();

  return `
    <!DOCTYPE html>
    <html>
    <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
    </head>
    <body style="margin: 0; padding: 0; font-family: Arial, sans-serif; background-color: #f4f4f4;">
      <div style="max-width: 600px; margin: 0 auto; padding: 20px;">
        <div style="background-color: #ffffff; border-radius: 8px; padding: 40px; box-shadow: 0 2px 4px rgba(0,0,0,0.1);">
          <div style="text-align: center; margin-bottom: 30px;">
            <h1 style="color: #2c3e50; margin: 0;">Dubai-Bank</h1>
          </div>
          <h2 style="color: #333; text-align: center;">Verify Your Email</h2>
          <p style="color: #666; text-align: center; font-size: 16px;">
            Welcome! Click the button below to verify your email and activate your account:
          </p>
          <div style="text-align: center; margin: 30px 0;">
            <a href="${verificationUrl}"
               style="background-color: #4CAF50; color: white; padding: 14px 28px;
                      text-decoration: none; border-radius: 6px; font-size: 16px;
                      font-weight: bold; display: inline-block;">
              ✓ Verify Email Address
            </a>
          </div>
          <p style="color: #999; text-align: center; font-size: 14px;">
            Or copy this link to your browser:
          </p>
          <p style="color: #4CAF50; text-align: center; font-size: 12px; word-break: break-all;">
            ${verificationUrl}
          </p>
          <hr style="border: none; border-top: 1px solid #eee; margin: 30px 0;">
          <p style="color: #bbb; text-align: center; font-size: 12px;">
            © ${year} Dubai-Bank. All rights reserved.
          </p>
        </div>
      </div>
    </body>
    </html>
  `;
};

async function sendVerificationEmail(email, token) {
  if (!config.email.brevoApiKey) throw new Error('Missing BREVO_API_KEY');
  if (!config.email.from) throw new Error('Missing EMAIL_FROM');
  if (!config.serverUrl) throw new Error('Missing SERVER_URL');

  const payload = {
    sender: { name: 'Dubai-Bank', email: config.email.from },
    to: [{ email }],
    subject: '🔐 Verify Your Email - Dubai-Bank',
    htmlContent: buildEmailTemplate(token),
  };

  const res = await brevo.post('/smtp/email', payload);
  logger.info(`Verification email sent to ${email} (brevoMessageId: ${res.data?.messageId ?? 'n/a'})`);
}

export const sendVerificationEmailAsync = (email, token) => {
  // fire-and-forget כמו שהיה לך
  sendVerificationEmail(email, token).catch((err) => {
    const status = err?.response?.status;
    const data = err?.response?.data;
    logger.error(`Async email failed for ${email}: status=${status} data=${JSON.stringify(data)}`);
  });
};
