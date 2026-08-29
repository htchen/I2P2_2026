#include <stdbool.h>

typedef struct {
    int numerator;
    int denominator;
} Rational;

bool rational_make(int numerator, int denominator, Rational *out) {
    /* TODO: validate out/denominator, normalize sign, and reduce by gcd. */
    (void)numerator;
    (void)denominator;
    (void)out;
    return false;
}
