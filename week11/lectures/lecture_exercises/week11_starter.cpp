#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>

template <class ForwardIt, class T>
std::size_t CountEqual(ForwardIt first, ForwardIt last, const T& key) {
  // TODO: use lower_bound/upper_bound on the sorted half-open range.
  (void)first;
  (void)last;
  (void)key;
  return 0;
}

template <class Range, class T>
std::optional<std::size_t> FindIndex(const Range& range, const T& key) {
  // TODO: return absence instead of a magic index.
  (void)range;
  (void)key;
  return std::nullopt;
}

static bool Check(bool condition, const char* message) {
  if (!condition) {
    std::cerr << "check failed: " << message << '\n';
    return false;
  }
  return true;
}

int main() {
  const std::vector<int> sorted{1, 2, 2, 2, 5, 8};
  if (!Check(CountEqual(sorted.begin(), sorted.end(), 2) == 3,
             "CountEqual finds duplicate range") ||
      !Check(CountEqual(sorted.begin(), sorted.end(), 4) == 0,
             "CountEqual handles a missing key")) {
    return 1;
  }

  const auto found = FindIndex(sorted, 5);
  const auto missing = FindIndex(sorted, 7);
  if (!Check(found && *found == 4, "FindIndex present branch") ||
      !Check(!missing, "FindIndex absent branch")) {
    return 1;
  }
  std::cout << "all Week 11 starter checks passed\n";
  return 0;
}
