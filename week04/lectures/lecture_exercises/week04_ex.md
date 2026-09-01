# Week 4 Lecture Exercises: Pointers and Dynamic Memory

[Starter code](week04_starter.c)

## Hour 1 — Address and alias trace

Draw the objects and arrows created by `int* p = &value` and by a pointer into an
array. Predict which writes change the original object and identify the valid
half-open pointer range.

## Hour 2 — Transactional allocation

Implement `make_sequence`. Handle zero length, multiplication overflow, and
allocation failure without publishing a partial result. State who owns the
returned block.

## Hour 3 — Cleanup audit

Complete `resize_sequence` using a temporary pointer. Exercise normal and
failure paths under AddressSanitizer/UndefinedBehaviorSanitizer and show that
each successful allocation has exactly one eventual `free`. Its contract is:

- `values` identifies the caller's owning pointer;
- when `old_size > 0`, that pointer owns a block of at least `old_size` integers;
- growth zero-initializes indices `[old_size, new_size)`;
- `new_size == 0` releases the block and publishes `NULL`; and
- overflow or allocation failure returns `false` without changing the owner.

## Compile and verify

From the repository root:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  week04/lectures/lecture_exercises/week04_starter.c -o /tmp/week04_starter
/tmp/week04_starter
```

The completed program prints `all Week 4 starter checks passed`. Repeat with
the sanitizers available on your system, then compare with
[`../examples.c`](../examples.c).
