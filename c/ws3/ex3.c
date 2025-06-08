#include <stdio.h> /* printf() */

enum week_day { SUN };      

int main()
{
    printf("char: %lu\n",            (unsigned long)sizeof(char));
    printf("signed char: %lu\n",     (unsigned long)sizeof(signed char));
    printf("unsigned char: %lu\n",   (unsigned long)sizeof(unsigned char));

    printf("short: %lu\n",           (unsigned long)sizeof(short));
    printf("unsigned short: %lu\n",  (unsigned long)sizeof(unsigned short));

    printf("int: %lu\n",             (unsigned long)sizeof(int));
    printf("unsigned int: %lu\n",    (unsigned long)sizeof(unsigned int));

    printf("long: %lu\n",            (unsigned long)sizeof(long));
    printf("unsigned long: %lu\n",   (unsigned long)sizeof(unsigned long));

    printf("float: %lu\n",           (unsigned long)sizeof(float));
    printf("double: %lu\n",          (unsigned long)sizeof(double));
    printf("long double: %lu\n",     (unsigned long)sizeof(long double));

    printf("enum: %lu\n",            (unsigned long)sizeof(enum week_day));

    printf("void *: %lu\n",          (unsigned long)sizeof(void *));

    return 0;
}

