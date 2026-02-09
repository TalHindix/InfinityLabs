// Jest configuration for ES modules
export default {
  // Use Node environment (not browser)
  testEnvironment: 'node',
  
  // Don't transform files (we're using ES modules)
  transform: {},
  
  // Map imports to work with ES modules
  moduleNameMapper: {
    '^(\\.{1,2}/.*)\\.js$': '$1',
  },
  
  // Find test files in __tests__ folders
  testMatch: ['**/__tests__/**/*.test.js'],
  
  // Collect coverage from source files (not tests)
  collectCoverageFrom: [
    'src/**/*.js',
    '!src/**/*.config.js',
    '!src/**/index.js',
  ],
  
  // Where to put coverage reports
  coverageDirectory: 'coverage',
  coverageReporters: ['text', 'lcov', 'html'],
};
