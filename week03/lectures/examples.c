#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  int numerator;
  int denominator;
} Rational;

static int gcd_positive(int left, int right) {
  if (left < 0) {
    left = -left;
  }
  if (right < 0) {
    right = -right;
  }
  while (right != 0) {
    const int remainder = left % right;
    left = right;
    right = remainder;
  }
  return left == 0 ? 1 : left;
}

static bool rational_make(int numerator, int denominator, Rational* out) {
  if (out == NULL || denominator == 0 || numerator == INT_MIN ||
      denominator == INT_MIN) {
    return false;
  }
  if (denominator < 0) {
    numerator = -numerator;
    denominator = -denominator;
  }
  const int divisor = gcd_positive(numerator, denominator);
  *out = (Rational){numerator / divisor, denominator / divisor};
  return true;
}

int main(void) {
  Rational value;
  assert(rational_make(-6, -8, &value));
  assert(value.numerator == 3 && value.denominator == 4);
  printf("%d/%d\n", value.numerator, value.denominator);
  return 0;
}
