#include <stdio.h>
#include "string.h"

int main()
{
    /* =========================
       === Testing StrLen() ===
       ========================= */
    printf("Testing StrLen:\n");

    /* Test 1: Regular word */
    printf("Length of \"Tal\" = %d\n", StrLen("Tal"));  /* expected: 3 */

    /* Test 2: Full name with space (tests white space handling) */
    printf("Length of \"Tal Hindi\" = %d\n", StrLen("Tal Hindi"));  /* expected: 9 */

    /* Test 3: Empty string (tests null terminator right at start) */
    printf("Length of \"\" = %d\n", StrLen(""));  /* expected: 0 */

    /* Test 4: Another regular word */
    printf("Length of \"Hindi\" = %d\n", StrLen("Hindi"));  /* expected: 5 */

    /* ==========================
       === Testing StrCmp() ====
       ========================== */
    printf("\nTesting StrCmp:\n");

    /* Test 1: Identical strings (should return 0) */
    printf("Compare \"Tal\" vs \"Tal\": %d\n", StrCmp("Tal", "Tal"));  /* expected: 0 */

    /* Test 2: Same letters, different case (checks uppercase vs lowercase) */
    printf("Compare \"Tal\" vs \"tal\": %d\n", StrCmp("Tal", "tal"));  /* expected: negative */

    /* Test 3: Identical full names with space (checks multi-word equality) */
    printf("Compare \"Tal Hindi\" vs \"Tal Hindi\": %d\n", StrCmp("Tal Hindi", "Tal Hindi"));  /* expected: 0 */

    /* Test 4: One string has space at the end (tests trailing whitespace difference) */
    printf("Compare \"Tal\" vs \"Tal \": %d\n", StrCmp("Tal", "Tal "));  /* expected: negative */

    /* Test 5: Reverse of test 4 (space in first string) */
    printf("Compare \"Tal \" vs \"Tal\": %d\n", StrCmp("Tal ", "Tal"));  /* expected: positive */

    /* Test 6: Last character is different (i vs a — tests difference at end) */
    printf("Compare \"Hindi\" vs \"Hinda\": %d\n", StrCmp("Hindi", "Hinda"));  /* expected: positive */

    /* Test 7: Last character is different (i vs z — tests difference at end) */
    printf("Compare \"Hindi\" vs \"Hindz\": %d\n", StrCmp("Hindi", "Hindz"));  /* expected: negative */

    /* Test 8: One string is empty (tests comparison with null string) */
    printf("Compare \"Tal\" vs \"\": %d\n", StrCmp("Tal", ""));  /* expected: positive */

    /* Test 9: One string is empty (reverse) (tests comparison with null string) */
    printf("Compare \"\" vs \"Tal\": %d\n", StrCmp("", "Tal"));  /* expected: negative */

    return 0;
}

