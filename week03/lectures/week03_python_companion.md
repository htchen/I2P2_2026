# Python Contrast Companion for Week 3

Read this companion beside the [Week 3 lecture
notes](week03_lecture.md). Records, enumerations, invariants, modules, tests, and
file parsing all have useful Python counterparts. C structure layout, unions,
headers, the preprocessor, object files, and linking do not.

## Relationship labels

- **Direct:** the design or algorithm carries over closely.
- **Same goal, different mechanism:** Python modules, exceptions, or returned
  values replace a C interface mechanism.
- **Closest Python model:** the example preserves one useful property while
  omitting C layout or build semantics.
- **No faithful equivalent:** the C feature depends on preprocessing, storage
  overlap, compilation, or linking absent from ordinary Python.

## Coverage map

Every fenced example in the Week 3 note is accounted for below.

| Source example | Relationship | Companion treatment |
|---|---|---|
| `Student` structure and object | Closest model | Use a dataclass; explain missing fixed character array and layout |
| Structure assignment | Partial | `copy.copy` creates another dataclass but may share mutable attributes |
| `TokenKind` enum and `Token` | Direct design | Use `Enum` and a dataclass |
| `typedef struct Token Token` | No typedef need | Python binds the class name directly; an alias does not create a new type |
| `Rational` structure and output factory | Same goal, different mechanism | Use a validated immutable value returned by a constructor/factory |
| Designated `Student` initializer | Direct construction goal | Use keyword arguments |
| Tagged union `Value` | Closest model | Pair an enum with a Python payload and validate matching alternatives |
| `rational.h` header | No header equivalent | Put the public class/functions in a module |
| `rational.c` implementation | Direct invariant logic, different integers/errors | Normalize in a class constructor; Python has no `INT_MIN` bound or output pointer |
| Three compile/link commands | No link-step equivalent | Byte-compile modules optionally, then execute/import them |
| Public `Counter` structure/functions | Direct abstraction goal | Use a class with methods and a read-only property |
| `BUFFER_CAPACITY` macro | Same constant goal | Bind an uppercase module constant |
| `BAD_SQUARE` macro | No textual-substitution equivalent | Use a function; arguments are evaluated once |
| Makefile | Tool, different dependency products | A script or Make can run checks, but Python modules need no object-file link graph |
| `assert` in `array_sum` | Direct development check | Use `assert`, while keeping public input validation separate |
| `students_read` file parser | Direct parsing goal | Return a list, raise for malformed/extra records, and enforce capacity |

## Records and copies

```python
from dataclasses import dataclass


@dataclass
class Student:
    student_id: int
    name: str
    grade: float


student = Student(student_id=1001, name="Ada", grade=92.5)
print(f"{student.name}: {student.grade:.1f}")
```

The named fields and construction goal are direct. A Python string is not an
embedded `char[32]`, and the dataclass does not promise C field order, padding,
or one contiguous record representation.

```python
import copy

duplicate = copy.copy(student)
```

The current attributes are immutable values, so this behaves much like an
independent record value. If a dataclass contains a list, `copy.copy` shares
that list; C structure assignment instead copies every embedded array element
and copies pointer values only for pointer fields. Always reason from the field
representations rather than the word “copy.”

Keyword arguments are the closest counterpart to designated initialization:

```python
student = Student(student_id=1001, name="Ada", grade=92.5)
```

Unlike a partially designated C initializer, omitted required dataclass fields
cause an error unless the class defines defaults.

## Enumerations and tagged records

```python
from __future__ import annotations

from dataclasses import dataclass
from enum import Enum, auto


class TokenKind(Enum):
    INTEGER = auto()
    PLUS = auto()
    MINUS = auto()
    END = auto()
    INVALID = auto()


@dataclass(frozen=True)
class Token:
    kind: TokenKind
    value: int = 0
```

This preserves the design: the tag determines how clients interpret the
record. Python's enum members are objects, not C integer constants with a
specified compatible representation.

The C typedef makes the short name `Token` available. Python already binds that
name when the class statement executes. Writing `TokenAlias = Token` creates a
second name for the same class, not a distinct representation or compile-time
typedef.

## Rational invariant and construction

```python
from dataclasses import dataclass
from math import gcd


@dataclass(frozen=True)
class Rational:
    numerator: int
    denominator: int

    def __post_init__(self) -> None:
        if self.denominator == 0:
            raise ValueError("zero denominator")
        numerator = self.numerator
        denominator = self.denominator
        if denominator < 0:
            numerator = -numerator
            denominator = -denominator
        divisor = gcd(numerator, denominator)
        object.__setattr__(self, "numerator", numerator // divisor)
        object.__setattr__(self, "denominator", denominator // divisor)

    def __str__(self) -> str:
        return f"{self.numerator}/{self.denominator}"
```

The invariant is shared: denominator positive, fraction reduced, and zero
denominator rejected. Python returns a constructed value or raises an exception
instead of filling a caller-provided output structure and returning success.
Its integers also have no `INT_MIN` negation boundary.

## Tagged union versus tagged Python payload

C's union stores alternatives in overlapping bytes. Python has no ordinary
storage-overlap feature, but it can preserve the tagged-alternative contract:

```python
from __future__ import annotations

from dataclasses import dataclass
from enum import Enum, auto


class ValueKind(Enum):
    INTEGER = auto()
    REAL = auto()
    ERROR = auto()


@dataclass(frozen=True)
class Value:
    kind: ValueKind
    payload: int | float | str

    def __post_init__(self) -> None:
        expected = {
            ValueKind.INTEGER: int,
            ValueKind.REAL: float,
            ValueKind.ERROR: str,
        }[self.kind]
        if type(self.payload) is not expected:
            raise TypeError("payload does not match its tag")
```

All payload objects exist through normal references; the alternatives do not
share one memory region. The explicit check retains the important invariant
that clients must not pair a tag with the wrong interpretation.

## Modules instead of headers and linking

A Python module can contain the public `Rational` class in `rational.py`, and a
client imports it:

```python
from rational import Rational


value = Rational(2, 4)
print(value)
```

There is no duplicate header declaration, include guard, or separately linked
implementation symbol. Import executes a module and binds its public names.
Underscore-prefixed names communicate internal intent by convention rather than
C `static` linkage.

Optional bytecode checking and execution look like:

```sh
python3 -m py_compile rational.py main.py
python3 main.py
```

This does not produce C object files or a link-time undefined-reference stage.
Import and name errors may instead appear when the program executes.

## Encapsulation and constants

The counter interface becomes a small class:

```python
class Counter:
    def __init__(self) -> None:
        self._value = 0

    def increment(self) -> None:
        self._value += 1

    @property
    def value(self) -> int:
        return self._value
```

The leading underscore is a convention, not C header-enforced opacity. Clients
can still access `_value`; the interface communicates the intended operations.

An uppercase binding replaces a simple object-like macro:

```python
BUFFER_CAPACITY = 256
```

Python does not enforce immutability, but tools and readers recognize the
constant convention.

Use a function instead of a function-like macro:

```python
def square(value: int) -> int:
    return value * value
```

The argument expression is evaluated once before the call. There is no textual
substitution corresponding to `BAD_SQUARE(i++)`, and Python has no `++`
operator.

## Build automation

The Makefile in the C note records object-file dependencies and link commands.
A small Python program normally needs neither product. Make or another task
runner can still automate checks:

```make
check:
	python3 -m py_compile rational.py main.py
	python3 main.py
```

This is tool reuse, not a translation of the C build graph.

## Assertions and input contracts

```python
from collections.abc import Sequence


def array_sum(values: Sequence[int]) -> int:
    assert values is not None
    return sum(values)
```

As in C, assertions document internal conditions and may be disabled. Public
input failures should use explicit validation rather than relying solely on
`assert`.

The bounded student parser can return its records directly:

```python
from collections.abc import Iterable


def students_read(lines: Iterable[str], capacity: int) -> list[Student]:
    students: list[Student] = []
    for line in lines:
        if not line.strip():
            continue
        if len(students) == capacity:
            raise ValueError("too many records")
        fields = line.split()
        if len(fields) != 3:
            raise ValueError("expected id, name, and grade")
        students.append(
            Student(
                student_id=int(fields[0]),
                name=fields[1],
                grade=float(fields[2]),
            )
        )
    return students
```

The same record grammar and capacity policy are visible. Python raises on bad
conversion and manages the returned list; it does not fill a fixed destination
array or update an output count pointer.

## What the Python versions must not hide

- Dataclasses do not define C layout, embedded arrays, or padding.
- A Python tagged payload is not an overlapping C union.
- Modules do not reproduce headers, object files, or linking.
- Functions avoid preprocessor multiple-evaluation hazards precisely because
  they are not textual macros.
- Returning a value or raising an exception does not teach C output-parameter
  validity and cleanup.

Runnable versions are collected in [`python_examples.py`](python_examples.py).
