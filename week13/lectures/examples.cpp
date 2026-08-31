#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

class Function {
 public:
  virtual ~Function() = default;
  virtual double Eval(double x) const = 0;
};

class Constant final : public Function {
 public:
  explicit Constant(double value) : value_{value} {
  }
  double Eval(double) const override {
    return value_;
  }

 private:
  double value_;
};

class Sine final : public Function {
 public:
  explicit Sine(std::unique_ptr<Function> argument)
      : argument_{std::move(argument)} {
    if (argument_ == nullptr) {
      throw std::invalid_argument{"sine argument is required"};
    }
  }
  double Eval(double x) const override {
    return std::sin(argument_->Eval(x));
  }

 private:
  std::unique_ptr<Function> argument_;
};

int main() {
  std::vector<std::unique_ptr<Function>> functions;
  functions.push_back(std::make_unique<Constant>(0.5));
  functions.push_back(std::make_unique<Sine>(std::make_unique<Constant>(0.5)));
  for (const auto& function : functions) {
    std::cout << function->Eval(0.0) << '\n';
  }
}
