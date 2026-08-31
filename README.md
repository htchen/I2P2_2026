# Introduction to Programming II in C and C++

**Syllabus — Fall 2026**

This syllabus records the Fall 2026 course schedule and grading structure.
Operational details explicitly listed under **Items to Finalize** may still be
updated before the semester begins.

## Course at a Glance

| Item | Information |
|------|-------------|
| Course | Introduction to Programming II |
| Languages | C and C++ |
| Prerequisite | One semester of programming in Python |
| Duration | 16 weeks |
| First meeting | Tuesday, September 8, 2026 |
| Tuesday meeting | Evening lecture or project demo |
| Regular lecture duration | Three hours |
| Thursday meeting | Lab or midterm exam |
| Lecture notes | 13 |
| Final project demo | Date TBD in Week 16 |

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
to C++ references, RAII, classes, templates, the standard library, value
semantics and ownership, polymorphism, and problem solving in a multi-file C++
codebase.

Each of the 13 lecture weeks also includes a **Python contrast companion**. Each
companion maps every C/C++ example to a direct Python translation, a closest
Python model, or an explanation of why no faithful translation exists. These
companions use students' prior knowledge as a bridge; they do not replace the
C/C++ representation, lifetime, ownership, or compilation model assessed in
the course. Each companion also provides a deterministic, standard-library-only
Python example file for executable comparisons.

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
7. Use templates, containers, iterators, algorithms, and lambdas to solve
   programming problems.
8. Use inheritance and runtime polymorphism when appropriate.
9. Read, navigate, modify, and test an unfamiliar multi-file C++ codebase.
10. Use coding AI productively while remaining accountable for specifications,
    design decisions, correctness, testing, and submitted code.
11. Demonstrate independent C and C++ programming fluency without network or AI
    assistance.

## Learning and Assessment Modes

The course deliberately develops two complementary abilities.

### Independent programming fluency

Students must be able to read, trace, debug, and write C and C++ without AI.
This ability is assessed through in-person quizzes and the programming portions
of exams, conducted in a secured, programming-contest-like judge environment
with no outward network connection. Midterm Exam 1 also includes the course's
in-person written assessment.

### AI-assisted software engineering

Students are allowed and encouraged to use coding AI for labs and projects.
These activities assess the ability to specify, generate, inspect, test,
debug, integrate, and explain software produced with AI assistance.

The governing principle is:

> Project grades measure demonstrated ownership of the software, not unaided
> authorship of every line.

For substantial AI-assisted work, students should use the following evidence
cycle:

1. **Predict:** state the expected behavior, ownership rule, or likely failure.
2. **Ask:** use AI for explanation, alternatives, tests, review, or debugging.
3. **Verify:** compile, run tests, use sanitizers, and trace representative cases.
4. **Audit:** challenge at least one AI assumption or unsuitable suggestion.
5. **Explain:** record why the accepted implementation satisfies its contract.
6. **Modify:** be prepared to make a small AI-free change during the demo.

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
The two in-person computer-lab quizzes are scheduled on the Thursday exactly two
weeks before the corresponding midterm: Quiz 1 before Midterm 1 and Quiz 2
before Midterm 2. The quiz dates are October 8 and November 19, respectively.

## Weekly Schedule

Lecture notes are assigned in Weeks 1–9 and 11–14; Week 10 is reserved for the
midterm project demo.

| Week | Tuesday | Tuesday topic or event | Thursday | Lab or exam |
|------|---------|------------------------|----------|-------------|
| [1](week01/) | Sep. 8 | From Python to C: toolchain, types, expressions, I/O, and control flow | Sep. 10 | Lab |
| [2](week02/) | Sep. 15 | C functions, arrays, strings, and Python comparisons | Sep. 17 | Lab |
| [3](week03/) | Sep. 22 | Structures, modular programs, headers, debugging, and builds | Sep. 24 | Lab |
| [4](week04/) | Sep. 29 | Pointers, stack and heap, `malloc`, and `free` | Oct. 1 | Lab |
| [5](week05/) | Oct. 6 | Linked lists and pointer-to-pointer techniques | Oct. 8 | **Quiz 1 + Lab** |
| [6](week06/) | Oct. 13 | Recursion, binary trees, and AST ownership | Oct. 15 | Lab |
| [7](week07/) | Oct. 20 | Expression parsing and syntax trees | Oct. 22 | **Midterm Exam 1 + Written Assessment** |
| [8](week08/) | Oct. 27 | From C to C++: values, references, exceptions, and RAII | Oct. 29 | Lab |
| [9](week09/) | Nov. 3 | Classes, invariants, constructors, member functions, and operators | Nov. 5 | Lab |
| [10](week10/) | Nov. 10 | **Midterm project demo; no lecture** | Nov. 12 | Lab |
| [11](week11/) | Nov. 17 | Templates, STL containers, iterators, algorithms, and `optional` | Nov. 19 | **Quiz 2 + Lab** |
| [12](week12/) | Nov. 24 | Ownership, destructors, copy/move operations, and Rule of Zero/Five | Nov. 26 | Lab |
| [13](week13/) | Dec. 1 | Inheritance, virtual functions, ownership, and composition | Dec. 3 | **Midterm Exam 2** |
| [14](week14/) | Dec. 8 | Graph and state-space search using modern C++ | Dec. 10 | Lab |
| [15](week15/) | Dec. 15 | Integration, review, and final project clinic | Dec. 17 | Lab |
| [16](week16/) | — | **Final project demo (date TBD)** | — | — |

The Midterm 1 scope is lecture material from Weeks 1–6, so material
first presented in the Week 7 lecture notes on October 20 is excluded from the
October 22 exam. The Midterm 2 scope is lecture material from Weeks 8,
9, 11, and 12—including templates, STL containers, iterators, algorithms,
boundary searches, `optional`, invalidation, complexity, ownership, and
copy/move semantics. Material first presented in the Week 13 lecture notes on
December 1 is excluded from the December 3 exam. This keeps each exam causally
downstream of its tested instruction rather than assessing a topic after only
two days.

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

The scaffold is released in Week 3. Weeks 3–5 establish its module map,
ownership model, token representation, and test workflow. Week 6 provides a
full AST construction, traversal, and cleanup lab. Because Midterm 1 replaces
the Week 7 lab, parser and code-generation verification continues through a
bounded asynchronous checkpoint before the Week 10 demonstration.

### Midterm project rubric — 20% of course grade

- **30%** Judge-system correctness
- **15%** Staged milestone evidence
- **10%** Student-designed tests
- **15%** Memory safety and code quality
- **25%** Individual demo and code explanation
- **5%** AI-use record and audit

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

The template is introduced in the Week 8 Thursday meeting. Subsequent labs
trace the event/update/draw lifecycle, add a small cross-file component, audit
container mutation, map ownership, and deliver one thin vertical slice before
the Week 15 integration clinic. Allegro-specific setup and codebase navigation
belong primarily in the labs; Tuesday notes retain transferable C++ concepts
and include short project connections.

### Final project rubric — 25% of course grade

- **15%** Correctness and completeness
- **10%** Staged milestone evidence
- **10%** Meaningful gameplay extension
- **20%** Architecture and integration
- **15%** C++ ownership and design quality
- **10%** Testing and verification
- **15%** Individual demo and code modification
- **5%** AI-use record and audit

## Midterm Exams

There will be two midterm exams on Thursdays. Programming portions are held in
person using a programming-contest-like judge system without outward network
access. Midterm Exam 1 also includes the single in-person Written Assessment,
worth 5% of the course grade. Midterm Exam 2 has no written component. AI tools,
external LLM services, communication software, and unapproved network resources
are unavailable and may not be used during any secured assessment.

Exam tasks assess independent ability to:

- read and trace code;
- implement functions and small programs;
- use pointers and data structures correctly;
- diagnose compile-time and runtime errors;
- handle boundary cases;
- use core C++ language and standard-library facilities.

Detailed exam scope and permitted local documentation will be announced before
each exam. Unless a later announcement explicitly provides more preparation
time, material first presented on the Tuesday immediately before a Thursday
midterm will not be tested on that midterm.

## Overall Assessment

The following components determine the final course grade:

| Component | Weight | Notes |
|-----------|-------:|-------|
| Midterm Exams | 20% + 20% | Two in-person midterm examinations |
| Quizzes | 5% + 5% | Quiz 1: Oct. 8; Quiz 2: Nov. 19—each two weeks before its midterm |
| Midterm Project | 20% | C expression-compiler project and demo |
| Written Assessment | 5% | One written assessment, administered with Midterm Exam 1 only |
| Final Project | 25% | C++ final project and demo |
| **Total** | **100%** | |

The project rubrics above describe how each project's own score is calculated;
the 20% and 25% entries in this table are their respective contributions to the
final course grade. Late policies, team-size rules, and project grade caps remain
to be finalized.

## Development Expectations

- Compile with warnings enabled and address all relevant warnings.
- Use deterministic, repeatable tests.
- Use sanitizers or equivalent tools for memory-sensitive C/C++ work.
- Keep headers self-contained and separate interfaces from implementations.
- Use explicit ownership rules in C and RAII in C++.
- Prefer the Rule of Zero for ordinary C++ classes.
- Keep generated files and build artifacts out of version control.
- Do not submit code that cannot be explained by the student or team.

Course examples use C17 and C++17. Exact supported compiler versions, operating
systems, and submission tools remain to be confirmed; see [SETUP.md](SETUP.md)
for the portable baseline workflow and [STYLE_GUIDE.md](STYLE_GUIDE.md) for the
course's Google-derived C and C++ conventions.

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
The week-by-week relationship among lectures, labs, projects, exams, and AI-use
evidence is recorded in [PROJECT_ALIGNMENT.md](PROJECT_ALIGNMENT.md). Reusable
submission forms for AI use, architecture/ownership mapping, verification, and
demos are in [`project_templates/`](project_templates/).

## Repository Layout

Each week contains a `README.md` overview and a `labs/` directory. Regular
lecture weeks additionally contain:

- `lectures/` — lecture note and examples
- `lectures/lecture_exercises/` — in-lecture exercises and starter code

No-lecture weeks may retain empty `lectures/` directories to keep navigation
predictable, but they do not contain a lecture note.

Each project-related lab contains an AI-free readiness exercise, a guided
project milestone, verification requirements, and a concrete submission
artifact. Exam weeks use asynchronous checkpoints instead of pretending that a
Thursday lab is available.

The 13 lecture notes are placed in Weeks 1–9 and 11–14. Weeks 10, 15, and 16 are
reserved for the midterm project demo, integration/review, and final project
demo.

## Items to Finalize

- Project team sizes and individual-grade adjustment policy
- Late submission and attendance policies
- Official compiler versions
- Supported operating systems and development environment
- Judge, learning-management, and project-submission systems
- Permitted local documentation during exams
- Final project demo date and format
