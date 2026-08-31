# Course Proposal: Introduction to Programming II in C and C++

**Status:** Draft 0.4; calendar and assessment placement aligned with the syllabus

**Academic term:** Fall 2026

**Last updated:** August 29, 2026

This document records the initial curriculum proposal. It distinguishes fixed
course constraints from working assumptions so that the schedule can be revised
without accidentally treating an unconfirmed idea as a final decision.

## 1. Source Material

The new course will be derived from two previous collections:

- [I2P NTHU course repository](https://github.com/htchen/i2p-nthu)
- [I2P2 2025 course folder](https://drive.google.com/drive/u/0/folders/1kIu5d8L7o3Dj5aY5_76_M_2SXgPSm6w9)

The repository contains the broad content of the former C and C++ courses. Its
C material includes compilation, formatted I/O, control flow, functions,
arrays, strings, pointers, dynamic allocation, structures, linked lists,
recursion, stacks, queues, and binary search trees. Its C++ material includes
the C++ language transition, classes, constructors, copy and move operations,
inheritance, abstract classes, templates, the standard library, and graph or
state-space problems.

The 2025 course folder provides a more recent instructional sequence:

1. Linked lists and the Josephus problem
2. Binary trees
3. Expression parsing, syntax trees, and compiler concepts
4. Transition to C++
5. Classes, constructors, and an integer-vector implementation
6. Copy and move operations
7. Standard-library containers and graphs
8. Problem solving and state-space search
9. Selected C++17/C++20 features
10. Enrichment topics such as Python interoperability and CUDA

The new course should reuse and reorganize these materials rather than attempt
to preserve every previous topic.

## 2. Confirmed Constraints

- The course lasts 16 weeks.
- Week 1 begins on Tuesday, September 8, 2026.
- Tuesday is the regular evening lecture meeting.
- Thursday is the regular lab meeting.
- Two midterm exams will take place on Thursdays and replace the corresponding
  labs.
- Midterm Exam 1 is scheduled for Thursday, October 22, 2026, and Midterm Exam 2
  is scheduled for Thursday, December 3, 2026.
- Quiz 1 is scheduled for Thursday, October 8, and Quiz 2 for Thursday,
  November 19, exactly two weeks before their respective midterms.
- A midterm project demo will take place on a Tuesday and replace that day's
  lecture.
- The final project demo will be scheduled during Week 16 on a date to be
  announced.
- The course will have 13 lecture notes.
- Students are assumed to have completed a 16-week introduction to programming
  in Python.
- Weeks 1–3 will be a C crash course taught by contrasting C with Python.
- After the crash course, the course will cover C pointers and dynamic data
  structures, including linked lists and binary trees, before moving to C++.

The midterm project demo is scheduled for Tuesday of Week 10.

## 3. Curriculum Strategy

The course should be organized around one conceptual transition:

> familiar programming ideas in Python → explicit representation and ownership
> in C → safer abstractions and resource management in C++

Students already know variables, conditions, loops, functions, collections,
recursion, and classes at a conceptual level. Lecture time should therefore
focus on what is new:

- compilation, linking, and program layout;
- static types and representation;
- arrays, addresses, and pointer arithmetic;
- stack, heap, lifetime, and explicit allocation;
- ownership and cleanup;
- data-structure implementation;
- value semantics, RAII, and C++ resource management;
- generic programming and the standard library.

The course should avoid presenting C and C++ as unrelated languages. Each C++
feature should answer a limitation or risk that students have already
experienced in C.

## 4. Proposed Learning Outcomes

By the end of the course, students should be able to:

1. Translate small Python programs into idiomatic C and explain the differences
   in types, compilation, and memory behavior.
2. Use arrays, strings, structures, pointers, and dynamic allocation safely in
   C.
3. Implement, test, traverse, and release linked and tree-based data structures.
4. Diagnose common C errors, including invalid access, leaks, dangling
   pointers, and undefined behavior.
5. Explain how C++ references, classes, RAII, and standard-library types improve
   resource safety.
6. Design C++ value types with correct construction, destruction, copying, and
   moving behavior.
7. Use templates, containers, iterators, algorithms, and lambdas to solve
   programming problems.
8. Use inheritance and runtime polymorphism when appropriate.
9. Compare a manual C implementation with a safer and more expressive C++
   implementation.
10. Build and demonstrate a nontrivial program using appropriate data
    structures and ownership rules.
11. Use coding AI for explanation, review, testing, and debugging while
    verifying its claims and documenting one repaired or rejected suggestion.
12. Demonstrate independent C and C++ fluency during secured AI-free exams and
    individual project checks.

## 5. Fall 2026 16-Week Schedule

The following is the Fall 2026 calendar. Lecture notes are assigned in
Weeks 1–9 and 11–14. Week 10 is scheduled for the midterm project
demo, Week 15 reuses existing notes for integration and review, and Week 16 is
reserved for the final project demo.

| Week | Tuesday | Tuesday topic | Thursday | Thursday activity |
|------|---------|---------------|----------|-------------------|
| 1 | Sep. 8 | From Python to C: toolchain, types, expressions, I/O, and control flow | Sep. 10 | Compiler and translation lab |
| 2 | Sep. 15 | C functions, arrays, strings, and Python comparisons | Sep. 17 | Arrays and strings lab |
| 3 | Sep. 22 | Structures, modular programs, headers, debugging, and builds | Sep. 24 | Midterm scaffold build and code map |
| 4 | Sep. 29 | Pointers, stack and heap, `malloc`, and `free` | Oct. 1 | Midterm ownership and sanitizer milestone |
| 5 | Oct. 6 | Linked lists and pointer-to-pointer techniques | Oct. 8 | **Quiz 1**; token-list tracing and testing milestone |
| 6 | Oct. 13 | Recursion, binary trees, and AST ownership | Oct. 15 | Recursive AST ownership lab |
| 7 | Oct. 20 | Expression parsing and syntax trees | Oct. 22 | **Midterm Exam 1 + Written Assessment**; asynchronous parser-integration checkpoint |
| 8 | Oct. 27 | From C to C++: values, references, exceptions, and RAII | Oct. 29 | Final-template build and architecture map |
| 9 | Nov. 3 | Classes, invariants, constructors, member functions, and operators | Nov. 5 | Event-loop and resource-lifecycle trace |
| 10 | Nov. 10 | **Midterm project demo; no lecture** | Nov. 12 | Small cross-file project component |
| 11 | Nov. 17 | Templates, STL containers, iterators, algorithms, and `optional` | Nov. 19 | **Quiz 2**; container, algorithm, and invalidation lab |
| 12 | Nov. 24 | Ownership, destructors, copy/move operations, and Rule of Zero/Five | Nov. 26 | Ownership implementation and sanitizer lab |
| 13 | Dec. 1 | Inheritance, virtual functions, ownership, and composition | Dec. 3 | **Midterm Exam 2**; asynchronous polymorphism checkpoint |
| 14 | Dec. 8 | Graph and state-space search using modern C++ | Dec. 10 | Testable game logic, FSM, or search extension |
| 15 | Dec. 15 | Integration, review, and final-project clinic | Dec. 17 | Final-project lab |
| 16 | — | **Final project demo (date TBD)** | — | No scheduled Thursday meeting |

The lecture files now follow this dependency order. Four deliberate bridges
prevent concepts from appearing without enough prior model:

- The Week 2 lecture notes give the minimal `&`/`*` address-passing vocabulary
  needed by ordinary C interfaces, while the Week 4 lecture notes retain the
  full pointer, lifetime, and ownership treatment.
- The Week 8 lecture notes introduce throwing, handler selection, library-value
  lifetimes, and stack unwinding before students define constructors and copy
  operations for their own classes.
- The Week 11 lecture notes explain templates and `std::optional` before the
  Week 13 lecture notes use generic polymorphic containers and offer
  `std::variant` as optional enrichment; Week 14 then returns optional search
  results.
- The Week 12 lecture notes make copy/move and smart-pointer ownership explicit
  before Week 13 combines polymorphism with owning base pointers and recursive
  composites.

Students may use `vector<T>` as library clients in the Week 8 lecture notes
before learning to author templates; the class model in the Week 9 lecture
notes provides the needed foundation for template definitions, iterators,
algorithms, and lambdas in the Week 11 lecture notes. Week 8 uses value returns
as an interface principle without showing explicit move operations; Week 12
then supplies the full copy/move and ownership model.
Both midterms exclude material first presented on the Tuesday immediately before
the exam.

## 6. Proposed Project Spine

The course uses two different previous projects: a C expression compiler for the
midterm and an Allegro C++ game extension for the final. They are not one
evolving codebase. Their continuity comes from a common engineering method:
students map unfamiliar code, state ownership, make incremental changes, build
tests, audit AI suggestions, and demonstrate individual understanding.

### C phase and midterm project

Students build a small expression-processing program in C:

1. Tokenize an expression.
2. Store or process tokens using explicit C data structures.
3. Parse the expression into a syntax tree.
4. Traverse, evaluate, print, and release the tree.
5. Demonstrate the C implementation during the midterm project demo.

This project gives pointers, allocation, linked structures, recursion, and trees
a shared purpose. Weekly labs should build components of the project rather than
introduce an unrelated assignment each time.

Release the scaffold by Week 3. Use Weeks 3–5 for code mapping, ownership, token
structures, and verification. Week 6 provides a full recursive-AST ownership
lab. Since the Week 7 Thursday meeting is Midterm 1, use a bounded asynchronous
checkpoint for parser, semantic, and code-generation verification before the
Week 10 demo.

### C++ phase and final project

Students read and extend the previous Allegro tower-defense template:

1. Build the supplied multi-file C++ codebase and trace its event/update/draw
   lifecycle.
2. Map class, header, resource, and ownership relationships before changing
   them.
3. Add a small feature across all required subsystems as a thin vertical slice.
4. Use copy/move behavior intentionally and improve one ownership or resource
   boundary, preferring RAII and the Rule of Zero.
5. Use standard-library containers and algorithms with correct invalidation and
   complexity reasoning.
6. Isolate non-graphical logic for automated tests and maintain a manual
   graphical integration plan.
7. Add an approved gameplay, finite-state-machine, or search extension and
   demonstrate the final version in Week 16.

The template is introduced in Week 8, not Week 15. Allegro setup and
template-specific navigation are taught in labs; lectures teach the underlying
C++ mechanisms and end with short project connections.

## 7. Suggested Assessment Alignment

The following scopes are aligned with the scheduled assessments.

- **Midterm 1:** Lecture material from Weeks 1–6; C syntax, compilation,
  arrays/strings, structures,
  pointers, allocation, linked-list reasoning, recursion, and binary trees.
  Week 7 parsing material is excluded because it is first presented on the
  Tuesday immediately before the exam.
- **Midterm project:** Lecture material from Weeks 4–7; ownership, linked structures, recursion,
  binary trees, parsing, and cleanup.
- **Midterm 2:** Lecture material from Weeks 8, 9, 11, and 12; the C-to-C++
  transition, classes, templates, STL
  containers, iterators, algorithms, boundary searches, `optional`, complexity,
  invalidation, ownership, and copy/move semantics. It may include comparisons
  with the C implementation. Week 13 material is excluded.
- **Final project:** Correctness, ownership and resource safety, data-structure
  choice, use of C++ abstractions, testing, and explanation during the demo.

## 8. Scope Priorities

### Required core

- C compilation, types, control flow, functions, arrays, and strings
- Structures, modular compilation, and debugging
- Pointers, lifetime, dynamic allocation, and cleanup
- Linked lists, recursion, and binary trees
- A small parser or syntax-tree application
- C++ references, `const`, strings, vectors, and RAII
- Classes, constructors, destructors, and value semantics
- Copy/move operations and the Rule of Zero/Five
- Basic templates, STL containers, iterators, algorithms, and lambdas
- Expected-absence results with `std::optional`
- Basic inheritance and runtime polymorphism
- Graph or state-space problem solving

### Optional or appendix material

- Detailed `printf`/`scanf` formatting rules
- Extensive bit manipulation and unions
- Function-pointer and `qsort` deep dives
- Doubly and circular linked-list variants beyond a focused exercise
- Standalone implementations of every stack and queue variant
- Assembly language and detailed computer architecture
- Advanced compiler construction
- Template metaprogramming
- C++ modules, parallel algorithms, advanced ranges, and advanced concepts
- Python/C++ interoperability
- CUDA

The small `std::optional` result model is part of the core because graph search
uses it in the Week 14 lecture notes. Other selected C++17 features, such as
structured bindings and `std::variant`, may be introduced inside relevant
examples. C++20 comparisons remain enrichment and should not displace the
ownership, classes, and STL core.

## 9. Lecture-Note Format

Each lecture note should use the same structure:

1. **Learning objectives**
2. **Python starting point** — the concept students already know
3. **C or C++ translation** — equivalent code and syntax
4. **Representation and memory model** — what exists in memory and who owns it
5. **Worked example** — connected to the semester project when possible
6. **Common mistakes** — compiler errors, undefined behavior, or design errors
7. **In-lecture exercises**
8. **Summary and comparison table**
9. **Thursday lab handoff**
10. **Optional enrichment** — material not required for assessment

The notes should emphasize contrasts rather than reteach general programming
ideas. Diagrams should be used heavily for pointer relationships, object
lifetime, linked structures, trees, and copy/move behavior.

### Python contrast companions

Each of the 13 lecture weeks adds a separate Python companion rather than
expanding the already full three-hour lecture note. Each companion accounts for
every fenced C/C++ example and labels the relationship as direct, a closest
Python model, or not faithfully translatable. The explanation must name missing
semantics such as fixed-width overflow, pointer arithmetic, `const`,
deterministic destruction, copy assignment, move operations, or exclusive
ownership.

The companions preserve inputs, outputs, and algorithms for direct
translations and include runnable standard-library-only Python files. They are
designed to help students reason from familiar Python concepts toward C/C++,
not to encourage mechanical transliteration. Companion coverage must remain
synchronized whenever a source lecture example changes.

## 10. Lab Design Principles

- Begin with a small Python reference or behavior specification when useful.
- Require students to predict memory or ownership behavior before running code.
- Introduce compiler warnings and sanitizers in Week 1 and use them throughout.
- Keep each lab connected to the current lecture and project milestone.
- Include tests for normal cases, boundary cases, and cleanup behavior.
- Assess explanation and debugging, not only final output.
- Use starter code selectively; students must still implement the central data
  structure or ownership decision.
- Require the predict → ask → verify → audit → explain cycle for significant AI
  assistance and preserve the evidence in the project milestone.

## 11. Risks and Mitigations

| Risk | Mitigation |
|------|------------|
| The first three weeks become a syntax dump | Teach through Python-to-C translation and omit concepts students already understand |
| Students use pointers mechanically without a memory model | Require diagrams, tracing, sanitizers, and explicit ownership contracts |
| C consumes too much of the semester | Freeze the required C scope after the midterm project and move extra C topics to appendices |
| C++ becomes a list of unrelated features | Connect each feature to a concrete ownership, architecture, container, or dispatch decision in the final-project template |
| Copy/move semantics overwhelm students | Use one small resource-owning class, then emphasize Rule of Zero in normal code |
| Projects and labs create excessive workload | Make labs incremental project milestones; keep each deliverable small and reuse it in the final submission |
| Thursday exams remove prerequisite labs | Move the Week 7 parser and Week 13 polymorphism checkpoints into bounded Tuesday/asynchronous work |
| AI produces plausible but unverified code | Require prediction, tests, an AI audit, explanation, and an AI-free live modification |
| Advanced material crowds out fundamentals | Treat modern C++, interoperability, CUDA, and systems topics as enrichment |

## 12. Alternative Compression Option

If six C++ lecture notes prove insufficient, remove expression parsing as a
required lecture topic. Use a binary-tree application for the midterm project,
begin the C++ transition one week earlier, and retain parsing and syntax trees as
an optional extension. This sacrifices some of the coherent compiler-project
story but creates more room for STL, testing, exceptions, and modern ownership.

## 13. Open Decisions

- Should Midterm 2 be cumulative or C++-focused?
- Will projects be individual or team-based?
- Which exact compiler versions, operating systems, and development tools will
  be officially supported for the C17 and C++17 course baselines?
- Which online judge or submission system will be used?
- What final-project extensions are feasible within the available lab support?

## 14. Revision Log

| Version | Date | Change |
|---------|------|--------|
| 0.1 | 2026-08-15 | Initial proposal based on the previous C/C++ materials and confirmed scheduling constraints |
| 0.2 | 2026-08-16 | Reordered templates before polymorphism; added pointer, exception, and `optional` prerequisite bridges; clarified midterm scope |
| 0.3 | 2026-08-16 | Aligned the separate compiler and game projects with staged labs, corrected the project narrative, and added AI-verification and exam-week mitigations |
| 0.4 | 2026-08-29 | Confirmed quiz, midterm, and project-demo placement; standardized the C17/C++17 baseline and assessment wording |
