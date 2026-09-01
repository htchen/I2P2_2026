#include <algorithm>
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

class RawBuffer {
 public:
  explicit RawBuffer(std::size_t size)
      : size_{size}, data_{size == 0 ? nullptr : new int[size]{}} {
  }

  ~RawBuffer() {
    delete[] data_;
  }

  RawBuffer(const RawBuffer& other) : RawBuffer(other.size_) {
    if (size_ > 0) {
      std::copy(other.data_, other.data_ + other.size_, data_);
    }
  }

  RawBuffer& operator=(const RawBuffer& other) {
    if (this != &other) {
      RawBuffer copy{other};
      Swap(copy);
    }
    return *this;
  }

  RawBuffer(RawBuffer&& other) noexcept
      : size_{other.size_}, data_{other.data_} {
    other.size_ = 0;
    other.data_ = nullptr;
  }

  RawBuffer& operator=(RawBuffer&& other) noexcept {
    if (this != &other) {
      delete[] data_;
      size_ = other.size_;
      data_ = other.data_;
      other.size_ = 0;
      other.data_ = nullptr;
    }
    return *this;
  }

  void Swap(RawBuffer& other) noexcept {
    std::swap(size_, other.size_);
    std::swap(data_, other.data_);
  }

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
  RawBuffer raw{3};
  raw[0] = 42;
  RawBuffer raw_copy = raw;
  raw_copy[0] = 7;
  RawBuffer raw_moved = std::move(raw_copy);
  RawBuffer raw_assigned{1};
  raw_assigned = raw;
  RawBuffer* same_object = &raw_assigned;
  raw_assigned = *same_object;
  RawBuffer raw_move_assigned{1};
  raw_move_assigned = std::move(raw_assigned);
  std::cout << raw[0] << ' ' << raw_moved[0] << ' '
            << raw_move_assigned[0] << ' ' << raw_copy.size() << ' '
            << raw_assigned.size() << '\n';

  Buffer first{3};
  first[0] = 42;
  Buffer copy = first;
  copy[0] = 7;
  Buffer moved = std::move(copy);
  Buffer assigned{1};
  assigned = first;
  Buffer move_assigned{1};
  move_assigned = std::move(assigned);
  std::cout << first[0] << ' ' << moved[0] << ' ' << move_assigned[0] << ' '
            << moved.size() << '\n';
}
