import { createTheme, type Theme } from '@mui/material/styles';


const baseTypography = {
  fontFamily: '"Inter", "Segoe UI", "Roboto", "Helvetica", "Arial", sans-serif',
  h1: {
    fontWeight: 700,
    letterSpacing: '-0.02em',
  },
  h2: {
    fontWeight: 700,
    letterSpacing: '-0.01em',
  },
  h3: {
    fontWeight: 600,
    letterSpacing: '-0.01em',
  },
  h4: {
    fontWeight: 600,
  },
  h5: {
    fontWeight: 600,
  },
  h6: {
    fontWeight: 600,
  },
  button: {
    textTransform: 'none' as const,
    fontWeight: 600,
  },
};

const baseShape = {
  borderRadius: 12,
};

// Light Theme
export const lightTheme: Theme = createTheme({
  palette: {
    mode: 'light',
    primary: {
      main: '#0D1B2A', // Deep navy - corporate trust
      light: '#1B3A4B',
      dark: '#051014',
      contrastText: '#FFFFFF',
    },
    secondary: {
      main: '#C9A227', // Dubai gold
      light: '#E5C158',
      dark: '#A17F1A',
      contrastText: '#0D1B2A',
    },
    background: {
      default: '#F5F5F0',
      paper: '#FFFFFF',
    },
    text: {
      primary: '#0D1B2A',
      secondary: '#5A6A7A',
    },
    success: {
      main: '#10B981',
      light: '#34D399',
    },
    error: {
      main: '#EF4444',
      light: '#F87171',
    },
  },
  typography: baseTypography,
  shape: baseShape,
  components: {
    MuiButton: {
      styleOverrides: {
        root: {
          borderRadius: 8,
          padding: '10px 24px',
          boxShadow: 'none',
          '&:hover': {
            boxShadow: 'none',
          },
        },
        contained: {
          background: 'linear-gradient(135deg, #C9A227 0%, #E5C158 100%)',
          color: '#0D1B2A',
          '&:hover': {
            background: 'linear-gradient(135deg, #A17F1A 0%, #C9A227 100%)',
          },
        },
        containedPrimary: {
          background: 'linear-gradient(135deg, #0D1B2A 0%, #1B3A4B 100%)',
          color: '#FFFFFF',
          '&:hover': {
            background: 'linear-gradient(135deg, #051014 0%, #0D1B2A 100%)',
          },
        },
        outlined: {
          borderColor: '#C9A227',
          color: '#C9A227',
          '&:hover': {
            borderColor: '#A17F1A',
            backgroundColor: 'rgba(201, 162, 39, 0.08)',
          },
        },
      },
    },
    MuiCard: {
      styleOverrides: {
        root: {
          boxShadow: '0 4px 24px rgba(13, 27, 42, 0.08)',
          border: '1px solid rgba(13, 27, 42, 0.06)',
        },
      },
    },
    MuiTextField: {
      styleOverrides: {
        root: {
          '& .MuiOutlinedInput-root': {
            '&:hover fieldset': {
              borderColor: '#C9A227',
            },
            '&.Mui-focused fieldset': {
              borderColor: '#C9A227',
            },
          },
          '& .MuiInputLabel-root.Mui-focused': {
            color: '#C9A227',
          },
        },
      },
    },
    MuiTableHead: {
      styleOverrides: {
        root: {
          '& .MuiTableCell-root': {
            backgroundColor: '#0D1B2A',
            color: '#FFFFFF',
            fontWeight: 600,
          },
        },
      },
    },
    MuiTableRow: {
      styleOverrides: {
        root: {
          '&:nth-of-type(even)': {
            backgroundColor: 'rgba(13, 27, 42, 0.02)',
          },
          '&:hover': {
            backgroundColor: 'rgba(201, 162, 39, 0.06)',
          },
        },
      },
    },
    MuiAlert: {
      styleOverrides: {
        standardSuccess: {
          backgroundColor: 'rgba(16, 185, 129, 0.1)',
          color: '#059669',
        },
        standardError: {
          backgroundColor: 'rgba(239, 68, 68, 0.1)',
          color: '#DC2626',
        },
      },
    },
    MuiStepper: {
      styleOverrides: {
        root: {
          '& .MuiStepIcon-root.Mui-active': {
            color: '#C9A227',
          },
          '& .MuiStepIcon-root.Mui-completed': {
            color: '#C9A227',
          },
        },
      },
    },
  },
});

// Dark Theme
export const darkTheme: Theme = createTheme({
  palette: {
    mode: 'dark',
    primary: {
      main: '#1B3A4B', // Lighter navy for dark mode
      light: '#2D5A6B',
      dark: '#0D1B2A',
      contrastText: '#FFFFFF',
    },
    secondary: {
      main: '#C9A227', // Dubai gold stays the same
      light: '#E5C158',
      dark: '#A17F1A',
      contrastText: '#0D1B2A',
    },
    background: {
      default: '#0A0F14',
      paper: '#121A22',
    },
    text: {
      primary: '#F0F4F8',
      secondary: '#94A3B8',
    },
    success: {
      main: '#10B981',
      light: '#34D399',
    },
    error: {
      main: '#EF4444',
      light: '#F87171',
    },
  },
  typography: baseTypography,
  shape: baseShape,
  components: {
    MuiButton: {
      styleOverrides: {
        root: {
          borderRadius: 8,
          padding: '10px 24px',
          boxShadow: 'none',
          '&:hover': {
            boxShadow: 'none',
          },
        },
        contained: {
          background: 'linear-gradient(135deg, #C9A227 0%, #E5C158 100%)',
          color: '#0D1B2A',
          '&:hover': {
            background: 'linear-gradient(135deg, #A17F1A 0%, #C9A227 100%)',
          },
        },
        containedPrimary: {
          background: 'linear-gradient(135deg, #1B3A4B 0%, #2D5A6B 100%)',
          color: '#FFFFFF',
          '&:hover': {
            background: 'linear-gradient(135deg, #0D1B2A 0%, #1B3A4B 100%)',
          },
        },
        outlined: {
          borderColor: '#C9A227',
          color: '#C9A227',
          '&:hover': {
            borderColor: '#E5C158',
            backgroundColor: 'rgba(201, 162, 39, 0.12)',
          },
        },
      },
    },
    MuiCard: {
      styleOverrides: {
        root: {
          boxShadow: '0 4px 24px rgba(0, 0, 0, 0.3)',
          border: '1px solid rgba(201, 162, 39, 0.1)',
          backgroundColor: '#121A22',
        },
      },
    },
    MuiTextField: {
      styleOverrides: {
        root: {
          '& .MuiOutlinedInput-root': {
            '&:hover fieldset': {
              borderColor: '#C9A227',
            },
            '&.Mui-focused fieldset': {
              borderColor: '#C9A227',
            },
          },
          '& .MuiInputLabel-root.Mui-focused': {
            color: '#C9A227',
          },
        },
      },
    },
    MuiTableHead: {
      styleOverrides: {
        root: {
          '& .MuiTableCell-root': {
            backgroundColor: '#1B3A4B',
            color: '#FFFFFF',
            fontWeight: 600,
          },
        },
      },
    },
    MuiTableRow: {
      styleOverrides: {
        root: {
          '&:nth-of-type(even)': {
            backgroundColor: 'rgba(201, 162, 39, 0.03)',
          },
          '&:hover': {
            backgroundColor: 'rgba(201, 162, 39, 0.08)',
          },
        },
      },
    },
    MuiTableCell: {
      styleOverrides: {
        root: {
          borderColor: 'rgba(201, 162, 39, 0.1)',
        },
      },
    },
    MuiAlert: {
      styleOverrides: {
        standardSuccess: {
          backgroundColor: 'rgba(16, 185, 129, 0.15)',
          color: '#34D399',
        },
        standardError: {
          backgroundColor: 'rgba(239, 68, 68, 0.15)',
          color: '#F87171',
        },
      },
    },
    MuiStepper: {
      styleOverrides: {
        root: {
          '& .MuiStepIcon-root.Mui-active': {
            color: '#C9A227',
          },
          '& .MuiStepIcon-root.Mui-completed': {
            color: '#C9A227',
          },
        },
      },
    },
    MuiPaper: {
      styleOverrides: {
        root: {
          backgroundImage: 'none',
        },
      },
    },
  },
});
