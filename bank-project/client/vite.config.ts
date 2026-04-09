import { defineConfig } from 'vitest/config'
import react from '@vitejs/plugin-react'

// https://vite.dev/config/
export default defineConfig({
  plugins: [react()],
  server: {
    port: 5173,
    proxy: {
      '/api': {
        target: 'http://localhost:3000',
        changeOrigin: true,
      },
    },
  },
  test: {
    // Use jsdom to simulate browser environment
    environment: 'jsdom',
    // Enable globals (describe, it, expect available without imports)
    globals: true,
    // Setup file runs before all tests
    setupFiles: './src/__tests__/setup.ts',
    // Coverage configuration
    coverage: {
      provider: 'v8',
      reporter: ['text', 'json', 'html'],
      exclude: ['node_modules/', 'src/__tests__/'],
    },
  },
  // eslint-disable-next-line @typescript-eslint/no-explicit-any -- vitest/config types conflict with vite types
} as any)
