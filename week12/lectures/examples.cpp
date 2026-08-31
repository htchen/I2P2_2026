#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

class Buffer {
 public:
  explicit Buffer(std::size_t size) : values_(size) {
  }

  int& operator[](std::size_t index) {
    return values_.at(index);
  }
  const int& operator[](std::size_t index) const {
    return values_.at(index);
  }
  std::size_t size() const {
    return values_.size();
  }

 private:
  // vector supplies destruction, copy, move, and assignment: Rule of Zero.
  std::vector<int> values_;
};

int main() {
  Buffer first{3};
  first[0] = 42;
  Buffer copy = first;
  copy[0] = 7;
  Buffer moved = std::move(copy);
  std::cout << first[0] << ' ' << moved[0] << ' ' << moved.size() << '\n';
}
