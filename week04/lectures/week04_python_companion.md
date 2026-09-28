# Python Contrast Companion for Week 4

This companion should be read beside the [Week 4 lecture
notes](week04_lecture.md). Python references objects automatically, but an
ordinary Python name is **not** a C pointer. The comparisons below explain what
can be translated and, more importantly, which address, lifetime, and ownership
properties disappear in Python.

## Relationship labels

- **Direct:** the algorithm and observable result transfer cleanly.
- **Closest Python model:** Python can express the goal but not the C mechanism.
- **No faithful equivalent:** using ordinary Python to imitate the syntax would
  teach an incorrect pointer or lifetime model.
- **Diagram or tool:** the source block is not a translatable C computation.

## Coverage map

The principal C examples in the Week 4 note are accounted for below.
Collapsible practice solutions, expected-output blocks, shell commands, and
ownership traces reuse these concepts rather than introducing separate Python
translations.

| Source example | Relationship | Companion treatment |
|---|---|---|
| `value`, `pointer = &value`, and `*pointer` | No faithful equivalent | Contrast a Python name and a mutable `Box`; warn that `id` is not a usable C address |
| Pointer-to-value Mermaid diagram | Diagram | Reinterpret it as reachability, not pointer storage |
| Separate pointer declarations | No faithful equivalent | Python names have no pointer declaration |
| `int *first, count` ambiguity | No faithful equivalent | Explain why Python annotations do not reproduce declarator grammar |
| One-declaration-per-line style | Closest model | Use separate annotated bindings for readability, without pointer meaning |
| C17 `Node` structure initialization | Closest model | Use a dataclass whose constructor initializes every field |
| Pointer-based `swap` | Closest model | Return two values or mutate explicit `Box` objects |
| `contains_zero(const int*, size_t)` | Direct algorithm, different contract | Accept a `Sequence[int]`; Python cannot enforce C `const` |
| Form-address/load/store fragment | No faithful equivalent | Attribute access on a `Box` demonstrates shared mutation, not machine load/store |
| `values[i] == *(values + i)` | No pointer-arithmetic equivalent | Retain indexing and explain that list indexing is not defined through pointers |
| Array `sizeof` versus pointer `sizeof` | No representation equivalent | Use `len`; Python does not expose the C array/pointer distinction |
| Returning a borrowed element pointer | Same search goal, different lifetime model | Return an index or value; no interior C pointer escapes |
| Pointer/`const`/callback declarations | Partial | Use type hints and `Callable`; they are not storage declarations or `const` |
| Pointer traversal and subtraction | Direct traversal goal | Use `enumerate` rather than one-past pointers |
| Returning `&local` | Opposite lifetime result | Returning a Python object keeps it reachable; there is no dangling local address |
| `read_values` allocation function | Closest model | Build and return a list; allocation size arithmetic and `NULL` disappear |
| `make_sequence(..., int** out)` | No double-pointer equivalent | Return a list or raise; explain why Python cannot publish through a caller's owning pointer |
| Caller checks and `free` | No explicit release equivalent | Catch conversion/allocation exceptions and let the list remain managed |
| `IntBuffer` and `buffer_push` | Closest model | Wrap a list and use `append`; Python hides capacity and relocation |
| Safe `realloc` pattern | No faithful equivalent | Contrast rebinding with in-place list mutation and alias behavior |
| `cc -O0 -S pointer_demo.c` | Tool, no direct pointer translation | `dis` can inspect Python bytecode but cannot validate C pointer lifetime |
| Ownership function declarations | Partial | Show observe/copy/clear operations and state what annotations cannot promise |
| `values_destroy(int**)` | No double-pointer equivalent | Clearing a shared list mutates it; rebinding a caller's name is impossible this way |
| Opaque `Counter` ownership interface | Closest abstraction goal | Use a class while explaining the missing manual allocation protocol |
| Direct function-pointer call | Closest callable model | Bind a Python function to a name and call it; no C signature or calling convention follows |
| `qsort` function-pointer declaration | Closest model | Use `sorted` with a callable `key` |
| `Student` comparator and `qsort` call | Direct ordering goal | Use a dataclass and a descending grade key |
| Sanitizer compilation command | Tool, no Python equivalent | Explain why ordinary Python diagnostics do not replace C sanitizers |

## Names, object references, and explicit boxes

Python names refer to objects, but the language does not provide C's address-of
and dereference operators. This code shares one mutable object:

```python
from dataclasses import dataclass


@dataclass
class Box:
    value: int


value = Box(7)
alias = value
print(value.value)
print(alias.value)
alias.value = 9
print(value.value)
```

Both names reach the same `Box`, so mutation is visible through either name.
This is a useful aliasing example, but `alias` does not contain a typed,
arithmetically adjustable address, and `.value` is attribute access rather than
C dereference. Python's `id(value)` is an identity token; even when an
implementation derives it from an address, the language does not make it a
pointer that can be dereferenced.

The Mermaid arrow in the C note can therefore be reused only as “this name
reaches this object.” It must not be read as a diagram of a Python pointer
object storing the address of an integer object.

## Declarations and structure initialization

Python has no counterpart to the C declarator distinction among:

```text
pointer to int
const pointer to int
pointer to const int
pointer to function
```

Annotations can describe intended values but do not create those C types:

```python
from typing import Optional


first: Optional[int] = None
count: int = 0
```

Here `first` is merely documented as an optional integer. It is not an
uninitialized `int*`, and `Optional[int]` says nothing about addresses or ownership.
Separate bindings are still good style because they keep one decision per line.

A C node's “initialize every field” goal has a close Python model:

```python
from __future__ import annotations
from dataclasses import dataclass


@dataclass
class Node:
    value: int
    next: Node | None = None


node = Node(value=0)
```

The generated constructor establishes both attributes. Python permits a class
attribute default, unlike a C17 structure member declaration, and object
storage is managed by the interpreter.

## Modifying a caller-visible object

Rebinding integer parameters cannot swap the caller's names:

```python
def ineffective_swap(left: int, right: int) -> None:
    left, right = right, left
```

Two honest Python rewrites are possible. Return the new values:

```python
def swapped(left: int, right: int) -> tuple[int, int]:
    return right, left


a = 10
b = 20
a, b = swapped(a, b)
```

Or make mutability explicit:

```python
def swap_boxes(left: Box, right: Box) -> None:
    left.value, right.value = right.value, left.value
```

The `Box` version resembles the caller-visible effect, but it still does not
teach `&a`, `*left`, null pointers, pointer types, or valid address lifetime.

## Read-only intent and sequence traversal

The zero test translates directly at the algorithm level:

```python
from collections.abc import Sequence


def contains_zero(values: Sequence[int]) -> bool:
    for value in values:
        if value == 0:
            return True
    return False
```

`Sequence[int]` communicates that indexing/iteration is sufficient and that
the function does not need mutation. Python does not enforce C's `const`: a
mutable object may still be passed, and another alias may change it. A static
type checker can catch some interface mistakes, but the run-time language does
not make the elements read-only.

Python indexing is the direct way to obtain an element:

```python
value = values[index]
```

There is no ordinary expression corresponding to `*(values + index)`. A Python
list is not specified as a C array, has no one-past value, and does not define
indexing through pointer arithmetic.

The pointer traversal example becomes an ordinary indexed traversal:

```python
values = [10, 20, 30, 40]
for index, value in enumerate(values):
    print(f"index={index} value={value}")
```

The output is comparable, but `index` is an integer produced by `enumerate`,
not a pointer difference. There is no valid fifth one-past list reference to
draw or accidentally dereference.

Returning an index is an honest contrast for the borrowed-element search:

```python
from collections.abc import Sequence
from typing import Optional


def find_index(values: Sequence[int], target: int) -> Optional[int]:
    for index, value in enumerate(values):
        if value == target:
            return index
    return None
```

This preserves the search and missing-result policy, but the integer is not a
pointer into element storage. It cannot be dereferenced or freed, and Python
does not expose whether later sequence mutation relocates internal storage.

For callable intent, Python can use an annotation:

```python
from collections.abc import Callable

Operation = Callable[[int, int], int]
```

This is related to a C function-pointer signature, but Python callables may be
functions, bound methods, or other callable objects, and no representation or
calling convention is promised by the annotation.

Binding and calling behavior can still be compared directly:

```python
def add(left: int, right: int) -> int:
    return left + right


operation: Operation = add
print(operation(3, 4))
```

This prints `7`; the annotation remains an optional static-checking aid rather
than a C function-pointer object.

## Returning a local value

The superficially similar Python function is safe:

```python
def make_value() -> int:
    local = 42
    return local
```

It returns an object reference as a normal Python result. The returned object
remains available while the caller holds it. Python does not let the function
return the address of an automatic-duration integer whose lifetime has ended, so
this code cannot reproduce the dangling pointer in `bad_address`.

## Dynamic input and storage

The closest ordinary rewrite of `read_values` builds a managed list:

```python
from collections.abc import Iterable


def read_values(tokens: Iterable[str], count: int) -> list[int]:
    if count < 0:
        raise ValueError("negative count")
    iterator = iter(tokens)
    values: list[int] = []
    for _ in range(count):
        try:
            token = next(iterator)
        except StopIteration as error:
            raise ValueError("not enough values") from error
        values.append(int(token))
    return values
```

The caller receives an independently managed list or an exception. There is no
`SIZE_MAX` multiplication check, `NULL` result, or matching `free`. Allocation
can still fail with `MemoryError`, but application code normally does not use it
as a recoverable analogue of a checked C allocation.

The closest rewrite of `make_sequence(size, &owner)` simply returns a value:

```python
def make_sequence(size: int) -> list[int]:
    if size < 0:
        raise ValueError("negative size")
    return [0] * size
```

There is no caller-owned pointer variable for the function to update through a
second pointer level. Assignment at the call site publishes the returned list,
and Python's exception mechanism separates failure from an empty list.

Python's list already provides the user-facing behavior of the C buffer:

```python
class IntBuffer:
    def __init__(self) -> None:
        self._data: list[int] = []

    def push(self, value: int) -> None:
        self._data.append(value)

    def values(self) -> tuple[int, ...]:
        return tuple(self._data)
```

The logical invariant is simply that `_data` contains the buffer elements.
Python does not expose a portable list capacity, the address of its element
storage, or whether `append` relocated that storage. Consequently this rewrite
cannot teach the C invariant `size <= capacity` or interior-pointer
invalidation.

There is also no faithful `realloc` translation. Compare two Python operations:

```python
values = [10, 20, 30]
alias = values

values = values[:2]   # rebind values; alias still reaches the old list
del alias[2:]         # mutate the object reached by alias
```

Neither operation exposes allocation failure while preserving an old pointer,
and Python references to element objects do not become C-style dangling
interior pointers after list growth.

## Ownership operations and the missing double pointer

Python can express the high-level intentions:

```python
from collections.abc import Sequence


def print_values(borrowed: Sequence[int]) -> None:
    print(*borrowed)


def values_clone(source: Sequence[int]) -> list[int]:
    return list(source)


def values_destroy(owned: list[int]) -> None:
    owned.clear()
```

The final function empties the shared list object, so every alias observes an
empty list. It does **not** release a manual allocation and cannot set the
caller's name to `None`. Rebinding `owned = None` inside the function would
change only the local parameter. This is why it is not a translation of
`values_destroy(int**)`.

An opaque C structure's abstraction goal resembles a Python class with private-
by-convention attributes, but Python does not require paired create/destroy
operations for ordinary object storage. The C interface additionally specifies
who owns the allocation and when its lifetime ends.

Python bytecode can be inspected with `dis`, but it does not validate C pointer
lifetime:

```sh
python3 -m dis python_examples.py
```

The C source and AddressSanitizer remain the appropriate evidence for `free`,
use-after-free, and out-of-bounds access.

## Generic sorting and callbacks

Python's sorting interface accepts callable behavior but normally asks for a
key rather than a three-way comparator:

```python
from dataclasses import dataclass


@dataclass(frozen=True)
class Student:
    student_id: int
    grade: float


students = [Student(1, 82.0), Student(2, 95.0), Student(3, 88.5)]
ranking = sorted(students, key=lambda student: student.grade, reverse=True)
```

This retains the descending-grade goal. Python knows the element objects and
does not need `void*`, an element byte size, or a cast inside the callback.
Those missing checks are precisely what the C `qsort` example is meant to make
visible. Both versions should define a policy if grades may be NaN.

## Memory diagnostics have no direct Python substitute

The sanitizer command in the C note deliberately targets invalid addresses,
use-after-free, double free, and undefined arithmetic. Ordinary Python prevents
or hides most of those operations and reports its own exceptions, so options
such as `python3 -X dev` are not substitutes for compiling the C program with
AddressSanitizer and UndefinedBehaviorSanitizer.

Use Python tests to check the shared algorithm, but use C warnings, sanitizers,
ownership traces, and bounds reasoning to validate the C implementation.

Runnable versions of the meaningful Python models are collected in
[`python_examples.py`](python_examples.py).
