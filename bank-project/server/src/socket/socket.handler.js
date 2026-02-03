import { processMessage } from '../services/chatbot.service.js';

export const initChatbotSocket = (io) => {
  console.log('[Chatbot] Socket.io initialized');
  
  const chatNamespace = io.of('/chat');
  
  chatNamespace.on('connection', (socket) => {
    const userId = socket.handshake.auth.userId || null;
    
    socket.emit('bot-message', {
      response: 'Hello! I\'m your virtual banking assistant 🏦',
      intent: 'greeting',
      timestamp: new Date().toISOString()
    });
    
    socket.on('user-message', async (message) => {
      console.log(`[Chatbot] Received: ${message}`);
      
      const context = { userId };
      const result = await processMessage(message, context);

      socket.emit('bot-message', {
        response: result.message,
        intent: result.intent,
        data: result.data,
        requiresAuth: result.requiresAuth,
        timestamp: new Date().toISOString()
      });
    });
    
    socket.on('disconnect', () => {
      console.log(`[Chatbot] User disconnected: ${socket.id}`);
    });
  });
};