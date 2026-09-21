#include <stdbool.h>
#include <limits.h>
#include <stdio.h>

typedef struct Rational {
  int numerator;
  int denominator;
} Rational;

bool rational_make(int numerator, int denominator, Rational* out) {
  /* TODO: validate out/denominator, normalize sign, and reduce by gcd. */
  /* The temporary (void) casts keep this incomplete starter warning-clean;
     remove them when the implementation uses the parameters. */
  (void)numerator;
  (void)denominator;
  (void)out;
  return false;
}

int main(void) {
  Rational value = {0, 1};
  if (!rational_make(6, 8, &value) || value.numerator != 3 ||
      value.denominator != 4) {
    fputs("check failed: reduce 6/8 to 3/4\n", stderr);
    return 1;
  }
  if (!rational_make(2, -4, &value) || value.numerator != -1 ||
      value.denominator != 2) {
    fputs("check failed: normalize 2/-4 to -1/2\n", stderr);
    return 1;
  }
  if (!rational_make(0, 5, &value) || value.numerator != 0 ||
      value.denominator != 1) {
    fputs("check failed: normalize zero to 0/1\n", stderr);
    return 1;
  }
  if (rational_make(1, 0, &value) || rational_make(1, 2, NULL) ||
      rational_make(INT_MIN, 1, &value) ||
      rational_make(1, INT_MIN, &value) || value.numerator != 0 ||
      value.denominator != 1) {
    fputs("check failed: reject invalid construction\n", stderr);
    return 1;
  }
  puts("all Week 3 starter checks passed");
  return 0;
}
