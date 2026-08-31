#include <stddef.h>

void clamp_all(int values[], size_t size, int low, int high) {
  /* TODO: require low <= high and clamp each logical element. */
  (void)values;
  (void)size;
  (void)low;
  (void)high;
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
