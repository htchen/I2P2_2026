#include <iostream>
#include <numeric>
#include <stdexcept>

class Rational {
public:
    Rational(int numerator, int denominator)
        : numerator_{numerator}, denominator_{denominator} {
        if (denominator_ == 0) {
            throw std::invalid_argument{"zero denominator"};
        }
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        const int divisor = std::gcd(numerator_, denominator_);
        numerator_ /= divisor;
        denominator_ /= divisor;
    }

    friend Rational operator+(const Rational &left, const Rational &right) {
        return {left.numerator_ * right.denominator_ +
                    right.numerator_ * left.denominator_,
                left.denominator_ * right.denominator_};
    }

    friend std::ostream &operator<<(std::ostream &out, const Rational &value) {
        return out << value.numerator_ << '/' << value.denominator_;
    }

private:
    int numerator_;
    int denominator_;
};

int main() {
    std::cout << Rational{1, 6} + Rational{1, 3} << '\n';
}
