# Week 8 Lecture Notes — From C to C++: Values, References, and RAII

> October 27, 2026 · Source lineage: previous C++ introduction and 2025 Week 7
> notebook; examples remain compatible with C++17

> Python bridge: [Python Contrast Companion for Week 8](week08_python_companion.md)

## Learning objectives

By the end of this lecture, you should be able to:

1. Explain how C++ extends rather than replaces the C machine model.
2. Use `std::string`, `std::vector`, references, and `const`.
3. Use range-based loops while preserving explicit mutation and ownership
   contracts.
4. Trace `throw`, handler selection, and stack unwinding.
5. Explain how deterministic destruction gives library values safe resource
   lifetimes.
6. Refactor a manual C resource into a standard-library value.

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | Which C++ library values replace common manual C representations? | Refactor a C input/array program to strings and vectors |
| 2 | How do references, `const`, and range loops express borrowing and mutation? | Annotate and implement parameter contracts |
| 3 | How do exceptions and RAII make failure paths safe? | Trace unwinding, resource lifetimes, and a file-processing pipeline |

## Hour 1 — C++ compilation, I/O, strings, and vectors

### 1. Compile C++ as C++

```sh
c++ -std=c++17 program.cpp -o program
```

This is the minimum command used in the first demonstration: `-std=c++17`
selects the language version, `program.cpp` is the source file, and `-o program`
names the executable. During development, add the warning and debug options used
throughout the course:

| Option | Purpose |
|--------|---------|
| `-Wall -Wextra -Wpedantic` | ask the compiler to report many suspicious constructs |
| `-g` | keep information that helps a debugger show source lines and variables |

These are compiler options, not C++ syntax. You do not need to memorize the
whole command; use the course build template and learn what each part controls.

Although much C syntax is accepted, C++ is a different language with stronger
type checking, overloading, references, classes, templates, exceptions, and a
large standard library. Do not compile a `.c` file as C++ and assume the result
describes portable C.

```cpp
#include <iostream>
#include <string>

int main() {
  std::string language = "C++";
  std::cout << "Now learning " << language << '\n';
}
```

`std` is a **namespace**: a named region that groups standard-library names.
The `::` operator selects a name from that region, so `std::cout` means “the
`cout` supplied by the standard library.” C has no namespace feature, which is
why this syntax is new here.

`std::cout << value` sends a value to the output stream, and
`std::cin >> value` reads a formatted value from the input stream. The symbols
`<<` and `>>` also mean bit shifts for integers; C++ allows library types to give
operators a type-appropriate meaning. For now, read a chain from left to right:

```cpp
int age{0};
std::cout << "Age? ";
if (!(std::cin >> age) || age < 0 || age > 150) {
  std::cerr << "expected an age from 0 through 150\n";
  return 1;
}
std::cout << "Next year: " << age + 1 << '\n';
```

Prefer `std::` qualification in teaching examples. A global `using namespace
std;` imports many names at once, which can create collisions and hides where a
facility came from. It is common in short contest solutions, but explicit
qualification is clearer while learning the library.

### 2. Prefer library values over manual buffers

```cpp
std::string name;
std::getline(std::cin, name);
std::cout << name << " has " << name.size() << " characters\n";
```

`std::string` manages its storage, maintains its own size, and supports value
operations such as assignment and comparison.

A `std::vector<T>` is a resizable sequence of values of type `T`. It plays a
role similar to a Python list, but every element has the same declared type:

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

## Hour 2 — References, `const`, and range-based traversal

### 3. References are aliases

```cpp
void SwapValues(int& left, int& right) {
  int temporary = left;
  left = right;
  right = temporary;
}
```

A reference must refer to an object when initialized and is used with ordinary
value syntax. It is not a reseatable, nullable handle like a pointer.

The change is visible through the original variable:

```cpp
int SetToSeventeen(int& value) {
  value = 17;
  return 15;
}

int number = 13;
std::cout << number << '\n';                 /* 13 */
std::cout << SetToSeventeen(number) << '\n'; /* 15 */
std::cout << number << '\n';                 /* 17 */
```

The function returns `15`, but its reference parameter changes `number` to
`17`. Contrast this with a value parameter, which would change only a copy.

To connect the C pointer model with the new reference syntax, run the same swap
through three parameter contracts:

```cpp
void SwapByValue(int left, int right) {
  int temporary = left;
  left = right;
  right = temporary;
}

void SwapByPointer(int* left, int* right) {
  int temporary = *left;
  *left = *right;
  *right = temporary;
}

void SwapByReference(int& left, int& right) {
  int temporary = left;
  left = right;
  right = temporary;
}
```

The value version changes two local copies. The pointer and reference versions
both change the caller's objects. The pointer call must explicitly pass
addresses; because this implementation dereferences both parameters, its
contract requires two non-null pointers. The reference call uses ordinary
expression syntax and requires two objects:

```cpp
int x = 5;
int y = 7;
SwapByValue(x, y);       /* x == 5, y == 7 */
SwapByPointer(&x, &y);   /* x == 7, y == 5 */
SwapByReference(x, y);   /* x == 5, y == 7 */
```

The standalone example prints every stage so the three contracts can be
predicted before compilation.

Parameter guidelines:

```cpp
void Print(const std::vector<int>& values);        /* borrow, read only */
void Sort(std::vector<int>& values);               /* borrow, may modify */
std::vector<int> Doubled(std::vector<int> values); /* local copy/value */
```

Use a pointer when null is meaningful or pointer arithmetic/low-level interop is
required. Use a reference for a required borrowed object.

### 4. `const` makes interfaces readable

```cpp
double Mean(const std::vector<int>& values) {
  if (values.empty()) return 0.0;
  double total = 0.0;
  for (int value : values) total += value;
  return total / static_cast<double>(values.size());
}
```

The signature says the vector is required, borrowed, and not modified. The
range loop copies each `int`, which is appropriate for a small scalar.
Accumulating in `double` avoids signed-integer overflow, although a large sum
can round and floating-point overflow must still be considered for an
unrestricted numeric interface.

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

### 5. A range loop is still an ownership decision

A range-based loop expresses “visit every element,” but the loop variable still
determines whether each element is copied, observed, or modified:

```cpp
for (int score : scores) {
  std::cout << score << '\n'; /* copy a small scalar */
}

for (const std::string& name : names) {
  std::cout << name << '\n'; /* borrow without copying */
}

for (double& weight : weights) {
  weight /= total_weight; /* borrow and modify */
}
```

The first form gives the loop body an independent value. The second and third
forms create aliases to elements owned by the container. Those aliases are
valid only while the elements remain alive and in place. Do not change a
vector's size inside a loop that holds references to its elements; reallocation
can invalidate the current reference and the loop's internal position.

This is not merely shorter loop syntax. It is another place to state the same
copy/borrow/mutate contract used by function parameters.

### Parameter and traversal audit

For each operation, choose `T`, `T&`, `const T&`, or `T*` and explain why:

1. print a vector;
2. sort it in place;
3. accept an optional output destination;
4. take ownership of a string for long-term storage;
5. return a filtered vector.

Then rewrite one index loop as a range loop. State whether its loop variable is
a copy, a read-only borrow, or a mutable borrow, and explain what container
changes would invalidate that borrow. Week 11 introduces iterators, algorithms,
and lambdas after classes and callable objects have a proper foundation.

## Hour 3 — Deterministic lifetime and value-oriented design

### 6. Exceptions transfer control to a handler

Python programmers already know exception-based failure. C++ makes object
destruction during that transfer part of the language model:

```cpp
#include <iostream>
#include <stdexcept>

void RequireNonnegative(int value) {
  if (value < 0) {
    throw std::invalid_argument{"negative value"};
  }
}

int main() {
  try {
    RequireNonnegative(-1);
  } catch (const std::invalid_argument& error) {
    std::cerr << error.what() << '\n';
  }
}
```

`throw` stops the current path and searches outward for a matching handler.
Automatic objects in exited scopes are destroyed in reverse construction order;
this is **stack unwinding**. Catch standard exceptions by `const` reference to
avoid copying and slicing. If no matching handler exists, the program terminates.

For a library value such as `std::string`, `std::vector`, or `std::ofstream`,
**construction** establishes a usable object and **destruction** ends its
lifetime and releases resources it owns. C++ invokes destruction automatically
for ordinary local objects when their scope is exited. Week 9 explains how to
define these lifetime rules for a class of our own; this week first uses the
guarantee as a client of well-designed library types.

Use exceptions for failures that prevent an operation from fulfilling its
contract, not for ordinary loop or selection logic. Constructors cannot return
an error code, so throwing is a conventional way to report construction failure.
Later lectures build on this model when constructors validate invariants and
copy operations promise exception-safety guarantees.

### 7. RAII: lifetime controls resources

RAII means **Resource Acquisition Is Initialization**. An object establishes
its invariant and acquires resources during construction; its destructor
releases them when the object's lifetime ends.

```cpp
#include <fstream>
#include <stdexcept>

void WriteReport(const std::string& path) {
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

### 8. Values first

Prefer automatic-duration value objects:

```cpp
std::vector<std::string> names;
```

Before requesting manual allocation, ask whether a standard container,
`std::string`, or an ordinary class member can own the resource. Week 12
introduces C++'s low-level allocation syntax only long enough to explain why
ordinary application code should prefer RAII owners.

Returning a value is idiomatic and efficient:

```cpp
std::vector<int> EvenValues(const std::vector<int>& input) {
  std::vector<int> result;
  for (int value : input) {
    if (value % 2 == 0) result.push_back(value);
  }
  return result;
}
```

The caller receives an independent vector that owns its elements. C++ is
designed to return such values efficiently; Week 12 explains the copy and
resource-transfer operations that support this model. The interface should
express the simple ownership result now without exposing those implementation
mechanisms prematurely.

### Nested RAII lifetimes

```cpp
std::vector<std::string> ReadWords(const std::string& path) {
  std::ifstream input{path};
  if (!input) throw std::runtime_error{"cannot open " + path};

  std::vector<std::string> words;
  for (std::string word; input >> word;) {
    words.push_back(word);
  }
  if (!input.eof()) throw std::runtime_error{"read failure"};
  return words;
}
```

Trace construction and destruction for `input`, each loop-local `word`, the
vector, its elements, and the returned value on normal return and on each throw.
No explicit cleanup appears because every owner has a destructor.

### Hour 3 integration task

Build a program that reads words from an `ifstream` into a `vector<string>` and
reports the total count and longest word. It must report open/read failures,
avoid global state, and return collected results by value. Compare its cleanup
proof with a C version that uses `FILE *`, allocated strings, and multiple error
labels. Sorting, iterators, and custom comparison behavior are developed in
Week 11.

## Final project connection — Event loop and resource lifetime

Before adding gameplay, trace the template's control flow from one input event
through state handling, update, and draw. These phases have different
responsibilities: input records intent, update changes model state, and draw
observes that state. A feature that mutates game state during drawing is harder
to reason about and test.

Choose one Allegro resource and trace acquisition, registration/use, normal
release, and initialization failure. The existing template may use explicit C
handles even though the surrounding program is C++. Treat that as an ownership
boundary: first document the actual lifetime, then consider a small RAII wrapper
rather than rewriting the resource subsystem. Thursday's evidence is an event
trace and resource-lifecycle table.

## Check yourself

1. When should a parameter be `const T&`, `T&`, or `T`?
2. Replace a C character buffer with `std::string` and list removed failure modes.
3. Why can changing a vector's size invalidate references to its elements?
4. Which objects are destroyed when an exception leaves two nested scopes?
5. What resource does a vector's destructor release?
6. Refactor a `malloc`/`free` integer array into `std::vector<int>`.

## Summary

- C++ preserves explicit performance while adding strong value abstractions.
- Strings and vectors own storage and know their sizes.
- References express required borrowing; `const` exposes non-mutation.
- Range loops and `auto` are useful only when their copy and borrowing behavior
  remains clear.
- Exceptions transfer control while stack unwinding destroys automatic objects.
- RAII binds resource cleanup to deterministic object lifetime.

## References and source materials

- [Introduction to C++](<https://github.com/htchen/i2p-nthu/blob/master/程式設計二/Intro/README.md>)
- [2025 Week 7 notebook (Colab)](https://colab.research.google.com/drive/1oHBcNeAXt4ZeQJsdG2q4RU5m9Yu_9CCw)
