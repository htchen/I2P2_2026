#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static bool make_sequence(size_t size, int **out) {
    if (out == NULL || size > SIZE_MAX / sizeof(int)) {
        return false;
    }
    *out = NULL;
    if (size == 0) {
        return true;
    }
    int *candidate = malloc(size * sizeof(*candidate));
    if (candidate == NULL) {
        return false;
    }
    for (size_t i = 0; i < size; ++i) {
        candidate[i] = 0;
    }
    *out = candidate;
    return true;
}

int main(void) {
    int *values = NULL;
    const size_t size = 6;
    if (!make_sequence(size, &values)) {
        fputs("allocation failed\n", stderr);
        return 1;
    }
    for (size_t i = 0; i < size; ++i) {
        printf("%d%c", values[i], i + 1 == size ? '\n' : ' ');
    }
    free(values);
    return 0;
}
