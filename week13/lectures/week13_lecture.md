# Lecture 12 — Templates, Containers, Iterators, and Algorithms

> December 1, 2026 · Source lineage: the legacy template and standard-library
> notes plus the 2025 Week 11 notebook

## Learning objectives

By the end of this lecture, you should be able to:

1. Write a small function or class template.
2. Select a standard container from its operations and complexity.
3. Use iterators as a range abstraction.
4. Compose algorithms with lambdas instead of rewriting loops.
5. Recognize iterator invalidation and accidental insertion into a map.

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | How do templates state operations required from a type? | Generalize concrete functions and diagnose instantiation errors |
| 2 | Which container and iterator guarantees fit a workload? | Select representations and trace invalidation |
| 3 | How do algorithms, lambdas, and maps form a complete solution? | Implement and benchmark a frequency/ranking pipeline |

## Hour 1 — Compile-time genericity and template requirements

### 1. Templates describe families of code

```cpp
template <typename T>
T maximum(const T& left, const T& right)
{
    return left < right ? right : left;
}
```

The compiler instantiates a version for each used type. The type must support
the expressions required by the template—here, comparison with `<` and copying
the selected result. Returning by value avoids handing the caller a reference
that could outlive a temporary argument.

```cpp
int largest_int = maximum(3, 8);
std::string largest_word = maximum(std::string{"ant"}, std::string{"bee"});
```

Templates are compile-time polymorphism. Unlike a virtual call, the concrete
operation is normally known during compilation and can be inlined.

### 2. A small class template

```cpp
template <typename T>
class Box {
public:
    explicit Box(T value) : value_{std::move(value)} {}

    const T& value() const& { return value_; }
    T value() && { return std::move(value_); }

private:
    T value_;
};
```

Template definitions normally live in headers because a translation unit needs
the definition when instantiating a concrete type.

Avoid making a template merely to avoid naming the actual abstraction. Generic
code is valuable when multiple types share the same meaningful operation.

### Requirements before C++20 concepts

In C++17, template requirements are implicit in expressions. For `maximum<T>`,
`left < right` must be valid and usable as a condition. A compiler error may be
long because it reports the failed instantiation path. Read from the first
expression involving your type, not only the final diagnostic line.

Create three types: one with a valid `<`, one with equality only, and one whose
comparison returns an unsuitable type. Instantiate `maximum` and classify the
errors. Then state the requirement in a comment beside the template.

### Function objects and generic lambdas

```cpp
auto absolute_less = [](const auto& left, const auto& right) {
    return std::abs(left) < std::abs(right);
};

std::sort(values.begin(), values.end(), absolute_less);
```

The lambda's call operator is a template. Its comparator must provide a strict
weak ordering; returning `<=` instead of `<` violates the sorting contract.
For signed integers, `std::abs(INT_MIN)` is not representable. Restrict the input
domain or compare magnitudes through a checked, wider representation when that
value is possible.

### Hour 1 template studio

Generalize `contains`, `print_range`, and `count_if` from `vector<int>` to
iterator pairs. For each, list the minimum iterator and element operations. Use
the exercise to make standard-algorithm contracts explicit.

## Hour 2 — Containers, iterators, complexity, and invalidation

### 3. Choose containers by required operations

| Container | Strengths | Important costs |
|-----------|-----------|-----------------|
| `vector<T>` | contiguous, fast indexing, cache friendly | middle insertion O(n) |
| `deque<T>` | fast insertion at both ends | not one contiguous block |
| `list<T>` | stable iterators, O(1) splice/erase at iterator | no indexing, allocation per node |
| `map<K,V>` | ordered keys, O(log n) operations | tree/node overhead |
| `unordered_map<K,V>` | average O(1) lookup | no order, rehashing, hash requirements |
| `set<T>` | ordered unique values | O(log n) operations |
| `queue<T>` | FIFO interface | intentionally limited access |
| `stack<T>` | LIFO interface | intentionally limited access |

Default to `vector` unless another container's semantics or complexity solves a
specific need. The legacy course used `list` frequently; modern code should not
choose it merely because insertions look O(1)—finding the position is still a
cost and locality often dominates.

### Container-selection scenarios

Choose and defend a representation for an ordered leaderboard, FIFO event queue,
entity table by numeric ID, unique visited puzzle states, stable splice-heavy
sequence, and dense objects updated every frame. Include iteration order,
lookup/insertion complexity, locality, reference stability, and duplicates.

### 4. Iterators represent positions and ranges

The half-open range `[first, last)` includes `first` and excludes `last`.

```cpp
auto position = std::find(values.begin(), values.end(), target);
if (position != values.end()) {
    std::cout << "found at " << std::distance(values.begin(), position) << '\n';
}
```

Half-open ranges compose cleanly: an empty range has `first == last`, and two
adjacent ranges can share a boundary.

Iterator categories expose supported movement. A vector iterator supports
random access; a list iterator does not. Generic algorithms express the weakest
category they require.

## Hour 3 — Algorithms, associative containers, and solution pipelines

### 5. Algorithms separate traversal from intent

```cpp
#include <algorithm>
#include <numeric>

std::sort(values.begin(), values.end());

int threshold = 10;
auto first_large = std::find_if(values.begin(), values.end(),
    [threshold](int value) { return value >= threshold; });

int total = std::accumulate(values.begin(), values.end(), 0);
```

Common algorithms include:

- `find`, `find_if`, `count_if`;
- `sort`, `stable_sort`, `lower_bound`;
- `copy`, `transform`, `remove_if`;
- `all_of`, `any_of`, `none_of`;
- `accumulate`.

An algorithm name states intent and centralizes boundary handling. A loop is
still correct when the operation does not fit an algorithm cleanly.

### 6. The erase-remove pattern

`std::remove_if` rearranges retained elements and returns a new logical end; it
does not resize the container.

```cpp
values.erase(
    std::remove_if(values.begin(), values.end(),
                   [](int value) { return value < 0; }),
    values.end());
```

C++20 adds `std::erase_if(values, predicate)` for supported containers, but the
older form remains important when reading C++17 projects.

### 7. Maps: lookup versus insertion

```cpp
std::map<std::string, int> frequency;
for (const std::string& word : words) {
    ++frequency[word]; /* deliberate default insertion */
}
```

`operator[]` inserts a missing key. For read-only lookup:

```cpp
auto found = frequency.find(query);
if (found != frequency.end()) {
    std::cout << found->second << '\n';
}
```

Or use `.at(query)` when absence should produce an exception. Do not accidentally
mutate a map while asking whether a key exists.

### Frequency-to-ranking pipeline

```cpp
std::vector<std::pair<std::string, int>> ranking{
    frequency.begin(), frequency.end()
};

std::sort(ranking.begin(), ranking.end(), [](const auto& left, const auto& right) {
    if (left.second != right.second) return left.second > right.second;
    return left.first < right.first;
});
```

The map builds counts; the vector supports ranking by a different order. This is
often clearer than forcing one container to serve incompatible access patterns.
Prove the comparator is strict for equal pairs.

### 8. Iterator invalidation

After a vector reallocates, pointers, references, and iterators to its elements
are invalid. Inserting or erasing can invalidate additional positions even
without reallocation.

```cpp
auto position = values.begin();
values.push_back(42);
/* position may now be invalid */
```

Read the operation's invalidation rules. A valid iterator is a lifetime and
ownership claim just as a valid C pointer is.

Use indices when a vector mutation may relocate storage and the index remains a
meaningful position, or reacquire the iterator after mutation.

### Invalidation trace

For a vector with size 3 and capacity 4, take iterators to all elements, then
push a fourth element, insert at index 1 without reallocation, push a fifth
element with reallocation, and erase index 2. Mark valid handles after each
operation. Repeat for `list` and `map` and compare their guarantees.

### 9. Complexity belongs to the interface

For an ordered map, lookup is O(log n). For an unordered map, lookup is average
O(1) but worst-case O(n). `lower_bound` on a sorted vector is O(log n), but
inserting into the middle is O(n). A good design considers the whole workload:
build frequency, query frequency, ordering requirements, and memory overhead.

The standard library specifies both semantics and complexity. Use those
guarantees instead of assuming an internal implementation.

### Hour 3 integration task

Read words, normalize case, count with a map/unordered map, remove stop words,
rank by frequency then spelling, and print the top `k`. Test empty input, ties,
repeated punctuation policy, and very large counts. Benchmark ordered versus
unordered counting on supplied data and explain results without overgeneralizing
from one machine.

## Check yourself

1. What operation does the `maximum` template require from `T`?
2. Select containers for a FIFO worklist, ordered dictionary, and dense table.
3. Why is `[begin, end)` easier to represent than an inclusive end?
4. What exactly does `map[key]` do when `key` is absent?
5. Identify invalid iterators after a vector insertion in the middle.

## Summary

- Templates express type-safe compile-time generality.
- Container selection follows operations, guarantees, and data layout.
- Iterators generalize positions and half-open ranges.
- Algorithms expose intent and reduce repeated traversal code.
- Mutation can invalidate iterators; complexity and lifetime remain correctness concerns.

## References and legacy sources

- [Classes III: templates and related material](<https://github.com/htchen/i2p-nthu/blob/master/程式設計二/Classes%20III/README.md>)
- [Standard library](<https://github.com/htchen/i2p-nthu/blob/master/程式設計二/week%2012%20Standard%20library/week%2012%20Standard%20library.md>)
- [2025 Week 11 notebook (Colab)](https://colab.research.google.com/drive/1RjtHSu-82v1dQt-p2teRmKwjV9bLlQsK)
