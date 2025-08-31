/**************************************
Exercise:   Projects - Knight Tour
Date:       31/8/2025
Developer:  Tal Hindi
Reviewer:   
Status:     
**************************************/

#include <stdio.h> /* printf */

#include "knight_tour.h" /* KnightTour */

knight_tour_status_e KnightTour(size_t board[CHESS_LEN][CHESS_LEN],
                                chess_square_t start_square,
                                int is_heuristic)
{
    return FAIL;
}

void PrintBoard(size_t board[CHESS_LEN][CHESS_LEN])
{
    size_t i = 0;
    size_t j = 0;
    
    printf("\n");
    for (i = 0; i < CHESS_LEN; ++i)
    {
        for (j = 0; j < CHESS_LEN; ++j)
        {
            printf("%lu ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

time_t StartTimer(void)
{
    return 0;
}

size_t EndTimer(time_t start_timer)
{
    return 5;
}