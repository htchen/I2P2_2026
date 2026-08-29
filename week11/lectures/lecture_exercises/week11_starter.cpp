#include <algorithm>
#include <cstddef>
#include <optional>

template <class ForwardIt, class T>
std::size_t count_equal(ForwardIt first, ForwardIt last, const T &key) {
    // TODO: use lower_bound/upper_bound on the sorted half-open range.
    (void)first;
    (void)last;
    (void)key;
    return 0;
}

template <class Range, class T>
std::optional<std::size_t> find_index(const Range &range, const T &key) {
    // TODO: return absence instead of a magic index.
    (void)range;
    (void)key;
    return std::nullopt;
}
