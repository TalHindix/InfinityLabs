#include <stdlib.h>

int main() {
    int *ptr = malloc(sizeof(int) * 10);
    ptr[0] = 42;
    return 0;
}

