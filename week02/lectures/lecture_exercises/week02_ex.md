# Week 2 Lecture Exercises: Functions, Arrays, and Strings

[Starter code](week02_starter.c)

## Hour 1 — Interface contracts

Write preconditions and postconditions for `clamp_all` in the starter. Implement
it without accessing outside `[0, size)`, including when `size == 0`.

## Hour 2 — Prefix and sorted-boundary queries

Complete a prefix table for the positive contributions of an array. Answer range
queries by subtracting two prefix entries. Then implement half-open lower and
upper bounds and use their difference to count duplicates in sorted data.

The starter names these operations `build_positive_prefix` and `query_total` and
provides their storage parameters. A table for `size` input values contains
`size + 1` entries, beginning with zero. This is the same boundary convention as
the lecture's general prefix table; only the per-element contribution changes.

## Hour 3 — Bounded strings

Implement `copy_word` so it always terminates the destination when capacity is
positive and returns `1` only when the complete source string fits. Test exact
fit, truncation, empty input, and zero capacity. Explain why the function needs
the destination capacity rather than only the source length. Unlike the
all-or-nothing `string_copy` shown in the lecture, this exercise intentionally
uses a **truncating** contract: it preserves the longest prefix that fits and
then appends `\0` when capacity is positive.

## Compile and verify

From the repository root:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  week02/lectures/lecture_exercises/week02_starter.c -o /tmp/week02_starter
/tmp/week02_starter
```

After all TODOs are correct, the final line is:

```text
all Week 2 starter checks passed
```

Attempt the starter first. Use [`../examples.c`](../examples.c) afterward for
complete neighboring examples of array traversal, boundary search, and bounded
copying. The reference deliberately does not provide the prefix-table bodies;
the driver and hand traces are the oracle for that exercise.
