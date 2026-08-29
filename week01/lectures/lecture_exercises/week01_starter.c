#include <stddef.h>
#include <stdio.h>

int sum_positive_even(const int values[], size_t size) {
    /* TODO: preserve the sum of positive even values seen so far. */
    (void)values;
    (void)size;
    return 0;
}

int main(void) {
    const int values[] = {3, 4, -2, 7, 8, 0};
    printf("%d\n", sum_positive_even(values, 6));
    return 0;
}
