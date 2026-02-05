import type { SxProps, Theme } from '@mui/material';

export const createFieldSx = (isDark: boolean): SxProps<Theme> => ({
  '& .MuiInputLabel-root': {
    color: isDark ? 'rgba(255,255,255,0.70)' : 'rgba(13,27,42,0.70)',
  },
  '& .MuiInputLabel-root.Mui-focused': {
    color: 'rgba(201, 162, 39, 0.95)',
  },
  '& .MuiOutlinedInput-root': {
    backgroundColor: isDark ? 'rgba(255,255,255,0.04)' : 'rgba(13,27,42,0.03)',
    borderRadius: 12,
    transition:
      'box-shadow 160ms ease, border-color 160ms ease, background-color 160ms ease',
    '& fieldset': {
      borderColor: isDark ? 'rgba(255,255,255,0.12)' : 'rgba(13,27,42,0.14)',
    },
    '&:hover fieldset': {
      borderColor: 'rgba(201, 162, 39, 0.45)',
    },
    '&.Mui-focused fieldset': {
      borderColor: 'rgba(201, 162, 39, 0.85)',
    },
    '&.Mui-focused': {
      boxShadow: '0 0 0 4px rgba(201,162,39,0.16)',
    },
    '& input': {
      color: isDark ? 'rgba(255,255,255,0.92)' : '#0D1B2A',
    },
  },
  '& .MuiFormHelperText-root': {
    color: isDark ? 'rgba(255,255,255,0.65)' : 'rgba(13,27,42,0.65)',
  },
  '& input:-webkit-autofill, & input:-webkit-autofill:hover, & input:-webkit-autofill:focus, & input:-webkit-autofill:active': {
    WebkitBoxShadow: isDark
      ? '0 0 0 1000px #0a1520 inset !important'
      : '0 0 0 1000px #f5f5f5 inset !important',
    WebkitTextFillColor: isDark ? '#fff !important' : '#0D1B2A !important',
    caretColor: isDark ? '#fff' : '#0D1B2A',
    transition: 'background-color 5000s ease-in-out 0s',
  },
});
