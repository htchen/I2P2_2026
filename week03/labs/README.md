# Week 3 Lab — Midterm Scaffold Build and Code Map

## Outcomes

By the end of this lab, students can build the supplied C project, reproduce a
baseline run, locate each compiler stage, and state the contracts of incomplete
functions without implementing them.

## Part A — AI-free readiness

Given a two-file C program and header, fix one declaration/definition mismatch
and one link error. Record whether each failure belongs to preprocessing,
compilation, or linking.

## Part B — Reproduce the baseline

1. Obtain the published midterm scaffold and record its revision.
2. Build with the course C standard and warning flags.
3. Run every public testcase without modifying source.
4. Record exact commands, compiler version, expected behavior, and observed
   behavior. A failing/incomplete baseline is acceptable when documented.

## Part C — Pipeline and TODO map

Trace one expression through:

```text
input → tokens/list → parser → AST → semantic check → instructions → cleanup
```

For every stage, record its input, output, failure signal, allocation behavior,
and caller. For every TODO, write a precondition and postcondition. Do not write
the implementation in this milestone.

## Part D — AI-assisted code reading

Ask an AI tool to explain one function using only the relevant declaration,
definition, and call site. Before asking, predict the function's contract.
Verify the response against an executed trace and record one unsupported or
incorrect assumption.

## Deliverable

- reproducible build record;
- one-page pipeline/TODO map;
- baseline public-test table;
- first entry in `AI_USAGE.md`.

Use the templates in [`project_templates/`](../../project_templates/).
