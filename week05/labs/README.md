# Week 5 Lab — Token Lists and Adversarial Tests

## Outcomes

Students can trace token-list invariants, verify list-to-array conversion, and
design tests for a specification without requesting a complete project solution.

## Part A — AI-free readiness

Implement or repair one standalone pointer-to-pointer list operation. Test empty,
head, middle, tail, and absent-element cases under AddressSanitizer.

## Part B — Token representation trace

Using the unmodified project scaffold, trace:

- empty or whitespace-only input;
- one identifier or constant;
- a multi-operator expression;
- parentheses;
- an invalid character.

Record list state after every append and the resulting indexed token sequence.
State which representation owns token storage before and after conversion.

## Part C — Test design

Create a table containing input, expected tokens or rejection, targeted rule,
and observed result. Include normal, boundary, invalid, and cleanup-oriented
cases. Preserve these tests for the Week 7 integration studio.

## Part D — AI-assisted adversarial review

Ask an AI tool for missing *categories* of lexer/list tests. Convert useful
suggestions into exact expected sequences. Reject suggestions outside the
published language or scaffold contract and document one such rejection.

## Deliverable

- token/list hand trace;
- at least eight specified tests across the required categories;
- sanitizer result;
- AI-use entry with one rejected suggestion.
