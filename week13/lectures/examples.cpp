#include <cmath>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

class Function {
public:
    virtual ~Function() = default;
    virtual double eval(double x) const = 0;
};

class Constant final : public Function {
public:
    explicit Constant(double value) : value_{value} {}
    double eval(double) const override { return value_; }

private:
    double value_;
};

class Sine final : public Function {
public:
    explicit Sine(std::unique_ptr<Function> argument)
        : argument_{std::move(argument)} {}
    double eval(double x) const override { return std::sin(argument_->eval(x)); }

private:
    std::unique_ptr<Function> argument_;
};

int main() {
    std::vector<std::unique_ptr<Function>> functions;
    functions.push_back(std::make_unique<Constant>(0.5));
    functions.push_back(
        std::make_unique<Sine>(std::make_unique<Constant>(0.5)));
    for (const auto &function : functions) {
        std::cout << function->eval(0.0) << '\n';
    }
}
