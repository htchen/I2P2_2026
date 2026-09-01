# Week 12 Lecture Exercises: Ownership and Value Semantics

[Starter code](week12_starter.cpp)

## Hour 1 — Ownership diagnosis

Trace two objects after the compiler-generated copy of a raw owning pointer.
Identify the alias, double-destruction risk, and missing ownership contract.

## Hour 2 — Copy/move exercise

Complete the size constructor, destructor, copy constructor, copy assignment
via copy-and-swap, and both move operations in the starter. These are one
ordinary constructor plus five ownership-sensitive special member functions,
not “six special members.” Test self-assignment, independent copies, and
moved-from destruction.

The starter declares `Swap` as the helper needed by copy-and-swap. Implement
copy assignment by first constructing an independent temporary copy and then
swapping it with `*this`; the temporary releases the old resource when it leaves
scope. This fixed-size `Buffer` has no capacity field. The lecture's `IntVec`
adds capacity to demonstrate growth, but that field is not part of this exercise.
Both move operations must leave the source with `size() == 0` and `data_ == nullptr`.

## Hour 3 — Rule-of-Zero refactor

Replace the raw array with `std::vector<int>`, remove special member functions,
and confirm that copying and moving retain the intended value semantics.

## Compile and verify

From the repository root, after defining the declared members:

```sh
c++ -std=c++17 -Wall -Wextra -Wpedantic \
  week12/lectures/lecture_exercises/week12_starter.cpp -o /tmp/week12_starter
/tmp/week12_starter
```

The final line should be `all Week 12 starter checks passed`. The complete
[`../examples.cpp`](../examples.cpp) uses `RawBuffer::Swap` for the same
copy-and-swap structure and then shows the Rule-of-Zero refactor.
