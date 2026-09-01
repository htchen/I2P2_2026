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
  explicit Constant(double value) : value_(value) {
  }
  double Eval(double x) const override;

 private:
  double value_;
};

class Variable final : public Function {
 public:
  double Eval(double x) const override;
};

using FunctionList = std::vector<std::unique_ptr<Function>>;

FunctionList MakeSampleFunctions() {
  // TODO: add one Constant and one Variable with std::make_unique.
  return {};
}

struct ConstantValue {
  double value;
};

struct VariableValue {};

using FunctionValue = std::variant<ConstantValue, VariableValue>;

double EvalValue(const FunctionValue& function, double x) {
  // TODO: evaluate the closed set of alternatives with std::visit.
  (void)function;
  (void)x;
  return 0.0;
}
