import type { Message, Transaction } from './useChatSocket';

export interface TransactionData {
  text: string;
  transactions: Transaction[];
  summary?: string;
}

// Mirror of the server-side parser in openaiFunctionCall.service.js. The
// server normally extracts the transaction JSON before sending, but if a
// message slips through with raw text+JSON mixed (or arrives via an older
// socket path) we re-parse here so the chat bubble can still render cards.
function findBalancedJson(str: string, startIndex: number): string | null {
  if (startIndex < 0 || str[startIndex] !== '{') return null;
  let depth = 0;
  let inString = false;
  let escape = false;

  for (let i = startIndex; i < str.length; i++) {
    const ch = str[i];
    if (escape) { escape = false; continue; }
    if (ch === '\\' && inString) { escape = true; continue; }
    if (ch === '"') { inString = !inString; continue; }
    if (inString) continue;
    if (ch === '{') depth++;
    else if (ch === '}') {
      depth--;
      if (depth === 0) return str.slice(startIndex, i + 1);
    }
  }
  return null;
}

function extractJsonFromText(text: string): TransactionData | null {
  let pos = 0;

  while (pos < text.length) {
    const start = text.indexOf('{', pos);
    if (start === -1) break;

    const jsonStr = findBalancedJson(text, start);
    if (!jsonStr) break;

    try {
      const parsed = JSON.parse(jsonStr);
      if (parsed && Array.isArray(parsed.transactions)) {
        const before = text.slice(0, start).trim();
        const afterStart = start + jsonStr.length;
        const after = text.slice(afterStart).trim();

        // Extract message from any remaining JSON object (e.g. separate balance response)
        let extraMessage = '';
        if (after) {
          const extraJsonStr = findBalancedJson(after, after.indexOf('{'));
          if (extraJsonStr) {
            try {
              const extraParsed = JSON.parse(extraJsonStr);
              if (extraParsed.message) extraMessage = extraParsed.message;
            } catch { /* ignore */ }
          } else if (!after.startsWith('{')) {
            extraMessage = after;
          }
        }

        const parts = [before, parsed.message, extraMessage].filter(Boolean);
        return { text: parts.join('\n\n'), transactions: parsed.transactions, summary: parsed.summary };
      }
    } catch { /* not valid JSON, skip */ }

    pos = start + (jsonStr ? jsonStr.length : 1);
  }

  return null;
}

export function getTransactionData(msg: Message): TransactionData | null {
  if (msg.transactions) {
    return { text: msg.text, transactions: msg.transactions, summary: msg.summary };
  }
  return extractJsonFromText(msg.text);
}
