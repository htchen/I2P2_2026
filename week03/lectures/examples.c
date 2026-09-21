#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct Rational {
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
  if (left == 0) {
    return 1;
  }
  return left;
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
  out->numerator = numerator / divisor;
  out->denominator = denominator / divisor;
  return true;
}

int main(void) {
  Rational value;
  bool made = rational_make(-6, -8, &value);
  if (!made) {
    fputs("could not construct rational value\n", stderr);
    return 1;
  }
  assert(value.numerator == 3 && value.denominator == 4);
  made = rational_make(0, 5, &value);
  assert(made);
  assert(value.numerator == 0 && value.denominator == 1);
  made = rational_make(1, 0, &value);
  assert(!made);
  assert(value.numerator == 0 && value.denominator == 1);
  made = rational_make(6, 8, &value);
  assert(made);
  printf("%d/%d\n", value.numerator, value.denominator);
  return 0;
}
