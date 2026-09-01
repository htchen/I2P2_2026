#include <stddef.h>
#include <stdio.h>
#include <string.h>

static void clamp_all(int values[], size_t size, int low, int high) {
  if (values == NULL || low > high) {
    return;
  }
  for (size_t index = 0; index < size; ++index) {
    if (values[index] < low) {
      values[index] = low;
    } else if (values[index] > high) {
      values[index] = high;
    }
  }
}

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

static int copy_word(char destination[], size_t capacity, const char source[]) {
  if (source == NULL || capacity == 0 || destination == NULL) {
    return 0;
  }
  size_t index = 0;
  while (source[index] != '\0' && index + 1 < capacity) {
    destination[index] = source[index];
    ++index;
  }
  destination[index] = '\0';
  return source[index] == '\0';
}

static int check(int condition, const char* message) {
  if (!condition) {
    fprintf(stderr, "check failed: %s\n", message);
    return 0;
  }
  return 1;
}

int main(void) {
  int values[] = {5, -2, 12};
  clamp_all(values, 3, 0, 10);
  if (!check(values[0] == 5 && values[1] == 0 && values[2] == 10,
             "clamp_all")) {
    return 1;
  }

  const int sorted[] = {1, 2, 2, 2, 5, 8};
  if (!check(upper_bound_int(sorted, 6, 2) - lower_bound_int(sorted, 6, 2) == 3,
             "duplicate count")) {
    return 1;
  }

  char word[5] = "";
  if (!check(copy_word(word, sizeof(word), "hello") == 0 &&
                 strcmp(word, "hell") == 0,
             "truncating copy")) {
    return 1;
  }

  puts("all Week 2 example checks passed");
  return 0;
}
