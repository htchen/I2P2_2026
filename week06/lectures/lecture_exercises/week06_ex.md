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

Use [`../examples.c`](../examples.c) afterward to compare reconstruction and
partial-allocation cleanup.
