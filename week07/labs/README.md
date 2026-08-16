# Week 7 Lab — Midterm Integration Studio

## Outcomes

Students can integrate parser, semantic, and instruction-generation changes,
verify each stage independently, and prepare evidence for the demonstration.

## Part A — AI-free readiness

Given a grammar and short token sequence, draw the AST and identify precedence,
associativity, and any required lvalue check. This exercise is structurally
similar to, but not copied from, a graded hidden case.

## Part B — Stage gates

Do not debug the complete pipeline as one black box. Pass these gates in order:

1. lexer/token tests from Week 5;
2. AST-shape traces for every precedence level;
3. valid and invalid semantic cases, especially increment/decrement operands;
4. instruction traces for constants, variables, assignment, unary operations,
   and nested binary operations;
5. ASMC execution and comparison with an independent expected result;
6. sanitizer-clean success and rejection paths.

The project specification, not the lecture's teaching grammar or stack-machine
example, is authoritative.

## Part C — AI-assisted review

Use AI for one of: generate counterexamples, review a trace, explain a failing
test, or inspect an ownership path. Require the response to cite the relevant
grammar production or function contract. Verify it and add any valid
counterexample to the regression suite.

## Part D — Demo rehearsal

Each student must be able to:

- trace one input through all stages;
- explain one ownership path and one semantic rule;
- interpret a failed testcase or sanitizer report;
- make a small change without AI.

## Deliverable

- completed stage-gate checklist;
- student-designed regression suite;
- one AST and instruction trace;
- memory-safety evidence;
- completed `AI_USAGE.md` with an AI audit;
- known-limitations list.

Instruction-cycle optimization is optional until every required correctness and
memory-safety gate passes.
