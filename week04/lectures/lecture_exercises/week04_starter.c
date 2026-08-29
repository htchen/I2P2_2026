#include <stdbool.h>
#include <stddef.h>

bool make_sequence(size_t size, int **out) {
    /* TODO: validate out, check overflow, allocate, initialize, publish. */
    (void)size;
    (void)out;
    return false;
}

bool resize_sequence(int **values, size_t old_size, size_t new_size) {
    /* TODO: preserve the old owner if reallocation fails. */
    (void)values;
    (void)old_size;
    (void)new_size;
    return false;
}
