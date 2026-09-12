# Week 1 Lecture Exercises: From Python to C

[Hour 3 starter code](week01_starter.c)

## Hour 1 — Build-pipeline prediction

Put preprocessing, compilation, assembly, linking, and execution in order. For
each stage, name its input and output. Then run the relevant `cc` commands from
the lecture and compare the prediction with the generated files.

## Hour 2 — Representation and conversion trace

Complete the table for `char`, `int`, `double`, and `bool`: example value,
`sizeof`, matching `printf` conversion, and one conversion that may lose
information.

## Hour 3 — Python-to-C translation

Complete the input loop that accumulates positive even integers until input
ends. Before editing, predict the output of the incomplete starter and explain
why it does not yet consume the supplied input. Then state the completed loop
invariant, and test empty input, negative values, zero, and several valid
values. Explain why the program updates the total only after both conditions
hold.
Assume that every numeric token is representable as `int`. The starter uses a
`long long` accumulator; before adding a positive value, check that the addition
cannot exceed `LLONG_MAX`, the greatest `long long` value supplied by
`<limits.h>`. Also distinguish normal end-of-file from a non-integer input
token. Arrays and reusable array functions begin in Week 2, so this exercise
deliberately uses only the selection and iteration model introduced this week.
Compile with all course warnings enabled.

The incomplete starter contains `(void)value;`. This explicit conversion to
`void` discards the value and tells the compiler that it is intentionally
unused while the TODO remains incomplete. Remove that statement once your loop
uses `value`.

## Compile and verify

From the repository root:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  week01/lectures/lecture_exercises/week01_starter.c -o /tmp/week01_starter
printf '2 -3 4 5\n' | /tmp/week01_starter
```

After the TODO is correct, the sample output is `6`. Also run the empty-input,
zero, and malformed-token cases named above. Because the input elements are
`int` and the accumulator begins at zero, reaching `LLONG_MAX` would require an
impractically large input on ordinary systems. Review the overflow condition
algebraically instead of attempting to generate billions of values. Compare
with [`../examples.c`](../examples.c) only after writing and testing the loop.
