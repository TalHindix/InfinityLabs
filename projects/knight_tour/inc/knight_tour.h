/**************************************
Exercise:   Projects - Knight Tour
Date:       31/8/2025
Developer:  Tal Hindi
Reviewer:   Avi Tobar
Status:     Approved
**************************************/

#ifndef __ILRD__KNIGHT_TOUR_H__
#define __ILRD__KNIGHT_TOUR_H__

#include <time.h> /* time_t */

#define CHESS_LEN (8)

typedef enum knight_tour_status
{
    SUCCESS, /* Knight's tour completed successfully */
    FAIL,    /* Knight's tour failed to find solution */
    TIMEOUT  /* Knight's tour timed out before completion */
} knight_tour_status_e;

typedef struct chess_square
{
    size_t row;    /* Row position on chess board (0-7) */
    size_t column; /* Column position on chess board (0-7) */
} chess_square_t;

/*============================================================================
                                API FUNCTIONS
============================================================================*/

/*
 * KnightTour - Solves knight's tour problem on 8x8 chessboard
 * 
 * Description:
 *   Attempts to find a path where a knight visits every square on the 
 *   chessboard exactly once. Can use Warnsdorf's heuristic for optimization.
 * 
 * Parameters:
 *   board        - 8x8 array to store the move sequence
 *   start_square - Starting position for the knight
 *   is_heuristic - Flag to enable Warnsdorf's heuristic (1=on, 0=off)
 * 
 * Returns:
 *   SUCCESS - Knight's tour completed successfully
 *   FAIL    - No valid tour found from starting position
 *   TIMEOUT - Solution timed out before completion
 * 
 * Time Complexity: O(8^n) without heuristic, O(n) with heuristic
 * Space Complexity: O(n) for recursion stack
 */
knight_tour_status_e KnightTour(size_t board[CHESS_LEN][CHESS_LEN], chess_square_t start_square, int is_heuristic);

/*
 * PrintBoard - Displays the knight's tour solution
 * 
 * Description:
 *   Prints the 8x8 chessboard showing the sequence of knight moves.
 *   Each cell shows the move number (1-64).
 * 
 * Parameters:
 *   board - 8x8 array containing the move sequence
 * 
 * Returns:
 *   None
 * 
 * Time Complexity: O(1) - fixed 8x8 board
 * Space Complexity: O(1)
 */
void PrintBoard(size_t board[CHESS_LEN][CHESS_LEN]);

/*
 * StartTimer - Begins timing measurement
 * 
 * Description:
 *   Records the current time to measure algorithm execution duration.
 *   Use with EndTimer to calculate elapsed time.
 * 
 * Parameters:
 *   None
 * 
 * Returns:
 *   Current time in seconds since epoch
 * 
 * Time Complexity: O(1)
 * Space Complexity: O(1)
 */
time_t StartTimer(void);

/*
 * EndTimer - Calculates elapsed time since StartTimer
 * 
 * Description:
 *   Computes the time difference between start_timer and current time.
 *   Returns duration in seconds.
 * 
 * Parameters:
 *   start_timer - Time returned by StartTimer function
 * 
 * Returns:
 *   Elapsed time in seconds
 * 
 * Time Complexity: O(1)
 * Space Complexity: O(1)
 */
size_t EndTimer(time_t start_timer);

#endif /* __ILRD__KNIGHT_TOUR_H__ */