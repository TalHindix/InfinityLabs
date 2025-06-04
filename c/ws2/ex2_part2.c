#include <stdio.h>
#include <string.h>
#include <assert.h>


int IsPalindrome(const char *str, size_t size)
{
	const char *start = NULL;
	const char *end = NULL;
	
	assert(str != NULL);
	
	if (0 == size)
	{
		return 0;
	}
	
	start = str;
	end = str + size - 1;
	
	while (start < end)
	{
		if(*start != *end)
		{
			return 0;
		}		
		start++;
		end--;	
	}
	return 1;
}

void TestIsPalindrome(const char *str)
{
	int result = IsPalindrome(str, strlen(str));

	if (1 == result)
	{
		printf("Testing \"%s\": PASS (Palindrome)\n", str);
	}
	else
	{
		printf("Testing \"%s\": FAIL (Not a palindrome)\n", str);
	}
}

int main()
{
	TestIsPalindrome("abba");
	TestIsPalindrome("madam");
	TestIsPalindrome("abcba");
	TestIsPalindrome("abcd");
	TestIsPalindrome("");
	TestIsPalindrome("a");
	TestIsPalindrome("aa");
	TestIsPalindrome("ab");
	TestIsPalindrome("!@##@!");
	
	return 0;
}
