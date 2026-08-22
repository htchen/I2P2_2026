# Week 2 Lab — Arrays, Strings, and Interface Contracts

## Outcomes

Students can implement array/length interfaces, distinguish capacity from
logical length, validate prefix and sorted-boundary queries, and handle
null-terminated strings without assuming Python-like bounds or resizing.

## Part A — AI-free readiness

Trace a supplied array function for empty, singleton, and full-capacity inputs.
Find one off-by-one access and repair it while preserving the function contract.

## Part B — Array and string functions

Implement one numeric array operation and one bounded string operation. For
each interface, state pointer/address parameters, logical length, capacity,
mutability, return/failure behavior, and null-termination requirements.

## Part C — Prefix-table design and verification

Before the general verification pass, build a boundary-indexed prefix table for
an instructor-supplied integer sequence. Specify at least four valid half-open
queries—including an empty range—and three invalid boundary pairs. Implement
the published interfaces only after writing expected answers, then compare each
valid result with a direct range loop. Use a wide accumulation type and state
the remaining overflow assumption.

## Part D — Sorted-boundary design trace

For an instructor-supplied sorted array containing duplicates, specify lower
and upper boundary contracts and trace the half-open candidate interval for
present and absent targets. Include empty, all-equal, below-minimum, and
above-maximum cases. Do not implement the search until the invariant and
expected boundary table have been reviewed.

## Part E — General adversarial verification

Test zero length, one element, exact capacity, insufficient capacity, embedded
whitespace where relevant, and invalid input. Compile with warnings and run
memory-sensitive cases under sanitizers.

## Part F — AI-assisted test review

After writing expected results, ask AI for missing boundary categories. Reject
tests that assume operations outside the stated interface, and verify retained
cases independently.

## Deliverable

- source and interface-contract table;
- annotated prefix table and direct-loop comparison;
- lower/upper boundary table and binary-search invariant trace;
- boundary-test table;
- warning/sanitizer evidence;
- one accepted and one rejected AI suggestion.
