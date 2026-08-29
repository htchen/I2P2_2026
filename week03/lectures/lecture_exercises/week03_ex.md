# Week 3 Lecture Exercises: Structures, Modules, and Debugging

[Starter code](week03_starter.c)

## Hour 1 — Record invariant

Complete `rational_make` so every nonzero rational has a positive denominator
and is reduced. Write the invariant before the code and include a defined policy
for a zero denominator.

## Hour 2 — Interface/build graph

Split the starter declaration and implementation into `rational.h`,
`rational.c`, and `main.c`. Draw the include and link dependencies, then compile
each translation unit separately.

## Hour 3 — Evidence-driven diagnosis

Seed one violated assertion and one linker error. Record the complete diagnostic,
form a hypothesis, make the smallest repair, and add a regression assertion.
