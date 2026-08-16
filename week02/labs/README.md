# Week 2 Lab — Arrays, Strings, and Interface Contracts

## Outcomes

Students can implement array/length interfaces, distinguish capacity from
logical length, and handle null-terminated strings without assuming Python-like
bounds or resizing.

## Part A — AI-free readiness

Trace a supplied array function for empty, singleton, and full-capacity inputs.
Find one off-by-one access and repair it while preserving the function contract.

## Part B — Array and string functions

Implement one numeric array operation and one bounded string operation. For
each interface, state pointer/address parameters, logical length, capacity,
mutability, return/failure behavior, and null-termination requirements.

## Part C — Adversarial verification

Test zero length, one element, exact capacity, insufficient capacity, embedded
whitespace where relevant, and invalid input. Compile with warnings and run
memory-sensitive cases under sanitizers.

## Part D — AI-assisted test review

After writing expected results, ask AI for missing boundary categories. Reject
tests that assume operations outside the stated interface, and verify retained
cases independently.

## Deliverable

- source and interface-contract table;
- boundary-test table;
- warning/sanitizer evidence;
- one accepted and one rejected AI suggestion.
