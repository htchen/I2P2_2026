# Week 6 Checkpoint — Recursive AST Ownership

Thursday is Midterm 1, so this is a bounded Tuesday/asynchronous checkpoint, not
a scheduled lab. It must not add material to the midterm scope.

## Task

Using a small instructor-supplied AST type that is separate from the graded
project TODOs:

1. draw the tree for one expression;
2. label each node's owner and child links;
3. trace preorder, inorder, and postorder visits;
4. implement or complete a postorder destructor;
5. demonstrate cleanup after a deliberately failed partial construction.

## AI rule

The initial tree and destruction trace are AI-free. Afterwards, an AI tool may
review the trace or propose failure cases. Verify every suggestion with a hand
drawing and sanitizer run.

## Deliverable

- one annotated tree and traversal trace;
- memory-clean normal and partial-failure runs;
- a two-sentence bridge explaining how the model applies to the project AST.

Expected effort: 30–45 minutes outside the midterm examination.
