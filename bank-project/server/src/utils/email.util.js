import nodemailer from 'nodemailer';
import config from '../config/index.js';
import logger from './logger.util.js';
import path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const MAX_RETRIES = 3;
const BASE_DELAY_MS = 1000;

const createTransporter = () => nodemailer.createTransport({
  service: 'gmail',
  auth: {
    user: config.email.user,
    pass: config.email.pass
  }
});

const buildVerificationUrl = (token) => 
  `${config.serverUrl}/api/v1/auth/verify?token=${token}`;

const delay = (ms) => new Promise((resolve) => setTimeout(resolve, ms));

const calculateBackoff = (attempt) => Math.pow(2, attempt + 1) * BASE_DELAY_MS;

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
            <h1 style="color: #2c3e50; margin: 0;"> Dubai-Bank</h1>
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
            © ${year} Bank App. All rights reserved.
          </p>
        </div>
      </div>
    </body>
    </html>
  `;
};

const sendVerificationEmail = async (email, token) => {
  // Debug log - remove after fixing
  logger.info(`Email config: user=${config.email.user}, pass=${config.email.pass ? '***SET***' : '***MISSING***'}`);
  
  const transporter = createTransporter();
  const mailOptions = {
    from: `"Dubai-Bank" <${config.email.user}>`,
    to: email,
    subject: '🔐 Verify Your Email - Dubai-Bank',
    html: buildEmailTemplate(token),
  };

  for (let attempt = 0; attempt < MAX_RETRIES; attempt++) {
    try {
      await transporter.sendMail(mailOptions);
      logger.info(`Verification email sent to ${email}`);
      return;
    } catch (error) {
      const isLastAttempt = attempt === MAX_RETRIES - 1;
      // Log the actual error message
      logger.error(`Email error: ${error.message}`);
      
      if (isLastAttempt) {
        logger.error(`Failed to send email to ${email}: ${error.message}`);
        throw error;
      }
      const delayMs = calculateBackoff(attempt);
      logger.warn(`Email attempt ${attempt + 1}/${MAX_RETRIES} failed. Retrying in ${delayMs}ms...`);
      await delay(delayMs);
    }
  }
};

export const sendVerificationEmailAsync = (email, token) => {
  sendVerificationEmail(email, token).catch((err) => {
    logger.error(`Async email failed for ${email}: ${err.message}`);
  });
};