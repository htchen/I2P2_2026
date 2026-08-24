# Week 13 Lab — Ownership, STL, Polymorphism, and a Vertical Slice

## Outcomes

Students consolidate ownership and STL practice, trace a recursive Composite,
and integrate one small polymorphic or composed feature through the final
project's complete runtime path.

## Part A — AI-free readiness and ownership follow-through

Complete a short container/algorithm exercise using `lower_bound`,
`upper_bound`, or `equal_range`; state the ordering precondition and interpret
the returned half-open range. Identify all invalidated iterators after a later
mutation. Then trace one virtual call through a leaf and a recursive composite,
identify every owning child edge, and explain why destruction through the base
is safe or unsafe. Revisit the Week 12 ownership proposal and identify the
smallest approved change that can be verified during this lab.

## Part B — Design the slice

For an approved object or behavior, map:

```text
construction → ownership/registration → input → update → interaction
             → draw → removal → destruction
```

Choose inheritance, composition, or a closed variant based on the expected
extension direction. State affected files before coding.

## Part C — Implement incrementally

Compile and test after each boundary rather than adding every file at once. Keep
logic testable without the window where practical. Exercise creation, normal
behavior, boundary behavior, and removal/shutdown.

## Part D — AI-assisted architecture review

AI may compare design options or inspect a list of affected files. Verify all
interfaces and reject suggestions based on invented functions, global state, or
unclear ownership.

## Deliverable

- vertical-slice map and design justification;
- working integrated slice;
- automated logic tests plus graphical smoke test;
- lifetime trace and AI-use entry.
