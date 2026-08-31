# Python Contrast Companion for Week 9

Read this companion beside the [Week 9 lecture
notes](week09_lecture.md). Classes, constructors, invariants, properties,
operators, factories, and tests have strong Python parallels. C++ access
control, `const` member functions, references to `*this`, overload resolution,
separate compilation, and dangling references do not.

## Relationship labels

- **Direct design:** the invariant or value operation transfers closely.
- **Same goal, different language contract:** Python uses conventions,
  properties, or special methods instead of C++ static rules.
- **Closest Python model:** one observable effect is similar but lifetime or
  overload behavior differs.
- **No faithful equivalent:** C++ compilation or reference lifetime is absent.

## Coverage map

Every fenced example in the Week 9 note is accounted for below.

| Source example | Relationship | Companion treatment |
|---|---|---|
| `Rational` class declaration | Direct abstraction goal | Define one Python class; underscores are conventions rather than `private` |
| Constructor and normalization | Direct invariant | Validate, normalize sign, and divide by GCD in `__init__` |
| `Interval` initialization order | Direct invariant, different mechanics | Assign fields then validate; Python has no C++ member-initializer order rule |
| `numerator() const` | Same observation goal | Use a read-only property; no `const` member-function type |
| `Print(const Rational&)` | Direct output goal | Print the value; Python cannot declare borrowed `const&` |
| `operator+=` and `this` | Closest model | Implement `__iadd__`, mutate `self`, and return it |
| Nonmember `operator+` | Same value result | Implement `__add__`; Python dispatches through special methods |
| Stream insertion operator | Same formatting goal | Implement `__str__` |
| Equality operator | Direct normalized-value comparison | Implement `__eq__` or use value fields |
| Qualified constructor declaration | No separate declaration form | `__init__` is defined once in the class/module |
| Compile/object/link commands | No link-step equivalent | Optionally byte-compile, import, and execute modules |
| `SampleWindow` named factory | Direct design goal | Use `@classmethod`; constructor privacy remains a convention |
| Normalization test | Direct | Assert public observations, not internal mutation steps |
| Delegating/converting constructors | No overload equivalent | Use defaults and named class methods |
| Returning reference to local | Opposite lifetime behavior | Returning a local Python object is safe while the caller references it |

## Rational as a validated Python value

```python
from math import gcd


class Rational:
    def __init__(self, numerator: int = 0, denominator: int = 1) -> None:
        if denominator == 0:
            raise ValueError("zero denominator")
        if denominator < 0:
            numerator = -numerator
            denominator = -denominator
        divisor = gcd(numerator, denominator)
        self._numerator = numerator // divisor
        self._denominator = denominator // divisor

    @property
    def numerator(self) -> int:
        return self._numerator

    @property
    def denominator(self) -> int:
        return self._denominator
```

Construction establishes the same normalized invariant. Python integers have no
C++ `int` minimum boundary, so the representation check differs. Leading
underscores communicate non-public implementation detail but do not prevent a
client from accessing the attributes.

## Initialization order and interval validation

```python
class Interval:
    def __init__(self, lower: int, upper: int) -> None:
        self._lower = lower
        self._upper = upper
        if self._lower > self._upper:
            raise ValueError("reversed interval")
```

The invariant is direct. Python assignments execute in written order and there
is no separate C++ rule that members initialize in declaration order regardless
of initializer-list spelling.

## Observation, `const`, and printing

The properties above provide observation without a setter. That makes normal
client syntax read-only, but it is not a C++ `const` guarantee: Python has no
type distinction between calling a method on a mutable or const object.

```python
def print_rational(value: Rational) -> None:
    print(f"{value.numerator}/{value.denominator}", end="")
```

The parameter is an ordinary Python reference. The annotation cannot promise
non-mutation or express `const Rational&` borrowing.

## Augmented addition and the implicit object

Python spells the receiver `self` explicitly:

```python
def __iadd__(self, other: "Rational") -> "Rational":
    numerator = (
        self._numerator * other._denominator
        + other._numerator * self._denominator
    )
    denominator = self._denominator * other._denominator
    replacement = Rational(numerator, denominator)
    self._numerator = replacement.numerator
    self._denominator = replacement.denominator
    return self
```

Returning `self` supports augmented assignment. Python may still rebind the
left-hand name to the returned object after `__iadd__`; C++ `operator+=` returns
a `Rational&` to the same object as part of its static interface.

Ordinary addition belongs to a special method rather than a nonmember overload:

```python
def __add__(self, other: "Rational") -> "Rational":
    return Rational(
        self.numerator * other.denominator
        + other.numerator * self.denominator,
        self.denominator * other.denominator,
    )
```

The result is a new normalized value. Python operator dispatch and fallback to
`NotImplemented` differ from C++ overload resolution and implicit conversions.

Formatting uses `__str__`:

```python
def __str__(self) -> str:
    return f"{self.numerator}/{self.denominator}"
```

There is no output-stream object returned to support C++ `<<` chaining; `print`
calls `str(value)` as needed.

Equality is direct because normalization makes fields canonical:

```python
def __eq__(self, other: object) -> bool:
    if not isinstance(other, Rational):
        return NotImplemented
    return (
        self.numerator == other.numerator
        and self.denominator == other.denominator
    )
```

## Definitions and modules

Python has no separate spelling corresponding to
`Rational::Rational(int, int)` outside a class declaration. The method is
defined in the class body, and the module is imported at run time.

```sh
python3 -m py_compile rational.py main.py
python3 main.py
```

These commands do not create C++ object files or perform symbol linking.

## Named factory

```python
import math


class SampleWindow:
    def __init__(self, start: float, finish: float) -> None:
        self._start = start
        self._finish = finish

    @classmethod
    def from_endpoints(
        cls, start: float, finish: float
    ) -> "SampleWindow":
        if not math.isfinite(start) or not math.isfinite(finish):
            raise ValueError("non-finite endpoint")
        if start > finish:
            raise ValueError("reversed sampling window")
        return cls(start, finish)
```

The named construction policy transfers directly. Python cannot make
`__init__` private in the C++ access-control sense; clients can still call it.
A stronger Python design may validate in `__init__` as well so every path
preserves the invariant.

## Testing public behavior

```python
def test_normalization() -> None:
    value = Rational(2, -4)
    assert value.numerator == -1
    assert value.denominator == 2
```

The test observes the same public invariant and does not depend on the sequence
of normalization assignments.

## Constructor alternatives

Python does not overload constructors by parameter count/type. Defaults cover
the zero and two-integer cases, while a named method makes intent explicit:

```python
@classmethod
def from_whole(cls, whole: int) -> "Rational":
    return cls(whole, 1)
```

There is no `explicit` keyword preventing an implicit conversion from `int`.
Python operations must decide explicitly whether to accept integers and convert
them inside special methods.

## Returning a local object is safe in Python

```python
def make_half() -> Rational:
    local = Rational(1, 2)
    return local
```

The returned reference keeps the object reachable, so this does not dangle.
Python does not expose a reference to stack-duration local storage equivalent to
the bad C++ factory. Use the C++ lifetime model to understand why returning
`Rational&` to a local object is invalid.

## What the Python versions must not hide

- Underscores and properties are conventions, not C++ access control or
  `const` member functions.
- `self` is an ordinary Python parameter, not the typed C++ `this` pointer.
- Python special-method dispatch is not C++ nonmember/member overload
  resolution.
- Module import is not separate compilation and linking.
- Returning a local Python object is safe and therefore cannot demonstrate a
  dangling C++ reference.

The complete class and tests are in [`python_examples.py`](python_examples.py).
