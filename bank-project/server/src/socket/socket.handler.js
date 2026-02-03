import { processMessage } from '../services/chatbot.service.js';

const getTimestamp = () => new Date().toISOString();

export const initChatbotSocket = (io) => {
  const chatNamespace = io.of('/chat');
  
  chatNamespace.on('connection', (socket) => {
    const userId = socket.handshake.auth?.userId || null;
    
    socket.emit('bot-message', {
      response: 'Hello! I\'m your virtual banking assistant 🏦',
      intent: 'greeting',
      timestamp: getTimestamp()
    });
    
    socket.on('user-message', async (message) => {
      try {
        const context = { userId };
        const result = await processMessage(message, context);

        socket.emit('bot-message', {
          response: result.message,
          intent: result.intent,
          data: result.data || null,
          requiresAuth: result.requiresAuth || false,
          timestamp: getTimestamp()
        });
      } catch (error) {
        socket.emit('bot-message', {
          response: 'Sorry, something went wrong. Please try again.',
          intent: 'error',
          timestamp: getTimestamp()
        });
      }
    });
    
    socket.on('disconnect', () => {});
  });
};