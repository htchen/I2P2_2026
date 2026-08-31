# Python Contrast Companion for Week 2

Read this companion beside the [Week 2 lecture
notes](week02_lecture.md). The algorithms for functions, traversal, prefix sums,
binary-search boundaries, and sorting transfer well. C arrays and strings add
fixed capacity, contiguous representation, sentinel termination, and
address-based output contracts that Python collections do not reproduce.

## Relationship labels

- **Direct:** the same algorithm and result translate naturally.
- **Same goal, different contract:** Python omits a C bound, output pointer, or
  fixed element type.
- **Closest Python model:** the Python form illustrates one effect but not the C
  representation.
- **No faithful equivalent:** the C example depends on storage or conversion
  rules absent from ordinary Python.

## Coverage map

Every fenced example in the Week 2 note is accounted for below.

| Source example | Relationship | Companion treatment |
|---|---|---|
| `mean` declaration | Same goal, different contract | A typed Python definition documents a sequence without a separate declaration |
| `mean` implementation | Direct algorithm | Sum a sequence and preserve the empty-input policy |
| Ineffective value swap | Direct observation | Rebinding integer parameters does not change caller names |
| `absolute_value` | Direct | Use the same branch or `abs` |
| Address-passing bridge | No faithful equivalent | Use a mutable box only as an aliasing model |
| Pointer-based `swap` | Same goal, different mechanism | Return a pair or mutate explicit boxes |
| Four decomposed function declarations | Partial | Type-hinted functions express roles; output pointers become returned values |
| Function-local `static` sequence | Closest model | Use a closure with `nonlocal` state |
| Fixed `scores[5]` array | Same values, different storage | Use a list and explain missing fixed capacity/type |
| `sizeof` element count | No representation equivalent | Use `len(scores)` |
| `maximum(..., result)` | Same goal, different absence policy | Return `None` for an empty sequence |
| Half-open traversal loop | Direct | Iterate or use `range(len(values))` |
| Prefix-sum values/boundaries diagram | Direct model | Preserve the same list and boundary interpretation |
| Prefix invariant equation | Direct | Define `prefix[i]` as the sum before boundary `i` |
| Range-query subtraction | Direct | Use the same `prefix[right] - prefix[left]` formula |
| Prefix build/query declarations | Same goal, different contract | Return a list/result or raise for invalid boundaries |
| Lower/equal/upper partition diagram | Direct algorithmic model | Use `bisect_left` and `bisect_right` |
| `char language[] = "C17"` | No character-array equivalent | Use `str` and explain the missing null sentinel |
| `strlen(language)` | Direct length goal | Use `len(language)`; no terminator is counted or stored |
| `char name[32]` | No fixed-capacity string equivalent | Use a string plus explicit length validation |
| `fgets` line input | Same goal, different mechanism | Use `readline`, distinguish EOF, and remove one newline |
| Manual length/copy | Direct algorithms, different representation | Count characters and return a validated copy rather than fill a destination array |
| One-pass minimum | Direct algorithm | Preserve the nonempty precondition |
| `matrix[3][4]` | Same shape, different layout | Build independent nested lists; no row-major guarantee follows |
| Variable-size matrix sum | Direct traversal | Validate rectangular shape and sum nested rows |
| Insertion sort | Direct algorithm | Mutate a list with the same shifting invariant |

## Functions and value parameters

Python needs no declaration separate from the definition:

```python
from collections.abc import Sequence


def mean(values: Sequence[int]) -> float:
    total = 0.0
    for value in values:
        total += value
    return 0.0 if not values else total / len(values)
```

`Sequence[int]` records the operations and intended element type, but normal
Python execution does not enforce C's element representation, address, or
explicit count parameter.

The ineffective swap behaves similarly because Python integer parameters are
local bindings:

```python
def ineffective_swap(a: int, b: int) -> None:
    a, b = b, a
```

An absolute-value function is direct:

```python
def absolute_value(value: int) -> int:
    return -value if value < 0 else value
```

Python integers do not have C's `INT_MIN` negation problem. The shared algorithm
does not remove the need to check the C input domain.

## Address passing and output results

Python has no `&value`, `int*`, or `*address`. A mutable object can demonstrate
shared mutation without pretending to be a pointer:

```python
from dataclasses import dataclass


@dataclass
class Box:
    value: int


box = Box(10)
alias = box
alias.value = 20
```

The idiomatic swap returns both results:

```python
def swapped(left: int, right: int) -> tuple[int, int]:
    return right, left


x = 1
y = 2
x, y = swapped(x, y)
```

The decomposed C interfaces become value-oriented Python contracts:

```python
from collections.abc import Sequence
from typing import Optional


def read_scores(fields: Sequence[str], capacity: int) -> list[int]: ...
def index_of_minimum(scores: Sequence[int]) -> int: ...
def remove_at(scores: list[int], index: int) -> None: ...
def maximum(values: Sequence[int]) -> Optional[int]: ...
```

Returning a list, index, or optional result replaces several C output pointers.
The C interfaces remain essential when fixed buffers and explicit mutation must
be represented.

## Persistent function state

A closure is the closest ordinary model of the C local `static` sequence:

```python
from collections.abc import Callable


def make_sequence() -> Callable[[], int]:
    value = 0

    def next_sequence() -> int:
        nonlocal value
        value += 1
        return value

    return next_sequence
```

The state survives calls through the returned closure. It is a heap-managed
closure cell, not a C static-storage object with an unsigned fixed-width type.

## Arrays, lengths, and boundaries

```python
scores = [91, 82, 73, 94, 85]
count = len(scores)
```

The values and count are direct, but a list can resize and hold mixed run-time
types. `len` asks the collection for its logical length; it is not derived from
the byte size of an array object.

An output-pointer maximum can become an optional return:

```python
from collections.abc import Sequence
from typing import Optional


def maximum(values: Sequence[int]) -> Optional[int]:
    if not values:
        return None
    result = values[0]
    for value in values[1:]:
        if value > result:
            result = value
    return result
```

A half-open traversal can retain its explicit indices:

```python
for index in range(len(values)):
    use(values[index])
```

When the index itself is unimportant, `for value in values` is clearer. The C
version must still reason about `size_t`, array bounds, and valid addresses.

## Prefix sums

The same boundary representation transfers directly:

```python
def build_prefix(values: Sequence[int]) -> list[int]:
    prefix = [0]
    for value in values:
        prefix.append(prefix[-1] + value)
    return prefix


def query_total(prefix: Sequence[int], left: int, right: int) -> int:
    if not 0 <= left <= right < len(prefix):
        raise IndexError("invalid half-open range")
    return prefix[right] - prefix[left]
```

For `[3, -1, 4, 2]`, the result is `[0, 3, 2, 6, 8]` and
`prefix[i] == sum(values[:i])`. The mathematics is identical. Python removes
the destination capacity, `int64_t` storage limit, and output pointer, so it
cannot validate the C overflow and buffer contracts.

## Lower and upper boundaries

Python's standard library names the same two boundaries:

```python
from bisect import bisect_left, bisect_right


lower = bisect_left(values, target)
upper = bisect_right(values, target)
count = upper - lower
```

The same sorted-range precondition applies. The three regions are
`values[:lower]`, `values[lower:upper]`, and `values[upper:]`. Python slicing
creates new lists, while the boundary indices themselves do not.

## Strings, sentinels, and capacity

```python
language = "C17"
length = len(language)
```

The visible characters and length match. A Python string is an immutable
sequence of Unicode characters and does not expose a trailing `\0` element.
It is therefore not a translation of a writable C `char[]` representation.

A fixed-capacity intent must be stated explicitly:

```python
def require_name_capacity(name: str, capacity: int = 32) -> str:
    if len(name) >= capacity:
        raise ValueError("name does not fit the C buffer")
    return name
```

This models the chosen character-count rule, not UTF-8 byte capacity. A C
buffer's capacity is measured in `char` objects/bytes and reserves one position
for the sentinel.

Line input can preserve EOF and newline handling:

```python
def read_line(input_file) -> str:
    line = input_file.readline()
    if line == "":
        raise EOFError("no line")
    return line.removesuffix("\n")
```

`readline` manages storage rather than receiving a fixed array. A Windows-style
line ending or an imposed maximum length needs an explicit policy if it matters.

The manual C algorithms have these closest rewrites:

```python
def string_length(text: str) -> int:
    length = 0
    for _ in text:
        length += 1
    return length


def string_copy(source: str, capacity: int) -> str:
    if len(source) >= capacity:
        raise ValueError("source does not fit")
    return source
```

There is no destination character array to fill and no sentinel to copy. The
Python code preserves only the counting and capacity decision.

## Minimum, matrices, and insertion sort

The one-pass minimum remains direct:

```python
def minimum(values: Sequence[int]) -> int:
    if not values:
        raise ValueError("minimum requires a nonempty sequence")
    result = values[0]
    for value in values[1:]:
        if value < result:
            result = value
    return result
```

A 3-by-4 zero matrix needs independent rows:

```python
matrix = [[0 for _ in range(4)] for _ in range(3)]
```

Do not write `[[0] * 4] * 3` when rows may be mutated; it aliases one row. C's
array is one row-major object, while Python specifies a list of three references
to independent row lists in the correct version.

```python
def sum_matrix(matrix: Sequence[Sequence[int]]) -> int:
    if not matrix:
        return 0
    columns = len(matrix[0])
    if any(len(row) != columns for row in matrix):
        raise ValueError("matrix must be rectangular")
    return sum(value for row in matrix for value in row)
```

This Python model accepts an empty matrix and a zero-column rectangular matrix.
The C VLA parameter instead requires a positive column bound, so that boundary
is a deliberate representation difference rather than an algorithmic one.

Insertion sort preserves the C shifting invariant:

```python
def insertion_sort(values: list[int]) -> None:
    for index in range(1, len(values)):
        current = values[index]
        position = index
        while position > 0 and values[position - 1] > current:
            values[position] = values[position - 1]
            position -= 1
        values[position] = current
```

Python's built-in `list.sort()` is the production choice; the manual version is
retained to study the algorithm and mutation invariant.

## What the Python versions must not hide

- Lists do not explain fixed C array capacity or contiguous element storage.
- Python strings do not explain null-terminated writable byte arrays.
- Returned values do not teach C output-pointer validity.
- Arbitrary-precision sums do not prove that C `int64_t` arithmetic is safe.
- `bisect` shares the sorted-boundary contract, making it a strong algorithmic
  comparison rather than a replacement for the C implementation.

Runnable versions are collected in [`python_examples.py`](python_examples.py).
