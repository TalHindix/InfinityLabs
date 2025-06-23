#include <stdio.h>
#include "ex.h"


void test_atoi(const char *s)
{
	int sys_atoi = atoi(s);
    int my_atoi  = AtoiBase(s, 10);
		
	if (NULL == s || '\0' == s[0]) 
	{
        printf("Test for: <empty string>\n");
        printf("Skipping: input is empty or null\n");
        return;
    }
    
  
    printf("Test for: \"%s\"\n", s);
    printf("atoi: %d\n", sys_atoi);
    printf("AtoiBase: %d\n", my_atoi);

    if (sys_atoi != my_atoi) 
    {
        printf("Error: Values do not match!\n");
    }
    else
    {
        printf("Success: Values match.\n");
    }
}

void run_edge_case_tests() {
    test_atoi("");                 /* Empty string */
    test_atoi("    ");             /* Whitespace only */
    test_atoi("0");                /* Zero */
    test_atoi("-0");               /* Negative zero */
    test_atoi("2147483647");       /* INT_MAX */
    test_atoi("-2147483648");      /* INT_MIN */
    test_atoi("123sssz");           /* Digits followed by letters */
    test_atoi("dsz123");           /* Letters before digits */
    test_atoi("   +316");          /* Leading spaces and plus sign */
    test_atoi("--159");            /* Invalid format */
}



int main()
{
    char buf[12];
    int nums[] = {0, 123, -456, 2147483647, -2147483648};    
    const char *stringNumbers[] = {"0", "123", "-456", " 42", " -99", "abd", " +0 ", NULL, " -001"};
    const char *baseTests[] = {"1010", "77", "1F", "2A", "abcd", "ZZ", "-1F", "  +10", "-001"}; 
    int bases[] = {2, 8, 16, 16, 16, 36, 16, 10, 8};  /* matching bases for each input above */

    int count = sizeof(nums) / sizeof(nums[0]);
    int count_strings = sizeof(stringNumbers) / sizeof(stringNumbers[0]);
    int count_base_tests = sizeof(baseTests) / sizeof(baseTests[0]);

    int i;

    printf("XXX itoa10 Tests XXX\n");
    for (i = 0; i < count; ++i) 
    {
        printf("itoa10(%d) -> %s\n", nums[i], Itoa10(nums[i], buf));
    }

    printf("\n===== atoi10 Tests =====\n");
    for (i = 0; i < count_strings; ++i) 
    {
        int temp = Atoi10(stringNumbers[i]);
        printf("atoi10(\"%s\") -> %d\n", stringNumbers[i], temp);
    }

    printf("\n@@@@ AtoiBase Tests @@@@\n");
    for (i = 0; i < count_base_tests; ++i)
    {
        long val = AtoiBase(baseTests[i], bases[i]);
        printf("AtoiBase(\"%s\", base %d) -> %ld\n", baseTests[i], bases[i], val);
    }
    
    printf("\n@@@@ Exerice 1: D @@@@\n");
    test_atoi("12345");
    
    printf("\n@@@@ Exerice 1: E @@@@\n");
    run_edge_case_tests();

    return 0;
}
