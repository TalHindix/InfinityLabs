import { validateTransactionAmount } from './src/utils/validation.util.js';

console.log('=== Valid Cases ===');
console.log('100:', validateTransactionAmount(100));
console.log('0.01:', validateTransactionAmount(0.01));
console.log('1000000:', validateTransactionAmount(1000000));
console.log('50.50:', validateTransactionAmount(50.50));

console.log('\n=== Invalid Cases ===');
console.log('null:', validateTransactionAmount(null));
console.log('undefined:', validateTransactionAmount(undefined));
console.log('"100" (string):', validateTransactionAmount("100"));
console.log('true:', validateTransactionAmount(true));
console.log('Infinity:', validateTransactionAmount(Infinity));
console.log('NaN:', validateTransactionAmount(NaN));
console.log('0:', validateTransactionAmount(0));
console.log('-100:', validateTransactionAmount(-100));
console.log('1000001:', validateTransactionAmount(1000001));
console.log('100.999:', validateTransactionAmount(100.999));
