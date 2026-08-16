# Lecture 1 — From Python to C

> September 8, 2026 · C17 · Source lineage: the legacy C introduction,
> formatted-I/O, operators, and looping notes

## Learning objectives

By the end of this lecture, you should be able to:

1. Describe preprocessing, compilation, linking, and execution.
2. Translate a small Python program into typed C.
3. Use formatted input/output and C control flow safely.
4. Distinguish a compile-time error, a link-time error, and a run-time fault.
5. Compile with warnings and treat diagnostics as useful evidence.

## 1. Same algorithms, different machine model

You already know sequencing, selection, iteration, functions, and values. C asks
you to make more of the representation explicit.

| Python | C |
|--------|---|
| A name is bound to an object | A variable has a declared type and storage |
| Integers grow as needed | Integer types have fixed ranges |
| Lists resize dynamically | Arrays normally have fixed size |
| Exceptions report many errors | Some mistakes produce undefined behavior |
| The interpreter executes a program | A compiler and linker build an executable |

The important question changes from only “what value does this expression
produce?” to “what value, of what type, stored where, for how long?”

## 2. The translation pipeline

For a source file named `hello.c`:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic -g hello.c -o hello
./hello
```

Conceptually, the toolchain performs four stages:

1. **Preprocess:** expand directives such as `#include` and `#define`.
2. **Compile:** check C and translate it to assembly/object code.
3. **Link:** combine object files and libraries into one executable.
4. **Run:** the operating system loads the executable and calls `main`.

`-Wall -Wextra -Wpedantic` requests useful warnings. `-g` records information
for a debugger. A program that compiles with a warning is not necessarily safe.

## 3. First program

```c
#include <stdio.h>

int main(void)
{
    int courses_completed = 1;
    printf("Programming courses completed: %d\n", courses_completed);
    return 0;
}
```

- `#include <stdio.h>` makes the declarations of standard I/O functions known.
- `main` is the program entry point.
- Braces delimit a block; semicolons terminate statements.
- `int courses_completed` declares storage and its interpretation.
- Returning zero conventionally reports success to the operating system.

## 4. Types and expressions

Common types for the first week are:

```c
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

char grade = 'A';
int count = 42;
double average = 87.5;
bool passed = true;
size_t length = 10;
int32_t exact_width = 1000;
```

Use `sizeof value` to ask how many bytes an object occupies. Except for `char`,
the exact size of basic types can depend on the implementation. Use the types
from `<stdint.h>` when an exact width is part of the data format.

### Integer division and conversion

```c
double wrong = 5 / 2;          /* 2.0: division happened as int */
double right = (double)5 / 2;  /* 2.5 */
```

Conversions in C can discard information. Compile with warnings and make a
conversion explicit when it is intentional.

### Truth values

In a condition, zero is false and any nonzero scalar value is true. Relational
and logical operators produce `0` or `1`.

```c
bool eligible = age >= 18 && has_id;
```

Do not confuse assignment (`=`) with comparison (`==`).

## 5. Formatted I/O

`printf` format specifiers must agree with the argument types.

```c
int score = 95;
double ratio = 0.875;
printf("score=%d ratio=%.2f\n", score, ratio);
```

For simple judge input, check the result of `scanf`:

```c
int a;
int b;
if (scanf("%d %d", &a, &b) != 2) {
    fprintf(stderr, "expected two integers\n");
    return 1;
}
printf("%d\n", a + b);
```

`scanf` needs the **addresses** of `a` and `b` so it can modify them. We will
explain addresses in Lecture 4. Until then, treat the format string and each
corresponding argument as a checked pair.

## 6. Selection and iteration

Python indentation becomes explicit braces:

```python
total = 0
for value in range(1, limit + 1):
    if value % 2 == 0:
        total += value
```

```c
int total = 0;
for (int value = 1; value <= limit; ++value) {
    if (value % 2 == 0) {
        total += value;
    }
}
```

C also provides `while`, `do ... while`, and `switch`. Prefer braces even for a
one-statement body; they prevent mistakes during later edits.

```c
switch (command) {
case 'q':
    running = false;
    break;
case 'h':
    print_help();
    break;
default:
    fprintf(stderr, "unknown command\n");
    break;
}
```

Without `break`, execution continues into the next `case`. Use fallthrough only
when it is deliberate and documented.

## 7. Undefined behavior is not an exception

Python normally stops and reports errors such as an out-of-range list access.
The C standard instead leaves some invalid operations without defined meaning.
Examples include:

- reading an uninitialized automatic variable;
- signed integer overflow;
- dividing an integer by zero;
- accessing outside an array;
- using a mismatched `printf` format.

The compiler is allowed to assume undefined behavior never occurs. “It worked
once” is therefore not evidence that the program is correct.

## Worked example: classify an integer

```c
#include <stdio.h>

int main(void)
{
    int value;
    if (scanf("%d", &value) != 1) {
        return 1;
    }

    const char *sign = value < 0 ? "negative" : value > 0 ? "positive" : "zero";
    const char *parity = value % 2 == 0 ? "even" : "odd";
    printf("%d is %s and %s\n", value, sign, parity);
    return 0;
}
```

Trace the types and values of every expression. Why is taking `value % 2` safe
even when `value` is negative? What special output does zero receive?

## Check yourself

1. Where does an “undefined reference” diagnostic occur in the pipeline?
2. What are the values of `7 / 3` and `(double)7 / 3`?
3. Why must the argument for `%d` have the expected integer type?
4. Translate a Python `while` loop that repeatedly reads until `0` into C.
5. Compile one example with a deliberate `=`/`==` mistake and inspect warnings.

## Summary

- Your programming knowledge transfers; C exposes types, storage, and failures.
- A C program is preprocessed, compiled, linked, and then executed.
- Declarations, format strings, and conversions are contracts.
- Warnings, exit status, and tests are part of normal development.
- Avoiding undefined behavior is a correctness requirement.
