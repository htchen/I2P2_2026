# Week 5 Lecture Exercises: Linked Lists

[Starter code](week05_starter.c)

## Hour 1 — Representation and ownership

Draw the empty, one-node, and three-node states of the starter list. Label the
owning head link, each owning `next` link, and the one-past-end null value.

## Hour 2 — Link-location mutation

Implement insertion and erasure with a `Node **link` cursor. Test the head,
middle, tail, and out-of-range cases. State whether an index equal to the list
length is accepted for insertion.

## Hour 3 — Segment reversal and cleanup

Reverse a bounded half-open segment without copying node payloads, then destroy
the complete list. Trace every changed link before running the code under a
sanitizer.
