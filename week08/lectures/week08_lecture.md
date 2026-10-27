# Week 8 Lecture Notes — From C to C++: Values, References, and RAII

> October 27, 2026 · Source lineage: previous C++ introduction and 2025 Week 7
> notebook; examples remain compatible with C++17

> Python bridge: [Python Contrast Companion for Week 8](week08_python_companion.md)

---

## Student route

- **Core:** use streams, `std::string`, `std::vector`, references, and `const`,
  then trace how RAII releases local resources on every exit path.
- **Practice:** complete the [Week 8 exercise](lecture_exercises/week08_ex.md)
  before comparing with [the complete example](examples.cpp).
- **Supporting ideas:** exception taxonomy and explicit cast syntax support the
  core model. Resource-transfer design is deferred to Week 12; this week uses
  standard-library values rather than custom ownership-taking interfaces.
- **Python bridge:** consult the companion for a familiar baseline, not as a
  substitute for C++ lifetime reasoning.

---

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

---

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | Which C++ library values replace common manual C representations? | Refactor a C input/array program to strings and vectors |
| 2 | How do references, `const`, and range loops express borrowing and mutation? | Annotate and implement parameter contracts |
| 3 | How do exceptions and RAII make failure paths safe? | Trace unwinding, resource lifetimes, and a file-processing pipeline |

Each hour alternates explanation, state or lifetime traces, live coding, and
short practice. Exercises labelled **Core live** belong to the planned
classroom route. Exercises labelled **Extension** may move to the Thursday lab
or independent study when the class needs more time for the C-to-C++ transition.

### Inline practice routine

For each **Try it now** activity:

1. predict the output, object state, parameter effect, exception path, or
   destruction order before running code;
2. classify every relevant value as an owner, copy, read-only borrow, or mutable
   borrow;
3. compile valid examples with
   `c++ -std=c++17 -Wall -Wextra -Wpedantic`;
4. compare the observation with the hidden solution; and
5. explain which behavior comes from a core programming idea and which part is
   specific C++ syntax or library support.

Only the question is visible initially. Expand **Reveal solution** after making
an attempt. Every solution states whether its text is terminal output, file
content, a state/lifetime trace, or an explanation that the fragment itself
performs no input/output.

- **Core live:** part of the planned in-class route.
- **Extension:** additional practice for the lab or later review.

The core-live activities total about 22 minutes in Hour 1, 21 minutes in Hour
2, and 17 minutes in Hour 3. The remaining time is for explanation, live
coding, questions, transitions, and a short break.

---

## Hour 1 — C++ compilation, I/O, strings, and vectors

> **Hour 1 route:** [Compile C++ as C++](#1-compile-c-as-c)
> → [Prefer library values over manual buffers](#2-prefer-library-values-over-manual-buffers)
> → [Stream state and robust input](#stream-state-and-robust-input)
> → [Initialization forms](#initialization-forms)
> → [refactoring studio](#hour-1-refactoring-studio)

### 1. Compile C++ as C++

```sh
c++ -std=c++17 program.cpp -o program
```

On success, the compiler normally prints nothing and creates the executable
named `program`. Diagnostics are compiler output; the program's output appears
only after the executable is run.

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

This program writes the following line to standard output:

```text
Now learning C++
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
#include <iostream>

int main() {
  int age{0};
  std::cout << "Age? ";
  if (!(std::cin >> age) || age < 0 || age > 150) {
    std::cerr << "expected an age from 0 through 150\n";
    return 1;
  }
  std::cout << "Next year: " << age + 1 << '\n';
}
```

Prefer `std::` qualification in teaching examples. A global `using namespace
std;` imports many names at once, which can create collisions and hides where a
facility came from. It is common in short contest solutions, but explicit
qualification is clearer while learning the library.

#### Try it now [Core live] — separate compilation from execution (3 minutes)

Compile and run the first program. What output belongs to the compiler, what
output belongs to the executable, and what does `std::` communicate?

<details>
<summary>Reveal solution</summary>

A successful compiler invocation ordinarily has no terminal output; it creates
`program`. Running that executable produces:

```text
Now learning C++
```

`std::string` and `std::cout` explicitly select names from the standard-library
namespace. The namespace qualification is C++ syntax; separating translation
from execution is the same core model introduced for C.

</details>

#### Try it now [Extension] — follow formatted input state (4 minutes)

For the age program, predict the visible output and exit status for inputs
`20`, `-1`, and `abc`. Identify which failures come from formatted extraction
and which come from the program's domain rule.

<details>
<summary>Reveal solution</summary>

With input `20`, the interaction is:

```text
Age? 20
Next year: 21
```

The `20` shown after the prompt is normally terminal echo, not text printed by
the program. The program returns success. Input `-1` is extracted as an integer
but violates the range rule; `abc` makes extraction fail. Both failure cases
write this diagnostic to standard error and return `1`:

```text
expected an age from 0 through 150
```

</details>

---

### 2. Prefer library values over manual buffers

```cpp
#include <iostream>
#include <string>

int main() {
  std::string name;
  std::getline(std::cin, name);
  std::cout << name << " has " << name.size() << " characters\n";
}
```

`std::string` manages its storage, maintains its own size, and supports value
operations such as assignment and comparison.

A `std::vector<T>` is a resizable sequence of values of type `T`. It plays a
role similar to a Python list, but every element has the same declared type:

```cpp
#include <iostream>
#include <vector>

int main() {
  std::vector<int> scores{91, 82, 73};
  scores.push_back(94);
  scores.push_back(85);

  for (int score : scores) std::cout << score << ' ';
  std::cout << '\n';
}
```

Neither `push_back` call prints anything. After both calls, `scores` owns five
integers, and the final loop prints them:

```text
91 82 73 94 85 
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

#### Try it now [Core live] — trace two owning library values (4 minutes)

If the string input is `Ada Lovelace`, predict its output. Then copy `scores`
to `backup`, change `scores.at(0)` to `100`, and state the contents of both
vectors. Which operation demonstrates value semantics?

<details>
<summary>Reveal solution</summary>

The string fragment prints:

```text
Ada Lovelace has 12 characters
```

After the vector copy and mutation:

```text
scores: 100 82 73 94 85
backup:  91 82 73 94 85
```

Assignment or initialization of `backup` creates an independent vector value;
changing an element owned by `scores` does not change `backup`. The two state
lines are an illustrative trace unless a driver explicitly prints them.

</details>

---

### Stream state and robust input

```cpp
#include <iostream>
#include <vector>

int main() {
  std::vector<int> values;
  for (int value; std::cin >> value;) {
    values.push_back(value);
  }

  if (!std::cin.eof()) {
    std::cerr << "input contained a non-integer token\n";
    return 1;
  }
  std::cout << "read " << values.size() << " integers\n";
}
```

For all-integer input such as `10 20 30`, the program stores every value and
prints `read 3 integers`. A non-integer token produces the shown diagnostic on
standard error and returns `1`.

The stream itself is tested as a condition. Extraction either succeeds and
updates `value`, or sets failure state. This mirrors the checked `scanf` loop
without exposing format strings or destination addresses.

When mixing `operator>>` with `getline`, remember that formatted extraction
usually leaves the newline in the stream. Consume the remainder deliberately or
use line-oriented input consistently.

#### Try it now [Core live] — distinguish failure from end of file (4 minutes)

Trace the loop for input `10 20 x 30`. Which values are appended, what stream
condition ends the loop, and why does the later `eof()` check not excuse the
failure?

<details>
<summary>Reveal solution</summary>

The first two extractions succeed, so the vector contains `10, 20`. Extraction
of `x` fails, sets the stream's failure state, and leaves `x` unread. The loop
ends because the stream no longer converts to a successful condition. End of
file was not encountered, so `eof()` is false and the program prints:

```text
input contained a non-integer token
```

The trailing `30` is never extracted. This is both a stream-state trace and the
exact standard-error output.

</details>

---

### Initialization forms

```cpp
#include <iostream>
#include <string>
#include <vector>

void PrintVector(const char* label, const std::vector<int>& values) {
  std::cout << label << ".size() = " << values.size() << ", elements =";
  for (int value : values) std::cout << ' ' << value;
  std::cout << '\n';
}

int main() {
  int count{0};
  std::string title{"I2P II"};
  std::vector<int> first(5, 0);  /* five zeroes */
  std::vector<int> second{5, 0}; /* two elements: 5 and 0 */

  std::cout << title << ' ' << count << '\n';
  PrintVector("first", first);
  PrintVector("second", second);
}
```

The declarations themselves produce no output; the statements after them print
the objects' state.

Brace initialization rejects many narrowing conversions, but constructor syntax
can differ when an initializer-list overload exists. Predict `size()` for both
vectors before compiling.

#### Try it now [Core live] — do not guess from punctuation (3 minutes)

Predict `first.size()`, `second.size()`, and every element. Why is this not a
general rule that parentheses always mean “repeat” and braces always mean
“list”?

<details>
<summary>Reveal solution</summary>

```text
I2P II 0
first.size() = 5, elements = 0 0 0 0 0
second.size() = 2, elements = 5 0
```

This is the exact program output; the declarations alone print nothing. The
difference comes from overload resolution: parentheses select the vector's
count/value constructor, while braces prefer its initializer-list constructor.
Other types and overload sets can behave differently.

</details>

---

### Hour 1 refactoring studio

#### Try it now [Core live] — preserve behavior while changing representation (8 minutes)

Take the Week 2 C program that reads a bounded character array and a fixed score
array. Replace ownership and capacity management with `std::string` and
`std::vector<int>`, while preserving validation and output. List which C failure
modes disappear and which domain errors remain.

<details>
<summary>Reveal solution</summary>

A suitable design uses `std::getline` for the name and formatted extraction
into a vector for the scores. The exact program depends on the chosen Week 2
example, but the contract comparison should include:

| Removed manual concern | Still required |
|------------------------|----------------|
| character-buffer capacity and terminator placement | decide whether an empty name is valid |
| array capacity bookkeeping and manual resizing | reject malformed or out-of-range scores |
| explicit release of dynamic storage | define behavior for empty score input |
| cleanup branches for owned buffers | report input failure clearly |

The refactoring should preserve the earlier program's visible output for valid
input. This solution is a design and failure-mode trace rather than one fixed
output transcript.

</details>

---

## Hour 2 — References, `const`, and range-based traversal

> **Hour 2 route:** [References are aliases](#3-references-are-aliases)
> → [`const` makes interfaces readable](#4-const-makes-interfaces-readable)
> → [A range loop is still an ownership decision](#5-a-range-loop-is-still-an-ownership-decision)
> → [Parameter and traversal audit](#parameter-and-traversal-audit)

### 3. References are aliases

```cpp
#include <iostream>

void SwapValues(int& left, int& right) {
  int temporary = left;
  left = right;
  right = temporary;
}

int main() {
  int a = 1;
  int b = 2;
  SwapValues(a, b);
  std::cout << a << ' ' << b << '\n'; /* 2 1 */
}
```

The function itself prints nothing. Its observable effect is the exchanged
state of the two caller-owned integers, which `main` prints as `2 1`.

A reference must refer to an object when initialized and is used with ordinary
value syntax. It is not a reseatable, nullable handle like a pointer.

The change is visible through the original variable:

```cpp
#include <iostream>

int SetToSeventeen(int& value) {
  value = 17;
  return 15;
}

int main() {
  int number = 13;
  std::cout << number << '\n';                 /* 13 */
  std::cout << SetToSeventeen(number) << '\n'; /* 15 */
  std::cout << number << '\n';                 /* 17 */
}
```

The function returns `15`, but its reference parameter changes `number` to
`17`. Contrast this with a value parameter, which would change only a copy.

#### Try it now [Core live] — separate a return value from a side effect (3 minutes)

Predict all three output lines from the `SetToSeventeen` example. Immediately
after the function returns, what value belongs to the expression and what value
belongs to `number`?

<details>
<summary>Reveal solution</summary>

```text
13
15
17
```

The call expression has the returned value `15`, while the reference parameter
has changed the caller's `number` to `17`. These are two independent effects of
one call.

</details>

To connect the C pointer model with the new reference syntax, run the same swap
through three parameter contracts:

```cpp
#include <iostream>

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

void PrintPair(const char* label, int left, int right) {
  std::cout << label << ": (" << left << ", " << right << ")\n";
}

int main() {
  int x = 5;
  int y = 7;
  PrintPair("initial", x, y);
  SwapByValue(x, y);     /* x == 5, y == 7 */
  PrintPair("by value", x, y);
  SwapByPointer(&x, &y); /* x == 7, y == 5 */
  PrintPair("by pointer", x, y);
  SwapByReference(x, y); /* x == 5, y == 7 */
  PrintPair("by reference", x, y);
}
```

The value version changes two local copies. The pointer and reference versions
both change the caller's objects. The pointer call must explicitly pass
addresses; because this implementation dereferences both parameters, its
contract requires two non-null pointers. The reference call uses ordinary
expression syntax and requires two objects.

The swap calls themselves print nothing; their comments state the expected
caller-visible state, and `PrintPair` prints every stage so the three contracts
can be predicted before compilation. The same calls appear in
[the complete example](examples.cpp).

#### Try it now [Core live] — compare copy, pointer, and reference parameters (5 minutes)

Starting from `(x, y) == (5, 7)`, trace the three calls in order. For each
function, identify what its parameters denote and predict the corresponding
line printed by the complete example.

<details>
<summary>Reveal solution</summary>

```text
initial: (5, 7)
by value: (5, 7)
by pointer: (7, 5)
by reference: (5, 7)
```

- Value parameters are independent local copies, so the first call cannot
  change `x` or `y`.
- Pointer parameters receive the addresses of the caller's integers and mutate
  them through dereferencing.
- Reference parameters are aliases for the caller's integers and swap them
  again using ordinary expression syntax.

The four lines are exact output from the program above and from the first part
of `examples.cpp`.

</details>

Parameter guidelines:

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

void Print(const std::vector<int>& values);        /* borrow, read only */
void Sort(std::vector<int>& values);               /* borrow, may modify */
std::vector<int> Doubled(std::vector<int> values); /* local copy/value */

void Print(const std::vector<int>& values) {
  for (int value : values) std::cout << value << ' ';
  std::cout << '\n';
}

void Sort(std::vector<int>& values) {
  std::sort(values.begin(), values.end());
}

std::vector<int> Doubled(std::vector<int> values) {
  for (int& value : values) value *= 2;
  return values;
}

int main() {
  std::vector<int> data{3, 1, 2};
  std::vector<int> twice = Doubled(data);
  Print(data);  /* 3 1 2 */
  Print(twice); /* 6 2 4 */
  Sort(data);
  Print(data);  /* 1 2 3 */
}
```

The three declarations at the top state each contract; the definitions and
`main` show the caller-visible effects. `std::sort` is used here only as a
ready-made in-place operation; Week 11 explains the iterator arguments.

Use a pointer when null is meaningful or pointer arithmetic/low-level interop is
required. Use a reference for a required borrowed object. Small scalar values
such as `int` are normally simpler to pass by value when no caller mutation is
needed.

#### Try it now [Core live] — read a contract from a signature (4 minutes)

For `Print`, `Sort`, and `Doubled`, identify whether the caller's vector is
copied, borrowed read-only, or borrowed mutably. Which function may change the
caller's vector, and which function returns independent owned state?

<details>
<summary>Reveal solution</summary>

| Function | Parameter contract | Caller-visible result |
|----------|--------------------|-----------------------|
| `Print` | required read-only borrow | prints values; cannot modify through `values` |
| `Sort` | required mutable borrow | may reorder the caller's vector |
| `Doubled` | independent local value | caller's argument is unchanged; returned vector owns its elements |

The table is a contract trace. In the program above, `data` is unchanged by
`Doubled` and reordered only by `Sort`. Passing an
lvalue to `Doubled` copies it; later C++ topics explain when value transfers can
avoid unnecessary copying.

</details>

---

### 4. `const` makes interfaces readable

```cpp
#include <iostream>
#include <vector>

double Mean(const std::vector<int>& values) {
  if (values.empty()) return 0.0;
  double total = 0.0;
  for (int value : values) total += value;
  return total / static_cast<double>(values.size());
}

int main() {
  std::cout << "Mean({2, 4, 6}) = " << Mean({2, 4, 6}) << '\n';
  std::cout << "Mean({})        = " << Mean({}) << '\n';
}
```

`Mean` returns a value and produces no output itself; `main` prints the
results.

The signature says the vector is required, borrowed, and not modified. The
range loop copies each `int`, which is appropriate for a small scalar.
`static_cast<double>(values.size())` explicitly converts the unsigned size to a
`double` before division. Read the syntax as “convert this expression to
`double`.” C++ provides several named cast forms because each conversion category
has different risks; `static_cast` is the ordinary checked-at-compilation form
for a numeric conversion such as this one. A C-style cast is shorter but hides
which kind of conversion was intended.

Accumulating in `double` avoids signed-integer overflow, although a large sum
can round and floating-point overflow must still be considered for an
unrestricted numeric interface.

#### Try it now [Core live] — trace read-only borrowing and conversion (4 minutes)

Trace `Mean` for `{2, 4, 6}` and for an empty vector. What is copied by the
range loop, what remains borrowed, and what would a caller print for each
result?

<details>
<summary>Reveal solution</summary>

The parameter `values` is a read-only borrow of the complete vector. Each loop
iteration copies one small `int` into `value`; no element is modified.

```text
Mean({2, 4, 6}) = 4
Mean({})        = 0
```

These are the exact lines printed by `main`: with default stream formatting,
`4.0` and `0.0` are shown as `4` and `0`. The function itself performs no
output. The cast makes
the divisor a `double`, although `total` already makes the division floating
point in this implementation.

</details>

For larger elements:

```cpp
#include <iostream>
#include <string>
#include <vector>

int main() {
  const std::vector<std::string> words{"red", "blue"};
  for (const std::string& word : words) {
    std::cout << word << '\n';
  }

  /* auto can reduce repetition without removing static types */
  for (const auto& word : words) {
    /* word is const std::string& */
    std::cout << word << " has " << word.size() << " letters\n";
  }
}
```

The first loop prints `red` and `blue` on separate lines. The `auto` loop has
the same traversal and borrowing behavior and prints `red has 3 letters` and
`blue has 4 letters`.

Use `auto` when the initializer makes the type clear; spell the type when it
communicates an important unit, conversion, or ownership decision.

---

### 5. A range loop is still an ownership decision

A range-based loop expresses “visit every element,” but the loop variable still
determines whether each element is copied, observed, or modified:

```cpp
#include <iostream>
#include <string>
#include <vector>

int main() {
  const std::vector<int> scores{91, 82};
  const std::vector<std::string> names{"Ada", "Linus"};
  std::vector<double> weights{2.0, 3.0};
  const double total_weight = 5.0;

  for (int score : scores) {
    std::cout << score << '\n'; /* copy a small scalar */
  }

  for (const std::string& name : names) {
    std::cout << name << '\n'; /* borrow without copying */
  }

  for (double& weight : weights) {
    weight /= total_weight; /* borrow and modify */
  }

  std::cout << "weights: " << weights.at(0) << ' ' << weights.at(1) << '\n';
}
```

The first form gives the loop body an independent value. The second and third
forms create aliases to elements owned by the container. Those aliases are
valid only while the elements remain alive and in place. Do not change a
vector's size inside a loop that holds references to its elements; reallocation
can invalidate every element reference, and even a `push_back` that does not
reallocate invalidates the old past-the-end position used by traversal.

This is not merely shorter loop syntax. It is another place to state the same
copy/borrow/mutate contract used by function parameters.

#### Try it now [Core live] — classify each loop variable (5 minutes)

For the concrete containers in the example, classify `score`, `name`, and
`weight` as a copy or borrow. Predict the terminal output and the final contents
of `weights`. Why would `weights.push_back(...)` inside the final loop be
unsafe?

<details>
<summary>Reveal solution</summary>

- `score` is a copy of each integer.
- `name` is a read-only reference to each owned string.
- `weight` is a mutable reference to each owned `double`.

**Terminal output:**

```text
91
82
Ada
Linus
weights: 0.4 0.6
```

The last loop prints nothing itself; it changes `weights` to `0.4, 0.6`, which
the final statement prints.
`push_back` invalidates the old past-the-end position used by the range loop. If
it reallocates the vector's element storage, it also invalidates all element
references, including the current `weight`.

</details>

---

### Parameter and traversal audit

#### Try it now [Extension] — design the interface before the body (8 minutes)

For each operation, choose `T`, `T&`, `const T&`, or `T*` and explain why:

1. print a vector;
2. sort it in place;
3. accept an optional output destination;
4. store an independent string value for long-term use (take it by value this
   week; Week 12 explains how that value can be moved into owned storage);
5. return a filtered vector.

Then rewrite one index loop as a range loop. State whether its loop variable is
a copy, a read-only borrow, or a mutable borrow, and explain what container
changes would invalidate that borrow. Week 11 introduces iterators, algorithms,
and lambdas after classes and callable objects have a proper foundation.

<details>
<summary>Reveal solution</summary>

1. Print a vector through `const std::vector<int>&`: required, borrowed, and
   read-only.
2. Sort through `std::vector<int>&`: required mutable access to caller-owned
   state.
3. Use `std::ostream*` for an optional output destination when `nullptr` means
   “do not write”; a required destination would normally be a reference.
4. Take `std::string` by value when the operation needs its own independent
   value under this week's model.
5. Return `std::vector<int>` by value; if an input vector is only observed,
   accept it through `const std::vector<int>&`.

For an index loop that only prints small integers, `for (int value : values)`
uses copies. For large read-only elements, `for (const auto& value : values)`
uses borrows. Changing vector size can invalidate the latter references. These
are interface and traversal decisions; no particular terminal output is implied.

</details>

---

## Hour 3 — Deterministic lifetime and value-oriented design

> **Hour 3 route:** [Exceptions transfer control to a handler](#6-exceptions-transfer-control-to-a-handler)
> → [RAII: lifetime controls resources](#7-raii-lifetime-controls-resources)
> → [Values first](#8-values-first)
> → [Nested RAII lifetimes](#nested-raii-lifetimes)
> → [integration task](#hour-3-integration-task)

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

For the shown call, the handler writes the following line to standard error:

```text
negative value
```

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

#### Try it now [Core live] — follow handler selection (4 minutes)

Trace control from `RequireNonnegative(-1)` through the handler. Which
statements after the `throw` run, what object does `error` refer to, and what
happens if the handler instead catches only `std::runtime_error`?

<details>
<summary>Reveal solution</summary>

The `throw` immediately ends the current path through `RequireNonnegative`; no
later statement in that function runs. The matching handler binds `error` as a
read-only reference to the active `std::invalid_argument` exception object and
prints:

```text
negative value
```

`std::invalid_argument` is not derived from `std::runtime_error`, so a handler
for only `std::runtime_error` would not match. With no other matching handler,
the program would terminate. This is a control-flow and handler-selection
trace; the single line is standard-error output.

</details>

---

### 7. RAII: lifetime controls resources

RAII means **Resource Acquisition Is Initialization**. An object establishes
its invariant and acquires resources during construction; its destructor
releases them when the object's lifetime ends.

```cpp
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

void WriteReport(const std::string& path) {
  std::ofstream output(path);
  if (!output) {
    throw std::runtime_error("cannot open report");
  }
  output << "complete\n";
} /* output is closed here, including during exception unwinding */

int main() {
  try {
    WriteReport("report.txt");
  } catch (const std::runtime_error& error) {
    std::cerr << error.what() << '\n';
    return 1;
  }
}
```

On success, this program produces no terminal output. It creates or replaces
`report.txt` in the current directory and writes the file content `complete`
followed by a newline. If the file cannot be opened, `main` writes
`cannot open report` to standard error and returns `1`.

The same principle manages vectors, strings, locks, sockets, and smart pointers.
RAII turns every control-flow path—normal return, early return, or exception—
into deterministic cleanup.

#### Try it now [Core live] — prove cleanup on every exit (4 minutes)

Trace the lifetime of `output` on normal return, open failure, and a hypothetical
exception thrown immediately after the write. At which point is the file closed,
and which path produces the file content shown above?

<details>
<summary>Reveal solution</summary>

- Normal return: `output` is constructed, the open check succeeds, the line is
  written, and the destructor closes the file at the closing brace.
- Open failure: the `ofstream` object still exists in a failed state. Throwing
  leaves the function, so its destructor runs; there may be no open handle to
  close and no valid report content.
- Later exception: stack unwinding destroys `output`, closing its owned file
  before a handler outside the function runs. Data already handed to the stream
  is finalized according to the stream's normal destruction behavior.

Only the successful path is required to leave this file content:

```text
complete
```

There is no normal terminal output.

</details>

---

### 8. Values first

Prefer automatic-duration value objects:

```cpp
#include <iostream>
#include <string>
#include <vector>

int main() {
  std::vector<std::string> names;
  names.push_back("Ada");
  names.push_back("Linus");
  std::cout << names.size() << " names\n";
} /* names releases both strings and its element storage here */
```

The declaration constructs an empty owning vector and produces no output. The
program then prints `2 names`; no explicit cleanup is written.

Before requesting manual allocation, ask whether a standard container,
`std::string`, or an ordinary class member can own the resource. Week 12
introduces C++'s low-level allocation syntax only long enough to explain why
ordinary application code should prefer RAII owners.

Returning a value is idiomatic and efficient:

```cpp
#include <iostream>
#include <vector>

std::vector<int> EvenValues(const std::vector<int>& input) {
  std::vector<int> result;
  for (int value : input) {
    if (value % 2 == 0) result.push_back(value);
  }
  return result;
}

void PrintVector(const char* label, const std::vector<int>& values) {
  std::cout << label;
  for (int value : values) std::cout << ' ' << value;
  std::cout << '\n';
}

int main() {
  const std::vector<int> input{1, 2, 3, 4};
  std::vector<int> result = EvenValues(input);
  result.at(0) = 20;
  PrintVector("input: ", input);
  PrintVector("result:", result);
}
```

`EvenValues` returns a vector value and prints nothing by itself.

The caller receives an independent vector that owns its elements. C++ is
designed to return such values efficiently; Week 12 explains the copy and
resource-transfer operations that support this model. The interface should
express the simple ownership result now without exposing those implementation
mechanisms prematurely.

#### Try it now [Core live] — return independent owned state (4 minutes)

Call `EvenValues` with an input vector containing `1, 2, 3, 4`. Predict the
returned vector, then change its first element to `20`. What remains in the
input vector, and what would printing both vectors show?

<details>
<summary>Reveal solution</summary>

The result initially contains `2, 4`. It is an independent vector value, so
changing its first element does not modify the borrowed input.

```text
input:  1 2 3 4
result: 20 4
```

These lines are the exact output of the program above; `EvenValues` itself
prints nothing. The core idea is returning independent owned state. Copy elision and
move mechanics are deliberately deferred to Week 12.

</details>

---

### Nested RAII lifetimes

```cpp
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<std::string> ReadWords(const std::string& path) {
  std::ifstream input{path};
  if (!input) throw std::runtime_error{"cannot open " + path};

  std::vector<std::string> words;
  for (std::string word; input >> word;) {
    words.push_back(word);
  }
  if (input.bad()) throw std::runtime_error{"read failure"};
  return words;
}

int main() {
  try {
    std::vector<std::string> words = ReadWords("words.txt");
    std::cout << words.size() << " words\n";
  } catch (const std::runtime_error& error) {
    std::cerr << error.what() << '\n';
    return 1;
  }
}
```

`ReadWords` produces no terminal output. On success it returns the words; on
failure it throws a `std::runtime_error`. If `words.txt` contains
`red green blue`, `main` prints `3 words`; if the file is missing, it writes
`cannot open words.txt` to standard error and returns `1`.

Trace construction and destruction for `input`, the loop-local `word`, the
vector, its elements, and the returned value on normal return and on each throw.
No explicit cleanup appears because every owner has a destructor.

#### Try it now [Core live] — order nested lifetimes (5 minutes)

Trace `input`, `words`, and the loop-local `word` on normal return, open
failure, and a hard input/output failure that sets `badbit`. Do not assume a
particular copy-versus-move implementation for the returned vector.

<details>
<summary>Reveal solution</summary>

Construction on the successful path occurs in this order:

```text
input -> words -> word
```

The `word` object is constructed once by the `for` initializer and reused for
each extraction. Its scope ends when the loop ends, so it is destroyed before
the later stream-state check. On normal return, a returned vector value retains
the collected strings; whether elision or another value-transfer mechanism is
used is a Week 12 topic. Remaining distinct local objects are destroyed in
reverse construction order, with `input` closing the file.

If opening fails, only `input` has been constructed; throwing destroys it. If a
hard read failure sets `badbit`, `word` has already left scope, then unwinding
destroys `words` and `input` in that order. Ordinary end of file sets `eofbit`
and ends extraction without being treated as an error. These are lifetime
traces; the function itself prints nothing.

</details>

---

### Hour 3 integration task

#### Try it now [Extension] — integrate values, failure, and lifetime (10 minutes)

Build a program that reads words from an `ifstream` into a `vector<string>` and
reports the total count and longest word. It must report open/read failures,
avoid global state, and return collected results by value. Compare its cleanup
proof with a C version that uses `FILE *`, allocated strings, and multiple error
labels. Sorting, iterators, and custom comparison behavior are developed in
Week 11.

<details>
<summary>Reveal solution</summary>

A complete design should have these stages:

1. `ReadWords(path)` owns the `ifstream` locally and returns a vector value.
2. A separate query observes the vector through `const&` and computes count and
   longest word without changing ownership.
3. `main` catches the documented exception type, writes one diagnostic, and
   returns failure; otherwise it prints the report.

For a file containing `red green blue`, one acceptable output is:

```text
count: 3
longest: green
```

The C++ cleanup proof names the destructors of the local stream, vector, and
strings. The corresponding C proof must show an explicit close/free action on
every successful and failing path. The exact implementation is intentionally
left for the studio rather than disclosed here.

</details>

---

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

#### Try it now [Extension] — draw one project lifetime boundary (6 minutes)

Choose a display, bitmap, audio sample, or other resource actually present in
the template. Fill in owner, acquisition, use, normal release, and partial-
initialization failure. Do not invent a wrapper before identifying the current
owner.

<details>
<summary>Reveal solution</summary>

The exact API depends on the selected template resource, but a valid table has
this form:

| Question | Required evidence |
|----------|-------------------|
| Who owns it? | one object or subsystem responsible for final release |
| When is it acquired? | the successful API call and the state established afterward |
| Who borrows it? | update/draw or registration sites that do not release it |
| How is it released? | the matching release call on ordinary shutdown |
| What if later initialization fails? | cleanup of this resource before propagating failure |

This is a project-specific ownership trace, not terminal output. A future RAII
wrapper is justified only if its constructor/destructor contract matches the
verified table.

</details>

---

## Check yourself

1. When should a parameter be `const T&`, `T&`, or `T`?
2. Replace a C character buffer with `std::string` and list removed failure modes.
3. Why can changing a vector's size invalidate references to its elements?
4. Which objects are destroyed when an exception leaves two nested scopes?
5. What resource does a vector's destructor release?
6. Refactor a `malloc`/`free` integer array into `std::vector<int>`.

<details>
<summary>Reveal solutions</summary>

1. Use `const T&` for a required read-only borrow, `T&` for a required mutable
   borrow, and `T` for an independent local value or an intentionally consumed
   value under the interface's contract.
2. `std::string` removes manual capacity tracking, terminator placement,
   resizing, and release. Input validity and domain length limits still require
   deliberate policies.
3. Increasing vector size may allocate a new element array and relocate the
   elements, so references into the old storage no longer designate live
   elements.
4. Automatic objects in exited scopes are destroyed in reverse construction
   order, from the inner scope outward, before a matching outer handler runs.
5. A vector destructor releases the dynamic element storage it owns and
   destroys its elements.
6. Keep the size and ownership in one value, for example:

   ```cpp
   #include <cstddef>
   #include <vector>

   std::vector<int> MakeZeroes(std::size_t count) {
     return std::vector<int>(count, 0);
   }

   int main() {
     std::vector<int> values = MakeZeroes(4);
     values.at(1) = 7;
     return values.at(1) == 7 ? 0 : 1;
   }
   ```

   Bounds and initialization policy still belong to the program, but no
   matching `free` call is needed.

Items 1–5 are contract and lifetime explanations. Item 6 is a complete program
that produces no output; it returns success after using the vector.

</details>

---

## Summary

- C++ preserves explicit performance while adding strong value abstractions.
- Strings and vectors own storage and know their sizes.
- References express required borrowing; `const` exposes non-mutation.
- Range loops and `auto` are useful only when their copy and borrowing behavior
  remains clear.
- Exceptions transfer control while stack unwinding destroys automatic objects.
- RAII binds resource cleanup to deterministic object lifetime.

---

## References and source materials

- [Introduction to C++](<https://github.com/htchen/i2p-nthu/blob/master/程式設計二/Intro/README.md>)
- [2025 Week 7 notebook (Colab)](https://colab.research.google.com/drive/1oHBcNeAXt4ZeQJsdG2q4RU5m9Yu_9CCw)
