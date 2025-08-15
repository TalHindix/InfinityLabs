
/**************************************
Exercise:   Calculator - FSM
Date:       12/8/2025
Developer:  Tal Hindi
Reviewer:   Ben Dabush
Status:     
**************************************/

#include <stdio.h>   /* printf          */
#include <assert.h>  /* assert          */

#include "calculator.h" /* Calculate */

#define CHECK(cond, msg)                                          \
    do                                                            \
    {                                                             \
        if (cond)                                                 \
        {                                                         \
            ++g_pass;                                             \
            printf("PASS: %s\n", msg);                            \
        }                                                         \
        else                                                      \
        {                                                         \
            ++g_fail;                                             \
            printf("FAIL: %s (line %d)\n", msg, __LINE__);        \
        }                                                         \
    } while (0)


static unsigned g_pass = 0;
static unsigned g_fail = 0;


typedef struct test_case_t
{
    const char* expr;
    double expected;
    calculator_status_t exp_stat;
} test_case_t;


static void RunOneTest(const test_case_t* tc);

static void TestBasicMath(void);
static void TestParentheses(void);
static void TestUnarySigns(void);
static void TestSyntaxErrors(void);

int main(void)
{
    printf("@@= Calculator Unit Tests =@@\n");

    TestBasicMath();
    TestParentheses();
    TestUnarySigns();
    TestSyntaxErrors();

    printf("\nTotal: PASS=%u, FAIL=%u\n", g_pass, g_fail);
    return (g_fail == 0) ? 0 : 1;
}

static void TestBasicMath(void)
{
    static test_case_t cases[] =
    {
        { "(2^3^1)+2*5", 18.0, SUCCESS },
        { "[(7-4)+2]", 5.0, SUCCESS },
        { "2*(3+4*(5-2))", 30.0, SUCCESS },
        { "8/0", 0.0, MATH_ERROR }
    };

    size_t i = 0;
    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i)
    {
        RunOneTest(&cases[i]);
    }
}

static void TestParentheses(void)
{
    static test_case_t cases[] =
    {
        { "(1+2)*3", 9.0, SUCCESS },
        { "4*(5+1)", 24.0, SUCCESS },
        { "[2+3]*2", 10.0, SUCCESS },
        { "{6/2}+1", 4.0, SUCCESS }
    };

    size_t i = 0;
    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i)
    {
        RunOneTest(&cases[i]);
    }
}

static void TestUnarySigns(void)
{
    static test_case_t cases[] =
    {
        { "-5", -5.0, SUCCESS },
        { "+7", 7.0, SUCCESS },
        { "--5", 5.0, SUCCESS },
        { "1+-2+3", 2.0, SUCCESS },
        { "-3*+5", -15.0, SUCCESS }

    };

    size_t i = 0;
    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i)
    {
        RunOneTest(&cases[i]);
    }
}

static void TestSyntaxErrors(void)
{
    static test_case_t cases[] =
    {
        { "", 0.0, SYNTAX_ERROR },
        { "   ", 0.0, SYNTAX_ERROR },
        { "1+", 0.0, SYNTAX_ERROR },
        { "abc", 0.0, SYNTAX_ERROR },
        { "5*(2", 0.0, SYNTAX_ERROR },
        { "^2", 0.0, SYNTAX_ERROR },
        {"-5^0.5",0.0,MATH_ERROR},
        {"0^0",0.0, MATH_ERROR}
    };

    size_t i = 0;
    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i)
    {
        RunOneTest(&cases[i]);
    }
}

static void RunOneTest(const test_case_t* tc)
{
    calculator_status_t status = SUCCESS;
    double result = 0.0;

    assert(tc);

    printf("[CHECK] \"%s\"\n", tc->expr);

    status = Calculate(tc->expr, &result);

    CHECK(status == tc->exp_stat, "Status matches expectation");

    if (status == SUCCESS)
    {
        CHECK(result == tc->expected, "Value matches expectation");
    }
}
