# Week 11 Lecture Notes — Templates, Containers, Iterators, and Algorithms

> November 17, 2026 · Source lineage: previous template and standard-library
> notes plus the 2025 Week 11 and Week 14 notebooks

> Python bridge: [Python Contrast Companion for Week 11](week11_python_companion.md)

## Learning objectives

By the end of this lecture, you should be able to:

1. Write a small function or class template.
2. Select a standard container from its operations and complexity.
3. Use iterators as a range abstraction.
4. Compose algorithms with lambdas instead of rewriting loops.
5. Represent an expected missing result with `std::optional`.
6. Recognize iterator invalidation and accidental insertion into a map.
7. Use `lower_bound`, `upper_bound`, and `equal_range` with a consistent
   ordering and interpret their iterator results.

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | How do templates state operations required from a type? | Generalize concrete functions and diagnose instantiation errors |
| 2 | Which container and iterator guarantees fit a workload? | Select representations and trace invalidation |
| 3 | How do algorithms, maps, and optional results form a solution? | Implement and benchmark a frequency/ranking pipeline |

## Hour 1 — Compile-time genericity and template requirements

Earlier lectures used `vector<int>` as a library client. Now that classes are
established, we can explain the template mechanism behind the angle brackets
and write generic abstractions ourselves. Week 12 develops copy/move and
smart-pointer ownership; this note relies on ordinary copying and library value
semantics without invoking explicit resource-transfer operations prematurely.

### 1. Templates describe families of code

```cpp
template <typename T>
T Maximum(const T& left, const T& right) {
  return left < right ? right : left;
}
```

The compiler instantiates a version for each used type. The type must support
the expressions required by the template—here, comparison with `<` and copying
the selected result. Returning by value avoids handing the caller a reference
that could outlive a temporary argument.

```cpp
int largest_int = Maximum(3, 8);
std::string largest_word = Maximum(std::string{"ant"}, std::string{"bee"});
```

Templates are compile-time polymorphism. Unlike a virtual call, the concrete
operation is normally known during compilation and can be inlined.

### 2. A small class template

```cpp
template <typename T>
class Box {
 public:
  explicit Box(const T& value) : value_{value} {
  }

  const T& value() const {
    return value_;
  }

 private:
  T value_;
};
```

Template definitions normally live in headers because a translation unit needs
the definition when instantiating a concrete type.

Avoid making a template merely to avoid naming the actual abstraction. Generic
code is valuable when multiple types share the same meaningful operation.

### Requirements before C++20 concepts

In C++17, template requirements are implicit in expressions. For `Maximum<T>`,
`left < right` must be valid and usable as a condition. A compiler error may be
long because it reports the failed instantiation path. Read from the first
expression involving your type, not only the final diagnostic line.

Create three types: one with a valid `<`, one with equality only, and one whose
comparison returns an unsuitable type. Instantiate `Maximum` and classify the
errors. Then state the requirement in a comment beside the template.

### Function objects and generic lambdas

An algorithm sometimes needs behavior supplied by its caller. For example,
sorting needs a comparison operation. A **function object** is an object that
can be called with function-call syntax; a **lambda expression** creates a small
unnamed function object at the point where the behavior is needed.

The lambda below has three parts: `[]` is the capture list, the parentheses
declare parameters, and the braced body computes the result. `auto` parameters
make its call operator generic, so the same comparison can be instantiated for
different suitable numeric types.

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

The empty capture list means the lambda uses no local variables from its
surrounding function. When local state is required, capture it deliberately:

```cpp
int threshold = 80;
auto passed = [threshold](int score) { return score >= threshold; };
```

`[threshold]` stores a copy in the function object. `[&threshold]` instead
stores an alias, so the lambda must not outlive `threshold`. Avoid broad `[=]`
and `[&]` captures in teaching code: naming each captured value makes lifetime
and mutation visible.

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
specific need. The previous course used `list` frequently; modern code should not
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

### 5. Iterator invalidation

After a vector reallocates, pointers, references, and iterators to its elements
are invalid. Inserting or erasing can invalidate additional positions even
without reallocation.

```cpp
auto position = values.begin();
values.push_back(42);
/* position may now be invalid */
```

Read the operation's invalidation rules. A valid iterator is a lifetime and
ownership claim just as a valid C pointer is. Use indices when a vector mutation
may relocate storage and the index remains meaningful, or reacquire the iterator
after mutation.

### Invalidation trace

For a vector with size 3 and capacity 4, take iterators to all elements, then
push a fourth element, insert at index 1 without reallocation, push a fifth
element with reallocation, and erase index 2. Mark valid handles after each
operation. Repeat for `list` and `map` and compare their guarantees.

### 6. Complexity belongs to the interface

For an ordered map, lookup is O(log n). For an unordered map, lookup is average
O(1) but worst-case O(n). `lower_bound` on a sorted vector is O(log n), but
inserting into the middle is O(n). A good design considers the whole workload:
build frequency, query frequency, ordering requirements, and memory overhead.

The standard library specifies both semantics and complexity. Use those
guarantees instead of assuming an internal implementation.

### Hour 2 container-and-handle checkpoint

Choose a container for a supplied workload, then mark every iterator, pointer,
and reference that survives each proposed mutation. State both the operation's
complexity and its invalidation rule before running the program.

## Hour 3 — Algorithms, associative containers, and solution pipelines

### 7. Algorithms separate traversal from intent

```cpp
#include <algorithm>
#include <numeric>

std::sort(values.begin(), values.end());

int threshold = 10;
auto first_large =
    std::find_if(values.begin(), values.end(),
                 [threshold](int value) { return value >= threshold; });

long long total = std::accumulate(values.begin(), values.end(), 0LL);
```

The initial value selects the accumulation type, so `0LL` avoids accumulating
in `int`. A wider type is still not a proof against overflow; the input contract
must bound the mathematical sum or the program must check it.

Common algorithms include:

- `find`, `find_if`, `count_if`;
- `sort`, `stable_sort`, `lower_bound`, `upper_bound`, `equal_range`;
- `copy`, `transform`, `remove_if`;
- `all_of`, `any_of`, `none_of`;
- `accumulate`.

An algorithm name states intent and centralizes boundary handling. A loop is
still correct when the operation does not fit an algorithm cleanly.

### 8. Boundary algorithms on partitioned ranges

For an ascending sorted range, the standard algorithms express the same
boundary contracts introduced with C arrays:

```cpp
auto lower = std::lower_bound(values.begin(), values.end(), target);
auto upper = std::upper_bound(values.begin(), values.end(), target);
auto equal = std::equal_range(values.begin(), values.end(), target);
```

- `lower` is the first iterator whose value is not less than `target`;
- `upper` is the first iterator whose value is greater than `target`;
- `equal` returns both boundaries as a pair.

The equal range is `[lower, upper)`. It is empty when the target is absent;
otherwise it contains every equivalent element. Use `std::distance(lower,
upper)` for a generic iterator pair. Direct subtraction works only for
random-access iterators such as `vector` iterators.

The precondition is more precisely that the range is partitioned for the
algorithm's comparison, usually because it was sorted with the same strict weak
ordering. Sorting by one key and searching as though another key defined the
order violates the contract even when the data looks mostly sorted.

For random-access iterators, these algorithms use O(log n) comparisons. With a
forward iterator they still use logarithmically many comparisons but may perform
O(n) iterator increments. This is why calling `std::lower_bound` on a linked
list does not create random access; an ordered associative container's member
`lower_bound` can follow its tree structure in O(log n).

### Comparator and projection exercise

Sort a vector of records by `(category, identifier)`. Specify, without writing
a complete query program, the comparator and search key required to find the
half-open block for one category. Explain why a comparator using `<=`, or one
that orders during sort by identifier alone, breaks the boundary precondition.
Test an empty vector, absent key, one match, repeated matches at both ends, and
a key outside the stored range.

### 9. The erase-remove pattern

`std::remove_if` rearranges retained elements and returns a new logical end; it
does not resize the container.

```cpp
values.erase(std::remove_if(values.begin(), values.end(),
                            [](int value) { return value < 0; }),
             values.end());
```

C++20 adds `std::erase_if(values, predicate)` for supported containers, but the
older form remains important when reading C++17 projects.

### 10. Maps: lookup versus insertion

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
std::vector<std::pair<std::string, int>> ranking{frequency.begin(),
                                                 frequency.end()};

std::sort(ranking.begin(), ranking.end(),
          [](const auto& left, const auto& right) {
            if (left.second != right.second) return left.second > right.second;
            return left.first < right.first;
          });
```

The map builds counts; the vector supports ranking by a different order. This is
often clearer than forcing one container to serve incompatible access patterns.
Prove the comparator is strict for equal pairs.

### 11. `optional` makes expected absence explicit

An index such as zero can be a valid answer, so it is a poor “not found”
sentinel. `std::optional<T>` contains either one `T` or no value:

```cpp
#include <iostream>
#include <optional>
#include <vector>

std::optional<std::size_t> IndexOf(const std::vector<int>& values, int target) {
  for (std::size_t index = 0; index < values.size(); ++index) {
    if (values[index] == target) return index;
  }
  return std::nullopt;
}

void ReportIndex(const std::vector<int>& values, int target) {
  if (auto index = IndexOf(values, target)) {
    std::cout << "found at " << *index << '\n';
  }
}
```

Testing an optional as a condition asks whether it contains a value. Dereference
with `*` only after that check; `.value()` instead throws `std::bad_optional_access`
when empty. Use `optional` when absence is an expected result, exceptions when an
operation cannot fulfill its contract, and a richer result type when callers
need distinct failure reasons. The graph lecture will use this distinction for
“no path exists.”

### Hour 3 integration task

Read words, normalize case, count with a map/unordered map, remove stop words,
rank by frequency then spelling, and print the top `k`. Test empty input, ties,
repeated punctuation policy, and very large counts. Benchmark ordered versus
unordered counting on supplied data and explain results without overgeneralizing
from one machine.

## Final project connection — Container mutation is a lifetime event

Locate one template loop that updates or removes game objects from a container.
Record the container type, element ownership, mutation operation, invalidated
iterators/references, and destruction point. Then compare three safe designs:
the container's erase-return idiom, erase/remove where appropriate, and a
two-phase mark-then-sweep update. Choose based on the actual container and
whether callbacks or collisions still borrow the object.

Thursday's lab turns these contracts into AI-free exam practice followed by a
container-mutation trace in the final-project template. Since this note is in
the Midterm 2 scope, students must be able to state the ordering, range,
complexity, and invalidation rules without AI. AI may review additional tests
only after the student records an independent prediction.

## Check yourself

1. What operation does the `Maximum` template require from `T`?
2. Select containers for a FIFO worklist, ordered dictionary, and dense table.
3. Why is `[begin, end)` easier to represent than an inclusive end?
4. Distinguish the results of `lower_bound`, `upper_bound`, and `equal_range`.
5. Why can `std::lower_bound` perform linear iterator movement on a list?
6. What exactly does `map[key]` do when `key` is absent?
7. When should a search return `optional<T>` rather than throw an exception?
8. Identify invalid iterators after a vector insertion in the middle.

## Summary

- Templates express type-safe compile-time generality.
- Container selection follows operations, guarantees, and data layout.
- Iterators generalize positions and half-open ranges.
- Algorithms expose intent and reduce repeated traversal code.
- Boundary algorithms require a consistently partitioned range and return
  iterators delimiting an equal block.
- `optional<T>` distinguishes an expected missing result from a stored value.
- Mutation can invalidate iterators; complexity and lifetime remain correctness concerns.

## References and source materials

- [Classes III: templates and related material](<https://github.com/htchen/i2p-nthu/blob/master/程式設計二/Classes%20III/README.md>)
- [Standard library](<https://github.com/htchen/i2p-nthu/blob/master/程式設計二/week%2012%20Standard%20library/week%2012%20Standard%20library.md>)
- [2025 Week 11 notebook (Colab)](https://colab.research.google.com/drive/1RjtHSu-82v1dQt-p2teRmKwjV9bLlQsK)
- [2025 Week 14 notebook: `optional` and modern C++ (Colab)](https://colab.research.google.com/drive/1CEwhynoePTk_ZG6pgAxJH4mMqsQsZyzu)
