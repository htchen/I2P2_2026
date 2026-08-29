# Week 12 Lecture Exercises: Ownership and Value Semantics

[Starter code](week12_starter.cpp)

## Hour 1 — Ownership diagnosis

Trace two objects after the compiler-generated copy of a raw owning pointer.
Identify the alias, double-destruction risk, and missing ownership contract.

## Hour 2 — Copy/move exercise

Complete the copy constructor, copy assignment via copy-and-swap, and move
operations in the starter. Test self-assignment and moved-from destruction.

## Hour 3 — Rule-of-Zero refactor

Replace the raw array with `std::vector<int>`, remove special member functions,
and confirm that copying and moving retain the intended value semantics.
