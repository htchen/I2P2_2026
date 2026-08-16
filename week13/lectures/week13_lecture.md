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

## 1. Templates describe families of code

```cpp
template <typename T>
const T& maximum(const T& left, const T& right)
{
    return left < right ? right : left;
}
```

The compiler instantiates a version for each used type. The type must support
the expressions required by the template—here, comparison with `<` and a return
compatible with `const T&`.

```cpp
int largest_int = maximum(3, 8);
std::string largest_word = maximum(std::string{"ant"}, std::string{"bee"});
```

Templates are compile-time polymorphism. Unlike a virtual call, the concrete
operation is normally known during compilation and can be inlined.

## 2. A small class template

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

## 3. Choose containers by required operations

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

## 4. Iterators represent positions and ranges

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

## 5. Algorithms separate traversal from intent

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

## 6. The erase-remove pattern

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

## 7. Maps: lookup versus insertion

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

## 8. Iterator invalidation

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

## 9. Complexity belongs to the interface

For an ordered map, lookup is O(log n). For an unordered map, lookup is average
O(1) but worst-case O(n). `lower_bound` on a sorted vector is O(log n), but
inserting into the middle is O(n). A good design considers the whole workload:
build frequency, query frequency, ordering requirements, and memory overhead.

The standard library specifies both semantics and complexity. Use those
guarantees instead of assuming an internal implementation.

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
