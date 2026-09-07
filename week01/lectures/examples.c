#include <limits.h>
#include <stdio.h>

int main(void) {
  int value;
  long long sum = 0;

  while (scanf("%d", &value) == 1) {
    if (value > 0 && value % 2 == 0) {
      if (sum > LLONG_MAX - value) {
        fprintf(stderr, "sum is outside the long long range\n");
        return 1;
      }
      sum += value;
    }
  }

  if (!feof(stdin)) {
    fprintf(stderr, "input contained a non-integer token\n");
    return 1;
  }
  printf("%lld\n", sum);
  return 0;
}
