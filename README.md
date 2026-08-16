# Introduction to Programming II in C and C++

**Draft syllabus — Fall 2026**

This syllabus is a working document. Confirmed course rules are stated as such;
proposed assessment dates, weights, and detailed requirements remain subject to
revision before the semester begins.

## Course at a Glance

| Item | Information |
|------|-------------|
| Course | Introduction to Programming II |
| Languages | C and C++ |
| Prerequisite | One semester of programming in Python |
| Duration | 16 weeks |
| First meeting | Tuesday, September 8, 2026 |
| Tuesday meeting | Evening lecture, project demo, or exam |
| Regular lecture duration | Three hours |
| Thursday meeting | Lab or midterm exam |
| Lecture notes | 13 |
| Final exam | Tuesday, December 22, 2026 |
| Final project demo | Date TBD, after the final exam |

## Course Description

This course introduces C and C++ to students who already understand fundamental
programming concepts through Python. Rather than reteaching variables,
conditions, loops, functions, collections, recursion, and classes from the
beginning, the course concentrates on what changes when programs are compiled
and when data representation, memory, lifetime, and ownership become explicit.

The course follows one conceptual progression:

> familiar programming ideas in Python → explicit representation and ownership
> in C → safer abstractions and resource management in C++

The first three weeks form a C crash course taught through direct comparisons
with Python. Students then study pointers, dynamic allocation, linked lists,
recursion, binary trees, parsing, and syntax trees. The second half transitions
to C++ references, RAII, classes, value semantics, polymorphism, templates, the
standard library, and problem solving in a multi-file C++ codebase.

## Learning Outcomes

By the end of the course, students should be able to:

1. Translate small Python programs into C and explain differences in types,
   compilation, representation, and memory behavior.
2. Use C arrays, strings, structures, pointers, and dynamic allocation safely.
3. Implement, traverse, test, and release linked and tree-based data structures.
4. Diagnose invalid access, leaks, dangling pointers, and other forms of
   undefined behavior.
5. Explain how C++ references, classes, RAII, and standard-library types improve
   safety and expressiveness.
6. Design C++ classes with clear invariants and appropriate construction,
   destruction, copying, and moving behavior.
7. Use inheritance and runtime polymorphism when appropriate.
8. Use templates, containers, iterators, algorithms, and lambdas to solve
   programming problems.
9. Read, navigate, modify, and test an unfamiliar multi-file C++ codebase.
10. Use coding AI productively while remaining accountable for specifications,
    design decisions, correctness, testing, and submitted code.
11. Demonstrate independent C and C++ programming fluency without network or AI
    assistance.

## Learning and Assessment Modes

The course deliberately develops two complementary abilities.

### Independent programming fluency

Students must be able to read, trace, debug, and write C and C++ without AI.
This ability is assessed through in-person exams conducted in a secured,
programming-contest-like judge environment with no outward network connection.

### AI-assisted software engineering

Students are allowed and encouraged to use coding AI for labs and projects.
These activities assess the ability to specify, generate, inspect, test,
debug, integrate, and explain software produced with AI assistance.

The governing principle is:

> Project grades measure demonstrated ownership of the software, not unaided
> authorship of every line.

## Coding AI Policy

### Permitted and encouraged uses

Students may use LLMs and coding assistants to:

- discuss requirements and design alternatives;
- generate or complete code;
- explain unfamiliar code;
- diagnose compiler, runtime, or test failures;
- suggest tests and edge cases;
- review, refactor, document, or optimize code;
- learn library and tool usage.

Using AI does not reduce a student's grade. Students remain responsible for
every submitted line, design decision, dependency, test, and failure.

### Required disclosure

Each project must include a concise `AI_USAGE.md`. Students should document
three to five significant interactions rather than submit complete private chat
histories. Each record should state:

1. The tool used
2. The task or question given to the tool
3. The important suggestion or output
4. What was accepted, rejected, or modified
5. How the result was verified
6. What was learned or remained uncertain

Each project must also include an **AI audit**: at least one generated or
AI-suggested implementation that the student challenged, tested, found
incorrect or unsuitable, and repaired.

### Accountability and academic integrity

- Students may use AI, but may not misrepresent their understanding or AI use.
- Students may not copy another student or team's project.
- External code and assets must be used consistently with their licenses and
  attributed when required.
- AI tools may not be used during secured exams or other explicitly designated
  AI-free activities.
- During a project demo, every student must be able to explain and safely modify
  the submitted code.
- Fabricated development records, undisclosed material assistance, and false
  claims of contribution are academic-integrity violations.

## Three-Hour Lecture Pattern

Each regular Tuesday lecture is a three-hour session. The 13 lecture notes are
organized explicitly as Hour 1, Hour 2, and Hour 3 rather than as a loose topic
list. A typical hour contains 45–50 minutes of explanation and live coding plus
10–15 minutes of tracing, pair work, a design checkpoint, or a short judge-style
implementation.

The three hours normally progress as follows:

1. **Model and vocabulary:** connect Python-familiar ideas or prior-week material
   to the C/C++ representation and tool model.
2. **Core implementation:** derive and trace the central data structure,
   language mechanism, or algorithm.
3. **Integration and verification:** solve a larger example, exercise ownership
   and error paths, test with tools, and connect the topic to an exam or project.

Short breaks may be placed between the hourly blocks. Every note includes an
hour-by-hour plan and concrete in-class production so that material can be
trimmed or extended without losing the conceptual sequence.

## Weekly Lab Pattern

On ordinary lab Thursdays, the recommended format is:

1. A short AI-free judge-style exercise for exam readiness
2. Guided practice related to the current lecture
3. AI-enabled project work, testing, or code review

The AI-free exercise emphasizes small, independently executable skills such as
pointer tracing, implementing one function, fixing a compiler error, finding a
boundary case, completing a list/tree operation, or using an STL component.
The remainder of the lab may use AI without restriction, subject to the course
accountability policy.

Two Thursday labs will be replaced by in-person midterm exams.

## Weekly Schedule

The following topic placement is proposed. The midterm dates and midterm project
demo date must be confirmed before the syllabus is finalized. This sequence uses
13 new lecture notes: Notes 1–7 in Weeks 1–7 and Notes 8–13 in Weeks 9–14.

| Week | Tuesday | Note | Tuesday topic or event | Thursday | Lab or exam |
|------|---------|------|------------------------|----------|-------------|
| 1 | Sep. 8 | 1 | From Python to C: toolchain, types, expressions, I/O, and control flow | Sep. 10 | Compiler and translation lab |
| 2 | Sep. 15 | 2 | C functions, arrays, strings, and Python comparisons | Sep. 17 | Arrays and strings lab |
| 3 | Sep. 22 | 3 | Structures, modular programs, headers, debugging, and builds | Sep. 24 | Multi-file C lab |
| 4 | Sep. 29 | 4 | Pointers, stack and heap, `malloc`, and `free` | Oct. 1 | Pointer tracing and sanitizers |
| 5 | Oct. 6 | 5 | Linked lists and pointer-to-pointer techniques | Oct. 8 | Linked-list implementation |
| 6 | Oct. 13 | 6 | Recursion and binary trees | Oct. 15 | **Proposed Midterm 1** |
| 7 | Oct. 20 | 7 | Expression parsing and syntax trees | Oct. 22 | Midterm project studio |
| 8 | Oct. 27 | — | **Proposed midterm project demo; no lecture** | Oct. 29 | Project debrief and C++ setup |
| 9 | Nov. 3 | 8 | From C to C++: references, `const`, strings, vectors, and RAII | Nov. 5 | C-to-C++ refactoring exercise |
| 10 | Nov. 10 | 9 | Classes, invariants, constructors, member functions, and operators | Nov. 12 | C++ value-class lab |
| 11 | Nov. 17 | 10 | Ownership, destructors, copy/move operations, and Rule of Zero/Five | Nov. 19 | Resource-management lab |
| 12 | Nov. 24 | 11 | Inheritance, virtual functions, and abstract interfaces | Nov. 26 | **Proposed Midterm 2** |
| 13 | Dec. 1 | 12 | Templates, STL containers, iterators, algorithms, and lambdas | Dec. 3 | C++ codebase extension lab |
| 14 | Dec. 8 | 13 | Graph and state-space search using modern C++ | Dec. 10 | Search and problem-solving lab |
| 15 | Dec. 15 | — | Integration, review, and final project clinic | Dec. 17 | Final project lab |
| 16 | Dec. 22 | — | **Final exam** | — | No scheduled Thursday meeting |

The final project demo will take place on a date after December 22.

## Midterm Project: C Expression Compiler

The midterm project is based on the previous
[CSI2P-II Mini Project](https://github.com/lightbulb12294/CSI2P-II-Mini).
Students extend a provided C compiler scaffold that processes a subset of C
expressions and emits instructions for a small simulated architecture.

The project integrates:

- tokenization and linked structures;
- expression grammar and precedence;
- recursive AST construction and traversal;
- semantic validation;
- assembly code generation;
- dynamic allocation and cleanup;
- testing with the ASMC reference tool;
- optional instruction-cycle optimization.

The new version should use staged milestones for code reading, parser work,
semantic checking, code generation, and verification. Students may use AI at
every stage, but must submit tests, an ownership explanation, an AI-use record,
and an AI audit. Each student must complete an individual code explanation or
small live modification during the demo.

### Proposed midterm project rubric

| Component | Weight |
|-----------|-------:|
| Judge-system correctness | 35% |
| Student-designed tests | 15% |
| Memory safety and code quality | 15% |
| Individual demo and code explanation | 25% |
| AI-use record and audit | 10% |

## Final Project: C++ Game Extension

The final project is based on the previous
[CSI2P2 Final Project Template v1.2.3](https://github.com/lightbulb12294/CSI2P2-Final-Project-Template/releases/tag/v1.2.3).
It is an Allegro 5 tower-defense codebase containing a game loop, resource and
operation centers, shapes, monsters, towers, levels, UI components, and
polymorphic object hierarchies.

The final project assesses students' ability to understand and extend an
existing multi-file C++ system. Each team should complete:

1. A meaningful gameplay extension
2. An architectural change that crosses multiple subsystems
3. A deliberate C++ design or ownership improvement
4. Automated tests for non-graphical logic and a manual integration-test plan
5. An AI-use record and AI audit
6. A working demonstration followed by individual code questions or changes

Art, animation, and audio may improve the demonstration, but the programming
grade should emphasize architecture, ownership, integration, verification, and
understanding rather than the number of visible features.

### Proposed final project rubric

| Component | Weight |
|-----------|-------:|
| Correctness and completeness | 20% |
| Meaningful gameplay extension | 15% |
| Architecture and integration | 20% |
| C++ ownership and design quality | 15% |
| Testing and verification | 10% |
| Individual demo and code modification | 15% |
| AI-use record and audit | 5% |

## Exams

There will be two midterm exams on Thursdays and one final exam on Tuesday,
December 22. Exams are held in person using a programming-contest-like judge
system without outward network access. AI tools, external LLM services,
communication software, and unapproved network resources are unavailable and
may not be used.

Exam tasks assess independent ability to:

- read and trace code;
- implement functions and small programs;
- use pointers and data structures correctly;
- diagnose compile-time and runtime errors;
- handle boundary cases;
- use core C++ language and standard-library facilities.

Detailed exam scope and permitted local documentation will be announced before
each exam.

## Overall Assessment

The course includes:

- Thursday labs and AI-free judge exercises
- Two in-person midterm exams
- The C midterm project and individual demo
- The C++ final project and individual demo
- The in-person final exam

Overall category weights, late policies, team-size rules, and project grade caps
remain TBD. Project rubrics above describe the proposed distribution within
each project, not the percentage of the final course grade.

## Development Expectations

- Compile with warnings enabled and address all relevant warnings.
- Use deterministic, repeatable tests.
- Use sanitizers or equivalent tools for memory-sensitive C/C++ work.
- Keep headers self-contained and separate interfaces from implementations.
- Use explicit ownership rules in C and RAII in C++.
- Prefer the Rule of Zero for ordinary C++ classes.
- Keep generated files and build artifacts out of version control.
- Do not submit code that cannot be explained by the student or team.

The exact compiler versions, C and C++ standards, supported operating systems,
and submission tools remain to be confirmed.

## Course Materials

The new course draws from:

- [Previous C and C++ course materials](https://github.com/htchen/i2p-nthu)
- [I2P2 2025 notebooks and assets](https://drive.google.com/drive/u/0/folders/1kIu5d8L7o3Dj5aY5_76_M_2SXgPSm6w9)
- [Previous C midterm project](https://github.com/lightbulb12294/CSI2P-II-Mini)
- [Previous C++ final project template](https://github.com/lightbulb12294/CSI2P2-Final-Project-Template/releases/tag/v1.2.3)

The detailed curriculum rationale, alternatives, risks, and unresolved design
questions are recorded in [COURSE_PROPOSAL.md](COURSE_PROPOSAL.md).
The reuse and reorganization of the previous C/C++ notes and 2025 notebooks is
recorded in [LECTURE_SOURCE_MAP.md](LECTURE_SOURCE_MAP.md).

## Repository Layout

Each `weekXX/` directory contains:

- `README.md` — weekly overview
- `lectures/` — lecture note and examples
- `lectures/lecture_exercises/` — in-lecture exercises and starter code
- `labs/` — Thursday lab material

The 13 lecture notes are placed in Weeks 1–7 and 9–14. Weeks 8, 15, and 16 are
reserved for the midterm project demo, integration/review, and final exam.

## Items to Finalize

- Dates of Midterm 1 and Midterm 2
- Date of the midterm project demo
- Overall assessment weights
- Project team sizes and individual-grade adjustment policy
- Late submission and attendance policies
- Official compiler versions and C/C++ standards
- Supported operating systems and development environment
- Judge, learning-management, and project-submission systems
- Permitted local documentation during exams
- Final project demo date and format
