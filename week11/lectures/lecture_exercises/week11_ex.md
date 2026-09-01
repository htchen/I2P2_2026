# Week 11 Lecture Exercises: Templates and the Standard Library

[Starter code](week11_starter.cpp)

## Exercise Overview

- instantiate one constrained-by-operations function template;
- trace iterator validity across container mutations;
- solve an equal-range counting query with standard algorithms.

---

## Exercise 1 — Generic boundary counting

### Task

Implement `CountEqual` for a sorted iterator range using `std::lower_bound` and
`std::upper_bound`. State the sorted-range precondition and complexity. Test an
empty range, a missing key, and duplicates.

## Exercise 2 — Invalidation audit

Predict whether saved iterators/references remain valid after `vector::push_back`,
`vector::erase`, and `list::erase`. Verify the rules from documentation, then
rewrite one unsafe mutation loop.

## Exercise 3 — Expected absence

Return `std::optional<std::size_t>` from a lookup rather than using a magic
index. Require the caller to handle both branches.

## Compile and verify

From the repository root:

```sh
c++ -std=c++17 -Wall -Wextra -Wpedantic \
  week11/lectures/lecture_exercises/week11_starter.cpp -o /tmp/week11_starter
/tmp/week11_starter
```

After both TODOs are complete, the final line is
`all Week 11 starter checks passed`. `CountEqual` requires a sorted range and at
least forward iterators; the separate invalidation audit remains a written trace.
Compare afterward with [`../examples.cpp`](../examples.cpp).
