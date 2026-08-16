# Lecture 9 — Classes, Invariants, and Operators

> November 10, 2026 · Source lineage: the legacy Classes I notes and Rational
> examples from the 2025 C++ notebook

## Learning objectives

By the end of this lecture, you should be able to:

1. Design a class around an invariant and a small public interface.
2. Use constructors and member-initializer lists.
3. Write `const` member functions and distinguish class from object state.
4. Separate class declarations from definitions.
5. Overload an operator without surprising its users.

## 1. A class protects a valid state

A C structure groups fields. A C++ class can additionally control how clients
create and mutate values.

```cpp
class Rational {
public:
    Rational(int numerator = 0, int denominator = 1);

    int numerator() const { return numerator_; }
    int denominator() const { return denominator_; }

    Rational& operator+=(const Rational& other);

private:
    int numerator_;
    int denominator_;

    void normalize();
};
```

The invariant is:

- `denominator_ > 0`;
- numerator and denominator are reduced by their greatest common divisor.

Data is private so every public operation can preserve the invariant. Privacy
is a design tool, not a demand to write trivial setters for every field.

## 2. Constructors establish the invariant

```cpp
#include <numeric>
#include <stdexcept>

Rational::Rational(int numerator, int denominator)
    : numerator_{numerator}, denominator_{denominator}
{
    if (denominator_ == 0) {
        throw std::invalid_argument{"zero denominator"};
    }
    normalize();
}

void Rational::normalize()
{
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

A constructor with usable default arguments also serves as a default
constructor here: `Rational value;` creates `0/1`.

## 3. `const` member functions

```cpp
int Rational::numerator() const
{
    return numerator_;
}
```

The trailing `const` promises not to modify the observable state of the object.
It also permits calls on a `const Rational`.

```cpp
void print(const Rational& value)
{
    std::cout << value.numerator() << '/' << value.denominator();
}
```

Make observer functions `const` by default. This is different from returning a
`const` scalar, which generally adds no useful guarantee.

## 4. The implicit object and `this`

Within a non-static member function, `this` points to the current object.

```cpp
Rational& Rational::operator+=(const Rational& other)
{
    numerator_ = numerator_ * other.denominator_
               + other.numerator_ * denominator_;
    denominator_ *= other.denominator_;
    normalize();
    return *this;
}
```

Returning `*this` by reference supports conventional chaining such as
`a += b += c`. The operator mutates the left operand and preserves its invariant.

## 5. Nonmember binary operators

Implement symmetric binary operators in terms of compound assignment:

```cpp
Rational operator+(Rational left, const Rational& right)
{
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

std::ostream& operator<<(std::ostream& stream, const Rational& value)
{
    return stream << value.numerator() << '/' << value.denominator();
}
```

Overload operators only when their meaning matches normal expectations. Do not
turn `+` into an unrelated command merely because the syntax is available.

## 6. Declaration and definition

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

## 7. `class` versus `struct`

The language difference is only the default access:

- `class`: members and base classes are private by default;
- `struct`: members and base classes are public by default.

Course convention:

- use `struct` for passive records whose fields are the intended interface;
- use `class` when operations protect an invariant or hide representation.

This is a design convention, not a rule enforced by the compiler.

## 8. Static members

A static data member belongs to the class rather than each object. A static
member function has no `this` pointer.

```cpp
class IdSource {
public:
    static int next() { return next_id_++; }

private:
    inline static int next_id_ = 1; /* C++17 */
};
```

Use static state sparingly: hidden global state can make tests interdependent.

## 9. Test the abstraction, not the fields

```cpp
void test_normalization()
{
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

## Check yourself

1. Which operations can create or change a `Rational`?
2. Why is a setter for `denominator_` a risky interface?
3. When does a member function need trailing `const`?
4. Why implement `operator+` in terms of `operator+=`?
5. Design an invariant and public interface for a `TimeOfDay` class.

## Summary

- A class is valuable when it protects a meaningful invariant.
- Constructors create valid objects; initializer lists construct members directly.
- `const` member functions expose safe observation.
- Public interfaces should be smaller and more stable than representations.
- Operator overloads should preserve invariants and conventional meaning.
