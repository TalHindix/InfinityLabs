/**************************************
Exercise:   Projects - Knight Tour
Date:       31/8/2025
Developer:  Tal Hindi
Reviewer:   
Status:     
**************************************/

#ifndef __ILRD__KNIGHT_TOUR_H__
#define __ILRD__KNIGHT_TOUR_H__

#include <time.h> /* time_t */

#define CHESS_LEN (8)

typedef enum knight_tour_status
{
    SUCCESS,
    FAIL,
    TIMEOUT
} knight_tour_status_e;

typedef struct chess_square
{
    size_t row;
    size_t column;
} chess_square_t;

knight_tour_status_e KnightTour(size_t board[CHESS_LEN][CHESS_LEN], chess_square_t start_square, int is_heuristic);

void PrintBoard(size_t board[CHESS_LEN][CHESS_LEN]);
time_t StartTimer(void);
size_t EndTimer(time_t start_timer);

#endif /* __ILRD__KNIGHT_TOUR_H__ */