#include <limits.h>
#include <stdio.h>

int main(void) {
  int value;
  long long sum = 0;

  while (scanf("%d", &value) == 1) {
    if (value > 0 && value % 2 == 0) {
      if (sum > LLONG_MAX - value) {
        fputs("sum is outside the long long range\n", stderr);
        return 1;
      }
      sum += value;
    }
  }

  if (!feof(stdin)) {
    fputs("input contained a non-integer token\n", stderr);
    return 1;
  }
  printf("positive even sum = %lld\n", sum);
  return 0;
}
