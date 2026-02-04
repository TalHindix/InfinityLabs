import dotenv from 'dotenv';
dotenv.config();

function validateRequiredEnvVars() {
  const required = [
    { key: 'JWT_SECRET', value: process.env.JWT_SECRET },
    { key: 'MONGO_URI', value: process.env.MONGO_URI }
  ];
  const missing = required.filter(({ value }) => !value);
  if (missing.length > 0) {
    const missingKeys = missing.map(({ key }) => key).join(', ');
    throw new Error(`Missing required environment variables: ${missingKeys}\nPlease check your .env file or environment configuration.`);
  }
}

validateRequiredEnvVars();

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
    maxAgeSeconds: 3600, // 1 hour, should match jwt.expiresIn
    sameSite: 'lax',
    secure: process.env.NODE_ENV === 'production',
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