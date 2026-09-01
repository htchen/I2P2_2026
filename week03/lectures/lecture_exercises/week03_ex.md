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

## Compile and verify

Before splitting the starter into three files, compile it from the repository
root:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  week03/lectures/lecture_exercises/week03_starter.c -o /tmp/week03_starter
/tmp/week03_starter
```

The completed constructor prints `all Week 3 starter checks passed`. Preserve
the same tests when moving the declaration to `rational.h`, the definition to
`rational.c`, and the driver to `main.c`. Consult [`../examples.c`](../examples.c)
only after the first implementation attempt.
