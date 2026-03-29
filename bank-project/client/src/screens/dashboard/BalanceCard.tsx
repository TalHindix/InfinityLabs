import { useNavigate } from 'react-router-dom';
import { ROUTES } from '../../constants/routePaths';
import { Card, CardContent, Typography, Button, Box } from '../../shared/muiExports';
import { formatAmount } from '../../shared/displayFormatters';
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
        <Typography sx={{ ...badgeTextSx, fontSize: { xs: '1.2rem', sm: '2.125rem' } }}>
          AED
        </Typography>
      </Box>

      <CardContent sx={cardContentSx}>
        <Typography variant="body1" sx={availableLabelSx}>
          Available Balance
        </Typography>

        <Typography sx={{ ...amountValueSx, fontSize: { xs: '1.75rem', sm: '3.75rem' } }}>
          {balance !== undefined ? formatAmount(balance) : '0.00'}
          <Typography component="span" sx={{ ...amountCurrencySx, fontSize: { xs: '1rem', sm: '1.5rem' } }}>
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
