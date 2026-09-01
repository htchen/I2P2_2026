# Week 14 Lecture Exercises: Graph and State-Space Search

[Starter code](week14_starter.cpp)

## Hour 1 — DFS components

Build an adjacency list for a small undirected graph and complete recursive DFS.
Mark a vertex before exploring neighbors. Then transfer the same visited-set
invariant to the supplied point representation: implement `WithinRadius`,
`GeometricDfs`, and `ClassifyComponents` without constructing an adjacency
matrix. The exercise input contract limits coordinate magnitudes and radius to
1,000,000 so the squared-distance calculation fits in `long long` when values
are widened before subtraction and multiplication. Validate every point once
before the component sweep, including an isolated point for which no pairwise
neighbor test would otherwise occur.

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

## Compile and verify

From the repository root:

```sh
c++ -std=c++17 -Wall -Wextra -Wpedantic \
  week14/lectures/lecture_exercises/week14_starter.cpp -o /tmp/week14_starter
/tmp/week14_starter
```

After all three hourly tasks are complete, the final line is
`all Week 14 starter checks passed`. Use [`../examples.cpp`](../examples.cpp)
afterward for complete adjacency-list DFS, grid BFS, and Water Jugs references.
The geometric component bodies remain an exercise; the supplied driver,
diagrams, and edge-case contract are their oracle.
