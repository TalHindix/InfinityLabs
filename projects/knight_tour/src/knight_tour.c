/**************************************
Exercise:   Projects - Knight Tour
Date:       31/8/2025
Developer:  Tal Hindi
Reviewer:   
Status:     
**************************************/

#include <stdio.h> /* printf */

#include "bit_arr.h"
#include "knight_tour.h" /* KnightTour */

typedef unsigned long bitboard_t; /* 64 bit */

int knight_moves_lut[8][2] = {
    {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
    {1, -2}, {1, 2}, {2, -1}, {2, 1}
};

static void BitToCoordinate(int bit, chess_square_t pos);
static size_t CoordinateToBit(chess_square_t pos);
static int IsValidBitPos(int bit);

knight_tour_status_e KnightTour(size_t board[CHESS_LEN][CHESS_LEN],
                                chess_square_t start_square,
                                int is_heuristic)
{
    start_square.column = 0;
    start_square.row = 0;

    if(is_heuristic == 0) /* Brute */
    {
       

    }
    else /* */
    {
        return  SUCCESS;
    }
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
    return time(NULL);
}

size_t EndTimer(time_t start_timer)
{
    return (size_t)(time(NULL) - start_timer);
}

static void BitToCoordinate(int bit, chess_square_t pos)
{
    pos.row = bit / 8;
    pos.column = bit % 8;
}

static size_t CoordinateToBit(chess_square_t pos)
{
    return pos.row * 8 + pos.column;
}

static int IsValidBitPos(int bit)
{
    return bit >= 0 && bit <= 64;
}