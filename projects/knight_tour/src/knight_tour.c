/**************************************
Exercise:   Projects - Knight Tour
Date:       31/8/2025
Developer:  Tal Hindi
Reviewer:   Avi Tobar
Status:     
**************************************/

#include <stdio.h> /* printf */
#include <stdint.h> /* SIZE_MAX */

#include "knight_tour.h" /* KnightTour */
#include "bit_arr.h" /* BitArrGetBit */

#define TIMEOUT_TIME 300
#define NUM_KNIGHT_MOVES 8
#define NUM_OF_SQUARES (CHESS_LEN * CHESS_LEN)
#define NO_VALID_MOVE 255

/* Pre-computed lookup tables for performance */
static size_t precomputed_moves[NUM_OF_SQUARES][NUM_KNIGHT_MOVES];
static int moves_table_ready = 0;

static const int knight_moves_lut[NUM_KNIGHT_MOVES][2] = {
    {2, 1}, {1, 2}, {-1, 2}, {-2, 1},
    {-2, -1}, {-1, -2}, {1, -2}, {2, -1}
};

typedef struct {
    size_t square_idx;
    size_t accessibility;
} move_t;

/* Function prototypes */
static size_t ConvertSquareToIndex(chess_square_t square);
static chess_square_t ConvertIndexToSquare(size_t idx);
static int IsValidPosition(size_t row, size_t col);
static void PrecomputeValidMoves(void);
static knight_tour_status_e KnightTourRecursive(size_t board[CHESS_LEN][CHESS_LEN], 
                                                 bit_arr_t* visited, 
                                                 size_t current_idx, 
                                                 size_t move_count,
                                                 int use_heuristic,
                                                 time_t start_time);
static knight_tour_status_e TryMoves(size_t board[CHESS_LEN][CHESS_LEN],
                                     bit_arr_t* visited,
                                     size_t current_idx,
                                     size_t move_count,
                                     int use_heuristic,
                                     time_t start_time);
static size_t GetValidMoves(size_t current_idx, bit_arr_t visited, move_t moves[]);
static size_t CountAccessibleMoves(size_t square_idx, bit_arr_t visited);
static void InsertionSortMoves(move_t moves[], size_t count);
static void InitializeBoard(size_t board[CHESS_LEN][CHESS_LEN]);

knight_tour_status_e KnightTour(size_t board[CHESS_LEN][CHESS_LEN],
                                chess_square_t start_square,
                                int is_heuristic)
{
    bit_arr_t visited = 0;
    time_t start_time = 0;
    knight_tour_status_e result = FAIL;
    size_t start_idx = 0;
    chess_square_t square = {0};
    
    if (0 == IsValidPosition(start_square.row, start_square.column))
    {
        return FAIL;
    }

    /* Initialize lookup table once */
    if (!moves_table_ready)
    {
        PrecomputeValidMoves();
        moves_table_ready = 1;
    }

    InitializeBoard(board);
    start_idx = ConvertSquareToIndex(start_square);
    
    /* Set starting position */
    square = ConvertIndexToSquare(start_idx);
    board[square.row][square.column] = 1;   
    visited = BitArrSetOn(0, start_idx);

    start_time = StartTimer();

    result = KnightTourRecursive(board, &visited, start_idx, 2, 
                                 is_heuristic, start_time);
    
    switch (result)
    {
        case SUCCESS:
            printf("Knight's Tour completed successfully%s!\n", 
                   is_heuristic ? " using Warnsdorff's heuristic" : "");
            break;
        case TIMEOUT:
            printf("Knight's Tour timed out after %d seconds\n", TIMEOUT_TIME);
            break;
        default:
            printf("No solution found%s\n", 
                   is_heuristic ? " using heuristic" : "");
            break;
    }
    
    printf("Time elapsed: %lu seconds\n", EndTimer(start_time));
    
    return result;
}

void PrintBoard(size_t board[CHESS_LEN][CHESS_LEN])
{
    size_t i = 0;
    size_t j = 0;
    
    printf("\n");
    for (i = 0; i < CHESS_LEN; i++)
    {
        for (j = 0; j < CHESS_LEN; j++)
        {
            printf("%2lu ", board[i][j]);
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

/* Enhanced indexing system for better performance */
static size_t ConvertSquareToIndex(chess_square_t square)
{
    return (CHESS_LEN * square.row + square.column);
}

static chess_square_t ConvertIndexToSquare(size_t idx)
{
    chess_square_t square = {0};
    square.column = idx % CHESS_LEN;
    square.row = idx / CHESS_LEN;
    return square;
}


/* Precompute all valid moves for performance optimization */
static void PrecomputeValidMoves(void)
{
    size_t i = 0;
    size_t j = 0;
    chess_square_t current_square = {0};
    int new_row = 0;
    int new_col = 0;
    chess_square_t next_square = {0};
    
    for (i = 0; i < NUM_OF_SQUARES; i++)
    {
        current_square = ConvertIndexToSquare(i);
        
        for (j = 0; j < NUM_KNIGHT_MOVES; j++)
        {
            new_row = (int)current_square.row + knight_moves_lut[j][0];
            new_col = (int)current_square.column + knight_moves_lut[j][1];
            
            if (IsValidPosition((size_t)new_row, (size_t)new_col) && 
                new_row >= 0 && new_col >= 0)
            {
                next_square.row = (size_t)new_row;
                next_square.column = (size_t)new_col;
                precomputed_moves[i][j] = ConvertSquareToIndex(next_square);
            }
            else
            {
                precomputed_moves[i][j] = NO_VALID_MOVE;
            }
        }
    }
}

static knight_tour_status_e KnightTourRecursive(size_t board[CHESS_LEN][CHESS_LEN], 
                                                 bit_arr_t* visited, 
                                                 size_t current_idx, 
                                                 size_t move_count,
                                                 int use_heuristic,
                                                 time_t start_time)
{
    if (EndTimer(start_time) > TIMEOUT_TIME)
    {
        return TIMEOUT;
    }

    /* Menny's cleaner base case */
    if (move_count > NUM_OF_SQUARES)
    {
        return SUCCESS;
    }
    
    return TryMoves(board, visited, current_idx, move_count, use_heuristic, start_time);
}

/* Dynamic accessibility counting with precomputed moves */
static size_t CountAccessibleMoves(size_t square_idx, bit_arr_t visited)
{
    size_t count = 0;
    size_t i = 0;
    
    for (i = 0; i < NUM_KNIGHT_MOVES; i++)
    {
        size_t next_idx = precomputed_moves[square_idx][i];
        
        if (next_idx != NO_VALID_MOVE && 0 == BitArrGetBit(visited, next_idx))
        {
            count++;
        }
    }
    
    return count;
}

static int IsValidPosition(size_t row, size_t col)
{
    return (row < CHESS_LEN && col < CHESS_LEN && 
            SIZE_MAX != row && SIZE_MAX != col);
}

static void InitializeBoard(size_t board[CHESS_LEN][CHESS_LEN])
{
    size_t i = 0;
    size_t j = 0;
    
    for (i = 0; i < CHESS_LEN; i++)
    {
        for (j = 0; j < CHESS_LEN; j++)
        {
            board[i][j] = 0;
        }
    }
}

/* Fast move generation using precomputed table */
static size_t GetValidMoves(size_t current_idx, bit_arr_t visited, move_t moves[])
{
    size_t valid_moves = 0;
    size_t i = 0;
    
    for (i = 0; i < NUM_KNIGHT_MOVES; i++)
    {
        size_t next_idx = precomputed_moves[current_idx][i];
        
        if (next_idx != NO_VALID_MOVE && 0 == BitArrGetBit(visited, next_idx))
        {
            moves[valid_moves].square_idx = next_idx;
            moves[valid_moves].accessibility = 0;  /* Will be set in heuristic */
            valid_moves++;
        }
    }
    
    return valid_moves;
}

static void InsertionSortMoves(move_t moves[], size_t count)
{
    size_t i = 0;
    size_t j = 0;
    move_t key = {0, 0};
    
    for (i = 1; i < count; i++)
    {
        key = moves[i];
        j = i;
        
        while (j > 0 && moves[j - 1].accessibility > key.accessibility)
        {
            moves[j] = moves[j - 1];
            j--;
        }
        
        moves[j] = key;
    }
}

static knight_tour_status_e TryMoves(size_t board[CHESS_LEN][CHESS_LEN],
                                     bit_arr_t* visited,
                                     size_t current_idx,
                                     size_t move_count,
                                     int use_heuristic,
                                     time_t start_time)
{
    move_t moves[NUM_KNIGHT_MOVES] = {0};
    size_t valid_moves = GetValidMoves(current_idx, *visited, moves);
    size_t i = 0;
    knight_tour_status_e result = FAIL;
    
    /* Dynamic Warnsdorff's heuristic with performance optimization */
    if (use_heuristic)
    {
        for (i = 0; i < valid_moves; i++)
        {
            *visited = BitArrSetOn(*visited, moves[i].square_idx);
            moves[i].accessibility = CountAccessibleMoves(moves[i].square_idx, *visited);
            *visited = BitArrSetOff(*visited, moves[i].square_idx);
        }
        
        InsertionSortMoves(moves, valid_moves);
    }
    
    /* Try each valid move */
    for (i = 0; i < valid_moves; i++)
    {
        chess_square_t square = ConvertIndexToSquare(moves[i].square_idx);
        
        board[square.row][square.column] = move_count;
        *visited = BitArrSetOn(*visited, moves[i].square_idx);
        
        result = KnightTourRecursive(board, visited, moves[i].square_idx, 
                                     move_count + 1, use_heuristic, start_time);
        
        if (FAIL != result)
        {
            return result;
        }
        
        /* Backtrack */
        board[square.row][square.column] = 0;
        *visited = BitArrSetOff(*visited, moves[i].square_idx);
    }
    
    return FAIL;
}