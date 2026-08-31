#include <cstddef>

class Buffer {
 public:
  explicit Buffer(std::size_t size);
  ~Buffer();
  Buffer(const Buffer& other);
  Buffer& operator=(const Buffer& other);
  Buffer(Buffer&& other) noexcept;
  Buffer& operator=(Buffer&& other) noexcept;

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

// TODO: define the six ownership-sensitive operations, then refactor the class
// to std::vector<int> and remove them.
