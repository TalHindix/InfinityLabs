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

const buildTransferNotificationEmailTemplate = ({
  receiverName,
  senderName,
  senderEmail,
  amount,
  description,
  transactionId,
  videoCallUrl,
}) => {
  const year = new Date().getFullYear();
  const formattedAmount = amount.toFixed(2);

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
          <h2 style="color: #333; text-align: center;">💰 Money Transfer Received</h2>
          <p style="color: #666; font-size: 16px;">
            Hello ${receiverName || 'there'},
          </p>
          <p style="color: #666; font-size: 16px;">
            You have received a money transfer:
          </p>
          <div style="background-color: #f9f9f9; border-radius: 6px; padding: 20px; margin: 20px 0;">
            <table style="width: 100%; border-collapse: collapse;">
              <tr>
                <td style="padding: 8px 0; color: #666;"><strong>Amount:</strong></td>
                <td style="padding: 8px 0; text-align: right; color: #2c3e50; font-size: 18px; font-weight: bold;">
                  ${formattedAmount} AED
                </td>
              </tr>
              <tr>
                <td style="padding: 8px 0; color: #666;"><strong>From:</strong></td>
                <td style="padding: 8px 0; text-align: right; color: #2c3e50;">
                  ${senderName} (${senderEmail})
                </td>
              </tr>
              <tr>
                <td style="padding: 8px 0; color: #666;"><strong>Description:</strong></td>
                <td style="padding: 8px 0; text-align: right; color: #2c3e50;">
                  ${description || '—'}
                </td>
              </tr>
              <tr>
                <td style="padding: 8px 0; color: #666;"><strong>Transaction ID:</strong></td>
                <td style="padding: 8px 0; text-align: right; color: #2c3e50;">
                  #${transactionId}
                </td>
              </tr>
            </table>
          </div>
          <div style="text-align: center; margin: 30px 0;">
            <a href="${videoCallUrl}"
               style="background-color: #4CAF50; color: white; padding: 14px 28px;
                      text-decoration: none; border-radius: 6px; font-size: 16px;
                      font-weight: bold; display: inline-block;">
              Join Video Call
            </a>
          </div>
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

const buildOtpEmailTemplate = (otp) => {
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
          <h2 style="color: #333; text-align: center;">Your Login Code</h2>
          <p style="color: #666; text-align: center; font-size: 16px;">
            Use the code below to complete your sign in. It expires in 5 minutes.
          </p>
          <div style="text-align: center; margin: 30px 0;">
            <div style="display: inline-block; background-color: #f9f9f9; border: 2px solid #C9A227; border-radius: 8px; padding: 20px 40px;">
              <span style="font-size: 36px; font-weight: bold; letter-spacing: 8px; color: #2c3e50;">${otp}</span>
            </div>
          </div>
          <p style="color: #999; text-align: center; font-size: 14px;">
            If you did not attempt to sign in, you can safely ignore this email.
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

async function sendOtpEmail(email, otp) {
  if (!config.email.brevoApiKey) throw new AppError('Missing BREVO_API_KEY', 500);
  if (!config.email.from) throw new AppError('Missing EMAIL_FROM', 500);

  const payload = {
    sender: { name: 'Dubai-Bank', email: config.email.from },
    to: [{ email }],
    subject: '🔐 Your Login Code - Dubai-Bank',
    htmlContent: buildOtpEmailTemplate(otp),
  };

  const res = await brevo.post('/smtp/email', payload);
  logger.info(`OTP email sent to ${email} (brevoMessageId: ${res.data?.messageId ?? 'n/a'})`);
}

export const sendOtpEmailAsync = (email, otp) => {
  if (!email || !otp) {
    logger.error('sendOtpEmailAsync: email and otp are required');
    return;
  }
  sendOtpEmail(email, otp).catch((err) => {
    const status = err?.response?.status;
    const data = err?.response?.data;
    logger.error(`Async OTP email failed for ${email}: status=${status} data=${JSON.stringify(data)}`, { error: err });
  });
};

export const buildVerificationResultPage = (success, errorMessage = null) => {
  const year = new Date().getFullYear();

  return `
    <!DOCTYPE html>
    <html>
    <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>${success ? 'Email Verified' : 'Verification Failed'} - Dubai-Bank</title>
    </head>
    <body style="margin: 0; padding: 0; font-family: Arial, sans-serif; background-color: #f4f4f4; min-height: 100vh; display: flex; align-items: center; justify-content: center;">
      <div style="max-width: 500px; margin: 20px; padding: 40px; background: white; border-radius: 12px; box-shadow: 0 4px 12px rgba(0,0,0,0.1); text-align: center;">
        <div style="text-align: center; margin-bottom: 20px;">
          <h1 style="color: #2c3e50; margin: 0; font-size: 28px;">Dubai-Bank</h1>
        </div>
        <div style="font-size: 60px; margin-bottom: 20px;">
          ${success ? '✅' : '❌'}
        </div>
        <h2 style="color: ${success ? '#4CAF50' : '#e74c3c'}; margin: 0 0 15px 0;">
          ${success ? 'Email Verified!' : 'Verification Failed'}
        </h2>
        <p style="color: #666; font-size: 16px; margin: 0 0 30px 0;">
          ${success
            ? 'Your email has been successfully verified. You can now log in to your account.'
            : (errorMessage ?? 'Verification failed.')}
        </p>
        <a href="${config.clientUrl}/login${success ? '?verified=true' : ''}" 
           style="display: inline-block; background-color: ${success ? '#4CAF50' : '#3498db'}; 
                  color: white; padding: 14px 32px; text-decoration: none; 
                  border-radius: 6px; font-size: 16px; font-weight: bold;">
          ${success ? 'Go to Login' : 'Back to Login'}
        </a>
        <hr style="border: none; border-top: 1px solid #eee; margin: 30px 0;">
        <p style="color: #bbb; text-align: center; font-size: 12px;">
          © ${year} Dubai-Bank. All rights reserved.
        </p>
      </div>
    </body>
    </html>
  `;
};

async function sendVerificationEmail(email, token) {
  if (!config.email.brevoApiKey) throw new AppError('Missing BREVO_API_KEY', 500);
  if (!config.email.from) throw new AppError('Missing EMAIL_FROM', 500);
  if (!config.serverUrl) throw new AppError('Missing SERVER_URL', 500);

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
  if (!email || !token) {
    logger.error('sendVerificationEmailAsync: email and token are required');
    return;
  }
  sendVerificationEmail(email, token).catch((err) => {
    const status = err?.response?.status;
    const data = err?.response?.data;
    logger.error(`Async email failed for ${email}: status=${status} data=${JSON.stringify(data)}`, { error: err });
  });
};

async function sendTransferNotificationEmail({
  receiverEmail,
  receiverName,
  senderName,
  senderEmail,
  amount,
  description,
  transactionId,
  videoCallUrl,
}) {
  if (!config.email.brevoApiKey) throw new AppError('Missing BREVO_API_KEY', 500);
  if (!config.email.from) throw new AppError('Missing EMAIL_FROM', 500);

  const payload = {
    sender: { name: 'Dubai-Bank', email: config.email.from },
    to: [{ email: receiverEmail, name: receiverName }],
    subject: `💰 Money Transfer Received - ${amount.toFixed(2)} AED from ${senderName}`,
    htmlContent: buildTransferNotificationEmailTemplate({
      receiverName,
      senderName,
      senderEmail,
      amount,
      description,
      transactionId,
      videoCallUrl,
    }),
  };

  const res = await brevo.post('/smtp/email', payload);
  logger.info(`Transfer notification email sent to ${receiverEmail} (brevoMessageId: ${res.data?.messageId ?? 'n/a'})`);
}

export const sendTransferNotificationEmailAsync = (params) => {
  if (!params?.receiverEmail) {
    logger.error('sendTransferNotificationEmailAsync: receiverEmail is required');
    return;
  }
  sendTransferNotificationEmail(params).catch((err) => {
    const status = err?.response?.status;
    const data = err?.response?.data;
    logger.error(`Async transfer email failed for ${params.receiverEmail}: status=${status} data=${JSON.stringify(data)}`, { error: err });
  });
};
