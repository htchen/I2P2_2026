#include <algorithm>
#include <cstddef>
#include <iostream>
#include <optional>
#include <vector>

template <class ForwardIt, class T>
std::size_t count_equal(ForwardIt first, ForwardIt last, const T &key) {
    const auto lower = std::lower_bound(first, last, key);
    const auto upper = std::upper_bound(lower, last, key);
    return static_cast<std::size_t>(std::distance(lower, upper));
}

template <class T>
std::optional<std::size_t> find_index(const std::vector<T> &values,
                                      const T &key) {
    const auto found = std::find(values.begin(), values.end(), key);
    if (found == values.end()) {
        return std::nullopt;
    }
    return static_cast<std::size_t>(std::distance(values.begin(), found));
}

int main() {
    const std::vector<int> values{1, 2, 2, 2, 5, 8};
    std::cout << "count = " << count_equal(values.begin(), values.end(), 2)
              << '\n';
    if (const auto index = find_index(values, 5)) {
        std::cout << "index = " << *index << '\n';
    }
}
