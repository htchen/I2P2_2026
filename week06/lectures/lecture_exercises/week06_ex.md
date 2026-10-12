# Week 6 Lecture Exercises: Recursion and Binary Trees

[Starter code](week06_starter.c)

## Hour 1 — Recursive contract

For `tree_size`, state the base case, recursive subproblems, and combination
rule. Trace the call stack on a three-node skewed tree.

## Hour 2 — Traversal timing

Complete preorder, inorder, and postorder printing by moving only the visit
operation. Explain why destruction must occur after both owned subtrees.

## Hour 3 — Reconstruction and ambiguity

Given distinct inorder and postorder sequences, reconstruct the tree recursively
and verify its preorder traversal. Then give two different trees with the same
preorder and postorder sequences to show why those two traversals alone may be
ambiguous. Implement `tree_build_from_inorder_postorder`: validate pointer and
distinct-label preconditions, use half-open subranges, return an owned root
through `out`, and release every partially built subtree if reconstruction or
allocation fails.

Use this precise input/output contract:

- `out` must be non-`NULL` for every call;
- when `size == 0`, the two traversal pointers may be `NULL`, the function
  succeeds, and it stores `NULL` in `*out`;
- when `size > 0`, both traversal pointers must be non-`NULL`, both sequences
  must contain the same distinct labels, and their orders must describe one
  consistent tree;
- every failure returns `false` and leaves `*out == NULL`; and
- allocate nodes through the supplied `tree_node_allocate()` function rather
  than by calling `malloc()` directly. The driver uses that wrapper to force a
  deterministic mid-construction allocation failure.

## Compile and verify

Print each visited value followed by one space; the driver supplies line breaks.
From the repository root:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  week06/lectures/lecture_exercises/week06_starter.c -o /tmp/week06_starter
/tmp/week06_starter
```

The completed program prints these traversal lines followed by its check summary:

```text
preorder: 1 2 3
inorder: 2 1 3
postorder: 2 3 1
all Week 6 starter checks passed
```

The driver also checks empty input, null pointers, duplicate labels, mismatched
label sets, inconsistent orders, and partial-allocation cleanup. Run it with
AddressSanitizer so the injected failure also detects an unreachable partial
tree. Use [`../examples.c`](../examples.c) afterward to compare the complete
implementation.
