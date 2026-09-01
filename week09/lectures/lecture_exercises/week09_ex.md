# Week 9 Lecture Exercises: Classes and Invariants

[Starter code](week09_starter.cpp)

## Hour 1 — Construction boundary

Complete the `Rational` constructor so the denominator is positive and the
fraction is reduced. Reject a zero denominator before publishing an invalid
object.

## Hour 2 — Conventional value operations

Implement `operator+=` and `operator-=` as mutating members that preserve the
invariant and return `*this`. The supplied nonmember `operator+` and `operator-`
then operate on a copy through those compound operators. Also implement unary
minus and `==`; retain the supplied C++17 definition of `!=` in terms of `==`.
Test negative values, reducible results, equality across different source forms,
and that binary arithmetic does not mutate either source operand.

## Hour 3 — Header/source separation

Move declarations to a self-contained header and definitions to a source file.
Compile a separate client and explain which changes require client recompilation.

## Compile and verify

Before splitting the file, compile it from the repository root:

```sh
c++ -std=c++17 -Wall -Wextra -Wpedantic \
  week09/lectures/lecture_exercises/week09_starter.cpp -o /tmp/week09_starter
/tmp/week09_starter
```

After every TODO is correct, the final line is
`all Week 9 starter checks passed`. Only then compare the design with
[`../examples.cpp`](../examples.cpp), which uses the same compound-operator API.
