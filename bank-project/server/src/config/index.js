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
    user: process.env.EMAIL_USER,
    pass: process.env.EMAIL_PASS,
    resendApiKey: process.env.RESEND_API_KEY
  },
  
  clientUrl: process.env.CLIENT_URL || 'http://localhost:5173',
  serverUrl: process.env.SERVER_URL || 'http://localhost:3000'
};

export default config;