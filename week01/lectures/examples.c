#include <stddef.h>
#include <stdio.h>

static int sum_positive_even(const int values[], size_t size) {
    int sum = 0;
    for (size_t i = 0; i < size; ++i) {
        if (values[i] > 0 && values[i] % 2 == 0) {
            sum += values[i];
        }
    }
    return sum;
}

int main(void) {
    const int values[] = {3, 4, -2, 7, 8, 0};
    const size_t size = sizeof values / sizeof values[0];
    printf("positive even sum = %d\n", sum_positive_even(values, size));
    return 0;
}
