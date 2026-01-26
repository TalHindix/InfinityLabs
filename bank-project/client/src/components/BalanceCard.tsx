import { useNavigate } from 'react-router-dom';
import { Card, CardContent, Typography, Button, Box } from './ui';
import { formatAmount } from '../utils/formatters';
import { balanceCardSx, balanceBadgeSx, primaryButtonSx } from '../styles/dashboard.styles';

interface BalanceCardProps {
  balance: number | undefined;
}

/**
 * Balance display card with transfer button.
 */
export const BalanceCard = ({ balance }: BalanceCardProps) => {
  const navigate = useNavigate();

  return (
    <Card sx={balanceCardSx}>
      <Box sx={balanceBadgeSx}>
        <Typography variant="h4" sx={{ color: '#0D1B2A' }}>AED</Typography>
      </Box>
      <CardContent sx={{ p: 4 }}>
        <Typography variant="body1" sx={{ opacity: 0.8, mb: 1 }}>
          Available Balance
        </Typography>
        <Typography variant="h2" sx={{ fontWeight: 700, mb: 3 }}>
          {balance !== undefined ? formatAmount(balance) : '0.00'}
          <Typography
            component="span"
            variant="h5"
            sx={{ ml: 1, opacity: 0.7, fontWeight: 400 }}
          >
            AED
          </Typography>
        </Typography>
        <Button
          variant="contained"
          onClick={() => navigate('/transfer')}
          sx={primaryButtonSx}
        >
          Transfer Money
        </Button>
      </CardContent>
    </Card>
  );
};
