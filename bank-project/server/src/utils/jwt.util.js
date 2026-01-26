import jwt from 'jsonwebtoken';
import config from '../config/index.js';

export const createToken = (user) => jwt.sign(
  { id: user.id, email: user.email }, config.jwt.secret,
  { expiresIn: config.jwt.expiresIn }
);


export const verifyToken = (token) => jwt.verify(token, config.jwt.secret);