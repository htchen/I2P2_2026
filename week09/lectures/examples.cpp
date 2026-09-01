#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>

class Rational {
 public:
  Rational(int numerator, int denominator)
      : numerator_{numerator}, denominator_{denominator} {
    if (denominator_ == 0) {
      throw std::invalid_argument{"zero denominator"};
    }
    if (numerator_ == std::numeric_limits<int>::min() ||
        denominator_ == std::numeric_limits<int>::min()) {
      throw std::invalid_argument{"unsupported rational representation"};
    }
    if (denominator_ < 0) {
      numerator_ = -numerator_;
      denominator_ = -denominator_;
    }
    const int divisor = std::gcd(numerator_, denominator_);
    numerator_ /= divisor;
    denominator_ /= divisor;
  }

  friend Rational operator+(const Rational& left, const Rational& right) {
    static_assert(std::numeric_limits<long long>::digits >=
                  2 * std::numeric_limits<int>::digits + 1);
    long long numerator =
        static_cast<long long>(left.numerator_) * right.denominator_ +
        static_cast<long long>(right.numerator_) * left.denominator_;
    long long denominator =
        static_cast<long long>(left.denominator_) * right.denominator_;
    const long long divisor = std::gcd(numerator, denominator);
    numerator /= divisor;
    denominator /= divisor;
    if (numerator < std::numeric_limits<int>::min() ||
        numerator > std::numeric_limits<int>::max() ||
        denominator > std::numeric_limits<int>::max()) {
      throw std::overflow_error{"rational result is outside the int range"};
    }
    return {static_cast<int>(numerator), static_cast<int>(denominator)};
  }

  friend std::ostream& operator<<(std::ostream& out, const Rational& value) {
    return out << value.numerator_ << '/' << value.denominator_;
  }

  friend bool operator==(const Rational& left, const Rational& right) {
    return left.numerator_ == right.numerator_ &&
           left.denominator_ == right.denominator_;
  }

  friend bool operator!=(const Rational& left, const Rational& right) {
    return !(left == right);
  }

 private:
  int numerator_;
  int denominator_;
};

int main() {
  const Rational result = Rational{1, 6} + Rational{1, 3};
  if (result != Rational{1, 2}) {
    return 1;
  }
  std::cout << result << '\n';
}
