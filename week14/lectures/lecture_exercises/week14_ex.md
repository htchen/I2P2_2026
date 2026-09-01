# Week 14 Lecture Exercises: Graph and State-Space Search

[Starter code](week14_starter.cpp)

## Hour 1 — DFS components

Build an adjacency list for a small undirected graph and complete recursive DFS.
Mark a vertex before exploring neighbors and count connected components.

## Hour 2 — Multi-source BFS

Seed all starting cells at distance zero, then compute shortest grid distances.
Predict the queue contents layer by layer and return absence when a target is
unreachable. Treat an empty or ragged grid, an out-of-bounds coordinate, or a
blocked source/target as invalid input rather than as an unreachable result.

## Hour 3 — Implicit states

Define state, legal transitions, visited identity, and goal test for a small
water-jug instance. Reconstruct one shortest path using predecessor information
without mixing visualization into the solver. Complete the supplied `State`,
`Transition`, `Successors`, and `ShortestJugSolution` scaffold, recording both
the parent and the action used to discover each state.
