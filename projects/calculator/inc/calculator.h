/**************************************
Exercise:   Calculator - FSM
Date:       12/8/2025
Developer:  Tal Hindi
Reviewer:   Avi Tobar
Status:     
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

calculator_status_t Calculate(const char* expression, double* res);

#endif /* _ILRD_CALC_H_ */