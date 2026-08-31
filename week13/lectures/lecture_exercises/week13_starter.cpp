#include <memory>
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
