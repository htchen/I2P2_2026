# Course Proposal: Introduction to Programming II in C and C++

**Status:** Draft 0.1 for discussion and revision

**Academic term:** Fall 2026

**Last updated:** August 15, 2026

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
- A midterm project demo will take place on a Tuesday and replace that day's
  lecture.
- The final exam will take place on Tuesday, December 22, 2026, during Week 16.
- The final project demo will be scheduled after the final exam.
- The course will have 13 lecture notes.
- Students are assumed to have completed a 16-week introduction to programming
  in Python.
- Weeks 1–3 will be a C crash course taught by contrasting C with Python.
- After the crash course, the course will cover C pointers and dynamic data
  structures, including linked lists and binary trees, before moving to C++.

The dates of the two midterms and the midterm project demo are not yet
confirmed.

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
7. Use inheritance and runtime polymorphism when appropriate.
8. Use templates, containers, iterators, algorithms, and lambdas to solve
   programming problems.
9. Compare a manual C implementation with a safer and more expressive C++
   implementation.
10. Build and demonstrate a nontrivial program using appropriate data
    structures and ownership rules.

## 5. Proposed 16-Week Schedule

The following is a working schedule, not a confirmed calendar. It deliberately
uses 13 new lecture notes: Notes 1–7 in Weeks 1–7 and Notes 8–13 in Weeks 9–14.
Week 8 is proposed for the midterm project demo, Week 15 reuses existing notes
for integration and review, and Week 16 is the final exam.

| Week | Tuesday | Note | Proposed Tuesday topic | Thursday | Proposed Thursday activity |
|------|---------|------|--------------------------|----------|-----------------------------|
| 1 | Sep. 8 | 1 | From Python to C: toolchain, types, expressions, I/O, and control flow | Sep. 10 | Compiler and translation lab |
| 2 | Sep. 15 | 2 | C functions, arrays, strings, and Python comparisons | Sep. 17 | Arrays and strings lab |
| 3 | Sep. 22 | 3 | Structures, modular programs, headers, debugging, and builds | Sep. 24 | Multi-file C lab |
| 4 | Sep. 29 | 4 | Pointers, stack and heap, `malloc`, and `free` | Oct. 1 | Pointer tracing and sanitizers |
| 5 | Oct. 6 | 5 | Linked lists and pointer-to-pointer techniques | Oct. 8 | Linked-list implementation |
| 6 | Oct. 13 | 6 | Recursion and binary trees | Oct. 15 | **Proposed Midterm 1** |
| 7 | Oct. 20 | 7 | Expression parsing and syntax trees | Oct. 22 | Midterm-project studio |
| 8 | Oct. 27 | — | **Proposed midterm project demo; no lecture** | Oct. 29 | Project debrief and C++ setup |
| 9 | Nov. 3 | 8 | From C to C++: references, `const`, strings, vectors, and RAII | Nov. 5 | Refactor a C program into C++ |
| 10 | Nov. 10 | 9 | Classes, invariants, constructors, member functions, and operators | Nov. 12 | C++ value-class lab |
| 11 | Nov. 17 | 10 | Ownership, destructors, copy/move operations, and Rule of Zero/Five | Nov. 19 | Resource-management lab |
| 12 | Nov. 24 | 11 | Inheritance, virtual functions, and abstract interfaces | Nov. 26 | **Proposed Midterm 2** |
| 13 | Dec. 1 | 12 | Templates, STL containers, iterators, algorithms, and lambdas | Dec. 3 | STL transformation lab |
| 14 | Dec. 8 | 13 | Graph and state-space search using modern C++ | Dec. 10 | Search and problem-solving lab |
| 15 | Dec. 15 | — | Integration, review, and final-project clinic | Dec. 17 | Final-project lab |
| 16 | Dec. 22 | — | **Final exam** | — | No scheduled Thursday meeting |

If this calendar is adopted, the current provisional lecture files will need to
be remapped to match it.

## 6. Proposed Project Spine

The recommended design is one evolving project rather than unrelated C and C++
projects.

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

### C++ phase and final project

Students refactor and extend the C project in C++:

1. Replace character arrays and manual resizable arrays with `std::string` and
   `std::vector`.
2. Encapsulate tokens, nodes, and evaluators in classes with clear invariants.
3. Replace manual ownership with RAII and smart ownership where appropriate.
4. Use copy/move behavior intentionally, preferring the Rule of Zero.
5. Use standard-library containers and algorithms.
6. Add an extension that requires graph or state-space search, alternative node
   types, or another approved feature.
7. Demonstrate the final version after the final exam.

This creates a direct comparison between C and C++ and makes the motivation for
C++ visible in the students' own code.

## 7. Suggested Assessment Alignment

The following scopes are proposals and should be revised after the assessment
weights and dates are confirmed.

- **Midterm 1:** Notes 1–5; C syntax, compilation, arrays/strings, structures,
  pointers, allocation, and linked-list reasoning. Material first presented on
  the Tuesday immediately before the exam should not be tested.
- **Midterm project:** Notes 4–7; ownership, linked structures, recursion,
  binary trees, parsing, and cleanup.
- **Midterm 2:** Notes 8–10; the C-to-C++ transition, classes, RAII, and resource
  management. It may include comparisons with the C implementation.
- **Final exam:** Cumulative conceptual coverage with greater emphasis on Notes
  11–13 and integration across C and C++.
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
- Basic inheritance and runtime polymorphism
- Basic templates, STL containers, iterators, algorithms, and lambdas
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

Selected C++17/C++20 features such as structured bindings, `std::optional`, and
`std::variant` may be introduced inside relevant examples. They should not
displace the ownership, classes, and STL core.

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

## 10. Lab Design Principles

- Begin with a small Python reference or behavior specification when useful.
- Require students to predict memory or ownership behavior before running code.
- Introduce compiler warnings and sanitizers in Week 1 and use them throughout.
- Keep each lab connected to the current lecture and project milestone.
- Include tests for normal cases, boundary cases, and cleanup behavior.
- Assess explanation and debugging, not only final output.
- Use starter code selectively; students must still implement the central data
  structure or ownership decision.

## 11. Risks and Mitigations

| Risk | Mitigation |
|------|------------|
| The first three weeks become a syntax dump | Teach through Python-to-C translation and omit concepts students already understand |
| Students use pointers mechanically without a memory model | Require diagrams, tracing, sanitizers, and explicit ownership contracts |
| C consumes too much of the semester | Freeze the required C scope after the midterm project and move extra C topics to appendices |
| C++ becomes a list of unrelated features | Introduce each feature while refactoring a limitation in the C project |
| Copy/move semantics overwhelm students | Use one small resource-owning class, then emphasize Rule of Zero in normal code |
| Projects and labs create excessive workload | Make labs incremental project milestones and reuse the same domain |
| Advanced material crowds out fundamentals | Treat modern C++, interoperability, CUDA, and systems topics as enrichment |

## 12. Alternative Compression Option

If six C++ lecture notes prove insufficient, remove expression parsing as a
required lecture topic. Use a binary-tree application for the midterm project,
begin the C++ transition one week earlier, and retain parsing and syntax trees as
an optional extension. This sacrifices some of the coherent compiler-project
story but creates more room for STL, testing, exceptions, and modern ownership.

## 13. Open Decisions

- Which Thursdays will contain Midterm 1 and Midterm 2?
- Which Tuesday will contain the midterm project demo?
- Will the expression/syntax-tree project be the common semester project?
- Should Midterm 2 be cumulative or C++-focused?
- What are the assessment weights?
- Will projects be individual or team-based?
- Which compiler, language standards, operating systems, and development tools
  will be officially supported?
- Should C use C17 and C++ use C++20, or should different standards be selected?
- Which online judge or submission system will be used?
- What final-project extensions are feasible within the available lab support?

## 14. Revision Log

| Version | Date | Change |
|---------|------|--------|
| 0.1 | 2026-08-15 | Initial proposal based on the previous C/C++ materials and confirmed scheduling constraints |
