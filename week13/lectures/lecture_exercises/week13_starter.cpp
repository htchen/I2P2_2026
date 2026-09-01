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
  double Eval(double x) const override {
    // TODO: a constant ignores x and returns value_.
    (void)x;
    (void)value_;
    return 0.0;
  }

 private:
  double value_;
};

class Variable final : public Function {
 public:
  double Eval(double x) const override {
    // TODO: the variable evaluates to the supplied x.
    (void)x;
    return 0.0;
  }
};

class Sum final : public Function {
 public:
  Sum(std::unique_ptr<Function> left, std::unique_ptr<Function> right)
      : left_{std::move(left)}, right_{std::move(right)} {
    if (left_ == nullptr || right_ == nullptr) {
      throw std::invalid_argument{"a Sum requires two operands"};
    }
  }

  double Eval(double x) const override {
    // TODO: evaluate both owned children and add their results.
    (void)x;
    return 0.0;
  }

 private:
  std::unique_ptr<Function> left_;
  std::unique_ptr<Function> right_;
};

using FunctionList = std::vector<std::unique_ptr<Function>>;

FunctionList MakeSampleFunctions() {
  // TODO: add Constant{0.5}, then Variable, with std::make_unique.
  return {};
}

static bool Check(bool condition, const char* message) {
  if (!condition) {
    std::cerr << "check failed: " << message << '\n';
    return false;
  }
  return true;
}

int main() {
  const Constant constant{2.5};
  const Variable variable;
  if (!Check(constant.Eval(9.0) == 2.5, "Constant override") ||
      !Check(variable.Eval(9.0) == 9.0, "Variable override")) {
    return 1;
  }

  const FunctionList functions = MakeSampleFunctions();
  if (!Check(functions.size() == 2, "heterogeneous owning list") ||
      !Check(functions[0]->Eval(3.0) == 0.5,
             "dispatch through first base pointer") ||
      !Check(functions[1]->Eval(3.0) == 3.0,
             "dispatch through second base pointer")) {
    return 1;
  }

  const Sum expression{std::make_unique<Constant>(4.0),
                       std::make_unique<Variable>()};
  if (!Check(expression.Eval(3.0) == 7.0, "composite evaluation")) {
    return 1;
  }

  std::cout << "all Week 13 starter checks passed\n";
  return 0;
}
