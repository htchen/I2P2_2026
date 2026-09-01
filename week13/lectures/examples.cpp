#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <variant>
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

class Variable final : public Function {
 public:
  double Eval(double x) const override {
    return x;
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
    return left_->Eval(x) + right_->Eval(x);
  }

 private:
  std::unique_ptr<Function> left_;
  std::unique_ptr<Function> right_;
};

using FunctionList = std::vector<std::unique_ptr<Function>>;

FunctionList MakeSampleFunctions() {
  FunctionList functions;
  functions.push_back(std::make_unique<Constant>(0.5));
  functions.push_back(std::make_unique<Variable>());
  return functions;
}

struct ConstantValue {
  double value;
};

struct VariableValue {};

using FunctionValue = std::variant<ConstantValue, VariableValue>;

// Optional closed-alternative comparison; the Function/Sum hierarchy above is
// the core exercise path.

struct EvalVisitor {
  double x;

  double operator()(const ConstantValue& constant) const {
    return constant.value;
  }
  double operator()(const VariableValue&) const {
    return x;
  }
};

double EvalValue(const FunctionValue& function, double x) {
  return std::visit(EvalVisitor{x}, function);
}

int main() {
  const FunctionList functions = MakeSampleFunctions();
  for (const auto& function : functions) {
    std::cout << function->Eval(2.0) << '\n';
  }

  const Sum expression{std::make_unique<Constant>(4.0),
                       std::make_unique<Variable>()};
  std::cout << expression.Eval(3.0) << '\n';

  // Optional std::variant comparison.
  const std::vector<FunctionValue> values{ConstantValue{0.5}, VariableValue{}};
  for (const FunctionValue& function : values) {
    std::cout << EvalValue(function, 2.0) << '\n';
  }
}
