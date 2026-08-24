# Week 11 Lab — Containers, Algorithms, and Invalidation

This Thursday session also includes **Quiz 2**, scheduled two weeks before
Midterm 2. Staff will announce which lab parts use the remaining in-person time
and which, if any, are completed asynchronously.

## Outcomes

Students can apply template and standard-library contracts independently before
Midterm 2, then audit one real final-project container mutation.

## Part A — AI-free exam readiness

Complete a short function-template instantiation trace and select containers for
three supplied workloads. For a sorted range with duplicates, compute
`lower_bound`, `upper_bound`, and `equal_range` results by hand and state the
ordering precondition. No AI is used in this part.

## Part B — Algorithm implementation

Implement one bounded word-frequency or record-query pipeline using standard
containers and algorithms. Test empty input, absent keys, duplicates, ties, and
keys outside the stored range. Explain every asymptotic complexity claim.

## Part C — Final-project invalidation trace

Select one final-template loop that inserts, removes, or reorders objects:

1. identify the container and element type;
2. state whether elements own objects or borrow them;
3. mark iterators, pointers, and references invalidated by the mutation;
4. trace object destruction and later callbacks or uses;
5. compare erase-return, erase/remove, and mark-then-sweep where applicable.

## Part D — AI audit

Predict invalidation before asking AI. Check its answer against the standard
library contract and the repository's actual mutation order, then retain one
accepted or rejected claim with evidence.

## Deliverable

- AI-free template/boundary-algorithm trace;
- tested standard-library pipeline;
- annotated project loop and recommended safe mutation pattern;
- complexity justification and AI-audit entry.
