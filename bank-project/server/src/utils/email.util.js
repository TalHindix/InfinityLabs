import config from '../config/index.js';
import { AppError } from './error.util.js';
import { sendEmail, fireAndForget } from './email.provider.js';
import { verificationEmailHtml, otpEmailHtml, transferNotificationEmailHtml } from './email.templates.js';

export { buildVerificationResultPage } from './email.templates.js';

export const sendVerificationEmailAsync = (email, token) =>
  fireAndForget('verification', { email, token }, () => {
    if (!config.serverUrl) throw new AppError('Missing SERVER_URL', 500);
    return sendEmail({ to: email, subject: '🔐 Verify Your Email - Dubai-Bank', html: verificationEmailHtml(token) });
  });

export const sendOtpEmailAsync = (email, otp) =>
  fireAndForget('OTP', { email, otp }, () =>
    sendEmail({ to: email, subject: '🔐 Your Login Code - Dubai-Bank', html: otpEmailHtml(otp) })
  );

export const sendTransferNotificationEmailAsync = (params) =>
  fireAndForget('transfer', { receiverEmail: params?.receiverEmail }, () =>
    sendEmail({
      to: params.receiverEmail,
      toName: params.receiverName,
      subject: `💰 Money Transfer Received - ${params.amount.toFixed(2)} AED from ${params.senderName}`,
      html: transferNotificationEmailHtml(params),
    })
  );
