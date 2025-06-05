#include <stdlib.h>
#include <stdio.h>

int main() {
    int *arr = malloc(sizeof(int) * 5);
    if (*arr > 0) {
        printf("Positive\n");
    } else {
        printf("Non-positive\n");
    }
    free(arr);
    return 0;
}

