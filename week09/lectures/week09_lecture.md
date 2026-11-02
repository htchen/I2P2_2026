# Week 9 Lecture Notes — Classes, Invariants, and Operators

> November 3, 2026 · Source lineage: previous Classes I notes and Rational
> examples from the 2025 C++ notebook

> Python bridge: [Python Contrast Companion for Week 9](week09_python_companion.md)

---

## Student route

- **Core:** establish a class invariant in its constructor, distinguish observer
  and mutating members, and implement conventional value operators consistently.
- **Practice:** complete the [Week 9 Rational exercise](lecture_exercises/week09_ex.md);
  its API matches the compound-operator design in this note. Compare afterward
  with [the complete example](examples.cpp).
- **Supporting ideas:** named factories and multi-file organization extend the
  same class boundary after the normalized value type works.
- **Python bridge:** use the companion to compare class syntax while retaining
  C++ value, reference, and `const` semantics.

---

## Learning objectives

By the end of this lecture, you should be able to:

1. Design a class around an invariant and a small public interface.
2. Use constructors and member-initializer lists.
3. Write `const` member functions and distinguish observation from mutation.
4. Separate class declarations from definitions.
5. Overload an operator without surprising its users.
6. Use a named factory when creation needs validation, alternate construction
   modes, or a deliberate ownership contract.

---

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | How does a class make invalid states hard to represent? | Design and construct a normalized Rational value |
| 2 | How do member functions and operators form a coherent value interface? | Implement arithmetic, comparison, and stream output |
| 3 | Which construction policies belong in a multi-file design? | Refactor, add a named factory, and test a complete value class |

Each hour alternates explanation, object-state or compile-time traces, live
coding, and short practice. Exercises labelled **Core live** belong to the
planned classroom route. Exercises labelled **Extension** may move to the
Thursday lab or independent study when the class needs more time.

### Inline practice routine

For each **Try it now** activity:

1. predict the output, object state, return value, exception path, or compiler
   decision before running code;
2. identify the invariant and decide which operation must preserve it;
3. compile valid examples with
   `c++ -std=c++17 -Wall -Wextra -Wpedantic`;
4. compare the observation with the hidden solution; and
5. explain which behavior is a general abstraction-design idea and which part
   is specific C++ syntax or library support.

Only the question is visible initially. Expand **Reveal solution** after making
an attempt. Every solution identifies whether its text is terminal output,
object-state/compile-time trace, or an explanation that the fragment performs
no input/output.

- **Core live:** part of the planned in-class route.
- **Extension:** additional practice for the lab or later review.

The core-live activities total about 21 minutes in Hour 1, 21 minutes in Hour
2, and 16 minutes in Hour 3. The remaining time is for explanation, live
coding, questions, transitions, and a short break.

---

## Hour 1 — Class boundaries and construction

> **Hour 1 route:** [A class protects a valid state](#1-a-class-protects-a-valid-state)
> → [Constructors establish the invariant](#2-constructors-establish-the-invariant)
> → [Initialization order](#initialization-order)
> → [design studio](#hour-1-design-studio)

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

This class declaration defines an interface and performs no run-time work by
itself. A client may construct values and call public members, but cannot name
`numerator_`, `denominator_`, or `Normalize()` directly.

#### Try it now [Core live] — identify the invariant boundary (4 minutes)

Treat each line after construction as an independent query. Decide whether it
is allowed by the public interface and whether it can change an existing
object:

```cpp
Rational value{2, 4};
int top = value.numerator();
value.denominator_ = 0;
value.Normalize();
value += Rational{1, 4};
```

Then explain why adding public setters for both fields would weaken the class.

<details>
<summary>Reveal solution</summary>

- Construction is allowed; after the constructor implementation is added, the
  intended state is `1/2`.
- `numerator()` is a public observer, so `top` becomes `1` and `value` is
  unchanged.
- Direct assignment to `denominator_` and a direct call to `Normalize()` are
  compile-time errors because both names are private.
- `operator+=` is public and may change `value`; the intended result is `3/4`.

Independent setters could temporarily or permanently create a zero denominator
or an unreduced pair. A smaller operation-oriented interface preserves the
invariant after every completed public call. This is an access and object-state
trace; the fragment has no terminal output.

</details>

---

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

`std::numeric_limits<int>::min()` is the smallest value representable by this
implementation's `int`; the information comes from `<limits>` rather than a
hard-coded machine assumption. `std::gcd`, declared by `<numeric>`, returns the
greatest common divisor used to reduce the two fields. Because a zero
denominator was rejected first, the divisor cannot be zero when both fields are
normalized.

The member-initializer list constructs members directly. Assignment in the
constructor body would occur after default initialization and fails entirely
for references, `const` members, and members without a default constructor.

The teaching representation rejects `INT_MIN`, whose magnitude cannot be stored
in an `int`; a production numeric class needs a deliberate wider or checked model.

The constructor and `Normalize()` print nothing. Their visible result is the
state of the completed object, or an exception before an object is made
available to the caller.

#### Try it now [Core live] — trace construction and normalization (5 minutes)

For each construction, predict either the normalized numerator/denominator or
the exception:

```cpp
Rational a{6, 8};
Rational b{2, -4};
Rational c{0, 5};
Rational d{1, 0};
```

Why must validation and normalization finish before the caller can use the new
object?

<details>
<summary>Reveal solution</summary>

| Construction | Result |
|--------------|--------|
| `Rational a{6, 8}` | `a` stores `3/4` |
| `Rational b{2, -4}` | `b` stores `-1/2` |
| `Rational c{0, 5}` | `c` stores `0/1` |
| `Rational d{1, 0}` | throws `std::invalid_argument`; no `d` object is published |

The constructor is the boundary between raw arguments and a valid class value.
If it returns normally, every public operation may rely on the invariant instead
of repeatedly checking whether the denominator is zero or the sign is stored in
the wrong field. This table is an object-state and exception trace; there is no
terminal output.

</details>

---

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

This definition produces no output. `Interval{2, 5}` completes with
`lower_ == 2` and `upper_ == 5`; `Interval{5, 2}` throws before a usable object
is returned to the caller.

#### Try it now [Core live] — follow declaration-order initialization (4 minutes)

Suppose the constructor were written with the initializer list reversed:

```cpp
Interval(int lower, int upper) : upper_{upper}, lower_{lower} {
  if (lower_ > upper_) throw std::invalid_argument{"reversed interval"};
}
```

Which member is actually initialized first? What warning should the compiler
help reveal, and does reversing the list change the final values in this
particular example?

<details>
<summary>Reveal solution</summary>

`lower_` is initialized first because it is declared first in the class;
`upper_` is initialized second. A warning-enabled compiler can report that the
initializer-list order disagrees with declaration order. Both initializers use
independent constructor parameters, so the final values happen to be unchanged
here. The mismatch becomes dangerous when one member's initializer depends on
another member.

This is an initialization-order and diagnostic trace. The class definition
itself produces no terminal output.

</details>

---

### Hour 1 design studio

#### Try it now [Core live] — design an invariant before methods (8 minutes)

Design a `BoundedCounter` whose value always lies in `[minimum, maximum]`.
Specify construction failure, increment-at-maximum behavior, observation, and
whether assignment from a plain integer should be allowed. Compare a class with
private fields against a public `struct` plus free functions.

<details>
<summary>Reveal solution</summary>

One coherent contract is:

| Decision | Example policy |
|----------|----------------|
| Invariant | `minimum_ <= value_ && value_ <= maximum_` |
| Construction | throw `std::invalid_argument` for reversed bounds or an initial value outside them |
| Observation | `value() const`, `minimum() const`, and `maximum() const` |
| Increment at maximum | return `false` and leave the object unchanged |
| Assignment from `int` | omit it; use a checked member such as `SetValue(int)` only if the use case needs it |

Private fields prevent clients from bypassing the checks. A public record plus
free functions can work only if every client voluntarily avoids direct invalid
mutation, so it does not enforce the same boundary. Other policies, such as
throwing or wrapping at maximum, are valid if documented consistently. This is
an interface and state-transition design; no fixed terminal output is implied.

</details>

---

## Hour 2 — Member functions, `this`, and value operators

> **Hour 2 route:** [`const` member functions](#3-const-member-functions)
> → [The implicit object and `this`](#4-the-implicit-object-and-this)
> → [Nonmember binary operators](#5-nonmember-binary-operators)
> → [Equality and ordering](#equality-and-ordering)
> → [implementation task](#hour-2-implementation-task)

### 3. `const` member functions

```cpp
int Rational::numerator() const {
  return numerator_;
}
```

The trailing `const` promises not to modify the observable state of the object.
It also permits calls on a `const Rational`.

The function returns one integer and produces no terminal output.

```cpp
void Print(const Rational& value) {
  std::cout << value.numerator() << '/' << value.denominator();
}
```

Make observer functions `const` by default. This is different from returning a
`const` scalar, which generally adds no useful guarantee.

For a normalized value representing one half, `Print` writes `1/2` without a
trailing newline and leaves the object unchanged.

#### Try it now [Core live] — read two kinds of `const` (4 minutes)

Given `const Rational value{2, 4};`, consider each declaration alternative
separately. Decide which alternatives allow `Print(value)` and
`value.numerator()` to compile:

```cpp
int numerator();
int numerator() const;
const int numerator() const;
```

Which `const` controls whether the function can be called on `value`, and why
is `const int` not the important guarantee here?

<details>
<summary>Reveal solution</summary>

- A member declared only as `int numerator();` cannot be called on a `const`
  object because it does not promise to preserve that object.
- `int numerator() const;` is the conventional observer declaration and works.
- `const int numerator() const;` can also be called, but top-level `const` on a
  returned scalar adds no useful caller-visible protection because the caller
  receives a separate integer value.

The trailing `const` after the parameter list qualifies the implicit object.
The return-type `const` qualifies a temporary scalar result. This is a
compile-time contract trace; the declarations produce no output.

</details>

---

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

The function itself prints nothing. For `a == 1/6` and `b == 1/3`, `a += b`
returns an alias to `a`, leaves `b` unchanged, and finishes with `a == 1/2`.

#### Try it now [Core live] — trace the implicit object and chaining (5 minutes)

Start with `a == 1/6`, `b == 1/3`, and `c == 1/2`. Trace:

```cpp
a += b += c;
```

Which object is `this` during each call, in what order do the calls occur, and
what are the final values?

<details>
<summary>Reveal solution</summary>

Compound assignment groups from right to left:

1. `b += c` runs first. Its `this` points to `b`; `b` becomes `5/6`, and the
   operator returns `b` by reference.
2. `a += b` then runs with the updated `b`. Its `this` points to `a`; `a`
   becomes `1/1`.
3. `c` remains `1/2`.

Final object state:

```text
a = 1/1
b = 5/6
c = 1/2
```

These lines are a state trace unless a driver explicitly prints them. The
operator calls themselves produce no terminal output.

</details>

---

### 5. Nonmember binary operators

Implement symmetric binary operators in terms of compound assignment:

```cpp
Rational operator+(Rational left, const Rational& right) {
  left += right;
  return left;
}
```

Passing `left` by value intentionally creates the result object. This design
is appropriate because neither operand conceptually owns addition, and it avoids
duplicating normalization logic. The optional enrichment later explains how
one-argument constructors can affect operator conversions; that mechanism is
not needed for this two-argument `Rational` constructor.

For `left_source == 1/6` and `right_source == 1/3`, the value parameter `left`
is a copy of `left_source`. The function returns `1/2` while both source objects
remain unchanged.

#### Try it now [Core live] — separate the result from its operands (5 minutes)

Trace the objects created or borrowed by:

```cpp
Rational left{1, 6};
Rational right{1, 3};
Rational sum = left + right;
```

Why is the first operator parameter passed by value, the second by `const&`,
and the result returned by value?

<details>
<summary>Reveal solution</summary>

- The operator's local `left` is an independent copy of the caller's left
  operand, so it can be changed by `+=` without changing the source object.
- `right` is a required read-only borrow because the operation only observes
  the right operand.
- The returned `Rational` is independent owned state for `sum`.

After the statement:

```text
left  = 1/6
right = 1/3
sum   = 1/2
```

This is an ownership and object-state trace, not output from the fragment.

</details>

Output is also a nonmember because the left operand is a stream:

```cpp
#include <ostream>

std::ostream& operator<<(std::ostream& stream, const Rational& value) {
  return stream << value.numerator() << '/' << value.denominator();
}
```

Overload operators only when their meaning matches normal expectations. Do not
turn `+` into an unrelated command merely because the syntax is available.

#### Try it now [Core live] — preserve the stream chain (3 minutes)

Predict the output and the reason for returning `std::ostream&`:

```cpp
std::cout << Rational{2, -4} << " + " << Rational{3, 6} << '\n';
```

<details>
<summary>Reveal solution</summary>

```text
-1/2 + 1/2
```

Each insertion returns the same stream by reference, allowing the next `<<` to
continue the chain. Both rational values are normalized before output. This is
exact terminal output, including the final newline.

</details>

---

### Equality and ordering

Once values are normalized, equality is simple:

```cpp
bool operator==(const Rational& left, const Rational& right) {
  return left.numerator() == right.numerator() &&
         left.denominator() == right.denominator();
}

bool operator!=(const Rational& left, const Rational& right) {
  return !(left == right);
}
```

This course compiles as C++17. In C++17, defining `operator==` does **not**
automatically provide `operator!=`, so a complete equality interface must define
both. Implement `!=` in terms of `==` so the two operations cannot disagree.
C++20 added comparison rewriting that can use `==` for an expression written
with `!=`; do not rely on that newer rule in this course.

Ordering via cross multiplication may overflow `int`. A correct interface must
either use a checked/wider intermediate representation or document a restricted
input range. Algebraic correctness alone is not machine-level correctness.

The equality functions return Boolean values and produce no output. Because
construction normalizes representation, `Rational{1, 2} == Rational{2, 4}` is
`true`, while the corresponding `!=` expression is `false`.

#### Try it now [Core live] — keep C++17 equality coherent (4 minutes)

For C++17, predict the result of each expression and explain what happens if
`operator!=` is removed:

```cpp
Rational{1, 2} == Rational{2, 4}
Rational{-1, 2} != Rational{1, -2}
Rational{1, 3} != Rational{1, 2}
```

<details>
<summary>Reveal solution</summary>

```text
true
false
true
```

Normalization gives the first pair the same fields and also gives the second
pair the same fields. The third pair differs. If `operator!=` is removed, the
two `!=` expressions fail to compile under this course's C++17 mode; C++17 does
not synthesize `!=` from `==`. These lines are expression-result traces unless
a driver prints them.

</details>

---

### Hour 2 implementation task

#### Try it now [Extension] — implement through one invariant path (8 minutes)

In the starter, complete normalization, `operator+=`, `operator-=`, unary minus,
and `operator==`. The supplied nonmember `operator+` and `operator-` delegate to
the compound operators; the supplied C++17 `operator!=` delegates to `==`; and
stream output delegates to the public accessors. Test all three delegations and
prove that a nonmember binary operator does not mutate either operand. Preserve
the constructor's rejection of `INT_MIN`, and reject an unsupported arithmetic
result before modifying the left operand.

<details>
<summary>Reveal solution</summary>

Before comparing with `examples.cpp`, verify this dependency chain:

```text
constructor -> Normalize
operator+=  -> checked arithmetic/reduction -> invariant
operator+   -> operator+= on a copy
operator!=  -> operator==
operator<<  -> public observers
```

The required behavioral checks are: reject a zero denominator, normalize signs
and common factors, preserve both operands of `+` and `-`, return `*this` from
compound assignment, and print a normalized value. The completed starter's
failure check must also leave the left operand unchanged. The completed
starter's final terminal line is:

```text
all Week 9 starter checks passed
```

The implementation remains in the starter exercise; this panel reveals the
design and verification target, not the source solution.

</details>

---

## Hour 3 — Multi-file class design, creation policies, and verification

> **Hour 3 route:** [Declaration and definition](#6-declaration-and-definition)
> → [`class` versus `struct`](#7-class-versus-struct)
> → [Named factories and private construction](#8-named-factories-and-private-construction)
> → [Factory design checkpoint](#factory-design-checkpoint)
> → [Header-dependency exercise](#header-dependency-exercise)
> → [Test the abstraction, not the fields](#9-test-the-abstraction-not-the-fields)
> → [invariant audit](#hour-3-invariant-audit)

### 6. Declaration and definition

`rational.hpp` contains the class declaration and small definitions that truly
belong inline. `rational.cpp` includes that header and defines the remaining
members.

The header can declare an observer without defining its body:

```cpp
class Rational {
 public:
  int numerator() const;

 private:
  int numerator_;

  // Other members are omitted here.
};
```

The source file then provides the definition:

```cpp
int Rational::numerator() const {
  return numerator_;
}
```

The `Rational::` scope qualifier states which class owns the member definition.
These declaration/definition fragments produce no output by themselves.
Compile and link just as with C:

```sh
c++ -std=c++17 -Wall -Wextra -Wpedantic -g -c rational.cpp
c++ -std=c++17 -Wall -Wextra -Wpedantic -g -c main.cpp
c++ rational.o main.o -o rational_demo
```

Headers should be self-contained and protected with `#pragma once` or consistent
include guards.

A forward declaration introduces a type name without supplying its complete
layout:

```cpp
class Renderer;

void Draw(const Renderer& renderer);
```

This is enough to declare a pointer or reference interface. Constructing a
`Renderer` by value, storing one as a data member, applying `sizeof`, or defining
an inline operation that uses its members requires the complete class
definition. The declarations above perform no run-time work and produce no
output.

On success, the two compilation commands normally print nothing and create
`rational.o` and `main.o`. The link command normally prints nothing and creates
the `rational_demo` executable. Program output appears only when that executable
is run.

#### Try it now [Core live] — locate a multi-file failure (4 minutes)

Classify each problem by the earliest stage that should report it:

1. `main.cpp` calls a member absent from `rational.hpp`;
2. the header declares `Rational::numerator()` but no source file defines it;
3. both `rational.cpp` and `main.cpp` define the same non-inline function;
4. all translation units compile and link successfully.

<details>
<summary>Reveal solution</summary>

1. Compiling `main.cpp` fails because its translation unit cannot see a matching
   declaration.
2. Compilation may succeed, but linking fails with a missing-definition
   diagnostic when the function is used.
3. Linking fails with a multiple-definition diagnostic.
4. A `rational_demo` executable is created; only running it can produce the
   program's run-time output.

This is a translation/link trace. Exact diagnostic wording varies by compiler
and linker.

</details>

---

### 7. `class` versus `struct`

The language difference is only the default access:

- `class`: members and base classes are private by default;
- `struct`: members and base classes are public by default.

Course convention:

- use `struct` for passive records whose fields are the intended interface;
- use `class` when operations protect an invariant or hide representation.

This is a design convention, not a rule enforced by the compiler.

Neither keyword inherently produces output or changes object lifetime. With an
explicit `public:` or `private:` label, either keyword can express the same
access control; their defaults differ.

#### Try it now [Core live] — choose a representation boundary (3 minutes)

Choose `struct` or `class` under the course convention and justify the choice:

1. a Cartesian point whose public `x` and `y` fields are the intended data;
2. a bank balance that must never be changed without transaction rules;
3. a token containing a public kind, source position, and numeric payload;
4. a rational number that must stay reduced with a positive denominator.

<details>
<summary>Reveal solution</summary>

Items 1 and 3 are passive records, so `struct` communicates that their fields
are the intended interface. Items 2 and 4 protect behavioral invariants, so
`class` communicates a private representation controlled by operations. The
compiler does not enforce this convention; explicit access labels determine
actual accessibility. This is a design classification with no terminal output.

</details>

---

### 8. Named factories and private construction

> **Supporting class-design technique:** ordinary constructors are the default
> starting point. Use a named factory only when its name or failure policy makes
> creation meaningfully clearer.

A `static` member function belongs to the class rather than to one existing
object, has no `this` pointer, and can access private constructors. It can
therefore expose a named creation policy instead of a large set of ambiguous
constructors. Consider a validated sampling interval:

```cpp
#include <cmath>
#include <limits>
#include <stdexcept>

class SampleWindow {
 public:
  static SampleWindow FromEndpoints(double start, double finish) {
    if (!std::isfinite(start) || !std::isfinite(finish) || start > finish) {
      throw std::invalid_argument{"invalid sampling window"};
    }
    return SampleWindow{start, finish};
  }

  double start() const {
    return start_;
  }
  double finish() const {
    return finish_;
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

`std::isfinite` is a numeric validation function: it is false for positive or
negative infinity and for NaN. The factory uses it so every returned window has
finite, ordered endpoints.

The class definition itself produces no output. A successful factory call
returns an independent value; invalid or non-finite endpoints throw before a
`SampleWindow` is returned.

#### Try it now [Core live] — trace a named creation policy (5 minutes)

Treat the calls independently and predict the returned state or exception for
each one:

```cpp
SampleWindow a = SampleWindow::FromEndpoints(1.5, 4.0);
SampleWindow b = SampleWindow::FromEndpoints(4.0, 1.5);
SampleWindow c = SampleWindow::FromEndpoints(
    0.0, std::numeric_limits<double>::infinity());
```

Why can callers not write `SampleWindow direct{1.5, 4.0};`?

<details>
<summary>Reveal solution</summary>

- `a` is returned with `start() == 1.5` and `finish() == 4.0`.
- Creating `b` throws `std::invalid_argument` because the endpoints are
  reversed; no `b` value is initialized.
- Creating `c` throws because the finish value is not finite; no `c` value is
  initialized.
- Direct construction is a compile-time access error because the constructor is
  private. The static member factory may call it because members of the class
  can access private names.

This is an object-state, exception, and access-control trace. The fragment has
no terminal output.

</details>

---

### Factory design checkpoint

#### Try it now [Extension] — name distinct creation meanings (6 minutes)

For a class that can be created from seconds, milliseconds, or a configuration
string, compare overloaded constructors with three named factories. State which
forms throw, which preserve the original object on failure, and which return a
value. Do not implement all factories; first make the creation table
unambiguous.

<details>
<summary>Reveal solution</summary>

One clear interface is:

```text
Timer::FromSeconds(double)               -> value; reject negative/non-finite input
Timer::FromMilliseconds(long long)       -> value; reject unsupported ranges
Timer::Parse(const std::string& text)    -> value or documented parse failure
```

The names expose units and interpretation at the call site. A failed factory
does not alter an already existing `Timer`; it either throws before returning a
new value or uses another explicitly documented failure result. Overloaded
constructors are reasonable only if argument types make the meanings equally
clear. This is an interface/failure-policy table, not terminal output.

</details>

---

### Header-dependency exercise

#### Try it now [Extension] — predict recompilation boundaries (6 minutes)

Split Rational into `rational.hpp`, `rational.cpp`, and `rational_test.cpp`.
Forward declare where a complete type is unnecessary, include what the header
itself uses, and keep implementation-only headers out of the public interface.
Then change the private representation and list which files must recompile.

<details>
<summary>Reveal solution</summary>

The header must contain the complete `Rational` class because clients construct
it by value and need its size. `rational.cpp` includes `rational.hpp` first, then
implementation-only headers. The test includes only the public header it uses.

Changing a private data member changes the class definition and object layout,
so every translation unit that includes the header must recompile. Changing
only the body of an out-of-line member normally recompiles `rational.cpp` and
then relinks clients. This is a dependency trace; successful compilation emits
no required terminal output.

</details>

---

### 9. Test the abstraction, not the fields

```cpp
#include <cassert>

void TestNormalization() {
  Rational value{2, -4};
  assert(value.numerator() == -1);
  assert(value.denominator() == 2);
}
```

With assertions enabled, this function completes silently because both
conditions are true. A false assertion writes an implementation-specific
diagnostic and terminates; do not promise one exact diagnostic string.

`assert(condition)` documents a condition that must be true if the program and
its internal reasoning are correct. If the condition is false, the program
prints diagnostic information and terminates. It is useful for executable
examples, tests, and internal invariants such as normalization; it is not a
replacement for validating user input or reporting an expected failure.
Including `<cassert>` declares the macro. A build that defines `NDEBUG` removes
ordinary `assert` checks, so never put an essential side effect inside the
condition and never make program correctness depend on an assertion running.

Also test zero denominator, zero numerator, arithmetic sign combinations,
chained compound assignment, and values near integer limits. The simple `int`
representation can overflow during intermediate multiplication even when the
mathematical result is small; that limitation belongs in the contract or a
stronger representation.

#### Try it now [Core live] — separate internal checks from input validation (4 minutes)

Classify each use as appropriate for `assert` or as requiring normal error
handling:

1. after normalization, verify `denominator_ > 0` inside the implementation;
2. reject a user-entered denominator of zero;
3. verify a known test case produces `-1/2`;
4. call a function with a side effect only inside `assert(...)`.

<details>
<summary>Reveal solution</summary>

1. An internal invariant check is an appropriate assertion.
2. Expected invalid input needs ordinary validation and a documented error path,
   because assertions may be disabled and are not user diagnostics.
3. An assertion is suitable in a small executable test, provided the test build
   keeps assertions enabled.
4. Never hide an essential side effect inside an assertion; defining `NDEBUG`
   removes the expression and would change program behavior.

Passing assertions produce no output. Failing assertion text is
implementation-specific. This is a verification-policy trace.

</details>

---

### Hour 3 invariant audit

#### Try it now [Extension] — test a property across operations (8 minutes)

For every public operation, fill a table with valid input, possible failure,
state change, and invariant restoration point. Seed one bug that bypasses
`Normalize`, generate several inputs, and check the representation properties
after every operation: `denominator() > 0` and the numerator and denominator
have greatest common divisor one. This is an **invariant-based repeated test**:
it checks a rule that must hold for many inputs rather than comparing only one
precomputed output. Once it exposes the bug, add the smallest failing input as
a permanent regression case.

<details>
<summary>Reveal solution</summary>

A useful audit table begins as follows:

| Operation | Failure | State change | Restoration/check point |
|-----------|---------|--------------|-------------------------|
| constructor | zero/unsupported representation | creates a new value | before normal return |
| `operator+=` | arithmetic outside the representation | mutates left operand only on success in the complete example | check/reduce temporaries before assignment |
| `operator+` | propagated arithmetic failure | mutates only its local copy on success | returned value is normalized |
| observers | none for a valid object | none | rely on the existing invariant |

For each generated valid result, check `denominator() > 0` and
`gcd(abs(numerator()), denominator()) == 1`. Preserve the smallest input that
exposes a seeded missing-normalization bug. These are repeated state checks;
the audit itself has no required terminal output.

</details>

---

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

#### Try it now [Extension] — audit one project class boundary (6 minutes)

Choose one class proposed for the project. Record its invariant, construction
site, owner, observers, mutating operations, failure policy, and one test that
runs without opening a graphics window. Identify one generated setter or public
field that would let a caller bypass the invariant.

<details>
<summary>Reveal solution</summary>

A valid audit names concrete evidence rather than merely listing files:

| Question | Required evidence |
|----------|-------------------|
| What must always be true? | one testable invariant over the class state |
| How is valid state created? | constructor/factory and its failure contract |
| Who owns the object? | one value or subsystem with a clear lifetime |
| Who only observes it? | `const` members or `const&` client interfaces |
| Which calls mutate it? | operations that restore the invariant before return |
| How is it tested? | a deterministic headless construction/state-transition test |

Remove or restrict any field/setter that permits a state transition outside
those operations. This is an integration and ownership trace; it has no fixed
terminal output.

</details>

---

## Check yourself

1. Which operations can create or change a `Rational`?
2. Why is a setter for `denominator_` a risky interface?
3. When does a member function need trailing `const`?
4. Why implement `operator+` in terms of `operator+=`?
5. When is a named factory clearer than an overloaded constructor?
6. Why should this week's named factories return values rather than references
   to local objects?
7. Design an invariant and public interface for a `TimeOfDay` class.

<details>
<summary>Reveal solutions</summary>

1. Constructors create a `Rational`. Compound assignment changes an existing
   value through its public interface; nonmember arithmetic creates and returns
   another value without changing its operands.
2. A denominator setter could create zero or break reduction/sign rules without
   coordinating the numerator. Prefer operations that preserve the whole
   invariant.
3. Use trailing `const` when a member observes rather than mutates the object's
   logical state and should be callable on a `const` object.
4. `operator+` can modify a local copy through the already-tested `operator+=`,
   keeping normalization and arithmetic policy in one path.
5. A named factory is clearer when a name communicates units, format, parsing,
   validation, or another creation policy that overloaded constructors would
   hide.
6. A local object dies when its function returns, so a reference to it would
   dangle. Returning a value gives the caller an independent live object.
7. One design stores hour/minute/second privately and maintains
   `0 <= hour < 24`, `0 <= minute < 60`, and `0 <= second < 60`. Construction
   rejects invalid components; read-only accessors observe them; an operation
   such as `AdvanceOneSecond()` performs a complete wraparound transition.

Items 1–7 are contract and state-design explanations; they produce no terminal
output by themselves.

</details>

---

## Summary

- A class is valuable when it protects a meaningful invariant.
- Constructors create valid objects; initializer lists construct members directly.
- Named factories make alternate creation policies and failure contracts explicit.
- `const` member functions expose safe observation.
- Public interfaces should be smaller and more stable than representations.
- Operator overloads should preserve invariants and conventional meaning.

---

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

The class declaration itself produces no output. `Rational{}` delegates to the
two-argument constructor and represents `0/1`; `Rational{3}` represents `3/1`.
With `explicit` present, direct initialization is accepted but copy
initialization written as `Rational r = 3;` is a compile-time error.

---

### Overloading and default arguments

Member functions may be overloaded by parameter types/count and by trailing
`const`, but not by return type alone. Default arguments are substituted at the
call site and should appear in one declaration, normally the header.

Avoid pairs of overloads whose conversions make a call ambiguous. Use a small
test call matrix to confirm which overload accepts `int`, `double`, `const
Rational`, and temporary arguments.

---

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

A warning-enabled compiler should diagnose the returned reference to the local
object. Calling the function and using that reference after return has undefined
behavior, so there is no valid output to predict or observe. The correct factory
return type is `Rational`, not `Rational&`.

---

## References and source materials

- [Classes I](<https://github.com/htchen/i2p-nthu/blob/master/程式設計二/Classes%20I/README.md>)
- [2025 Week 7 notebook (Colab)](https://colab.research.google.com/drive/1oHBcNeAXt4ZeQJsdG2q4RU5m9Yu_9CCw)
