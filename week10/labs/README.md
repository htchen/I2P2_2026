# Week 10 Lab — Cross-File Component and Invariant

## Outcomes

Students can introduce a small C++ class with a stable invariant, deliberate
constructor/factory policy, correct header dependencies, and automated tests
before graphical integration.

## Part A — AI-free readiness

Repair a supplied header that relies on transitive includes and creates a
circular dependency. Explain when a forward declaration is sufficient.

## Part B — Change-impact map

For an instructor-approved non-graphical value or policy component, identify:

- public header and implementation;
- invariant and constructor validation;
- constructor or named-factory choice and its return/lifetime contract;
- factory/creation site and owner;
- observers and mutation sites;
- automated test target;
- later integration seam.

## Part C — Implement and test

Add the smallest complete component. Keep Allegro types outside its logic when
possible. Test valid construction, boundary behavior, invalid input policy, and
one state transition. Verify a clean build from the documented command.

## Part D — AI-assisted review

AI may propose an interface or tests after the student writes the invariant.
Reject any suggestion that invents repository APIs, weakens the invariant, or
adds an unjustified ownership mechanism.

## Deliverable

- change-impact map;
- header/source/tests for the component;
- invariant and ownership explanation;
- test output and AI-use entry.
