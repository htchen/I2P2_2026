# Week 8 Lecture Exercises: From C to C++

[Starter code](week08_starter.cpp)

## Hour 1 — Value containers

Translate a C array/length loop to `std::vector<int>`. Predict the effects of
copying the vector and contrast them with copying a raw pointer.

## Hour 2 — References and `const`

Complete `normalize` with a mutable reference and queries with `const&`.
Classify each parameter as value, mutable borrow, or read-only borrow.

## Hour 3 — RAII and exceptions

Trace construction and destruction through both the normal and throwing paths
of the starter. Replace manual acquisition/release with an object whose lifetime
performs cleanup, and verify that stack unwinding does not leak the resource.
