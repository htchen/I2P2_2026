# Week 2 Lecture Exercises: Functions, Arrays, and Strings

[Starter code](week02_starter.c)

## Hour 1 — Interface contracts

Write preconditions and postconditions for `clamp_all` in the starter. Implement
it without accessing outside `[0, size)`, including when `size == 0`.

## Hour 2 — Prefix and sorted-boundary queries

Complete a prefix table for the positive contributions of an array. Answer range
queries by subtracting two prefix entries. Then implement half-open lower and
upper bounds and use their difference to count duplicates in sorted data.

## Hour 3 — Bounded strings

Implement `copy_word` so it always terminates the destination when capacity is
positive and returns `1` only when the complete source string fits. Test exact
fit, truncation, empty input, and zero capacity. Explain why the function needs
the destination capacity rather than only the source length.
