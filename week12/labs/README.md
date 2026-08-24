# Week 12 Lab — Ownership and Value Semantics

## Outcomes

Students can distinguish values, owners, and borrowers; trace a resource across
its complete lifetime; and implement one bounded ownership improvement with
tests and sanitizer evidence.

## Part A — AI-free readiness

Classify supplied handles as values, unique owners, shared owners, or borrowers.
For one short resource-owning class, predict the effects of copying, moving,
removal from a container, and destruction before compiling the example.

## Part B — Ownership map

Trace one project object or resource through creation, observation, transfer,
removal, destruction, and scene shutdown. Identify one shallow-copy,
double-destruction, leak, or dangling-observer risk. Compare a unique-tree and
immutable-shared model for one small recursive composite.

## Part C — Implement and verify

Implement one instructor-approved Rule-of-Zero or smart-pointer improvement.
Update every affected construction, observation, removal, and cleanup site.
Test normal construction, transfer or movement, removal, shutdown, and one
failure path. Run the relevant sanitizer configuration and preserve its output.

## Part D — AI audit

Draw and label the ownership graph before asking AI. Reject any suggestion that
mechanically replaces all raw pointers or introduces shared ownership without a
genuine shared lifetime. Verify one accepted or rejected claim against the
actual code and runtime evidence.

## Deliverable

- annotated ownership/lifetime map;
- recursive-composite ownership comparison;
- bounded implementation and integration-risk note;
- test and sanitizer output;
- AI-use record with one verified claim.
