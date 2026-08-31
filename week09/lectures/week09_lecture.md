# Week 9 Lecture Notes — Classes, Invariants, and Operators

> November 3, 2026 · Source lineage: previous Classes I notes and Rational
> examples from the 2025 C++ notebook

> Python bridge: [Python Contrast Companion for Week 9](week09_python_companion.md)

## Learning objectives

By the end of this lecture, you should be able to:

1. Design a class around an invariant and a small public interface.
2. Use constructors and member-initializer lists.
3. Write `const` member functions and distinguish observation from mutation.
4. Separate class declarations from definitions.
5. Overload an operator without surprising its users.
6. Use a named factory when creation needs validation, alternate construction
   modes, or a deliberate ownership contract.

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | How does a class make invalid states hard to represent? | Design and construct a normalized Rational value |
| 2 | How do member functions and operators form a coherent value interface? | Implement arithmetic, comparison, and stream output |
| 3 | Which construction policies belong in a multi-file design? | Refactor, add a named factory, and test a complete value class |

## Hour 1 — Class boundaries and construction

### 1. A class protects a valid state

A C structure groups fields. A C++ class can additionally control how clients
create and mutate values.

```cpp
class Rational {
 public:
  Rational(int numerator, int denominator);

  int numerator() const {
    return numerator_;
  }
  int denominator() const {
    return denominator_;
  }

  Rational& operator+=(const Rational& other);

 private:
  int numerator_;
  int denominator_;

  void Normalize();
};
```

The invariant is:

- `denominator_ > 0`;
- numerator and denominator are reduced by their greatest common divisor.

Data is private so every public operation can preserve the invariant. Privacy
is a design tool, not a demand to write trivial setters for every field.

### 2. Constructors establish the invariant

```cpp
#include <limits>
#include <numeric>
#include <stdexcept>

Rational::Rational(int numerator, int denominator)
    : numerator_{numerator}, denominator_{denominator} {
  if (denominator_ == 0 || numerator_ == std::numeric_limits<int>::min() ||
      denominator_ == std::numeric_limits<int>::min()) {
    throw std::invalid_argument{"unsupported rational representation"};
  }
  Normalize();
}

void Rational::Normalize() {
  if (denominator_ < 0) {
    numerator_ = -numerator_;
    denominator_ = -denominator_;
  }
  int divisor = std::gcd(numerator_, denominator_);
  numerator_ /= divisor;
  denominator_ /= divisor;
}
```

The member-initializer list constructs members directly. Assignment in the
constructor body would occur after default initialization and fails entirely
for references, `const` members, and members without a default constructor.

The teaching representation rejects `INT_MIN`, whose magnitude cannot be stored
in an `int`; a production numeric class needs a deliberate wider or checked model.

### Initialization order

Members are initialized in declaration order, not the textual order in the
initializer list. Compile with warnings and keep both orders consistent:

```cpp
class Interval {
  int lower_;
  int upper_;

 public:
  Interval(int lower, int upper) : lower_{lower}, upper_{upper} {
    if (lower_ > upper_) throw std::invalid_argument{"reversed interval"};
  }
};
```

### Hour 1 design studio

Design a `BoundedCounter` whose value always lies in `[minimum, maximum]`.
Specify construction failure, increment-at-maximum behavior, observation, and
whether assignment from a plain integer should be allowed. Compare a class with
private fields against a public `struct` plus free functions.

## Hour 2 — Member functions, `this`, and value operators

### 3. `const` member functions

```cpp
int Rational::numerator() const {
  return numerator_;
}
```

The trailing `const` promises not to modify the observable state of the object.
It also permits calls on a `const Rational`.

```cpp
void Print(const Rational& value) {
  std::cout << value.numerator() << '/' << value.denominator();
}
```

Make observer functions `const` by default. This is different from returning a
`const` scalar, which generally adds no useful guarantee.

### 4. The implicit object and `this`

Within a non-static member function, `this` points to the current object.

The following compact teaching implementation has a precondition: every
intermediate multiplication and addition, and every magnitude used during
normalization, must be representable as `int`.

```cpp
Rational& Rational::operator+=(const Rational& other) {
  numerator_ =
      numerator_ * other.denominator_ + other.numerator_ * denominator_;
  denominator_ *= other.denominator_;
  Normalize();
  return *this;
}
```

Returning `*this` by reference supports conventional chaining such as
`a += b += c`. The operator mutates the left operand and preserves its invariant.

### 5. Nonmember binary operators

Implement symmetric binary operators in terms of compound assignment:

```cpp
Rational operator+(Rational left, const Rational& right) {
  left += right;
  return left;
}
```

Passing `left` by value intentionally creates the result object. This design
supports implicit conversion on either operand more symmetrically than a member
`operator+` and avoids duplicating normalization logic.

Output is also a nonmember because the left operand is a stream:

```cpp
#include <ostream>

std::ostream& operator<<(std::ostream& stream, const Rational& value) {
  return stream << value.numerator() << '/' << value.denominator();
}
```

Overload operators only when their meaning matches normal expectations. Do not
turn `+` into an unrelated command merely because the syntax is available.

### Equality and ordering

Once values are normalized, equality is simple:

```cpp
bool operator==(const Rational& left, const Rational& right) {
  return left.numerator() == right.numerator() &&
         left.denominator() == right.denominator();
}
```

Ordering via cross multiplication may overflow `int`. A correct interface must
either use a checked/wider intermediate representation or document a restricted
input range. Algebraic correctness alone is not machine-level correctness.

### Hour 2 implementation task

Complete `operator-=`, unary minus, `operator-`, `operator==`, and stream output.
Each compound operation must preserve normalization. Add a test proving that a
nonmember binary operator does not mutate either operand.

## Hour 3 — Multi-file class design, creation policies, and verification

### 6. Declaration and definition

`rational.hpp` contains the class declaration and small definitions that truly
belong inline. `rational.cpp` includes that header and defines the remaining
members.

```cpp
Rational::Rational(int numerator, int denominator)
```

The `Rational::` scope qualifier states which class owns the member definition.
Compile and link just as with C:

```sh
c++ -std=c++17 -Wall -Wextra -Wpedantic -g -c rational.cpp
c++ -std=c++17 -Wall -Wextra -Wpedantic -g -c main.cpp
c++ rational.o main.o -o rational_demo
```

Headers should be self-contained and protected with `#pragma once` or consistent
include guards.

### 7. `class` versus `struct`

The language difference is only the default access:

- `class`: members and base classes are private by default;
- `struct`: members and base classes are public by default.

Course convention:

- use `struct` for passive records whose fields are the intended interface;
- use `class` when operations protect an invariant or hide representation.

This is a design convention, not a rule enforced by the compiler.

### 8. Named factories and private construction

A `static` member function belongs to the class rather than to one existing
object, has no `this` pointer, and can access private constructors. It can
therefore expose a named creation policy instead of a large set of ambiguous
constructors. Consider a validated sampling interval:

```cpp
#include <cmath>
#include <stdexcept>

class SampleWindow {
 public:
  static SampleWindow FromEndpoints(double start, double finish) {
    if (!std::isfinite(start) || !std::isfinite(finish) || start > finish) {
      throw std::invalid_argument{"invalid sampling window"};
    }
    return SampleWindow{start, finish};
  }

 private:
  SampleWindow(double start, double finish) : start_{start}, finish_{finish} {
  }

  double start_;
  double finish_;
};
```

The factory name communicates how arguments are interpreted, validation occurs
before a value is published, and returning by value gives ordinary value
semantics. Pass small scalar inputs by value rather than by `const` reference.
Week 12 compares copying and resource transfer for owned string parameters; do
not add an ownership-taking overload here before that cost model is available.

A factory is not automatically better than a constructor. Prefer an ordinary
constructor when there is one obvious, valid interpretation. Use named factories
for alternate units/formats, fallible parsing, hidden concrete types, or
explicit failure. This week's factories return ordinary values, so the caller
receives an independent object with automatic lifetime. Week 12 extends this
decision to factories that transfer ownership of dynamically allocated
resources.

### Factory design checkpoint

For a class that can be created from seconds, milliseconds, or a configuration
string, compare overloaded constructors with three named factories. State which
forms throw, which preserve the original object on failure, and which return a
value. Do not implement all factories; first make the creation table
unambiguous.

### Header-dependency exercise

Split Rational into `rational.hpp`, `rational.cpp`, and `rational_test.cpp`.
Forward declare where a complete type is unnecessary, include what the header
itself uses, and keep implementation-only headers out of the public interface.
Then change the private representation and list which files must recompile.

### 9. Test the abstraction, not the fields

```cpp
void TestNormalization() {
  Rational value{2, -4};
  assert(value.numerator() == -1);
  assert(value.denominator() == 2);
}
```

Also test zero denominator, zero numerator, arithmetic sign combinations,
chained compound assignment, and values near integer limits. The simple `int`
representation can overflow during intermediate multiplication even when the
mathematical result is small; that limitation belongs in the contract or a
stronger representation.

### Hour 3 invariant audit

For every public operation, fill a table with valid input, possible failure,
state change, and invariant restoration point. Seed one bug that bypasses
`Normalize`, use a property test (`denominator() > 0` and gcd equals one) to find
it, then add the smallest regression case.

## Final project connection — A class must enter the system safely

Adding a project class requires more than writing its declaration. Before
implementation, make a change-impact map covering its header and source file,
construction site, owner, observers, update or query path, and tests. Forward
declare a type when only a pointer/reference declaration needs it; include the
complete definition where members, inheritance, `sizeof`, or inline operations
require it.

For Thursday's bounded component, state one invariant and test it without
opening the Allegro window. If a factory creates the component, specify whether
it returns a value or reports construction failure, and explain why. Factories
that transfer dynamic ownership are postponed until Week 12. An AI-generated
class skeleton is only a draft until its includes, ownership, and integration
sites have been checked against the repository.

## Check yourself

1. Which operations can create or change a `Rational`?
2. Why is a setter for `denominator_` a risky interface?
3. When does a member function need trailing `const`?
4. Why implement `operator+` in terms of `operator+=`?
5. When is a named factory clearer than an overloaded constructor?
6. Why should this week's named factories return values rather than references
   to local objects?
7. Design an invariant and public interface for a `TimeOfDay` class.

## Summary

- A class is valuable when it protects a meaningful invariant.
- Constructors create valid objects; initializer lists construct members directly.
- Named factories make alternate creation policies and failure contracts explicit.
- `const` member functions expose safe observation.
- Public interfaces should be smaller and more stable than representations.
- Operator overloads should preserve invariants and conventional meaning.

## Optional enrichment — Additional class syntax

The core lecture uses one invariant-establishing constructor and a small set of
conventional operators. The following language mechanisms are useful when an
interface genuinely needs them, but they should not drive the initial design.

### Delegating and converting constructors

```cpp
class Rational {
 public:
  Rational() : Rational{0, 1} {
  }
  explicit Rational(int whole) : Rational{whole, 1} {
  }
  Rational(int numerator, int denominator);
};
```

A delegating constructor chooses one implementation as the
invariant-establishing path. `explicit` prevents surprising implicit
conversions such as passing an `int` where a `Rational` is expected. Remove
`explicit`, compile examples such as `Rational r = 3`, and discuss when numeric
conversion is desirable.

### Overloading and default arguments

Member functions may be overloaded by parameter types/count and by trailing
`const`, but not by return type alone. Default arguments are substituted at the
call site and should appear in one declaration, normally the header.

Avoid pairs of overloads whose conversions make a call ambiguous. Use a small
test call matrix to confirm which overload accepts `int`, `double`, `const
Rational`, and temporary arguments.

### Class state and fluent lifetime

A static data member belongs to the class rather than each object. Use such
state sparingly: hidden global state can make tests interdependent.

Returning `*this` by reference supports fluent mutation only while the object
remains alive. Never return a reference to a temporary or local object:

```cpp
Rational& BadFactory() {
  Rational local{1, 2};
  return local; /* dangling reference */
}
```

Factories should return values. A read-only query should return a value or a
carefully justified borrowed reference.

## References and source materials

- [Classes I](<https://github.com/htchen/i2p-nthu/blob/master/程式設計二/Classes%20I/README.md>)
- [2025 Week 7 notebook (Colab)](https://colab.research.google.com/drive/1oHBcNeAXt4ZeQJsdG2q4RU5m9Yu_9CCw)
