# Week 11 Lab — Ownership Audit and Bounded Refactor

## Outcomes

Students can compare recursive-object ownership models, trace one project
lifetime end-to-end, and improve ownership without performing an unsafe
repository-wide pointer replacement.

## Part A — AI-free readiness

Classify supplied examples as value, unique owner, shared owner, or borrower.
Find one shallow-copy defect and refactor it toward the Rule of Zero.
For a small recursive composite diagram, compare unique-tree and immutable-
shared ownership and mark where a transformation would clone or share children.

## Part B — Ownership audit

Choose one entity collection or external resource. Record creation, owner,
observers, transfers, callbacks, removal, destruction, and scene-shutdown
behavior. Include iterator/reference invalidation when removal occurs during an
update loop.

## Part C — Bounded improvement

With instructor approval, improve one boundary—for example, a scoped resource
wrapper or one owning collection. Update construction, observation, removal,
and destruction sites together. Raw non-owning pointers may remain when their
lifetime contract is clear.

## Part D — Verification and AI audit

Test normal use, removal, shutdown, and relevant initialization failure with
warnings and sanitizers. Ask AI to review the ownership map, then document one
mechanical or overbroad suggestion that you rejected.

## Deliverable

- before/after ownership map;
- focused code change;
- sanitizer and regression evidence;
- integration-risk note and AI audit.
