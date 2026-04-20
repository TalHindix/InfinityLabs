// Provides theme context and MUI theme; persists mode in localStorage.
import { useEffect, useState, type ReactNode } from 'react';
import { ThemeProvider as MuiThemeProvider } from '@mui/material/styles';
import { CssBaseline } from '@mui/material';

import { lightTheme, darkTheme } from '../shared/muiTheme';
import { ThemeContext, type ThemeMode } from './ThemeContext';

type Props = {
  children: ReactNode;
};

/** Read saved theme from localStorage on first load; default to dark. */
function getInitialThemeFromStorage(): ThemeMode {
  const saved = localStorage.getItem('theme');
  return saved === 'light' || saved === 'dark' ? saved : 'dark';
}

export function ThemeContextProvider({ children }: Props) {
  const [mode, setMode] = useState<ThemeMode>(getInitialThemeFromStorage);

  useEffect(() => {
    localStorage.setItem('theme', mode);
  }, [mode]);

  function toggleTheme() {
    setMode((m) => (m === 'light' ? 'dark' : 'light'));
  }

  const isDark = mode === 'dark';
  const theme = isDark ? darkTheme : lightTheme;

  return (
    <ThemeContext.Provider value={{ mode, isDark, toggleTheme }}>
      <MuiThemeProvider theme={theme}>
        <CssBaseline />
        {children}
      </MuiThemeProvider>
    </ThemeContext.Provider>
  );
}
