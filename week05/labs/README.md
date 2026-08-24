# Week 5 Lab — Token Lists and Adversarial Tests

This Thursday session also includes **Quiz 1**, scheduled two weeks before
Midterm 1. Staff will announce which lab parts use the remaining in-person time
and which, if any, are completed asynchronously.

## Outcomes

Students can trace token-list invariants, verify list-to-array conversion, and
design indexed sequence edits and tests from invariants without relying on a
complete reference implementation.

## Part A — AI-free readiness

Implement or repair one standalone pointer-to-pointer list operation. Test empty,
head, middle, tail, and absent-element cases under AddressSanitizer.

## Part B — Sequence-editor design sheet

Using an instructor-supplied playlist specification and integer payloads, draw
and specify—but do not fully implement—insert-before, remove-at, remove-if, and
reverse-range operations. Use zero-based positions and half-open `[first, last)`
ranges. For each operation, identify the incoming link that changes, the
unchanged-on-failure rule, and empty/front/end/adjacent-match cases. Compare
head-pointer and sentinel representations without mixing their invariants.

## Part C — Token representation trace

Using the unmodified project scaffold, trace:

- empty or whitespace-only input;
- one identifier or constant;
- a multi-operator expression;
- parentheses;
- an invalid character.

Record list state after every append and the resulting indexed token sequence.
State which representation owns token storage before and after conversion.

## Part D — Test design

Create a table containing input, expected tokens or rejection, targeted rule,
and observed result. Include normal, boundary, invalid, and cleanup-oriented
cases. Preserve these tests for the Week 7 asynchronous integration checkpoint
and the verification work leading to the Week 10 demo.

## Part E — AI-assisted adversarial review

Ask an AI tool for missing *categories* of lexer/list tests. Convert useful
suggestions into exact expected sequences. Reject suggestions outside the
published language or scaffold contract and document one such rejection.

## Deliverable

- token/list hand trace;
- sequence-editor diagrams and contract table;
- at least eight specified tests across the required categories;
- sanitizer result;
- AI-use entry with one rejected suggestion.
