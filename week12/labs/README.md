# Week 12 Checkpoint — Containers and Invalidation

Thursday is Midterm 2, so this is a bounded asynchronous code-reading
checkpoint. Note 11 is excluded from the midterm scope.

## Task

Select one final-template loop that inserts, removes, or reorders objects.
Without changing project code:

1. identify the container and element type;
2. state whether elements own objects or borrow them;
3. mark iterators, pointers, and references invalidated by the mutation;
4. trace object destruction and later callbacks/uses;
5. compare erase-return, erase/remove, and mark-then-sweep designs where they
   apply to the actual container.

## AI rule

Predict invalidation before asking AI. Check the answer against the C++ standard
library contract and the repository's actual mutation order.

## Deliverable

- one annotated loop/invalidation trace;
- one recommended safe mutation pattern with justification;
- one AI claim accepted or rejected with evidence.

Expected effort: 30–45 minutes outside the midterm examination.
