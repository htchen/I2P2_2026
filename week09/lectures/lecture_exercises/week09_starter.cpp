class Rational {
 public:
  Rational(int numerator, int denominator);
  int numerator() const {
    return numerator_;
  }
  int denominator() const {
    return denominator_;
  }
  friend bool operator==(const Rational& left, const Rational& right);
  friend Rational operator+(const Rational& left, const Rational& right);

 private:
  int numerator_;
  int denominator_;
  void Normalize();
};
