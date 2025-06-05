#include <stdlib.h>
#include <stdio.h>

int main() {
    int *ptr = malloc(sizeof(int));
    *ptr = 10;
    free(ptr);
    printf("%d\n", *ptr);
    return 0;
}

