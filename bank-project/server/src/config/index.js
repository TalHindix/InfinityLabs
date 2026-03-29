import dotenv from 'dotenv';
dotenv.config();

function validateRequiredEnvVars() {
  if (!process.env.JWT_SECRET) throw new Error('Missing required environment variable: JWT_SECRET');
  if (!process.env.MONGO_URI) throw new Error('Missing required environment variable: MONGO_URI');
}

validateRequiredEnvVars();

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
    secure: process.env.NODE_ENV === 'production' || isCrossOrigin,
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