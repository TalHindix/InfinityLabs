/**************************************
Exercise:   Calculator - FSM
Date:       12/8/2025
Developer:  Tal Hindi
Reviewer:   Ben Dabush
Status:     Approved
**************************************/

#ifndef _ILRD_CALC_H_
#define _ILRD_CALC_H_

typedef enum status
{
    SUCCESS,
    MATH_ERROR,
    SYNTAX_ERROR,
    ALLOC_ERROR
} calculator_status_t;


/**
 * @brief Evaluates a mathematical expression
 * 
 * Parses and calculates the result of a mathematical expression string.
 * Supports basic arithmetic operations (+, -, *, /, ^) with proper
 * operator precedence and associativity. Handles parentheses, brackets,
 * and braces for grouping operations.
 * 
 * @param expression Null-terminated string containing the mathematical expression
 * @param res Pointer to store the calculated result
 * 
 * @return calculator_status_t indicating success or type of error:
 *         - SUCCESS: Calculation completed successfully
 *         - MATH_ERROR: Mathematical error (e.g., division by zero)
 *         - SYNTAX_ERROR: Invalid expression syntax
 *         - ALLOC_ERROR: Memory allocation failed
 */
calculator_status_t Calculate(const char* expression, double* res);

#endif /* _ILRD_CALC_H_ */