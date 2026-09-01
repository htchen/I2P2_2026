#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

void Normalize(std::vector<double>& values) {
  // TODO: divide by the sum; throw unless the sum is finite and nonzero.
  (void)values;
}

double Total(const std::vector<double>& values) {
  // TODO: read through a const reference without copying the vector.
  (void)values;
  return 0.0;
}

static bool Check(bool condition, const char* message) {
  if (!condition) {
    std::cerr << "check failed: " << message << '\n';
    return false;
  }
  return true;
}

int main() {
  std::vector<double> values{1.0, 2.0, 3.0};
  if (!Check(Total(values) == 6.0, "Total")) {
    return 1;
  }
  Normalize(values);
  if (!Check(std::abs(Total(values) - 1.0) < 1e-12,
             "Normalize produces unit total")) {
    return 1;
  }

  std::vector<double> invalid{1.0, -1.0};
  bool rejected = false;
  try {
    Normalize(invalid);
  } catch (const std::invalid_argument&) {
    rejected = true;
  }
  if (!Check(rejected && invalid[0] == 1.0 && invalid[1] == -1.0,
             "failure leaves input unchanged")) {
    return 1;
  }
  std::cout << "all Week 8 starter checks passed\n";
  return 0;
}
