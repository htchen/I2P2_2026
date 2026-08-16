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

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | How does typed C become an executable? | Compile, deliberately break, and repair a small program |
| 2 | How are Python-familiar values represented and formatted? | Type/conversion worksheet and robust input fragment |
| 3 | How do we translate control flow without inheriting C-specific bugs? | Complete and test a judge-style classification program |

Each hour is planned as about 45–50 minutes of explanation/live coding followed
by a 10–15 minute trace, pair exercise, or judge check. Short breaks can be
placed between the hours without changing the sequence.

## Hour 1 — Program translation and the C execution model

### 1. Same algorithms, different machine model

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

### 2. The translation pipeline

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

### Hour 1 live build: classify the diagnostic

Start from the first program below and introduce one defect at a time:

```c
#include <stdio.h>

int twice(int value);

int main(void)
{
    printf("%d\n", twice(21));
    return 0;
}

int twice(int value)
{
    return value * 2;
}
```

1. Remove the semicolon after `return value * 2` — compile-time syntax error.
2. Keep the prototype but remove the definition — link-time undefined reference.
3. Change the format to `%s` — compile warning and undefined run-time behavior.
4. Run `cc -E` and locate the original source among preprocessed declarations.
5. Run `cc -c`, inspect the object filename, and link it in a separate command.

Students should record the stage, diagnostic evidence, and smallest repair. The
goal is not to memorize messages but to locate responsibility in the pipeline.

### 3. First program

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

## Hour 2 — Types, representation, conversion, and formatted I/O

### 4. Types and expressions

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

### Integer ranges and signed/unsigned interactions

The legacy notes introduced `sizeof`; connect it to the limits headers rather
than assuming a fixed machine:

```c
#include <limits.h>
#include <stdio.h>

printf("int: %zu bytes, range %d through %d\n",
       sizeof(int), INT_MIN, INT_MAX);
printf("unsigned int maximum: %u\n", UINT_MAX);
```

Unsigned arithmetic wraps modulo one more than the maximum value. Signed
overflow is undefined behavior. Mixing signed and unsigned values can convert a
negative number to a very large unsigned value:

```c
int index = -1;
size_t count = 10;
/* index < count may be false after conversion of index to size_t. */
```

Do not “fix” every warning with a cast. First decide which domain the program
means. Loop indices for array sizes commonly use `size_t`; values that must
represent `-1` need a signed type or a different absence representation.

### 5. Formatted I/O

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

### Format-contract reference

| Value type | `printf` | `scanf` |
|------------|----------|---------|
| `int` | `%d` | `%d` with `int *` |
| `unsigned int` | `%u` | `%u` with `unsigned int *` |
| `long` | `%ld` | `%ld` with `long *` |
| `double` | `%f` | `%lf` with `double *` |
| character | `%c` | `%c` with `char *` |
| string | `%s` | bounded `%Ns` with a sufficiently large array |
| pointer value | `%p` with `(void *)p` | not normally read with `scanf` |

For `printf`, a `float` argument is promoted to `double`, so `%f` is used. For
`scanf`, `%f` expects `float *` and `%lf` expects `double *`. This asymmetry is a
common source of memory corruption.

### Hour 2 checkpoint

Predict the type and value before compiling:

```c
int a = 7;
int b = 2;
double x = a / b;
double y = (double)a / b;
unsigned int z = 0U - 1U;
```

Then compile a program that prints the results and the relevant limits. Explain
every warning instead of stopping after the numerical answer.

## Hour 3 — Selection, iteration, EOF, and judge-style translation

### 6. Selection and iteration

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

### Input-driven loops and EOF

Judge data often contains an unknown number of records. In Python you might
iterate over standard-input lines. In C, the conversion count controls the loop:

```c
int value;
long long total = 0;
size_t count = 0;

while (scanf("%d", &value) == 1) {
    total += value;
    ++count;
}

if (!feof(stdin)) {
    fprintf(stderr, "invalid token after %zu integers\n", count);
    return 1;
}
printf("count=%zu total=%lld\n", count, total);
```

`scanf` can return `EOF` or a smaller conversion count. Never write
`while (!feof(stdin))`: EOF is observed only after a read attempt fails, so that
pattern commonly processes stale data once.

### Hour 3 guided translation

Translate this Python behavior without changing its input/output contract:

```python
values = [int(token) for token in input().split()]
answer = sum(value * value for value in values if value > 0)
print(answer)
```

Use a fixed maximum of 100 inputs for this week, reject a 101st value, and check
every input conversion. Test:

- an empty line/end-of-file;
- all negative values;
- zero mixed with positives;
- exactly 100 values;
- a noninteger token;
- values whose square or total could overflow `int`.

The final discussion should distinguish translation of the algorithm from the
new representation and range decisions demanded by C.

### 7. Undefined behavior is not an exception

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
