# Repository Guidelines

## Project Structure & Module Organization

This is a course-material repository rather than one production program.
`README.md` is the syllabus; `COURSE_PROPOSAL.md`, `LECTURE_SOURCE_MAP.md`, and
`PROJECT_ALIGNMENT.md` record curriculum decisions and provenance. Each
`weekXX/` directory contains a weekly overview and lab guide. Regular lecture
weeks additionally contain one three-hour lecture note, a standalone example,
an exercise guide, and starter code under `lectures/lecture_exercises/`.

Keep instructor references under `assets/references/` and reusable student forms
under `project_templates/`. Do not commit generated binaries, object files, or
build directories.

## Build, Test, and Development Commands

Use C17 for Weeks 1–7 examples and C++17 for Weeks 8–14 examples. Validate them
from the repository root:

```sh
for source in week0[1-7]/lectures/examples.c; do
    cc -std=c17 -Wall -Wextra -Wpedantic "$source" -o /tmp/i2p2-example
done

for source in week0[89]/lectures/examples.cpp week1[1-4]/lectures/examples.cpp; do
    c++ -std=c++17 -Wall -Wextra -Wpedantic "$source" -o /tmp/i2p2-example
done
```

When a project uses CMake, retain an out-of-source workflow:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

The first command configures the project, the second compiles it, and the third
runs registered tests with failure details. See `SETUP.md` for the portable
student workflow.

## Coding Style & Naming Conventions

Follow `STYLE_GUIDE.md` and the repository `.clang-format` file. Use two spaces
for C/C++ indentation and no C/C++ tabs. C functions use `snake_case`; C++ ordinary
functions use `MixedCase`; property accessors may follow the property spelling.
Types use `PascalCase`. C macros use `UPPER_SNAKE_CASE`, while C++ named
compile-time or namespace/class-scope constants use `kPascalCase`. Keep headers
self-contained and preserve strict C17 syntax in Weeks 1–7 and C++17 syntax in
Weeks 8–14.

Use descriptive Markdown headings, CommonMark-compatible lists, and relative
links within the repository. Refer to notes by week rather than ordinal lecture
number. Follow modern C++ ownership practices: prefer RAII, standard-library
types, and the Rule of Zero.

## Testing Guidelines

Compile every changed example with warnings enabled and run it. Syntax-check
starter files even when their TODOs intentionally leave behavior incomplete.
Check internal Markdown links, calendar dates, assessment scopes, and grading
totals after schedule changes. Tests and examples must be deterministic and
independent of machine-specific paths or network access.

## Commit & Pull Request Guidelines

Use concise, imperative subjects such as `Align midterm schedule` or
`Add Week 6 tree exercises`, and keep unrelated changes separate. Pull requests should
summarize curriculum or material changes and list verification commands. Note
whether a change affects published dates, assessment scope, or grading.
