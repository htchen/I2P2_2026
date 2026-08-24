# Week 6 Lab — Recursive AST Ownership

## Outcomes

Students can construct, traverse, and release a recursive tree; explain its
ownership invariant; and verify cleanup after partial construction failure.

## Part A — AI-free readiness

Given a small tree, state the base case and decreasing measure for recursive
size, traversal, and destruction functions. Trace preorder, inorder, and
postorder visits without running code.

## Part B — AST construction and traversal

Using a small instructor-supplied AST type that is separate from the graded
project TODOs:

1. draw the tree for one expression;
2. label each node's owner and child links;
3. trace preorder, inorder, and postorder visits;
4. implement or complete construction and traversal functions.

## Part C — Cleanup and failure paths

Implement or complete a postorder destructor. Force one allocation failure
after a child exists, release the partial tree, and verify both normal and
failure paths under AddressSanitizer.

## Part D — AI audit

The initial tree and destruction trace are AI-free. Afterwards, an AI tool may
review the trace or propose failure cases. Verify every suggestion with a hand
drawing and sanitizer run, and retain one accepted or rejected claim.

## Deliverable

- one annotated tree and traversal trace;
- memory-clean normal and partial-failure runs;
- a two-sentence bridge explaining how the model applies to the project AST;
- AI-use record with one verified claim.
