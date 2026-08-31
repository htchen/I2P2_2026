# Python Contrast Companion for Week 12

This companion should be read beside the [Week 12 lecture
notes](week12_lecture.md). Python makes many ordinary ownership tasks easier,
but its assignment, garbage collection, and copying model is not a translation
of C++ destruction, copy construction, copy assignment, or move operations.
The comparisons below preserve that distinction.

## Relationship labels

- **Direct goal:** both versions provide the same high-level value behavior.
- **Closest Python model:** an explicit method or library operation reproduces
  one effect but is not an automatic language operation of the same kind.
- **No faithful equivalent:** Python omits or deliberately hides the C++
  lifetime/ownership mechanism.
- **Failure analogy:** Python can expose aliasing, but not the same invalid
  release or undefined behavior.

## Coverage map

Every fenced example in the Week 12 note is accounted for below.

| Source example | Relationship | Companion treatment |
|---|---|---|
| `new int[count]` and `delete[]` | No faithful equivalent | Create a list; no matching manual release operation exists |
| Raw-owning `IntVec` declaration | Closest Python model | Store a list attribute, while noting that the list already manages its storage |
| `IntVec` destructor | No deterministic-destructor equivalent | Do not use `__del__` as a translation; rely on managed members or context managers |
| Compiler-generated shallow-copy experiment | Failure analogy | `copy.copy` shares the inner list and exposes aliasing, but cannot double-free it |
| Size/capacity invariant text | Partial | Preserve logical size/content; Python exposes no portable list capacity or data pointer |
| `reserve` implementation | No faithful equivalent | `list.append` grows automatically; there is no portable reserve request |
| Six special member declarations | Partial mapping only | Compare with `__init__`, `__copy__`, and explicit methods; assignment/move/destruction differ |
| Copy, assignment, and `std::move` calls | No syntax-level translation | Show rebinding, `copy.copy`, and an explicit `move_from` teaching method |
| Deep-copy constructor | Closest Python model | Implement `__copy__` by copying the inner list |
| `swap` and copy assignment | Closest Python model | Use an explicit `assign_copy` with a replacement list; Python `=` cannot be overloaded |
| Move constructor/assignment | Closest Python model | Explicitly transfer a list and clear the source through `move_from` |
| Rule-of-Zero `vector` member | Direct design goal | A class with a list member needs no cleanup code |
| `unique_ptr` creation and move | No exclusive-owner equivalent | Rebinding does not prove uniqueness; aliases may remain |
| `unique_ptr` tree | Direct tree shape, different ownership enforcement | Dataclass nodes are reclaimed through Python reachability/GC |
| Deleted copy/defaulted move `Connection` | Partial | Reject `copy.copy`/`deepcopy` explicitly; Python has no corresponding defaulted move |

## Allocation and release

The closest high-level result of allocating `count` integer elements is:

```python
values = [0] * count
```

There is no `delete[] values`. Rebinding `values = None` removes one reference
but does not force destruction if another reference still reaches the list.
Python reclaims unreachable objects according to its implementation and
garbage-collection rules. Consequently this code cannot teach matching
`new[]`/`delete[]`, release on every C++ control-flow path, or invalid access
after release.

## An owning-looking class whose member already manages itself

```python
class IntVec:
    def __init__(self, size: int = 0) -> None:
        if size < 0:
            raise ValueError("negative size")
        self._values = [0] * size

    def __len__(self) -> int:
        return len(self._values)
```

The object conceptually owns its `_values` attribute, but the list already
manages allocation, copying of references to its elements, growth, and
reclamation. Adding `__del__` to clear the list would not recreate the C++
destructor model and can make Python finalization harder to reason about.

When a resource has an explicit external action such as closing a file, use a
context manager. For an ordinary list, the Rule-of-Zero-like design is to write
no cleanup method.

## Shallow-copy aliasing without double free

Python's `copy.copy` normally copies an object's outer attribute dictionary but
reuses referenced inner objects. This creates an instructive alias:

```python
import copy


class ShallowIntVec:
    def __init__(self, size: int) -> None:
        self.values = [0] * size


original = ShallowIntVec(3)
duplicate = copy.copy(original)
duplicate.values[0] = 99
assert original.values[0] == 99
```

The unexpected shared mutation is analogous to copying an owning pointer value.
It is **not** the C++ failure: both Python objects refer to one valid managed
list, and Python will not call `delete[]` twice. Use the C++ experiment and
AddressSanitizer to study the actual double-free consequence.

## Size, capacity, and reserve

For the Python list-backed class, the useful invariant is:

```text
len(_values) is the number of logical elements
every position 0 <= index < len(_values) contains a reachable Python object
```

Python exposes no portable list `capacity`, allocation pointer, or `reserve`
operation. The closest application-level operation is simply:

```python
self._values.append(value)
```

The interpreter may grow internal storage, but Python code cannot use that fact
to reason about C++ allocation, exception guarantees, or invalidated element
addresses. The C++ `reserve` example therefore has no faithful translation.

## Special members versus Python operations

The six C++ special members do not map one-to-one:

| C++ operation | Closest Python facility | Important difference |
|---|---|---|
| Default/value constructor | `__init__` | Python initializes an already created object |
| Copy constructor | `copy.copy` and `__copy__` | Called explicitly by library convention, not by ordinary assignment |
| Copy assignment | No overload for `=` | Assignment rebinds a name |
| Move constructor | No language equivalent | An explicit method can simulate transfer |
| Move assignment | No language equivalent | Rebinding does not clear the source object |
| Destructor | `__del__` is not equivalent | Finalization timing and cycle behavior differ |

Compare the call sites carefully:

```python
import copy

a = IntVec(10)
b = copy.copy(a)  # explicit copy protocol
b = a             # rebind b; now a and b are the same object
c = IntVec.move_from(a)  # explicit teaching method, not Python move syntax
```

The final line is a deliberately designed method shown below. Python has no
`std::move` expression that changes overload resolution.

## Explicit deep copy

For an integer list, a safe `__copy__` can copy the managed member:

```python
class IntVec:
    def __init__(self, size: int = 0) -> None:
        self._values = [0] * size

    def __copy__(self) -> "IntVec":
        result = IntVec()
        result._values = self._values.copy()
        return result
```

The two objects now contain equal integers in different list objects. For
nested mutable elements, `list.copy()` is still shallow at the element level;
the class contract must decide whether `copy.deepcopy` or custom cloning is
required.

Python assignment cannot call a copy-assignment operator. An explicit method
can model the strong “prepare replacement, then commit” structure:

```python
def assign_copy(self, other: "IntVec") -> None:
    replacement = other._values.copy()
    self._values = replacement
```

If list copying raises, the old `_values` binding is unchanged. That resembles
the strong-guarantee reasoning behind C++ copy-and-swap, but it is not invoked
by `left = right`, and Python object allocation/finalization still follows
Python rules.

## An explicit move analogy

An educational method can transfer the list object and leave the source empty:

```python
@classmethod
def move_from(cls, other: "IntVec") -> "IntVec":
    result = cls()
    result._values = other._values
    other._values = []
    return result
```

The before/after ownership picture resembles a C++ move: the result reaches the
old list and the source remains valid with an empty list. However:

- the call is an ordinary explicitly named method;
- Python has no rvalue-reference overload selection;
- other aliases to the old list may still exist; and
- containers do not use a `noexcept` move decision during reallocation.

Use this only to compare state diagrams. It is not a recommendation to add move
methods to normal Python classes.

## Rule of Zero

The production Python design is the simplest one:

```python
from dataclasses import dataclass, field


@dataclass
class ScoreBuffer:
    values: list[int] = field(default_factory=list)
```

The managed list member gives useful default behavior without manual cleanup.
Assignment still aliases the same `ScoreBuffer`; use an explicit copy only when
the application needs an independent value. Thus the design goal resembles the
C++ Rule of Zero while the default value semantics do not.

## Exclusive ownership has no ordinary Python type

This code does not create a Python `unique_ptr`:

```python
node = Node(value=10)
owner = node
node = None
```

It merely creates, copies, and removes references. Another alias could still
reach the object, and the type system does not reject copying `owner`. Exclusive
ownership must be a program convention or enforced by a separate discipline;
ordinary Python annotations do not provide the C++ `unique_ptr` guarantee.

The tree shape itself translates naturally:

```python
from __future__ import annotations
from dataclasses import dataclass


@dataclass
class Node:
    value: int = 0
    left: Node | None = None
    right: Node | None = None


root = Node(value=10)
root.left = Node(value=5)
```

When the root and all other references become unreachable, Python can reclaim
the nodes. The attributes do not enforce unique parent ownership, so sharing a
child or creating a cycle remains possible. That is a different invariant from
the C++ `unique_ptr` tree.

## Rejecting explicit copy requests

A Python class can reject the standard copy protocol:

```python
class Connection:
    def __copy__(self) -> "Connection":
        raise TypeError("Connection cannot be copied")

    def __deepcopy__(self, memo: dict[int, object]) -> "Connection":
        del memo
        raise TypeError("Connection cannot be copied")
```

This makes `copy.copy(connection)` and `copy.deepcopy(connection)` fail. It does
not prevent `alias = connection`, because that assignment creates no new
connection. Python also has no defaulted move constructor corresponding to the
C++ declaration; an ownership-transfer API must be designed explicitly around
the actual resource.

## What the Python versions must not hide

- Python `=` is name rebinding, not C++ copy assignment.
- `copy.copy` is an explicit protocol call, not an automatic copy constructor.
- Python has no ordinary move operation or `noexcept` move selection.
- `__del__` is not a replacement for deterministic C++ destruction.
- A Python reference does not enforce exclusive or borrowed ownership.
- Python aliasing examples can explain shared state, but only the C++ program
  can demonstrate double deletion, dangling ownership, and special-member rules.

Runnable versions of the explicit comparisons are collected in
[`python_examples.py`](python_examples.py).
