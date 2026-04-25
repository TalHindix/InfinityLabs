const MONTH_LABELS = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun',
                      'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'];

interface ChartDataPoint {
  label: string;
  spent: number;
  received: number;
}

export function mergeMonthlyData(
  spending: { year: number; month: number; totalSpent: number }[],
  received: { year: number; month: number; totalReceived: number }[],
): ChartDataPoint[] {
  const map = new Map<string, ChartDataPoint>();

  for (const s of spending) {
    const key = `${s.year}-${s.month}`;
    map.set(key, { label: `${MONTH_LABELS[s.month - 1]} ${s.year}`, spent: s.totalSpent, received: 0 });
  }
  for (const r of received) {
    const key = `${r.year}-${r.month}`;
    const existing = map.get(key);
    if (existing) {
      existing.received = r.totalReceived;
    } else {
      map.set(key, { label: `${MONTH_LABELS[r.month - 1]} ${r.year}`, spent: 0, received: r.totalReceived });
    }
  }

  return [...map.entries()]
    .sort(([a], [b]) => {
      const [ay, am] = a.split('-').map(Number);
      const [by, bm] = b.split('-').map(Number);
      return ay !== by ? ay - by : am - bm;
    })
    .map(([, v]) => v);
}
