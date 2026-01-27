import { StrictMode } from 'react';
import { createRoot } from 'react-dom/client';
import { ThemeContextProvider } from './context/ThemeContextProvider';
import App from './App';
import './index.css';

// '!' - trust me path 'root'  not null )
createRoot(document.getElementById('root')!).render(
  <StrictMode>
    <ThemeContextProvider>
      <App />
    </ThemeContextProvider>
  </StrictMode>
);
