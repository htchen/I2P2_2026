#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

bool make_sequence(size_t size, int** out) {
  /* TODO: validate out, check overflow, allocate, initialize, publish. */
  (void)size;
  (void)out;
  return false;
}

bool resize_sequence(int** values, size_t old_size, size_t new_size) {
  /* TODO: resize a block that currently contains old_size ints. Preserve the
     old owner on failure, zero-initialize newly added elements, and make
     new_size == 0 release the block and publish NULL. */
  (void)values;
  (void)old_size;
  (void)new_size;
  return false;
}
