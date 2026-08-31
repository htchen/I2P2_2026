# Week 8 Lecture Exercises: From C to C++

[Starter code](week08_starter.cpp)

## Hour 1 — Value containers

Translate a C array/length loop to `std::vector<int>`. Predict the effects of
copying the vector and contrast them with copying a raw pointer.

## Hour 2 — References and `const`

Complete `Normalize` with a mutable reference and queries with `const&`.
Classify each parameter as value, mutable borrow, or read-only borrow.

## Hour 3 — RAII and exceptions

Trace the lifetime of a local `std::vector` and `std::ifstream` through normal,
early-return, and throwing paths. Replace one manual C acquisition/release pair
with an appropriate standard-library value and explain why each path releases
the resource. Defining custom constructors and destructors begins in Week 9.
