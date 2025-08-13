/**************************************
Exercise:   Calculator - FSM
Date:       12/8/2025
Developer:  Tal Hindi
Reviewer:   Avi Tobar
Status:     
**************************************/

#include <stddef.h> /* size_t */
#include <stdlib.h> /* strtod */
#include <string.h> /* strlen */
#include <assert.h> /* assert */

#include "stack.h"      /* stack_t */
#include "calculator.h" /* Calculate */

#define STATE_ROWS 3
#define CHAR_SIZE 255

typedef enum event
{
    EV_PLUS,
    EV_MINUS,
    EV_NUM,
    EV_MUL,
    EV_DIV,
    EV_LPAREN,
    EV_RPAREN,
    EV_LBRACKET,
    EV_RBRACKET,
    EV_LBRACE,
    EV_RBRACE,
    EV_SPACE,
    EV_END,
    EV_OTHER,
    EV_COUNT
} event_e;

typedef enum state
{
    WAITING_FOR_NUM = 0,
    WAITING_FOR_OP  = 1,
    AFTER_UNARY = 2
} state_e;

typedef struct calc_data
{
    const char* cursor;
    stack_t* values_stack;
    stack_t* ops_stack;
    int sign;
    int finish;
    double* out;
    calculator_status_t status;
} calc_data_t;

typedef calculator_status_t (*action_func_t)(calc_data_t* ctx);
typedef double (*binary_op_t)(double lhs, double rhs);

typedef struct cell
{
    state_e next_state;
    action_func_t action_func;
} cell_t;

/* ---------- Globals variables ---------- */
static unsigned char g_char_LUT[CHAR_SIZE] = {0};
static unsigned char g_op_prec_LUT[CHAR_SIZE] = {0};
static binary_op_t g_binop_LUT[CHAR_SIZE] = { NULL };

static int g_inited = 0;

static void InitTablesOnce(void);

static void ValueStackPush(calc_data_t* ctx, double v);
static double ValueStackPop(calc_data_t* ctx);

static void OpStackPush(calc_data_t* ctx, char op);
static char OpStackPop(calc_data_t* ctx);

static calculator_status_t ApplyTopOperator(calc_data_t* ctx);
static calculator_status_t ReduceOperators(calc_data_t* ctx, char op);
static calculator_status_t ReduceUntilOpen(calc_data_t* ctx, char op);

static calculator_status_t ActSpace(calc_data_t* ctx);
static calculator_status_t ActUnaryPlus(calc_data_t* ctx);
static calculator_status_t ActUnaryMinus(calc_data_t* ctx);
static calculator_status_t ActPushNumber(calc_data_t* ctx);
static calculator_status_t ActPushBinaryOp(calc_data_t* ctx, char op);
static calculator_status_t ActPlus(calc_data_t* ctx);
static calculator_status_t ActMinus(calc_data_t* ctx);
static calculator_status_t ActMul(calc_data_t* ctx);
static calculator_status_t ActDiv(calc_data_t* ctx);
static calculator_status_t ActEnd(calc_data_t* ctx);
static calculator_status_t ActError(calc_data_t* ctx);
static calculator_status_t ActLParen(calc_data_t* ctx);
static calculator_status_t ActLBracket(calc_data_t* ctx);
static calculator_status_t ActLBrace(calc_data_t* ctx);
static calculator_status_t ActRParen(calc_data_t* ctx);
static calculator_status_t ActRBracket(calc_data_t* ctx);
static calculator_status_t ActRBrace(calc_data_t* ctx);

static calculator_status_t FsmStep(calc_data_t* ctx, state_e* state);
static calculator_status_t InitStacks(calc_data_t* ctx, size_t input_length);

static double OpAdd(double lhs, double rhs) { return lhs + rhs; }
static double OpSub(double lhs, double rhs) { return lhs - rhs; }
static double OpMul(double lhs, double rhs) { return lhs * rhs; }
static double OpDiv(double lhs, double rhs) { return lhs / rhs; }

/* ---------- Transition table ---------- */
static const cell_t g_trans_LUT[STATE_ROWS][EV_COUNT] =
{
/* WAITING_FOR_NUM */
{
/* EV_PLUS   */ { AFTER_UNARY, ActUnaryPlus },
/* EV_MINUS  */ { AFTER_UNARY, ActUnaryMinus },
/* EV_NUM    */ { WAITING_FOR_OP,  ActPushNumber },
/* EV_MUL    */ { WAITING_FOR_NUM, ActError },
/* EV_DIV    */ { WAITING_FOR_NUM, ActError },
/* EV_LPAREN    */ { WAITING_FOR_NUM, ActLParen},
/* EV_RPAREN    */ { WAITING_FOR_NUM, ActError},
/* EV_LBRACKET  */ { WAITING_FOR_NUM, ActLBracket},
/* EV_RBRACKET  */ { WAITING_FOR_NUM, ActError},
/* EV_LBRACE    */ { WAITING_FOR_NUM, ActLBrace},
/* EV_RBRACE    */ { WAITING_FOR_NUM, ActError},
/* EV_SPACE  */ { WAITING_FOR_NUM, ActSpace },
/* EV_END    */ { WAITING_FOR_NUM, ActError },
/* EV_OTHER  */ { WAITING_FOR_NUM, ActError }
},
/* WAITING_FOR_OP */
{
/* EV_PLUS   */ { WAITING_FOR_NUM, ActPlus },
/* EV_MINUS  */ { WAITING_FOR_NUM, ActMinus },
/* EV_NUM    */ { WAITING_FOR_OP,  ActError },
/* EV_MUL    */ { WAITING_FOR_NUM, ActMul },
/* EV_DIV    */ { WAITING_FOR_NUM, ActDiv },
/* EV_LPAREN    */ { WAITING_FOR_NUM, ActError},
/* EV_RPAREN    */ { WAITING_FOR_OP, ActRParen},
/* EV_LBRACKET  */ { WAITING_FOR_NUM, ActError},
/* EV_RBRACKET  */ { WAITING_FOR_OP, ActRBracket},
/* EV_LBRACE    */ { WAITING_FOR_NUM, ActError},
/* EV_RBRACE    */ { WAITING_FOR_OP, ActRBrace},
/* EV_SPACE  */ { WAITING_FOR_OP,  ActSpace },
/* EV_END    */ { WAITING_FOR_OP,  ActEnd },
/* EV_OTHER  */ { WAITING_FOR_OP,  ActError }
},
/* AFTER_UNARY */
{
/* EV_PLUS   */ { AFTER_UNARY,     ActUnaryPlus  },  
/* EV_MINUS  */ { AFTER_UNARY,     ActUnaryMinus },  
/* EV_NUM    */ { WAITING_FOR_OP,  ActPushNumber },
/* EV_MUL    */ { AFTER_UNARY,     ActError      },
/* EV_DIV    */ { AFTER_UNARY,     ActError      },
/* EV_LPAREN    */ { WAITING_FOR_NUM, ActLParen},
/* EV_RPAREN    */ { WAITING_FOR_NUM, ActError},
/* EV_LBRACKET  */ { WAITING_FOR_NUM, ActLBracket},
/* EV_RBRACKET  */ { WAITING_FOR_NUM, ActError},
/* EV_LBRACE    */ { WAITING_FOR_NUM, ActLBrace},
/* EV_RBRACE    */ { WAITING_FOR_NUM, ActError},
/* EV_SPACE  */ { AFTER_UNARY, ActSpace},
/* EV_END    */ { AFTER_UNARY, ActError},
/* EV_OTHER  */ { AFTER_UNARY, ActError}
}
};

calculator_status_t Calculate(const char* expression, double* res)
{
    calc_data_t ctx = {0};
    state_e current_state = WAITING_FOR_NUM;
    size_t input_length = 0;
    calculator_status_t init_stack_status = SUCCESS;

    assert(expression);
    assert(res);

    InitTablesOnce();

    input_length = strlen(expression);
    if (input_length == 0)
    {
        return SYNTAX_ERROR;
    }

    init_stack_status = InitStacks(&ctx, input_length);
    if (init_stack_status != SUCCESS)
    {
        return init_stack_status;
    }

    ctx.cursor = expression;
    ctx.sign = +1;
    ctx.finish = 0;
    ctx.out = res;
    ctx.status = SUCCESS;

    while (!ctx.finish && ctx.status == SUCCESS)
    {
        ctx.status = FsmStep(&ctx, &current_state);
    }

    StackDestroy(ctx.values_stack);
    StackDestroy(ctx.ops_stack);

    return ctx.status;
}

static void InitTablesOnce(void)
{
    size_t i = 0;

    if (g_inited)
    {
        return;
    }

    for (i = 0; i < CHAR_SIZE; ++i)
    {
        g_char_LUT[i] = EV_OTHER;
        g_op_prec_LUT[i] = 0;
        g_binop_LUT[i] = 0;
    }

    g_op_prec_LUT['+'] = 1; 
    g_op_prec_LUT['-'] = 1;
    g_op_prec_LUT['*'] = 2;
    g_op_prec_LUT['/'] = 2;
    
    g_binop_LUT['+'] = OpAdd;
    g_binop_LUT['-'] = OpSub;
    g_binop_LUT['*'] = OpMul;
    g_binop_LUT['/'] = OpDiv;

    g_char_LUT['\0'] = EV_END;
    g_char_LUT['+'] = EV_PLUS;
    g_char_LUT['-'] = EV_MINUS;
    g_char_LUT['*'] = EV_MUL;
    g_char_LUT['/'] = EV_DIV;
    g_char_LUT['(']  = EV_LPAREN;
    g_char_LUT[')']  = EV_RPAREN;
    g_char_LUT['[']  = EV_LBRACKET;
    g_char_LUT[']']  = EV_RBRACKET;
    g_char_LUT['{']  = EV_LBRACE;
    g_char_LUT['}']  = EV_RBRACE;

   
    
    g_char_LUT[' ']  = EV_SPACE;
    g_char_LUT['\t'] = EV_SPACE;
    g_char_LUT['\n'] = EV_SPACE;
    g_char_LUT['\r'] = EV_SPACE;
    g_char_LUT['\v'] = EV_SPACE;
    g_char_LUT['\f'] = EV_SPACE;

    for (i = '0'; i <= '9'; ++i)
    {
        g_char_LUT[i] = EV_NUM;
    }
    g_char_LUT['.'] = EV_NUM;

    g_inited = 1;
}

static void ValueStackPush(calc_data_t* ctx, double value)
{
    StackPush(ctx->values_stack, (const void*)&value);
}

static double ValueStackPop(calc_data_t* ctx)
{
    void* top = NULL;
    double value = 0.0;

    top = StackPeek(ctx->values_stack);

    value = *(double*)top;

    StackPop(ctx->values_stack);

    return value;
}

static void OpStackPush(calc_data_t* ctx, char op)
{
    StackPush(ctx->ops_stack, (const void*)&op);
}

static char OpStackPop(calc_data_t* ctx)
{
    void* top = NULL;
    char op = 0;

    top = StackPeek(ctx->ops_stack);
    op = *(char*)top;
    StackPop(ctx->ops_stack);

    return op;
}

static calculator_status_t ApplyTopOperator(calc_data_t* ctx)
{
    char op = 0;
    double rhs_value = 0.0; /* right-hand operand: popped first */
    double lhs_value = 0.0; /* left-hand operand: popped second */
    double result_value  = 0.0;
    binary_op_t func = {0};

    op = OpStackPop(ctx);
    lhs_value = ValueStackPop(ctx);
    rhs_value = ValueStackPop(ctx);

    func = g_binop_LUT[(unsigned char)op];

    if(!func)
    {
        return SYNTAX_ERROR;
    }
    
    result_value = func(rhs_value, lhs_value);

    ValueStackPush(ctx, result_value);

    return SUCCESS;
}

static calculator_status_t ReduceOperators(calc_data_t* ctx, char op)
{

    while (!StackIsEmpty(ctx->ops_stack))
    {
        char top_op = *(char *)StackPeek(ctx->ops_stack);
        unsigned char top_prec = g_op_prec_LUT[(unsigned char)top_op];
        unsigned char in_prec  = g_op_prec_LUT[(unsigned char)op];

        
        if (top_prec == 0) { break; }

        if (top_prec < in_prec) { break; }
        

        if (ApplyTopOperator(ctx) != SUCCESS)
        {
            return SYNTAX_ERROR;
        }
    }

    if (op == '\0' && !StackIsEmpty(ctx->ops_stack))
    {
        return SYNTAX_ERROR;
    }

    return SUCCESS;
}

static calculator_status_t ReduceUntilOpen(calc_data_t* ctx, char open)
{
    calculator_status_t status = SUCCESS;
    char top_op = 0;

    while (!StackIsEmpty(ctx->ops_stack))
    {
        top_op = 0;

        top_op  = *(char *)StackPeek(ctx->ops_stack);

        if (top_op == open)
        {
            OpStackPop(ctx);
            return SUCCESS;
        }

        if(top_op == '(' || top_op == '[' || top_op == '{')
        {
            return SYNTAX_ERROR;
        }

        status = ApplyTopOperator(ctx);
        if (status != SUCCESS)
        {
            return status;
        }
    }

    return SYNTAX_ERROR;
}
/* ----- actions ----- */
static calculator_status_t ActSpace(calc_data_t* ctx)
{
    ctx->cursor += 1;
    return SUCCESS;
}

static calculator_status_t ActUnaryPlus(calc_data_t* ctx)
{
    ctx->sign *= +1;
    ctx->cursor += 1;
    return SUCCESS;
}

static calculator_status_t ActUnaryMinus(calc_data_t* ctx)
{
    ctx->sign *= -1;
    ctx->cursor += 1;
    return SUCCESS;
}

static calculator_status_t ActPushNumber(calc_data_t* ctx)
{
    char* endp = NULL;
    double value = 0.0;

    value = strtod(ctx->cursor, &endp);
    if (endp == ctx->cursor)
    {
        return SYNTAX_ERROR;
    }

    value *= (double)ctx->sign;

    ValueStackPush(ctx, value);
    
    ctx->sign = +1;

    ctx->cursor = endp;

    return SUCCESS;
}

static calculator_status_t ActPushBinaryOp(calc_data_t* ctx, char op)
{
    calculator_status_t status = SUCCESS;

    status = ReduceOperators(ctx,op);

    OpStackPush(ctx, op);

    ctx->cursor += 1;

    return status;
}

static calculator_status_t ActPlus(calc_data_t* ctx)
{
    return ActPushBinaryOp(ctx, '+');
}

static calculator_status_t ActMinus(calc_data_t* ctx)
{
    return ActPushBinaryOp(ctx, '-');
}

static calculator_status_t ActMul(calc_data_t* ctx)
{
    return ActPushBinaryOp(ctx, '*');
}

static calculator_status_t ActDiv(calc_data_t* ctx)
{
    return ActPushBinaryOp(ctx, '/');
}

static calculator_status_t ActLParen(calc_data_t* ctx)
{
    OpStackPush(ctx, '(');
    ctx->cursor += 1;

    return SUCCESS;
}

static calculator_status_t ActLBracket(calc_data_t* ctx)
{
    OpStackPush(ctx, '[');
    ctx->cursor += 1;

    return SUCCESS;
}

static calculator_status_t ActLBrace(calc_data_t* ctx)
{
    OpStackPush(ctx, '{');
    ctx->cursor += 1;

    return SUCCESS;
}

static calculator_status_t ActRParen(calc_data_t* ctx)
{
    calculator_status_t status = SUCCESS;

    status = ReduceUntilOpen(ctx,'(');
    if (status != SUCCESS)
    {
        return status;
    }

    ctx->cursor += 1;
    return SUCCESS;
}

static calculator_status_t ActRBracket(calc_data_t* ctx)
{
    calculator_status_t status = SUCCESS;

    status = ReduceUntilOpen(ctx,'[');
    if (status != SUCCESS)
    {
        return status;
    }

    ctx->cursor += 1;
    return SUCCESS;
}

static calculator_status_t ActRBrace(calc_data_t* ctx)
{
    calculator_status_t status = SUCCESS;

    status = ReduceUntilOpen(ctx,'{');
    if (status != SUCCESS)
    {
        return status;
    }

    ctx->cursor += 1;
    return SUCCESS;
}

static calculator_status_t ActEnd(calc_data_t* ctx)
{
    calculator_status_t status = SUCCESS;

    status = ReduceOperators(ctx, '\0');
    if (status != SUCCESS)
    {
        return status;
    }

    *(ctx->out) = *(double*)StackPeek(ctx->values_stack);
    ctx->finish = 1;

    return SUCCESS;
}

static calculator_status_t ActError(calc_data_t* ctx)
{
    (void)ctx;
    return SYNTAX_ERROR;
}

static calculator_status_t FsmStep(calc_data_t* ctx, state_e* state)
{
    unsigned char current_char = 0;
    event_e current_event = {0};
    const cell_t* transition = {0};

    current_char = (unsigned char)(*ctx->cursor);
    current_event = g_char_LUT[current_char];
    transition = &g_trans_LUT[*state][current_event];

    *state = transition->next_state;
    return transition->action_func(ctx);
}

static calculator_status_t InitStacks(calc_data_t* ctx, size_t input_length)
{
    calculator_status_t status = SUCCESS;

    ctx->values_stack = StackCreate(input_length + 1, sizeof(double));
    ctx->ops_stack = StackCreate(input_length + 1, sizeof(char));

    if (!ctx->values_stack || !ctx->ops_stack)
    {
        if (ctx->values_stack)
        {
            StackDestroy(ctx->values_stack);
            ctx->values_stack = NULL;
        }
        if (ctx->ops_stack)
        {
            StackDestroy(ctx->ops_stack);
            ctx->ops_stack = NULL;
        }
        status = ALLOC_ERROR;
    }

    return status;
}

