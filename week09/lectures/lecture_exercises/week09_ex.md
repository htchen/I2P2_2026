# Week 9 Lecture Exercises: Classes and Invariants

[Starter code](week09_starter.cpp)

## Hour 1 — Construction boundary

Complete the `Rational` constructor so the denominator is positive and the
fraction is reduced. Reject a zero denominator before publishing an invalid
object.

## Hour 2 — Conventional value operations

Implement equality and addition without exposing representation fields. Test
negative values, reducible results, and equality across different source forms.

## Hour 3 — Header/source separation

Move declarations to a self-contained header and definitions to a source file.
Compile a separate client and explain which changes require client recompilation.
