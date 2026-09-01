#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <stdexcept>

class Rational {
 public:
  Rational(int numerator, int denominator)
      : numerator_{numerator}, denominator_{denominator} {
    if (denominator_ == 0 || numerator_ == std::numeric_limits<int>::min() ||
        denominator_ == std::numeric_limits<int>::min()) {
      throw std::invalid_argument{"unsupported rational representation"};
    }
    Normalize();
  }

  int numerator() const {
    return numerator_;
  }
  int denominator() const {
    return denominator_;
  }

  Rational& operator+=(const Rational& other) {
    const long long numerator =
        static_cast<long long>(numerator_) * other.denominator_ +
        static_cast<long long>(other.numerator_) * denominator_;
    const long long denominator =
        static_cast<long long>(denominator_) * other.denominator_;
    AssignReduced(numerator, denominator);
    return *this;
  }

  Rational& operator-=(const Rational& other) {
    const long long numerator =
        static_cast<long long>(numerator_) * other.denominator_ -
        static_cast<long long>(other.numerator_) * denominator_;
    const long long denominator =
        static_cast<long long>(denominator_) * other.denominator_;
    AssignReduced(numerator, denominator);
    return *this;
  }

  Rational operator-() const {
    return Rational{-numerator_, denominator_};
  }

 private:
  int numerator_;
  int denominator_;

  void Normalize() {
    if (denominator_ < 0) {
      numerator_ = -numerator_;
      denominator_ = -denominator_;
    }
    const int divisor = std::gcd(numerator_, denominator_);
    numerator_ /= divisor;
    denominator_ /= divisor;
  }

  void AssignReduced(long long numerator, long long denominator) {
    static_assert(std::numeric_limits<long long>::digits >=
                  2 * std::numeric_limits<int>::digits + 1);
    const long long divisor = std::gcd(numerator, denominator);
    numerator /= divisor;
    denominator /= divisor;
    if (numerator < std::numeric_limits<int>::min() ||
        numerator > std::numeric_limits<int>::max() ||
        denominator > std::numeric_limits<int>::max()) {
      throw std::overflow_error{"rational result is outside the int range"};
    }
    numerator_ = static_cast<int>(numerator);
    denominator_ = static_cast<int>(denominator);
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
  return left.numerator() == right.numerator() &&
         left.denominator() == right.denominator();
}

bool operator!=(const Rational& left, const Rational& right) {
  return !(left == right);
}

std::ostream& operator<<(std::ostream& stream, const Rational& value) {
  return stream << value.numerator() << '/' << value.denominator();
}

static bool check(bool condition, const char* message) {
  if (!condition) {
    std::cerr << "check failed: " << message << '\n';
    return false;
  }
  return true;
}

int main() {
  const Rational left{1, 6};
  const Rational right{1, 3};
  const Rational sum = left + right;
  const Rational difference = right - left;
  if (!check(sum == Rational{1, 2}, "addition") ||
      !check(difference == Rational{1, 6}, "subtraction") ||
      !check(-difference == Rational{-1, 6}, "unary minus") ||
      !check(left == Rational{1, 6} && right == Rational{1, 3},
             "binary arithmetic preserves operands")) {
    return 1;
  }

  std::ostringstream output;
  output << sum;
  if (!check(output.str() == "1/2", "stream output")) {
    return 1;
  }

  std::cout << sum << '\n';
  return 0;
}
