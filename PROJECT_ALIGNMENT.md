# Lecture, Lab, and Project Alignment

This document is the implementation map for the Fall 2026 projects. The
syllabus defines policy; this file identifies when each assessed capability is
taught, practiced, verified, and demonstrated.

## Shared learning cycle

Project milestones use the same six-step cycle:

1. **Predict** expected behavior, ownership, or likely failure before using AI.
2. **Ask** AI for explanation, alternatives, tests, review, or debugging help.
3. **Verify** with compiler diagnostics, tests, sanitizers, and manual traces.
4. **Audit** at least one incorrect, incomplete, or unsuitable AI suggestion.
5. **Explain** why the accepted code satisfies its contract.
6. **Modify** a small part without AI during the project demo.

Lecture notes teach transferable models and devote a short section to the
relevant project. Labs use the real scaffold/template. They must not provide a
copyable implementation of a graded TODO.

## Midterm project alignment

The C expression compiler is released in Week 3 so that Week 7 is an integration
week rather than first exposure.

| Week | Lecture foundation | Project practice | Evidence |
|------|--------------------|------------------|----------|
| 3 | Modules, headers, builds, debugging | Build scaffold; trace input → tokens → AST → semantics → instructions; classify TODO contracts | Build record, pipeline map, baseline tests |
| 4 | Pointers, lifetime, allocation | Quiz 1; mark owners and borrowers; trace success/error cleanup; run sanitizers | Quiz result, ownership table, sanitizer log, repaired seeded defect |
| 5 | Linked lists and pointer-to-pointer mutation | Trace token-list creation/conversion; test empty, invalid, and boundary inputs | Token trace and student test table |
| 6 | Recursion and tree ownership | Tuesday AST checkpoint: construct, traverse, and destroy a small tree | Hand trace plus memory-clean run; no Thursday lab |
| 7 | Grammar, recursive descent, semantics, code generation | Integrate one parser level, lvalue checks, ISA emission, and ASMC/differential tests | Grammar trace, semantic cases, instruction trace, final verification |
| 9 | No lecture; Tuesday is reserved for the midterm project demo | Individual demonstration and small AI-free modification | Working program, explanation, AI audit |

Required project-specific emphasis includes prefix/postfix effects, lvalue
rules, evaluation order, simulated-register use, instruction tracing, cleanup on
all paths, and hidden-test-resistant verification. Cycle optimization is an
optional extension after correctness.

## Final project alignment

The Allegro template is introduced in Week 8. Week 15 is reserved for
integration, not onboarding.

| Week | Lecture foundation | Project practice | Evidence |
|------|--------------------|------------------|----------|
| 8 | Values, references, exceptions, RAII | Clone/build/run; identify event loop, centers, entities, resources, and major directories | Reproducible build record and architecture map |
| 9 | No new lecture; apply the Week 8 C++ foundation | Trace input → event/state → update → draw; trace one resource on normal/failure paths | Event trace and resource-lifecycle table |
| 10 | Classes, invariants, headers | Quiz 2; add a small non-graphical component across header/source/test files | Quiz result, change-impact map, tests, invariant explanation |
| 11 | Copy/move, Rule of Zero, smart pointers | Audit one raw-owning collection or resource boundary; make one bounded improvement | Before/after ownership map and sanitizer evidence |
| 12 | Templates, containers, algorithms, invalidation | Asynchronous code-reading checkpoint on a project container mutation | Invalidation trace; no Thursday lab |
| 13 | Inheritance, composition, `variant` | Add one thin vertical slice through creation, registration, update/interaction, and draw | Working slice, hierarchy/design justification |
| 14 | Graph/state-space search and test boundaries | Add a testable FSM, game rule, or approved search/pathfinding extension | Automated logic tests and manual graphics plan |
| 15 | Integration and review | Failure handling, regression pass, feature freeze, demo rehearsal | Release checklist, known-limitations list, rehearsal change |
| 16+ | Final exam, then project demo | Individual explanation and AI-free modification | Final release and demonstration |

Allegro API details, environment setup, asset paths, and template-specific call
graphs belong in labs. Lectures retain general C++ design principles while
using the template as a recurring case study.

## Assessment coverage rule

Every graded requirement must appear in at least one earlier lecture, lab, or
published project guide. The staff alignment check is:

| Assessed requirement | Prior learning evidence |
|----------------------|-------------------------|
| C judge correctness | Notes 1–7 and Weeks 3–7 milestone tests |
| C memory safety | Note 4, Weeks 4 and 6 sanitizer runs |
| Parser/semantic/codegen reasoning | Note 7 and Week 7 studio |
| C++ architecture and integration | Notes 9–12 and Weeks 8–13 maps/slices |
| C++ ownership | Notes 8 and 10; Weeks 9 and 11 audits |
| Container correctness | Note 11; Week 12 checkpoint and Week 13 lab |
| Non-graphical testing | Notes 9, 11, and 13; Weeks 10 and 14 labs |
| AI accountability | Every project milestone and both demonstrations |

## Staff review checklist

Before publishing a project revision, verify that:

- milestone dates leave time to act on feedback;
- neither Thursday midterm is described as a normal lab;
- lecture examples do not disclose graded TODO solutions;
- all required tools have a documented installation and smoke test;
- public tests include normal, boundary, invalid, and cleanup behavior;
- rubrics reward reasoning and verification rather than visible feature count;
- every team member has an individual explanation/modification obligation;
- links from weekly READMEs point to the correct lab guide.
