#include <stdbool.h>
#include <stdio.h>

typedef struct {
  int numerator;
  int denominator;
} Rational;

bool rational_make(int numerator, int denominator, Rational* out) {
  /* TODO: validate out/denominator, normalize sign, and reduce by gcd. */
  (void)numerator;
  (void)denominator;
  (void)out;
  return false;
}

int main(void) {
  Rational value = {0, 1};
  if (!rational_make(2, -4, &value) || value.numerator != -1 ||
      value.denominator != 2) {
    fputs("check failed: normalize 2/-4 to -1/2\n", stderr);
    return 1;
  }
  if (rational_make(1, 0, &value) || rational_make(1, 2, NULL)) {
    fputs("check failed: reject invalid construction\n", stderr);
    return 1;
  }
  puts("all Week 3 starter checks passed");
  return 0;
}
