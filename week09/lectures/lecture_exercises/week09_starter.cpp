#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <stdexcept>

class Rational {
 public:
  Rational(int numerator, int denominator)
      : numerator_{numerator}, denominator_{denominator} {
    // TODO: reject unsupported input and call Normalize().
  }

  int numerator() const {
    return numerator_;
  }
  int denominator() const {
    return denominator_;
  }

  Rational& operator+=(const Rational& other) {
    // TODO: add other, preserve the invariant, and return *this.
    (void)other;
    return *this;
  }

  Rational& operator-=(const Rational& other) {
    // TODO: subtract other, preserve the invariant, and return *this.
    (void)other;
    return *this;
  }

  Rational operator-() const {
    // TODO: return a negated value without modifying *this.
    return *this;
  }

 private:
  int numerator_;
  int denominator_;

  void Normalize() {
    // TODO: make the denominator positive and reduce by the gcd.
  }
};

Rational operator+(Rational left, const Rational& right) {
  left += right;
  return left;
}

Rational operator-(Rational left, const Rational& right) {
  left -= right;
  return left;
}

bool operator==(const Rational& left, const Rational& right) {
  // TODO: normalized values are equal exactly when both fields are equal.
  (void)left;
  (void)right;
  return false;
}

bool operator!=(const Rational& left, const Rational& right) {
  return !(left == right);
}

std::ostream& operator<<(std::ostream& stream, const Rational& value) {
  return stream << value.numerator() << '/' << value.denominator();
}

static bool Check(bool condition, const char* message) {
  if (!condition) {
    std::cerr << "check failed: " << message << '\n';
    return false;
  }
  return true;
}

int main() {
  bool rejected_zero = false;
  try {
    (void)Rational{1, 0};
  } catch (const std::invalid_argument&) {
    rejected_zero = true;
  }
  if (!Check(rejected_zero, "zero denominator is rejected")) {
    return 1;
  }

  const Rational normalized{2, -4};
  const Rational left{1, 6};
  const Rational right{1, 3};
  const Rational sum = left + right;
  const Rational difference = right - left;
  if (!Check(normalized == Rational{-1, 2}, "normalization") ||
      !Check(sum == Rational{1, 2}, "addition") ||
      !Check(difference == Rational{1, 6}, "subtraction") ||
      !Check(-difference == Rational{-1, 6}, "unary minus") ||
      !Check(left == Rational{1, 6}, "binary operator preserves left") ||
      !Check(right == Rational{1, 3}, "binary operator preserves right")) {
    return 1;
  }

  std::ostringstream output;
  output << sum;
  if (!Check(output.str() == "1/2", "stream output")) {
    return 1;
  }

  bool rejected_unsupported_minimum = false;
  try {
    (void)(Rational{std::numeric_limits<int>::min() + 1, 1} +
           Rational{-1, 1});
  } catch (const std::overflow_error&) {
    rejected_unsupported_minimum = true;
  }
  if (!Check(rejected_unsupported_minimum,
             "arithmetic preserves the representation policy")) {
    return 1;
  }

  std::cout << "all Week 9 starter checks passed\n";
  return 0;
}
