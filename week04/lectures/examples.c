#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static bool make_sequence(size_t size, int** out) {
  if (out == NULL || *out != NULL || size > SIZE_MAX / sizeof(int)) {
    return false;
  }
  int* candidate = NULL;
  if (size > 0) {
    candidate = malloc(size * sizeof(*candidate));
    if (candidate == NULL) {
      return false;
    }
    for (size_t i = 0; i < size; ++i) {
      candidate[i] = 0;
    }
  }
  *out = candidate;
  return true;
}

static bool resize_sequence(int** values, size_t old_size, size_t new_size) {
  if (values == NULL || (old_size == 0 && *values != NULL) ||
      (old_size > 0 && *values == NULL) ||
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

static bool check(bool condition, const char* message) {
  if (!condition) {
    fprintf(stderr, "check failed: %s\n", message);
    return false;
  }
  return true;
}

int main(void) {
  int* values = NULL;
  const size_t size = 6;
  if (!check(make_sequence(size, &values), "initial allocation")) {
    return 1;
  }

  int* original = values;
  if (!check(!make_sequence(3, &values) && values == original,
             "reject replacement of an existing owner")) {
    free(values);
    return 1;
  }

  const size_t larger_size = 9;
  if (!check(resize_sequence(&values, size, larger_size), "growth")) {
    free(values);
    return 1;
  }
  for (size_t i = 0; i < larger_size; ++i) {
    printf("%d", values[i]);
    if (i + 1 == larger_size) {
      fputc('\n', stdout);
    } else {
      fputc(' ', stdout);
    }
  }

  int* before_failure = values;
  if (!check(!resize_sequence(&values, larger_size, SIZE_MAX) &&
                 values == before_failure,
             "overflow failure preserves the owner") ||
      !check(resize_sequence(&values, larger_size, 3), "shrink") ||
      !check(resize_sequence(&values, 3, 0) && values == NULL, "release") ||
      !check(resize_sequence(&values, 0, 0) && values == NULL,
             "repeated empty release") ||
      !check(!make_sequence(SIZE_MAX, &values) && values == NULL,
             "allocation-size overflow")) {
    free(values);
    return 1;
  }
  puts("all Week 4 example checks passed");
  return 0;
}
