# Week 15 Lab — Integration and Demo Rehearsal

## Outcomes

Students can stabilize a final release, report limitations honestly, and explain
or modify their work without AI assistance.

## Part A — AI-free readiness

Complete one short judge-style C++ task from the final-exam scope. Then, without
AI, trace one project object's dynamic dispatch and lifetime.

## Part B — Feature freeze and regression pass

Stop adding unapproved features. From a clean checkout:

1. build using documented commands;
2. run all automated tests;
3. run warnings and available sanitizers;
4. test missing/invalid assets or initialization failures where supported;
5. execute the manual integration plan;
6. verify packaging on a second clean environment when practical.

## Part C — Evidence and release review

Complete the project checklist, architecture/ownership map, known-limitations
list, attribution, and `AI_USAGE.md`. Remove generated files and secrets. Record
the exact submitted revision.

## Part D — Individual rehearsal

Each team member draws one random subsystem and must:

- explain its contract and owner;
- trace one update or failure path;
- interpret one test or diagnostic;
- make and verify a small change without AI.

## Deliverable

- release candidate and recorded revision;
- automated and manual verification evidence;
- final architecture/ownership map;
- known limitations and completed AI audit;
- successful individual rehearsal record.
