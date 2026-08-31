#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class ScopeTrace {
public:
    explicit ScopeTrace(std::string name) : name_{std::move(name)} {
        std::cout << "acquire " << name_ << '\n';
    }
    ~ScopeTrace() { std::cout << "release " << name_ << '\n'; }

private:
    std::string name_;
};

void normalize(std::vector<double> &values) {
    ScopeTrace trace{"normalize"};
    const double sum = std::accumulate(values.begin(), values.end(), 0.0);
    if (sum == 0.0) {
        throw std::invalid_argument{"cannot normalize a zero sum"};
    }
    for (double &value : values) {
        value /= sum;
    }
}

int main() {
    std::vector<double> values{1.0, 2.0, 3.0};
    normalize(values);
    for (double value : values) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
}
