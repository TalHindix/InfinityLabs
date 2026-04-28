import dotenv from 'dotenv';
dotenv.config();

const isProd = process.env.NODE_ENV === 'production';

const alwaysRequired = ['JWT_SECRET'];
const prodOnly = ['MONGO_URI', 'BREVO_API_KEY', 'EMAIL_FROM'];

for (const v of alwaysRequired) {
  if (!process.env[v]) throw new Error(`Missing required env var: ${v}`);
}

if (isProd) {
  for (const v of prodOnly) {
    if (!process.env[v]) throw new Error(`Missing required env var: ${v}`);
  }
} else {
  for (const v of prodOnly) {
    if (!process.env[v]) console.warn(`[config] Missing env var: ${v} (using default)`);
  }
}

// When the frontend and API live on different origins (e.g. Vercel + Render),
// browsers will only send the auth cookie if it's SameSite=None + Secure. We
// flip both defaults below based on this.
const isCrossOrigin = (() => {
  try {
    const clientOrigin = new URL(process.env.CLIENT_URL || '').origin;
    const serverOrigin = new URL(process.env.SERVER_URL || '').origin;
    return clientOrigin !== serverOrigin;
  } catch {
    return false;
  }
})();

const config = {
  port: Number(process.env.PORT) || 3000,
  nodeEnv: process.env.NODE_ENV || 'development',
  openAiApiKey: process.env.OPENAI_API_KEY,
  mongoUri: process.env.MONGO_URI || 'mongodb://localhost:27017/Dubai-Bank',

  jwt: {
    secret: process.env.JWT_SECRET,
    expiresIn: process.env.JWT_EXPIRES_IN || '1h',
  },

  cookie: {
    tokenName: 'token',
    maxAgeSeconds: 3600,
    sameSite: process.env.COOKIE_SAME_SITE || (isCrossOrigin ? 'none' : 'lax'),
    secure: isProd || isCrossOrigin,
  },

  email: {
    brevoApiKey: process.env.BREVO_API_KEY,
    from: process.env.EMAIL_FROM,
  },

  clientUrl: process.env.CLIENT_URL || 'http://localhost:5173',
  serverUrl: process.env.SERVER_URL || 'http://localhost:3000',
};

Object.freeze(config.jwt);
Object.freeze(config.cookie);
Object.freeze(config.email);
Object.freeze(config);

export default config;