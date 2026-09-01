# Week 1 Lecture Exercises: From Python to C

[Starter code](week01_starter.c)

## Hour 1 — Build-pipeline prediction

Put preprocessing, compilation, assembly, linking, and execution in order. For
each stage, name its input and output. Then run the relevant `cc` commands from
the lecture and compare the prediction with the generated files.

## Hour 2 — Representation and conversion trace

Complete the table for `char`, `int`, `double`, and `bool`: example value,
`sizeof`, matching `printf` conversion, and one conversion that may lose
information. Predict the output of the starter before compiling it.

## Hour 3 — Python-to-C translation

Complete the input loop that accumulates positive even integers until input
ends. State the loop invariant, and test empty input, negative values, zero, and
several valid values. Explain why the program updates the total only after both
conditions hold. The starter uses a `long long` accumulator; before adding a
positive value, check that the addition cannot exceed `LLONG_MAX`. Also
distinguish normal end-of-file from a non-integer input token. Arrays and
reusable array functions begin in Week 2, so this exercise deliberately uses
only the selection and iteration model introduced this week. Compile with all
course warnings enabled.
