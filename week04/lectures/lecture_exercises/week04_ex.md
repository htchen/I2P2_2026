# Week 4 Lecture Exercises: Pointers and Dynamic Memory

[Starter code](week04_starter.c)

## Hour 1 — Address and alias trace

Draw the objects and arrows created by `int* p = &value` and by a pointer into an
array. Predict which writes change the original object and identify the valid
half-open pointer range.

## Hour 2 — Transactional allocation

Implement `make_sequence`. The `int** out` parameter points to the caller's
owning `int*`, so successful construction can replace that caller variable.
Its contract is:

- `out` points to an initialized owner whose current value is `NULL`;
- size zero succeeds and keeps that owner equal to `NULL`;
- positive-size success publishes a block of `size` zero-initialized integers;
- the caller owns the published block and must eventually release it; and
- invalid input, multiplication overflow, or allocation failure returns
  `false` without changing the owner.

Test zero length, a null output parameter, multiplication overflow, and an
attempt to overwrite an owner that already holds an allocation.

## Hour 3 — Cleanup audit

Complete `resize_sequence` using a temporary pointer. Exercise normal and
failure paths under AddressSanitizer/UndefinedBehaviorSanitizer and show that
each successful allocation has exactly one eventual `free`. Its contract is:

- `values` identifies the caller's owning pointer;
- `old_size == 0` exactly when the current owner is `NULL`;
- when `old_size > 0`, the owner designates a block of at least `old_size`
  integers;
- growth zero-initializes indices `[old_size, new_size)`;
- `new_size == 0` releases the block and publishes `NULL`; and
- overflow or allocation failure returns `false` without changing the owner.

The supplied checks cover growth, shrinking, overflow preservation, release,
and a repeated empty release. Allocation failure caused only by resource
exhaustion is not deterministic; the overflow case exercises the same
failure-preservation contract without trying to exhaust the machine.

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
