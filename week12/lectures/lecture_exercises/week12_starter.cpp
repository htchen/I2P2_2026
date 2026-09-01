#include <cstddef>
#include <iostream>
#include <utility>

class Buffer {
 public:
  explicit Buffer(std::size_t size);
  ~Buffer();
  Buffer(const Buffer& other);
  Buffer& operator=(const Buffer& other);
  Buffer(Buffer&& other) noexcept;
  Buffer& operator=(Buffer&& other) noexcept;
  void Swap(Buffer& other) noexcept;

  std::size_t size() const {
    return size_;
  }
  int& operator[](std::size_t index) {
    return data_[index];
  }
  const int& operator[](std::size_t index) const {
    return data_[index];
  }

 private:
  std::size_t size_ = 0;
  int* data_ = nullptr;
};

// TODO: define Swap, the size constructor, and the five ownership-sensitive
// special member functions (destructor, copy/move constructors, and copy/move
// assignments), then refactor the class to std::vector<int> and remove the
// five.

static bool Check(bool condition, const char* message) {
  if (!condition) {
    std::cerr << "check failed: " << message << '\n';
    return false;
  }
  return true;
}

int main() {
  Buffer original{3};
  original[0] = 10;
  original[1] = 20;
  original[2] = 30;

  Buffer copy{original};
  copy[0] = 99;
  if (!Check(original[0] == 10 && copy[0] == 99,
             "copy owns independent storage")) {
    return 1;
  }

  Buffer assigned{1};
  assigned = original;
  Buffer* same_object = &assigned;
  assigned = *same_object;
  if (!Check(assigned.size() == 3 && assigned[2] == 30,
             "copy assignment and self-assignment")) {
    return 1;
  }

  Buffer moved{std::move(copy)};
  Buffer move_assigned{1};
  move_assigned = std::move(assigned);
  if (!Check(moved.size() == 3 && moved[0] == 99 && copy.size() == 0,
             "move construction") ||
      !Check(move_assigned.size() == 3 && move_assigned[1] == 20 &&
                 assigned.size() == 0,
             "move assignment")) {
    return 1;
  }

  std::cout << "all Week 12 starter checks passed\n";
  return 0;
}
