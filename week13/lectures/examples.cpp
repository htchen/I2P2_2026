#include <iostream>
#include <memory>
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

  const std::vector<FunctionValue> values{ConstantValue{0.5}, VariableValue{}};
  for (const FunctionValue& function : values) {
    std::cout << EvalValue(function, 2.0) << '\n';
  }
}
