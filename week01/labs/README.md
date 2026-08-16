# Week 1 Lab — Compiler and Python-to-C Translation

## Outcomes

Students can compile and run a small C program, interpret diagnostics, and
translate Python-familiar control flow while accounting for static types and
undefined behavior.

## Part A — Environment and AI-free readiness

Record the compiler and version. Compile a supplied program with the course
standard, warnings, debug information, and sanitizers. Fix one syntax error and
one warning by reading the diagnostic before using AI.

## Part B — Contrastive translation

Translate a small Python loop/condition program into C. Before running, predict
the result of integer division, conversions, and one boundary input. Use
formatted input only with checked conversion counts.

## Part C — Verification

Test normal, zero, negative, and invalid-input cases. Run the valid cases under
sanitizers and record exact build/test commands for reuse in later labs.

## Part D — AI-assisted review

Ask AI to review the C translation for one semantic difference from Python and
one missing test. Verify both claims by compiling or consulting the published
language/tool contract; record any unsupported claim.

## Deliverable

- source and reproducible command record;
- prediction/observation table;
- warning-clean and sanitizer-clean result;
- short AI review note.
