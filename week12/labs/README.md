# Week 12 Checkpoint — Ownership and Value Semantics

Thursday is Midterm 2, so this is a bounded asynchronous code-reading
checkpoint. Note 11 is excluded from the midterm scope.

## Task

Without changing project code:

1. classify supplied handles as values, unique owners, shared owners, or
   borrowers;
2. trace one project's object or resource through creation, observation,
   transfer, removal, destruction, and scene shutdown;
3. identify one shallow-copy, double-destruction, leak, or dangling-observer
   risk;
4. compare a unique-tree and immutable-shared model for one small recursive
   composite;
5. propose one bounded Rule-of-Zero or smart-pointer improvement and list every
   affected construction, observation, removal, and cleanup site.

## AI rule

Draw and label the ownership graph before asking AI. Reject any suggestion that
mechanically replaces all raw pointers or introduces shared ownership without a
genuine shared lifetime.

## Deliverable

- one annotated ownership/lifetime map;
- one recursive-composite ownership comparison;
- one bounded improvement proposal and integration-risk note;
- one AI claim accepted or rejected with evidence.

Expected effort: 30–45 minutes outside the midterm examination. Implementation
and sanitizer verification continue in the December 3 lab.
