#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static bool make_sequence(size_t size, int** out) {
  if (out == NULL || size > SIZE_MAX / sizeof(int)) {
    return false;
  }
  *out = NULL;
  if (size == 0) {
    return true;
  }
  int* candidate = malloc(size * sizeof(*candidate));
  if (candidate == NULL) {
    return false;
  }
  for (size_t i = 0; i < size; ++i) {
    candidate[i] = 0;
  }
  *out = candidate;
  return true;
}

static bool resize_sequence(int** values, size_t old_size, size_t new_size) {
  if (values == NULL || (old_size > 0 && *values == NULL) ||
      new_size > SIZE_MAX / sizeof(**values)) {
    return false;
  }
  if (new_size == 0) {
    free(*values);
    *values = NULL;
    return true;
  }

  int* candidate = realloc(*values, new_size * sizeof(*candidate));
  if (candidate == NULL) {
    return false;
  }
  for (size_t i = old_size; i < new_size; ++i) {
    candidate[i] = 0;
  }
  *values = candidate;
  return true;
}

int main(void) {
  int* values = NULL;
  const size_t size = 6;
  if (!make_sequence(size, &values)) {
    fputs("allocation failed\n", stderr);
    return 1;
  }
  const size_t larger_size = 9;
  if (!resize_sequence(&values, size, larger_size)) {
    fputs("resize failed\n", stderr);
    free(values);
    return 1;
  }
  for (size_t i = 0; i < larger_size; ++i) {
    printf("%d%c", values[i], i + 1 == larger_size ? '\n' : ' ');
  }
  if (!resize_sequence(&values, larger_size, 3) ||
      !resize_sequence(&values, 3, 0) || values != NULL) {
    fputs("shrink or release failed\n", stderr);
    free(values);
    return 1;
  }
  return 0;
}
