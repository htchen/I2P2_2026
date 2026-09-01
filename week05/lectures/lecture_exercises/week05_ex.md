# Week 5 Lecture Exercises: Linked Lists

[Starter code](week05_starter.c)

The lecture uses `struct List { Node* head; size_t size; }`; this focused starter
uses only `Node* head`. Consequently, its functions receive `Node** head` and do
not update a cached size. The owning-link algorithm is the same in both forms.

## Hour 1 — Representation and ownership

Draw the empty, one-node, and three-node states of the starter list. Label the
owning head link, each owning `next` link, and the one-past-end null value.

## Hour 2 — Link-location mutation

Implement insertion and erasure with a `Node** link` cursor. Test the head,
middle, tail, and out-of-range cases. State whether an index equal to the list
length is accepted for insertion.

## Hour 3 — Segment reversal and cleanup

Reverse a bounded half-open segment without copying node payloads, then destroy
the complete list. Trace every changed link before running the code under a
sanitizer. Implement `list_reverse_range(head, first, last)` with the following
contract: `[first, last)` is measured in node indices; equal boundaries are a
successful no-op; and an invalid or out-of-range boundary returns `false`
without changing the list.

## Compile and verify

From the repository root:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  week05/lectures/lecture_exercises/week05_starter.c -o /tmp/week05_starter
/tmp/week05_starter
```

After every TODO is correct, the final line is
`all Week 5 starter checks passed`. Run the same executable with AddressSanitizer
before consulting [`../examples.c`](../examples.c).
