/**************************************
Exercise:   Projects - Knight Tour
Date:       31/8/2025
Developer:  Tal Hindi
Reviewer:   Avi Tobar
Status:     
**************************************/

#include <stdio.h> /* printf */

#include "knight_tour.h" /* KnightTour */


static void TestPosition(chess_square_t pos, int use_heuristic)
{
    size_t board[CHESS_LEN][CHESS_LEN] = {0};
    knight_tour_status_e result = FAIL;
    
    printf("Testing knight tour from position (%lu,%lu) %s\n", 
           pos.row, pos.column,
           use_heuristic ? "with Warnsdorff's heuristic" : "with backtracking");
    
    result = KnightTour(board, pos, use_heuristic);
    
    if (result == SUCCESS)
    {
        printf("SUCCESS: Knight tour completed from (%lu,%lu)\n", 
               pos.row, pos.column);
        PrintBoard(board);
    }
    else if (result == TIMEOUT)
    {
        printf("TIMEOUT: Knight tour timed out from (%lu,%lu)\n", 
               pos.row, pos.column);
    }
    else
    {
        printf("FAILED: No solution found from (%lu,%lu)\n", 
               pos.row, pos.column);
    }
    printf("----------------------------------------\n\n");
}

int main(void)
{
    chess_square_t test_positions[] = {
        {0, 0},  /* Corner */
        {0, 7},  /* Corner */
        {7, 0},  /* Corner */
        {7, 7},  /* Corner */
        {3, 3},  /* Center */
        {5, 1},  /* Original test position */
        {8, 8},  /* Invalid position - should fail validation */
        {2, 6}   /* Edge */
    };
    size_t num_positions = sizeof(test_positions) / sizeof(test_positions[0]);
    size_t i = 0;
    
    printf("Starting Work On KNIGHT_TOUR PROBLEM~ \n\n");
    
    /* Test a few positions with Warnsdorff's heuristic */
    printf("=== Testing with Warnsdorff's Heuristic ===\n\n");
    for (i = 0; i < 3 && i < num_positions; i++)
    {
        TestPosition(test_positions[i], 1);
    }
    
    printf("=== Testing with Backtracking (first position only) ===\n\n");
    TestPosition(test_positions[0], 0);
    
    return 0;
}