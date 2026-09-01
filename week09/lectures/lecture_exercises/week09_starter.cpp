class Rational {
 public:
  Rational(int numerator, int denominator);
  int numerator() const {
    return numerator_;
  }
  int denominator() const {
    return denominator_;
  }
 private:
  int numerator_;
  int denominator_;
  void Normalize();
};

bool operator==(const Rational& left, const Rational& right);
bool operator!=(const Rational& left, const Rational& right);
Rational operator+(const Rational& left, const Rational& right);
