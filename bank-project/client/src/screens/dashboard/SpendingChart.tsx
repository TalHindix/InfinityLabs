import { Box, Typography, CircularProgress, Alert, Button } from '../../shared/muiExports';
import { BarChart, Bar, XAxis, YAxis, Tooltip, ResponsiveContainer, CartesianGrid } from 'recharts';
import { useSpendingAnalytics } from './useSpendingAnalytics';
import { formatAmount } from '../../shared/displayFormatters';
import { mergeMonthlyData } from './chartUtils';
import {
  dashboardRootSx,
  headerRowSx,
  titleSx,
  filterGroupSx,
  filterBtnSx,
  summaryGridSx,
  summaryCardSx,
  summaryLabelSx,
  summaryValueSx,
  chartSectionSx,
  chartTitleSx,
  legendRowSx,
  legendItemSx,
  legendDotSx,
  legendTextSx,
  recipientsSectionSx,
  recipientsTitleSx,
  recipientRowSx,
  rankSx,
  avatarSx,
  recipientInfoSx,
  recipientEmailSx,
  recipientCountSx,
  progressBarContainerSx,
  progressBarFillSx,
  recipientAmountSx,
  loadingContainerSx,
  emptyTextSx,
  COLORS,
} from './SpendingChart.styles';

const FILTER_OPTIONS = [
  { label: '3M', value: 3 },
  { label: '6M', value: 6 },
  { label: '12M', value: 12 },
];

function getInitials(email: string): string {
  const name = email.split('@')[0];
  const parts = name.split(/[._-]/);
  if (parts.length >= 2) return (parts[0][0] + parts[1][0]).toUpperCase();
  return name.slice(0, 2).toUpperCase();
}

export const SpendingChart = () => {
  const {
    months, setMonths,
    monthlySpending, monthlyReceived, topRecipients,
    totalSpent, totalReceived,
    loading, error,
  } = useSpendingAnalytics();

  if (loading) {
    return (
      <Box sx={loadingContainerSx}>
        <CircularProgress sx={{ color: COLORS.GOLD }} />
      </Box>
    );
  }

  if (error) return <Alert severity="error">{error}</Alert>;

  const chartData = mergeMonthlyData(monthlySpending, monthlyReceived);
  const netBalance = totalReceived - totalSpent;
  const netColor = netBalance >= 0 ? COLORS.GREEN : COLORS.RED;
  const maxRecipientAmount = topRecipients.length > 0 ? topRecipients[0].totalSent : 1;

  return (
    <Box sx={dashboardRootSx}>
      {/* Header + Filters */}
      <Box sx={headerRowSx}>
        <Typography sx={titleSx}>Spending Analytics</Typography>
        <Box sx={filterGroupSx}>
          {FILTER_OPTIONS.map((opt) => (
            <Button
              key={opt.value}
              size="small"
              sx={filterBtnSx(months === opt.value)}
              onClick={() => setMonths(opt.value)}
            >
              {opt.label}
            </Button>
          ))}
        </Box>
      </Box>

      {/* Summary Cards */}
      <Box sx={summaryGridSx}>
        <Box sx={summaryCardSx(COLORS.RED)}>
          <Typography sx={summaryLabelSx}>Total Spent</Typography>
          <Typography sx={summaryValueSx(COLORS.RED)}>
            {formatAmount(totalSpent)} AED
          </Typography>
        </Box>
        <Box sx={summaryCardSx(COLORS.GREEN)}>
          <Typography sx={summaryLabelSx}>Total Received</Typography>
          <Typography sx={summaryValueSx(COLORS.GREEN)}>
            {formatAmount(totalReceived)} AED
          </Typography>
        </Box>
        <Box sx={summaryCardSx(netColor)}>
          <Typography sx={summaryLabelSx}>Net Balance</Typography>
          <Typography sx={summaryValueSx(netColor)}>
            {netBalance >= 0 ? '+' : ''}{formatAmount(netBalance)} AED
          </Typography>
        </Box>
      </Box>

      {/* Bar Chart */}
      <Box sx={chartSectionSx}>
        <Box sx={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', flexWrap: 'wrap', gap: 1 }}>
          <Typography sx={chartTitleSx}>Monthly Overview</Typography>
          <Box sx={legendRowSx}>
            <Box sx={legendItemSx}>
              <Box sx={legendDotSx(COLORS.RED)} />
              <Typography sx={legendTextSx}>Spent</Typography>
            </Box>
            <Box sx={legendItemSx}>
              <Box sx={legendDotSx(COLORS.GREEN)} />
              <Typography sx={legendTextSx}>Received</Typography>
            </Box>
          </Box>
        </Box>

        {chartData.length === 0 ? (
          <Typography sx={emptyTextSx}>No transaction data available</Typography>
        ) : (
          <ResponsiveContainer width="100%" height={280}>
            <BarChart data={chartData} barGap={2} barCategoryGap="20%">
              <CartesianGrid strokeDasharray="3 3" stroke="rgba(255,255,255,0.06)" vertical={false} />
              <XAxis
                dataKey="label"
                tick={{ fontSize: 11, fill: 'rgba(255,255,255,0.4)' }}
                axisLine={{ stroke: 'rgba(255,255,255,0.08)' }}
                tickLine={false}
              />
              <YAxis
                tick={{ fontSize: 11, fill: 'rgba(255,255,255,0.4)' }}
                axisLine={false}
                tickLine={false}
                tickFormatter={(v) => formatAmount(v)}
              />
              <Tooltip
                formatter={(value) => [`${formatAmount(Number(value))} AED`]}
                contentStyle={{
                  backgroundColor: COLORS.NAVY,
                  border: `1px solid ${COLORS.GOLD}`,
                  borderRadius: 8,
                  color: '#fff',
                  fontSize: 13,
                }}
                cursor={{ fill: 'rgba(255,255,255,0.04)' }}
              />
              <Bar dataKey="spent" name="Spent" fill={COLORS.RED} radius={[4, 4, 0, 0]} />
              <Bar dataKey="received" name="Received" fill={COLORS.GREEN} radius={[4, 4, 0, 0]} />
            </BarChart>
          </ResponsiveContainer>
        )}
      </Box>

      {/* Top Recipients */}
      {topRecipients.length > 0 && (
        <Box sx={recipientsSectionSx}>
          <Typography sx={recipientsTitleSx}>Top Recipients</Typography>
          {topRecipients.map((recipient, index) => (
            <Box key={recipient.email} sx={recipientRowSx}>
              <Typography sx={rankSx}>{index + 1}</Typography>
              <Box sx={avatarSx}>{getInitials(recipient.email)}</Box>
              <Box sx={recipientInfoSx}>
                <Typography sx={recipientEmailSx}>{recipient.email}</Typography>
                <Typography sx={recipientCountSx}>
                  {recipient.transactionCount} transfer{recipient.transactionCount !== 1 ? 's' : ''}
                </Typography>
              </Box>
              <Box sx={progressBarContainerSx}>
                <Box sx={progressBarFillSx((recipient.totalSent / maxRecipientAmount) * 100)} />
              </Box>
              <Typography sx={recipientAmountSx}>
                {formatAmount(recipient.totalSent)} AED
              </Typography>
            </Box>
          ))}
        </Box>
      )}
    </Box>
  );
};
