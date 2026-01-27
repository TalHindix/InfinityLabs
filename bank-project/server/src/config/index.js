import dotenv from 'dotenv';
dotenv.config();


const config = {
  port: process.env.PORT || 3000,
  nodeEnv: process.env.NODE_ENV || 'development',
  
  mongoUri: process.env.MONGO_URI || 'mongodb://localhost:27017/Dubai-Bank',
  
  jwt: {
    secret: process.env.JWT_SECRET,
    expiresIn: process.env.JWT_EXPIRES_IN || '1h'
  },
  
  email: {
    brevoApiKey: process.env.BREVO_API_KEY,
    from: process.env.EMAIL_FROM
  },
  
  clientUrl: process.env.CLIENT_URL || 'http://localhost:5173',
  serverUrl: process.env.SERVER_URL || 'http://localhost:3000'
};

console.log({
  hasBrevoKey: Boolean(config.email.brevoApiKey),
  emailFrom: config.email.from,
  serverUrl: config.serverUrl
});

export default config;