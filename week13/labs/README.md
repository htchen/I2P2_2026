# Week 13 Checkpoint — Polymorphism and Vertical-Slice Design

Thursday is Midterm 2, so this is a bounded asynchronous design checkpoint, not
a scheduled lab. The Week 13 lecture notes are excluded from the midterm scope.

## Outcomes

Students trace a recursive Composite and design one small polymorphic or
composed feature through the final project's complete runtime path before
implementing it in Week 14.

## Part A — AI-free readiness and ownership follow-through

Trace one virtual call through a leaf and a recursive composite, identify every
owning child edge, and explain why destruction through the base is safe or
unsafe. Revisit the verified Week 12 ownership change and identify how the new
feature will preserve its ownership contract.

## Part B — Design the slice

For an approved object or behavior, map:

```text
construction → ownership/registration → input → update → interaction
             → draw → removal → destruction
```

Choose inheritance or composition based on the required substitution and
expected extension direction. A closed variant is an optional alternative only
if the set of cases is known and the design accounts for every operation. State
affected files before coding.

## Part C — Plan incremental verification

List the smallest compile-and-test steps for each boundary. Keep logic testable
without the window where practical. Specify cases for creation, normal behavior,
boundary behavior, removal, and shutdown. Implementation begins in Week 14.

## Part D — AI-assisted architecture review

AI may compare design options or inspect a list of affected files. Verify all
interfaces and reject suggestions based on invented functions, global state, or
unclear ownership.

## Deliverable

- vertical-slice map and design justification;
- affected-file and incremental implementation plan;
- automated logic-test cases and graphical smoke-test plan;
- lifetime trace and AI-use entry.

Expected effort: 30–45 minutes outside the midterm examination.
