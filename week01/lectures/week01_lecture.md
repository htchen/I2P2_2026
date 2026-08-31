# Week 1 Lecture Notes — From Python to C

> September 8, 2026 · C17 · Source lineage: previous C introduction,
> formatted-I/O, operators, and looping notes plus the instructor-provided
> *From C to Assembly* handout

> Python bridge: [Python Contrast Companion for Week 1](week01_python_companion.md)

## Learning objectives

By the end of this lecture, you should be able to:

1. Describe preprocessing, compilation, assembly, linking, and execution, and
   inspect the generated assembly as evidence of translation.
2. Translate a small Python program into typed C.
3. Use formatted input/output and C control flow safely.
4. Distinguish a compile-time error, a link-time error, and a run-time fault.
5. Compile with warnings and treat diagnostics as useful evidence.

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | How does typed C become an executable? | Compile, inspect, deliberately break, and repair a small program |
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

Conceptually, the build performs four translation stages before execution:

1. **Preprocess:** expand directives such as `#include` and `#define`.
2. **Compile:** check C and translate it to target assembly.
3. **Assemble:** encode assembly instructions and data into an object file.
4. **Link:** combine object files and libraries into one executable.

At run time, the operating system loader maps the executable and required
libraries into memory, establishes the process environment, and transfers
control through the language implementation to `main`. A compiler driver such
as `cc` normally runs several of these tools for us, but we can stop after each
stage:

```sh
cc -std=c17 -E hello.c -o hello.i  # preprocessed C
cc -std=c17 -O0 -S hello.c -o hello.s
cc -std=c17 -c hello.c -o hello.o
cc hello.o -o hello
```

`-Wall -Wextra -Wpedantic` requests useful warnings. `-g` records information
for a debugger. A program that compiles with a warning is not necessarily safe.

### Hour 1 live build: classify the diagnostic

Start from the first program below and introduce one defect at a time:

The line `int twice(int value);` is a **declaration**: it tells the compiler the
function's name, parameter type, and result type before the call is compiled.
The later braced block is the **definition** that supplies the work. This
minimal distinction is enough to observe compilation and linking today; Week 2
develops function contracts, parameter passing, and decomposition in detail.

```c
#include <stdio.h>

int twice(int value);

int main(void) {
  printf("%d\n", twice(21));
  return 0;
}

int twice(int value) {
  return value * 2;
}
```

1. Remove the semicolon after `return value * 2` — compile-time syntax error.
2. Keep the prototype but remove the definition — link-time undefined reference.
3. Change the format to `%s` — compile warning and undefined run-time behavior.
4. Run `cc -E` and locate the original source among preprocessed declarations.
5. Run `cc -S`, find the code for `twice`, and then compare it with an `-O2`
   build without expecting a line-for-line correspondence.
6. Run `cc -c`, inspect the object filename, and link it in a separate command.

Students should record the stage, diagnostic evidence, and smallest repair. The
goal is not to memorize messages but to locate responsibility in the pipeline.

### Assembly is an observation window

Generated assembly exposes the compiler's choices, not a portable translation
recipe. Instruction names, register names, symbol spelling, calling conventions,
and section names depend on the target architecture, object format, compiler,
options, and optimization level. On an x86 target, `-masm=intel` may request
Intel syntax; it is not meaningful for every target.

Common object-file regions make C storage duration visible:

| Common section | Typical contents |
|----------------|------------------|
| `.text` | executable machine instructions |
| `.rodata` | read-only constants, including some string literals |
| `.data` | writable static-storage objects with nonzero initial data |
| `.bss` | zero-initialized static-storage objects represented compactly |

These names are common in ELF-based systems, not promises made by C. An
uninitialized or explicitly zero-initialized object with static storage duration
starts as zero even when the executable does not store every zero byte. An
automatic local variable has different duration and is not initialized merely
because a platform happens to obtain stack memory from the operating system.

Compile this file with both `-O0 -S` and `-O2 -S`:

```c
static int zero_count;
static int initial_count = 7;

int add_one(int value) {
  int result = value + 1;
  return result;
}
```

Locate evidence for the two static objects and for the calculation. At `-O2`,
`result` may have no memory location at all. Record semantic observations such
as “the returned value is one greater,” not fragile claims about an exact
register or instruction sequence.

### 3. First program

```c
#include <stdio.h>

int main(void) {
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
double wrong = 5 / 2;         /* 2.0: division happened as int */
double right = (double)5 / 2; /* 2.5 */
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

Connect `sizeof` to the limits headers rather than assuming a fixed machine:

```c
#include <limits.h>
#include <stdio.h>

printf("int: %zu bytes, range %d through %d\n", sizeof(int), INT_MIN, INT_MAX);
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
explain addresses in the Week 4 lecture notes. Until then, treat the format string and each
corresponding argument as a checked pair.

### Format-contract reference

| Value type | `printf` | `scanf` |
|------------|----------|---------|
| `int` | `%d` | `%d` with `&integer_variable` |
| `unsigned int` | `%u` | `%u` with `&unsigned_variable` |
| `long` | `%ld` | `%ld` with `&long_variable` |
| `double` | `%f` | `%lf` with `&double_variable` |
| character | `%c` | `%c` with `&character_variable` |

For `printf`, a `float` argument is promoted to `double`, so `%f` is used. For
`scanf`, `%f` requires the address of a `float`, while `%lf` requires the address
of a `double`. This asymmetry is a common source of memory corruption. String
and pointer formatting are introduced only after Week 2 establishes array
representation and Week 4 establishes the pointer model.

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

Process each integer as it is read, without storing an array. Accept at most 100
inputs, reject a 101st value, and check every input conversion. Test:

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

int main(void) {
  int value;
  if (scanf("%d", &value) != 1) {
    return 1;
  }

  printf("%d is ", value);
  if (value < 0) {
    printf("negative");
  } else if (value > 0) {
    printf("positive");
  } else {
    printf("zero");
  }

  if (value % 2 == 0) {
    printf(" and even\n");
  } else {
    printf(" and odd\n");
  }
  return 0;
}
```

Trace the condition selected by each input. Why is taking `value % 2` defined
when `value` is negative? What special output does zero receive? This version
uses only integer values and control flow. Week 2 introduces character arrays,
and Week 4 explains pointer-valued references to strings.

## Check yourself

1. Where does an “undefined reference” diagnostic occur in the pipeline?
2. What distinct artifacts do `-E`, `-S`, and `-c` produce?
3. What are the values of `7 / 3` and `(double)7 / 3`?
4. Why must the argument for `%d` have the expected integer type?
5. Translate a Python `while` loop that repeatedly reads until `0` into C.
6. Why can the assembly produced at `-O2` omit a named local variable?

## Summary

- Your programming knowledge transfers; C exposes types, storage, and failures.
- A C program is preprocessed, compiled, assembled, linked, and then executed.
- Generated assembly is target- and option-dependent evidence, not the C
  language definition.
- Declarations, format strings, and conversions are contracts.
- Warnings, exit status, and tests are part of normal development.
- Avoiding undefined behavior is a correctness requirement.

## References and source materials

- [Instructor handout: *From C to Assembly*](../../assets/references/from_c_to_assembly.pdf)
- [Introduction to programming](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/Introduction%20to%20programming/README.md>)
- [Operators, expressions, and statements](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/Operators%2C%20Expressions%2C%20and%20Statements/README.md>)
- [Looping](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/Looping/README.md>)
- [`printf` and `scanf` summary](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/Printf%20and%20Scanf/總整理.md>)
