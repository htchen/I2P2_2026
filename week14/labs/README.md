# Week 14 Lab — Testable Game Logic, FSM, or Search

## Outcomes

Students can trace geometric connected components and multi-source BFS layers,
separate a deterministic game rule from graphics, and verify an AI-assisted
state machine or search algorithm with properties and counterexamples.

## Part A — AI-free readiness

For a small set of integer-coordinate stations and a radius, compute the
squared-distance predicate by hand, draw the implicit undirected graph, and
trace a connected-component DFS. Include a transitive chain whose endpoints are
not direct neighbors and classify singleton versus multi-vertex components.
Then trace a small grid BFS seeded by two simultaneous sources. Fill the distance
matrix by layers, identify the last reachable required cell, and detect one
required cell isolated by walls. Explain why visited state is recorded when a
cell enters the queue. Finally, for a small explicit graph or state machine,
enumerate successors or transitions and identify one missing transition or
illegal state.

## Part B — Select an approved extension

Continue the approved Week 13 vertical-slice design or choose one bounded
behavior such as an enemy mode, cooldown, targeting policy, wave FSM, or
pathfinding rule. Define state, inputs, outputs, invariants, tie rules, and
impossible states without Allegro handles.

## Part C — Implement and verify

Test normal, boundary, unreachable/invalid, and repeated-update cases. For
search, verify path legality and optimality on tiny enumerable examples. For an
FSM, cover relevant state/event pairs. Integrate only after logic tests pass.

## Part D — AI audit

Ask AI for counterexamples and a code review. Check for late visited marking,
missing transitions, invalid successors, dangling captures, and unstated tie
rules. Preserve one failing AI-generated case as a regression test.

## Deliverable

- geometric neighbor/component trace with arithmetic-bound justification;
- multi-source queue-layer trace, target-time aggregate, and unreachable case;
- state/transition or search model;
- automated non-graphical tests;
- manual graphical integration plan and smoke-test result;
- AI audit with a concrete counterexample.
