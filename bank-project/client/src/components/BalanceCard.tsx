import { useNavigate } from 'react-router-dom';
import { ROUTES } from '../constants/routes';
import { Card, CardContent, Typography, Button, Box } from '../utils/ui';
import { formatAmount } from '../utils/formatters';
import {
  balanceCardSx,
  balanceBadgeSx,
  badgeTextSx,
  cardContentSx,
  availableLabelSx,
  amountValueSx,
  amountCurrencySx,
  primaryButtonSx,
} from './BalanceCard.styles';

interface BalanceCardProps {
  balance: number | undefined;
}

export const BalanceCard = ({ balance }: BalanceCardProps) => {
  const navigate = useNavigate();

  return (
    <Card sx={balanceCardSx}>
      <Box sx={balanceBadgeSx}>
        <Typography variant="h4" sx={badgeTextSx}>
          AED
        </Typography>
      </Box>

      <CardContent sx={cardContentSx}>
        <Typography variant="body1" sx={availableLabelSx}>
          Available Balance
        </Typography>

        <Typography variant="h2" sx={amountValueSx}>
          {balance !== undefined ? formatAmount(balance) : '0.00'}
          <Typography component="span" variant="h5" sx={amountCurrencySx}>
            AED
          </Typography>
        </Typography>

        <Button variant="contained" onClick={() => navigate(ROUTES.TRANSFER)} sx={primaryButtonSx}>
          Transfer Money
        </Button>
      </CardContent>
    </Card>
  );
};
