#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
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

static bool check(bool condition, const char* message) {
  if (!condition) {
    fprintf(stderr, "check failed: %s\n", message);
    return false;
  }
  return true;
}

int main(void) {
  int* values = NULL;
  if (!check(make_sequence(2, &values), "initial allocation") ||
      !check(values != NULL && values[0] == 0 && values[1] == 0,
             "initial zero values")) {
    free(values);
    return 1;
  }
  values[0] = 7;
  values[1] = 9;
  if (!check(resize_sequence(&values, 2, 4), "growth") ||
      !check(
          values[0] == 7 && values[1] == 9 && values[2] == 0 && values[3] == 0,
          "preserve old values and initialize growth") ||
      !check(resize_sequence(&values, 4, 0) && values == NULL,
             "release through resize to zero")) {
    free(values);
    return 1;
  }
  puts("all Week 4 starter checks passed");
  return 0;
}
