# Week 14 Lab — Testable Game Logic, FSM, or Search

## Outcomes

Students can separate a deterministic game rule from graphics and verify an AI-
assisted state machine or search algorithm with properties and counterexamples.

## Part A — AI-free readiness

For a small graph or state machine, enumerate successors/transitions and trace
BFS or event handling by hand. Identify one missing transition or illegal state.

## Part B — Select an approved extension

Choose one bounded behavior such as an enemy mode, cooldown, targeting policy,
wave FSM, or pathfinding rule. Define state, inputs, outputs, invariants, tie
rules, and impossible states without Allegro handles.

## Part C — Implement and verify

Test normal, boundary, unreachable/invalid, and repeated-update cases. For
search, verify path legality and optimality on tiny enumerable examples. For an
FSM, cover relevant state/event pairs. Integrate only after logic tests pass.

## Part D — AI audit

Ask AI for counterexamples and a code review. Check for late visited marking,
missing transitions, invalid successors, dangling captures, and unstated tie
rules. Preserve one failing AI-generated case as a regression test.

## Deliverable

- state/transition or search model;
- automated non-graphical tests;
- manual graphical integration plan and smoke-test result;
- AI audit with a concrete counterexample.
