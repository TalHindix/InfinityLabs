import { Box, Typography, Card, CardContent, CircularProgress, Alert } from '../../shared/muiExports';
import { BarChart, Bar, XAxis, YAxis, Tooltip, ResponsiveContainer, CartesianGrid } from 'recharts';
import { useSpendingAnalytics } from './useSpendingAnalytics';
import { formatAmount } from '../../shared/displayFormatters';
import { useThemeContext } from '../../context/ThemeContext';
import {
  chartHeaderSx,
  chartTitleSx,
  chartCardSx,
  chartContainerSx,
  recipientsSectionSx,
  recipientRowSx,
  recipientEmailSx,
  recipientAmountSx,
} from './SpendingChart.styles';

const MONTH_LABELS = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun',
                      'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'];

const GOLD = '#C9A227';

interface ChartDataPoint {
  label: string;
  totalSpent: number;
  transactionCount: number;
}

export const SpendingChart = () => {
  const { monthlySpending, topRecipients, loading, error } = useSpendingAnalytics();
  const { isDark } = useThemeContext();

  if (loading) {
    return (
      <Card sx={chartCardSx}>
        <CardContent sx={{ display: 'flex', justifyContent: 'center', py: 6 }}>
          <CircularProgress sx={{ color: 'primary.main' }} />
        </CardContent>
      </Card>
    );
  }

  if (error) return <Alert severity="error">{error}</Alert>;

  const chartData: ChartDataPoint[] = monthlySpending.map((item) => ({
    label: `${MONTH_LABELS[item.month - 1]} ${item.year}`,
    totalSpent: item.totalSpent,
    transactionCount: item.transactionCount,
  }));

  const gridColor = isDark ? 'rgba(255,255,255,0.1)' : 'rgba(0,0,0,0.1)';
  const tooltipBg = isDark ? '#0D1B2A' : '#FFFFFF';
  const tooltipColor = isDark ? '#fff' : '#0D1B2A';
  const tickColor = isDark ? '#ccc' : '#666';

  return (
    <Box>
      <Box sx={chartHeaderSx}>
        <Typography variant="h5" sx={chartTitleSx}>
          Monthly Spending
        </Typography>
      </Box>

      <Card sx={chartCardSx}>
        <CardContent>
          {chartData.length === 0 ? (
            <Typography color="text.secondary" sx={{ textAlign: 'center', py: 4 }}>
              No spending data available
            </Typography>
          ) : (
            <Box sx={chartContainerSx}>
              <ResponsiveContainer width="100%" height={300}>
                <BarChart data={chartData}>
                  <CartesianGrid strokeDasharray="3 3" stroke={gridColor} />
                  <XAxis dataKey="label" tick={{ fontSize: 12, fill: tickColor }} />
                  <YAxis tick={{ fontSize: 12, fill: tickColor }} tickFormatter={(v) => formatAmount(v)} />
                  <Tooltip
                    formatter={(value: number) => [`${formatAmount(value)} AED`, 'Spent']}
                    labelFormatter={(label) => label}
                    contentStyle={{
                      backgroundColor: tooltipBg,
                      border: `1px solid ${GOLD}`,
                      borderRadius: 8,
                      color: tooltipColor,
                    }}
                  />
                  <Bar dataKey="totalSpent" fill={GOLD} radius={[4, 4, 0, 0]} />
                </BarChart>
              </ResponsiveContainer>
            </Box>
          )}
        </CardContent>
      </Card>

      {topRecipients.length > 0 && (
        <Card sx={{ ...chartCardSx, mt: 2 }}>
          <CardContent>
            <Typography variant="subtitle1" fontWeight={600} sx={{ mb: 1 }}>
              Top Recipients
            </Typography>
            {topRecipients.map((recipient) => (
              <Box key={recipient.email} sx={recipientRowSx}>
                <Typography variant="body2" sx={recipientEmailSx}>
                  {recipient.email}
                </Typography>
                <Typography variant="body2" fontWeight={600} sx={recipientAmountSx}>
                  {formatAmount(recipient.totalSent)} AED
                </Typography>
              </Box>
            ))}
          </CardContent>
        </Card>
      )}
    </Box>
  );
};
