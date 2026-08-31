#include <stddef.h>
#include <stdio.h>

static size_t lower_bound_int(const int values[], size_t size, int key) {
  size_t left = 0;
  size_t right = size;
  while (left < right) {
    const size_t middle = left + (right - left) / 2;
    if (values[middle] >= key) {
      right = middle;
    } else {
      left = middle + 1;
    }
  }
  return left;
}

static size_t upper_bound_int(const int values[], size_t size, int key) {
  size_t left = 0;
  size_t right = size;
  while (left < right) {
    const size_t middle = left + (right - left) / 2;
    if (values[middle] > key) {
      right = middle;
    } else {
      left = middle + 1;
    }
  }
  return left;
}

int main(void) {
  const int sorted[] = {1, 2, 2, 2, 5, 8};
  const size_t size = sizeof sorted / sizeof sorted[0];
  const int key = 2;
  printf(
      "count(%d) = %zu\n", key,
      upper_bound_int(sorted, size, key) - lower_bound_int(sorted, size, key));
  return 0;
}
