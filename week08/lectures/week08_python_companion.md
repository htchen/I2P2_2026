# Python Contrast Companion for Week 8

This companion should be read beside the [Week 8 lecture
notes](week08_lecture.md). Many C++ library values have familiar Python uses,
but C++ references, `const`, deterministic destruction, and value operations
must not be reduced to “Python with different syntax.”

## Relationship labels

- **Direct:** the algorithm and normal output translate closely.
- **Same goal, different contract:** Python provides the behavior but cannot
  state the same static type, borrowing, or mutation contract.
- **Closest Python model:** a mutable object or context manager approximates one
  important effect while other C++ semantics remain absent.
- **No faithful equivalent:** ordinary Python has no matching language feature.

## Coverage map

Every fenced example in the Week 8 note is accounted for below.

| Source example | Relationship | Companion treatment |
|---|---|---|
| `c++ -std=c++17 ...` | Same goal, different toolchain | Run a `.py` file directly; there is no C++17 selection or link step |
| `std::string` hello program | Direct output | Use a Python string and `print` |
| `cin`/`cout` age interaction | Direct behavior | Use `input`, `int`, and an f-string |
| `getline` into `std::string` | Direct behavior | `input` returns one line without its newline |
| `std::vector<int>` and `push_back` | Same goal, different type contract | Use `list[int]` and `append` |
| Stream loop with failure-state check | Same goal, different failure model | Iterate tokens and catch `ValueError` |
| Brace/parenthesized initialization | No overload-resolution equivalent | Show `[0] * 5` versus `[5, 0]` explicitly |
| `SwapValues(int&, int&)` | No reference-parameter equivalent | Return a pair or mutate explicit boxes |
| `SetToSeventeen(int&)` | Closest model | Mutate an `IntBox`; rebinding an integer parameter would not affect the caller |
| Value/pointer/reference swap definitions | Partial | Contrast local rebinding, mutable objects, and returned values |
| Three swap calls | Partial | Trace Python caller-visible state through the honest rewrites |
| `Print`, `Sort`, and `Doubled` signatures | Same goal, different contract | Use `Sequence`, mutable `list`, and a returned list |
| Read-only `Mean` | Direct algorithm | Accept a sequence; Python cannot enforce `const` |
| Loop over `const std::string&` | Direct observation goal | Iterate over strings; no reference declaration is exposed |
| Loop with `const auto&` | Direct observation goal | Same Python loop; dynamic names make `auto` unnecessary |
| Copy/borrow/mutate range loops | Partial | Explain rebinding versus indexed mutation of a list |
| Throw/catch example | Direct control-flow idea | Raise and catch `ValueError`; destruction guarantees differ |
| `ofstream` RAII example | Closest model | Use `with open(...)` for deterministic block-scoped cleanup |
| Empty `vector<string>` | Direct collection goal | Use an annotated empty list |
| `EvenValues` returning a vector | Direct algorithm and value result | Return a new list comprehension |
| `ReadWords` with nested RAII | Closest model | Use `with open`, split words, and propagate `OSError` |

## Running a Python program

```sh
python3 program.py
```

Python does not select a C++ language standard or link an executable in this
ordinary workflow. Static analysis and bytecode compilation may be added as
separate tools, but they are not translations of `-std=c++17`.

## Output, input, and namespaces

The first C++ program has a direct Python result:

```python
def main() -> None:
    language = "C++"
    print(f"Now learning {language}")
```

Python modules group names, but there is no reason to manufacture a `std::cout`
analogue. `print` and `input` are built-ins:

```python
age = int(input("Age? "))
if not 0 <= age <= 150:
    raise ValueError("expected an age from 0 through 150")
print(f"Next year: {age + 1}")
```

`int` conversion raises `ValueError` on invalid text. C++ formatted extraction
instead records failure in stream state. Both versions then apply the same
domain check. The error paths are related, but their interfaces are not
interchangeable.

Reading a complete name is similarly direct:

```python
name = input()
print(f"{name} has {len(name)} characters")
```

Unlike the C++ interaction between `operator>>` and `getline`, `input` consumes
one complete line and removes the trailing newline.

## Strings, lists, and initialization

```python
scores: list[int] = [91, 82, 73]
scores.append(94)
scores.append(85)
```

A Python list resizes and owns references to its elements, but the run-time
language does not restrict every element to `int`. The annotation documents the
intent and helps a static checker; `std::vector<int>` enforces its element type
as part of C++ compilation and stores integer values as its elements.

The stream-loop behavior can be modeled by token conversion:

```python
import sys

values: list[int] = []
try:
    for line in sys.stdin:
        for token in line.split():
            values.append(int(token))
except ValueError:
    print("input contained a non-integer token", file=sys.stderr)
    raise SystemExit(1)
```

Python iteration observes EOF naturally. Conversion failure is an exception,
not a persistent stream state that must later be distinguished from EOF.

The C++ initialization example should be translated by meaning, not punctuation:

```python
count = 0
title = "I2P II"
first = [0] * 5  # five zeroes
second = [5, 0]  # two elements
```

There is no initializer-list constructor overload that changes the meaning of
parentheses versus braces. Python also does not perform C++ compile-time
narrowing checks on these assignments.

## References versus Python binding

Python cannot declare an integer reference parameter. This function only
rebinds local names and does not swap the caller's integers:

```python
def ineffective_swap(left: int, right: int) -> None:
    left, right = right, left
```

The idiomatic rewrite returns the new values:

```python
def swapped(left: int, right: int) -> tuple[int, int]:
    return right, left


x = 5
y = 7
x, y = swapped(x, y)
```

To approximate caller-visible mutation, introduce an explicitly mutable object:

```python
from dataclasses import dataclass


@dataclass
class IntBox:
    value: int


def swap_boxes(left: IntBox, right: IntBox) -> None:
    left.value, right.value = right.value, left.value


def set_to_seventeen(value: IntBox) -> int:
    value.value = 17
    return 15
```

This reproduces the visible state change of the C++ reference examples. It does
not create a non-nullable alias type, pointer syntax, or C++ overload behavior.
The boxes are ordinary objects shared through Python references.

An honest translation of the three-way swap trace is therefore:

```python
x = 5
y = 7
ineffective_swap(x, y)  # still (5, 7)
x, y = swapped(x, y)    # now (7, 5)
x, y = swapped(x, y)    # back to (5, 7)
```

The two returned-value calls stand in for the two C++ mutation mechanisms; they
do not claim that Python has either an `int*` or an `int&` parameter.

## Parameter intent and `const`

Python type hints can record related intent:

```python
from collections.abc import Sequence


def print_values(values: Sequence[int]) -> None:
    print(*values)


def sort_values(values: list[int]) -> None:
    values.sort()


def doubled(values: Sequence[int]) -> list[int]:
    return [value * 2 for value in values]
```

`Sequence[int]` says the implementation needs only read-oriented sequence
operations, while `list[int]` advertises that `sort_values` requires a mutable
list. These hints are not C++ `const` or reference declarations, and normal
Python execution does not enforce them.

The mean calculation translates directly:

```python
from collections.abc import Sequence


def mean(values: Sequence[int]) -> float:
    if not values:
        return 0.0
    return sum(values) / len(values)
```

Python integers avoid the fixed-width accumulation issue, but `/` and an empty
input policy still belong to the function's contract.

## Traversal, rebinding, and mutation

Both C++ read-only string loops become the same Python loop:

```python
for word in words:
    print(word)
```

There is no `auto` decision because names are dynamically bound, and no
`const&` spelling guarantees that an element is borrowed read-only.

The three C++ range loops need different explanations:

```python
for score in scores:
    print(score)

for name in names:
    print(name)

for index, weight in enumerate(weights):
    weights[index] = weight / total_weight
```

Rebinding `weight` alone would not replace the element in `weights`, so indexed
assignment is the closest rewrite of `double& weight`. If elements are mutable
objects, mutating an element attribute is visible without indexed replacement.

Python list iteration does not expose C++ vector references or iterators, so its
mutation rules are different. Changing a list while iterating can still skip or
repeat logical work and should normally be avoided, but the explanation is not
C++ reallocation invalidation.

## Exceptions and cleanup

The control transfer is direct:

```python
def require_nonnegative(value: int) -> None:
    if value < 0:
        raise ValueError("negative value")


try:
    require_nonnegative(-1)
except ValueError as error:
    print(error)
```

Python also unwinds call frames and executes `finally` blocks and active context
manager exits. It does **not** generally promise C++'s deterministic destructor
call for every ordinary local object at block exit. Do not translate a C++ RAII
proof into “Python will call `__del__` at the same brace.”

For files, a context manager gives the closest scoped-resource model:

```python
def write_report(path: str) -> None:
    with open(path, "w", encoding="utf-8") as output:
        output.write("complete\n")
```

The file's `__exit__` operation runs on normal exit and exception exit from the
`with` block. This resembles the cleanup goal of the C++ `ofstream`, but the
syntax and general object-lifetime rules differ.

## Values, filtering, and nested resource scopes

An empty sequence of names is direct:

```python
names: list[str] = []
```

Returning selected values is also direct and naturally independent:

```python
from collections.abc import Sequence


def even_values(values: Sequence[int]) -> list[int]:
    return [value for value in values if value % 2 == 0]
```

For the nested file/vector/string example:

```python
def read_words(path: str) -> list[str]:
    with open(path, encoding="utf-8") as input_file:
        return input_file.read().split()
```

The returned list keeps the strings reachable after the file is closed. Open
or read failures raise `OSError` subclasses. The context manager gives a clear
file lifetime, while list and string reclamation remains Python-managed rather
than C++ destructor order.

## What the Python versions must not hide

- Python binding is not a C++ reference declaration.
- Type hints do not enforce `const`, ownership, or one concrete element type at
  run time.
- List mutation rules do not explain C++ vector reallocation invalidation.
- A context manager is a useful RAII analogy, but arbitrary Python object
  destruction is not tied to closing braces.
- Returning a Python list does not demonstrate C++ copy elision or move
  semantics; those remain Week 12 C++ topics.

Runnable versions of the main comparisons are collected in
[`python_examples.py`](python_examples.py).
