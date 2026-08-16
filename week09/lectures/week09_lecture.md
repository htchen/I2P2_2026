# Lecture 8 — From C to C++: Values, References, and RAII

> November 3, 2026 · Source lineage: the legacy C++ introduction and 2025 Week 7
> notebook; examples remain compatible with C++17

## Learning objectives

By the end of this lecture, you should be able to:

1. Explain how C++ extends rather than replaces the C machine model.
2. Use `std::string`, `std::vector`, references, and `const`.
3. Write range-based loops and small lambdas.
4. Explain deterministic destruction and RAII.
5. Refactor a manual C resource into a standard-library value.

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | Which C++ library values replace common manual C representations? | Refactor a C input/array program to strings and vectors |
| 2 | How do references, `const`, `auto`, and lambdas express borrowing and behavior? | Annotate and implement parameter/capture contracts |
| 3 | How does RAII make every control-flow path safe? | Trace resource lifetimes and build a file-processing pipeline |

## Hour 1 — C++ compilation, I/O, strings, and vectors

### 1. Compile C++ as C++

```sh
c++ -std=c++17 -Wall -Wextra -Wpedantic -g program.cpp -o program
```

Although much C syntax is accepted, C++ is a different language with stronger
type checking, overloading, references, classes, templates, exceptions, and a
large standard library. Do not compile a `.c` file as C++ and assume the result
describes portable C.

```cpp
#include <iostream>
#include <string>

int main()
{
    std::string language = "C++";
    std::cout << "Now learning " << language << '\n';
}
```

Prefer `std::` qualification in teaching examples. A global `using namespace
std;` can make names collide and hides where facilities originate.

### 2. Prefer library values over manual buffers

```cpp
std::string name;
std::getline(std::cin, name);
std::cout << name << " has " << name.size() << " characters\n";
```

`std::string` manages its storage, maintains its own size, and supports value
operations such as assignment and comparison.

```cpp
std::vector<int> scores{91, 82, 73};
scores.push_back(94);
scores.push_back(85);
```

A vector combines a dynamic array with ownership and size information.

| Manual C dynamic array | `std::vector<int>` |
|------------------------|--------------------|
| pointer + size + capacity variables | one value maintains all three |
| explicit allocation and `free` | automatic resource management |
| manual copying and resizing | assignment, `push_back`, `reserve` |
| easy to leak on early return | destructor releases storage |

Use `.at(i)` when checked access is useful and `operator[]` when bounds have
already been established.

### Stream state and robust input

```cpp
std::vector<int> values;
for (int value; std::cin >> value;) {
    values.push_back(value);
}

if (!std::cin.eof()) {
    std::cerr << "input contained a non-integer token\n";
    return 1;
}
```

The stream itself is tested as a condition. Extraction either succeeds and
updates `value`, or sets failure state. This mirrors the checked `scanf` loop
without exposing format strings or destination addresses.

When mixing `operator>>` with `getline`, remember that formatted extraction
usually leaves the newline in the stream. Consume the remainder deliberately or
use line-oriented input consistently.

### Initialization forms

```cpp
int count{0};
std::string title{"I2P II"};
std::vector<int> first(5, 0);  /* five zeroes */
std::vector<int> second{5, 0}; /* two elements: 5 and 0 */
```

Brace initialization rejects many narrowing conversions, but constructor syntax
can differ when an initializer-list overload exists. Predict `size()` for both
vectors before compiling.

### Hour 1 refactoring studio

Take the Week 2 C program that reads a bounded character array and a fixed score
array. Replace ownership and capacity management with `std::string` and
`std::vector<int>`, while preserving validation and output. List which C failure
modes disappear and which domain errors remain.

## Hour 2 — References, `const`, deduction, and local callables

### 3. References are aliases

```cpp
void swap_values(int& left, int& right)
{
    int temporary = left;
    left = right;
    right = temporary;
}
```

A reference must refer to an object when initialized and is used with ordinary
value syntax. It is not a reseatable, nullable handle like a pointer.

Parameter guidelines:

```cpp
void print(const std::vector<int>& values); /* borrow, read only */
void sort(std::vector<int>& values);        /* borrow, may modify */
std::vector<int> doubled(std::vector<int> values); /* local copy/value */
```

Use a pointer when null is meaningful or pointer arithmetic/low-level interop is
required. Use a reference for a required borrowed object.

### 4. `const` makes interfaces readable

```cpp
double mean(const std::vector<int>& values)
{
    if (values.empty()) return 0.0;
    long long total = 0;
    for (int value : values) total += value;
    return static_cast<double>(total) / values.size();
}
```

The signature says the vector is required, borrowed, and not modified. The
range loop copies each `int`, which is appropriate for a small scalar.

For larger elements:

```cpp
for (const std::string& word : words) {
    std::cout << word << '\n';
}
```

`auto` can reduce repetition without removing static types:

```cpp
for (const auto& word : words) {
    /* word is const std::string& */
}
```

Use `auto` when the initializer makes the type clear; spell the type when it
communicates an important unit, conversion, or ownership decision.

### `decltype` preserves an expression's type

The legacy C++ introduction included `decltype`:

```cpp
int value = 0;
int& alias = value;

decltype(value) another = 1;          /* int */
decltype((value)) reference = value;  /* int&: (value) is an lvalue */
```

This distinction matters in generic code, but should not replace readable
explicit types in ordinary application code. Parentheses can change the result.

### 5. Lambdas are local callable objects

```cpp
#include <algorithm>

std::sort(scores.begin(), scores.end(),
          [](int left, int right) { return left > right; });
```

The capture list states which surrounding values the lambda may use:

```cpp
int threshold = 80;
auto passed = [threshold](int score) { return score >= threshold; };
```

- `[]`: capture nothing.
- `[threshold]`: capture one value by value.
- `[&threshold]`: capture by reference; the referred object must remain alive.

Avoid broad `[&]` or `[=]` captures in long-lived lambdas because ownership and
lifetime become difficult to see.

### Parameter and capture audit

For each operation, choose `T`, `T&`, `const T&`, or `T*` and explain why:

1. print a vector;
2. sort it in place;
3. accept an optional output destination;
4. take ownership of a string for long-term storage;
5. return a filtered vector.

Then audit three lambdas: one invoked immediately, one stored in a local vector
of callbacks, and one registered with a longer-lived game scene. A reference
capture safe in the first case may dangle in the third.

### Algorithm preview

```cpp
std::vector<int> positives;
std::copy_if(values.begin(), values.end(), std::back_inserter(positives),
             [](int value) { return value > 0; });

std::transform(positives.begin(), positives.end(), positives.begin(),
               [](int value) { return value * value; });
```

This connects Python comprehensions to typed algorithms. Check overflow before
claiming the result is equivalent for all Python integers.

## Hour 3 — Deterministic lifetime and value-oriented design

### 6. RAII: lifetime controls resources

RAII means **Resource Acquisition Is Initialization**. An object establishes
its invariant and acquires resources during construction; its destructor
releases them when the object's lifetime ends.

```cpp
#include <fstream>
#include <stdexcept>

void write_report(const std::string& path)
{
    std::ofstream output(path);
    if (!output) {
        throw std::runtime_error("cannot open report");
    }
    output << "complete\n";
} /* output is closed here, including during exception unwinding */
```

The same principle manages vectors, strings, locks, sockets, and smart pointers.
RAII turns every control-flow path—normal return, early return, or exception—
into deterministic cleanup.

### 7. Values first

Prefer automatic-duration value objects:

```cpp
std::vector<std::string> names;
```

Before writing `new`, ask whether a standard container, `std::string`, or an
ordinary class member can own the resource. Modern C++ application code rarely
needs direct `new`/`delete`.

Returning a value is idiomatic and efficient:

```cpp
std::vector<int> even_values(const std::vector<int>& input)
{
    std::vector<int> result;
    for (int value : input) {
        if (value % 2 == 0) result.push_back(value);
    }
    return result;
}
```

Copy elision and moves allow the language to transfer or construct the result
efficiently without exposing manual ownership.

### Nested RAII lifetimes

```cpp
std::vector<std::string> read_words(const std::string& path)
{
    std::ifstream input{path};
    if (!input) throw std::runtime_error{"cannot open " + path};

    std::vector<std::string> words;
    for (std::string word; input >> word;) {
        words.push_back(std::move(word));
    }
    if (!input.eof()) throw std::runtime_error{"read failure"};
    return words;
}
```

Trace construction and destruction for `input`, each loop-local `word`, the
vector, its elements, and the returned value on normal return and on each throw.
No explicit cleanup appears because every owner has a destructor.

### Hour 3 integration task

Build a word-frequency program using `ifstream`, `string`, `vector`, sorting,
and a lambda comparator. It must report open/read failures, avoid global state,
and return all results by value. Compare its cleanup proof with the C version
that uses `FILE *`, allocated strings, and multiple error labels.

## Check yourself

1. When should a parameter be `const T&`, `T&`, or `T`?
2. Replace a C character buffer with `std::string` and list removed failure modes.
3. Why does a captured reference have a lifetime requirement?
4. What resource does a vector's destructor release?
5. Refactor a `malloc`/`free` integer array into `std::vector<int>`.

## Summary

- C++ preserves explicit performance while adding strong value abstractions.
- Strings and vectors own storage and know their sizes.
- References express required borrowing; `const` exposes non-mutation.
- Range loops, `auto`, and lambdas make generic code readable when used precisely.
- RAII binds resource cleanup to deterministic object lifetime.
