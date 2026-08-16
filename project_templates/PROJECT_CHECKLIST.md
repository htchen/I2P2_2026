# Project Milestone Checklist

Use this checklist for each graded milestone and the final release.

## Scope and design

- [ ] The requested behavior and non-goals are stated.
- [ ] Affected files and interfaces are identified before implementation.
- [ ] Ownership, borrowing, lifetime, and failure behavior are documented.
- [ ] The change is small enough to test and review independently.

## Build and verification

- [ ] The project builds from a clean checkout using documented commands.
- [ ] Warnings are enabled and relevant warnings are resolved.
- [ ] Normal, boundary, invalid, and regression cases are tested.
- [ ] Memory-sensitive paths are checked with sanitizers or an approved tool.
- [ ] Non-graphical logic has automated tests where practical.
- [ ] Graphical or tool-integrated behavior has a repeatable manual test plan.

## AI accountability

- [ ] Predictions were recorded before significant AI assistance.
- [ ] AI-produced claims and code were verified rather than trusted directly.
- [ ] `AI_USAGE.md` records accepted and rejected suggestions.
- [ ] The required AI audit contains a concrete counterexample or diagnostic.
- [ ] Every author can explain the code and make a small AI-free modification.

## Submission

- [ ] Required filenames and directory layout are correct.
- [ ] Generated files, binaries, and private credentials are excluded.
- [ ] Tests, ownership/architecture map, and known limitations are included.
- [ ] The submitted revision or commit is recorded.
