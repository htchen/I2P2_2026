#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void clamp_all(int values[], size_t size, int low, int high) {
  /* TODO: require low <= high and clamp each logical element. */
  (void)values;
  (void)size;
  (void)low;
  (void)high;
}

int build_positive_prefix(const int values[], size_t size, int64_t prefix[],
                          size_t prefix_capacity) {
  /* TODO: write size + 1 boundary totals; count only positive values. */
  (void)values;
  (void)size;
  (void)prefix;
  (void)prefix_capacity;
  return 0;
}

int query_total(const int64_t prefix[], size_t prefix_count, size_t left,
                size_t right, int64_t* result) {
  /* TODO: validate [left, right) and publish prefix[right] - prefix[left]. */
  (void)prefix;
  (void)prefix_count;
  (void)left;
  (void)right;
  (void)result;
  return 0;
}

size_t lower_bound_int(const int values[], size_t size, int key) {
  /* TODO: return the first i in [0, size] with values[i] >= key. */
  (void)values;
  (void)size;
  (void)key;
  return 0;
}

size_t upper_bound_int(const int values[], size_t size, int key) {
  /* TODO: return the first i in [0, size] with values[i] > key. */
  (void)values;
  (void)size;
  (void)key;
  return 0;
}

int copy_word(char destination[], size_t capacity, const char source[]) {
  /* TODO: copy as much as fits, terminate when capacity > 0, and report
     whether the complete source string fit. */
  (void)destination;
  (void)capacity;
  (void)source;
  return 0;
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

  const int contributions[] = {3, -1, 4, 2, -5};
  int64_t prefix[6] = {0};
  int64_t total = 0;
  if (!check(build_positive_prefix(contributions, 5, prefix, 6),
             "build_positive_prefix returned failure") ||
      !check(prefix[0] == 0 && prefix[1] == 3 && prefix[2] == 3 &&
                 prefix[3] == 7 && prefix[4] == 9 && prefix[5] == 9,
             "positive prefix values") ||
      !check(query_total(prefix, 6, 1, 4, &total) && total == 6,
             "query_total for [1, 4)")) {
    return 1;
  }

  const int sorted[] = {1, 2, 2, 2, 5, 8};
  if (!check(lower_bound_int(sorted, 6, 2) == 1, "lower bound") ||
      !check(upper_bound_int(sorted, 6, 2) == 4, "upper bound")) {
    return 1;
  }

  char word[5] = "";
  if (!check(copy_word(word, sizeof(word), "hello") == 0 &&
                 strcmp(word, "hell") == 0,
             "truncating copy") ||
      !check(copy_word(word, sizeof(word), "C") == 1 && strcmp(word, "C") == 0,
             "complete copy")) {
    return 1;
  }

  puts("all Week 2 starter checks passed");
  return 0;
}
