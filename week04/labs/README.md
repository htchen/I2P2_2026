# Week 4 Lab — Midterm Ownership and Sanitizers

This Thursday session also includes **Quiz 1**, scheduled two weeks before
Midterm 1. Staff will announce which lab parts use the remaining in-person time
and which, if any, are completed asynchronously.

## Outcomes

Students can distinguish owners from borrowers in the compiler scaffold, trace
cleanup on success and failure, and use diagnostics to repair a memory defect.

## Part A — AI-free readiness

Draw stack and heap objects for a supplied allocation function. Mark the owner,
one borrower, the lifetime end, and one dangling use. Then repair the function
without changing its public contract.

## Part B — Project ownership table

For tokens, token containers, AST nodes, and temporary storage, record:

| Resource | Creator | Owner | Borrowers | Success cleanup | Failure cleanup |
|----------|---------|-------|-----------|-----------------|-----------------|

Trace at least one valid input, one syntax error after partial construction, and
one semantic rejection. Every successful allocation must have one reachable
release on every path.

## Part C — Diagnostic exercise

Run the supplied seeded program with warnings and address/undefined-behavior
sanitizers. For each report:

1. identify the invalid operation;
2. locate the allocation or lifetime start;
3. state the violated ownership contract;
4. make the smallest contract-preserving repair;
5. add a regression case and rerun the full supplied set.

Do not modify a graded project TODO during the seeded exercise.

## Part D — AI audit

Give an AI tool the diagnostic plus the smallest relevant code region. Require
three hypotheses rather than a replacement function. Test the hypotheses and
record why at least one is wrong, incomplete, or unsuitable.

## Deliverable

- completed ownership table;
- before/after diagnostic log;
- regression test;
- AI-audit entry.
