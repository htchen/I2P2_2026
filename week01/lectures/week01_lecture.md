# Week 1 Lecture Notes — From Python to C

> September 8, 2026 · C17 · Source lineage: previous C introduction,
> formatted-I/O, operators, and looping notes plus the instructor-provided
> *From C to Assembly* handout

> Python bridge: [Python Contrast Companion for Week 1](week01_python_companion.md)

## Student route

- **Core:** follow one program from source to executable, classify compile/link/
  run-time failures, then write typed expressions, formatted I/O, branches, and
  loops in C.
- **Practice:** complete the [Week 1 exercise](lecture_exercises/week01_ex.md)
  before comparing with [the complete example](examples.c).
- **First-reading limit:** in the translation section, remember the pipeline and
  diagnostic categories. Assembly sections are evidence for that model, not a
  requirement to memorize instructions.
- **Python bridge:** consult the companion only when a C behavior is hard to
  connect to prior Python knowledge.

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

Each hour interleaves about 35–40 minutes of explanation/live coding with about
20–25 minutes of prediction, editing, testing, and discussion. Short breaks can
be placed between the hours without changing the sequence.

### Inline practice routine

Each **Try it now** stop is a one-to-four-minute exercise unless a longer time
is stated. Work in a small scratch source file and follow the same cycle:

1. predict the result or diagnostic before running a command;
2. make the requested change yourself;
3. compile with `-std=c17 -Wall -Wextra -Wpedantic`;
4. run at least the stated tests; and
5. explain the evidence to a partner in one sentence.

The exercises are deliberately small. Their purpose is immediate retrieval and
feedback, not copying a complete solution from the note. Only the question is
visible initially; expand **Reveal solution** after making and testing your own
attempt.

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

> **Try it now — name the artifact (2 minutes):** without running the commands,
> write the expected output filename after each line. Then run them later with
> the complete `hello.c` program below and correct your predictions. Which
> command produces something that can be executed directly?

<details>
<summary>Reveal solution</summary>

| Command stops after | Output | Directly executable? |
|---------------------|--------|----------------------|
| Preprocessing | `hello.i` | No |
| Compilation to assembly | `hello.s` | No |
| Assembly to object code | `hello.o` | No |
| Linking | `hello` | Yes |

The compiler driver command that performs the complete build also produces the
executable named by `-o`.

</details>

`-Wall -Wextra -Wpedantic` requests useful warnings. `-g` records information
for a debugger. A program that compiles with a warning is not necessarily safe.

> **Minimum takeaway for the first reading:** source code is checked and
> translated before it runs; the linker combines separately translated pieces;
> and failures at compilation, linking, and execution are different evidence.
> You do not need to memorize file suffixes, loader details, or assembly
> instructions yet. Use the commands above to observe the stages and return to
> their lower-level details after writing the first C program.

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

#### Try it now — locate the failure stage (12 minutes)

1. Remove the semicolon after `return value * 2`. Which stage rejects the
   program first?
2. Keep the prototype but remove the definition. Which stage now fails?
3. Change the format to `%s`. What does the warning say, and why should you not
   run the resulting program?
4. Run `cc -E` and locate the original source among preprocessed declarations.
5. Run `cc -S`, find the code for `twice`, and then compare it with an `-O2`
   build without expecting a line-for-line correspondence.
6. Run `cc -c`, inspect the object filename, and link it in a separate command.

<details>
<summary>Reveal solution</summary>

1. The missing semicolon makes the C translation unit syntactically invalid, so
   compilation fails before an object file is produced.
2. The call agrees with the visible declaration, so compilation can succeed.
   Linking fails because no linked object supplies the definition of `twice`.
3. `%s` requires a pointer to a null-terminated character sequence, but
   `twice(21)` supplies an `int`. A warning-enabled compiler diagnoses the
   mismatch. Running it would give `printf` an invalid argument type and invoke
   undefined behavior.
4. `hello.i` contains the included declarations plus recognizable lines from
   the original file.
5. An unoptimized build normally contains code corresponding to `twice`; an
   optimized build may simplify or inline the call while preserving the result.
6. `cc -c hello.c -o hello.o` produces `hello.o`, and
   `cc hello.o -o hello` produces the executable.

</details>

Students should record the stage, diagnostic evidence, and smallest repair. The
goal is not to memorize messages but to locate responsibility in the pipeline.

### Assembly is an observation window

> **Supporting observation:** use generated assembly as evidence that C is
> translated, but do not memorize instruction names, executable sections, or
> machine-specific encodings. The translation stages and diagnostic categories
> are the required ideas.

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

#### Try it now — observe without memorizing (4 minutes)

Locate evidence for the two static objects and for the calculation in both
assembly files. Which observations remain true even when instruction sequences,
registers, and labels differ?

<details>
<summary>Reveal solution</summary>

`zero_count` has static storage duration and an initial value of zero, so it is
commonly represented in a zero-initialized region such as `.bss`.
`initial_count` is writable and begins as seven, so it is commonly represented
in `.data`. The function's portable meaning is that it returns a value one
greater than its argument. At `-O2`, the named local `result` may have no memory
location, and a caller may contain the calculation after `add_one` is inlined.
Those optimization choices do not change the required result.

</details>

### 3. First program

```c
#include <stdio.h>

int main(void) {
  int courses_completed = 1;
  printf("Programming courses completed: %d\n", courses_completed);
  return 0;
}
```

#### Try it now — edit, compile, run (3 minutes)

Change `courses_completed` to match your own experience and change the printed
label without changing `%d`. Compile and run the program. Then remove one
semicolon, predict which translation stage rejects the file, and restore it.

<details>
<summary>Reveal solution</summary>

One possible edit is:

```c
int courses_completed = 2;
printf("Previous programming courses: %d\n", courses_completed);
```

Your number and wording may differ. `%d` remains correct because the matching
argument is still an `int`. Removing a required semicolon makes compilation
fail; restoring it makes the translation unit syntactically valid again.

</details>

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

#### Try it now — choose a representation (2 minutes)

Use the earlier first program as a scaffold. Add variables for a whole-number
student count, a fractional temperature, and a letter grade. Choose the type
before the initial value, then print them with `%d`, `%.1f`, and `%c`,
respectively. These format spellings are a preview of the formatted-I/O section
below. Compile and run; do not copy demonstration variables that your program
does not use.

<details>
<summary>Reveal solution</summary>

One possible program is:

```c
#include <stdio.h>

int main(void) {
  int student_count = 40;
  double temperature = 26.5;
  char letter_grade = 'A';
  printf("students=%d temperature=%.1f grade=%c\n", student_count,
         temperature, letter_grade);
  return 0;
}
```

The exact values may differ. The types express the important promises: a whole
number, a fractional numeric value, and one character.

</details>

Use `sizeof value` to ask how many bytes an object occupies. Except for `char`,
the exact size of basic types can depend on the implementation. Use the types
from `<stdint.h>` when an exact width is part of the data format.

### Basic operators and precedence

The underlying operations are familiar from Python, but several spellings and
type rules differ. Start with these groups:

| Purpose | C operators | Important rule |
|---------|-------------|----------------|
| Arithmetic | `+`, `-`, `*`, `/`, `%` | `/` uses the operand types; `%` requires integer operands |
| Comparison | `<`, `<=`, `>`, `>=`, `==`, `!=` | the result is `0` or `1` |
| Logic | `&&`, `||`, `!` | `&&` and `||` short-circuit from left to right |
| Assignment | `=`, `+=`, `-=`, `*=`, `/=`, `%=` | a compound assignment reads, computes, and stores |
| Change by one | `++`, `--` | these modify an object; initially use them as separate statements |

Multiplication, division, and remainder bind more tightly than addition and
subtraction. Comparison happens after arithmetic, `&&` after comparison, and
`||` after `&&`. Parentheses are preferable whenever the intended grouping is
not immediately obvious:

```c
int quotient = 7 / 3;          /* 2: both operands are int */
int remainder = 7 % 3;         /* 1 */
int precedence = 2 + 3 * 4;    /* 14 */
int grouped = (2 + 3) * 4;     /* 20 */

int score = 10;
score += 5;                    /* score is now 15 */
++score;                       /* score is now 16 */
```

#### Try it now — predict before printing (3 minutes)

Put this fragment in `main`, then change the two divisions to `11 / 4` and
`11 % 4`; change the precedence pair to `5 + 2 * 6` and `(5 + 2) * 6`; and
initialize `score` to 7 before adding 4 and incrementing it. Print the five
final values with `%d` and predict them before running.

<details>
<summary>Reveal solution</summary>

The modified values are `quotient == 2`, `remainder == 3`,
`precedence == 17`, `grouped == 42`, and `score == 12`. In the unparenthesized
expression, multiplication happens first. Parentheses make the addition happen
first in the second expression.

</details>

Prefix and postfix `++`/`--` differ when their value is used inside a larger
expression. That distinction is rarely worth the reduced readability in an
introductory program: prefer a separate `++index;` or `--count;` statement and
do not modify the same object multiple times in one expression.

### Integer division and conversion

```c
double wrong = 5 / 2;         /* 2.0: division happened as int */
double right = (double)5 / 2; /* 2.5 */
```

#### Try it now — move the conversion (2 minutes)

Print both values with one digit after the decimal point. Then try the pairs
`-5` and `2`, and `5` and `-2`, predicting each result before running. Finally,
move the cast from the numerator to the denominator and determine whether that
changes the result.

<details>
<summary>Reveal solution</summary>

For positive operands, `wrong` prints `2.0` and `right` prints `2.5`. Integer
division in C truncates toward zero, so `-5 / 2` and `5 / -2` both produce `-2`
before conversion to `double`; placing the cast before division produces
`-2.5`. Casting either operand to `double` is sufficient, so
`5 / (double)2` also produces `2.5`.

</details>

Conversions in C can discard information. Compile with warnings and make a
conversion explicit when it is intentional.

### Truth values

In a condition, zero is false and any nonzero scalar value is true. Relational
and logical operators produce `0` or `1`.

```c
int age = 18;
bool has_id = true;
bool eligible = age >= 18 && has_id;
```

#### Try it now — test the boundary (2 minutes)

Add an `if`/`else` that prints `eligible` or `not eligible`. Run the four
combinations formed by ages 17 and 18 and ID values `false` and `true`. Identify
which part of the expression rejects each unsuccessful case.

<details>
<summary>Reveal solution</summary>

```c
if (eligible) {
  puts("eligible");
} else {
  puts("not eligible");
}
```

Only age 18 with `has_id == true` is eligible. At age 17, the left operand of
`&&` is false, so short-circuit evaluation does not need the right operand. At
age 18 without an ID, the right operand is false.

</details>

Do not confuse assignment (`=`) with comparison (`==`).

### Integer ranges and signed/unsigned interactions

Connect `sizeof` to the limits headers rather than assuming a fixed machine:

```c
#include <limits.h>
#include <stdio.h>

printf("int: %zu bytes, range %d through %d\n", sizeof(int), INT_MIN, INT_MAX);
printf("unsigned int maximum: %u\n", UINT_MAX);
```

#### Try it now — ask the implementation (2 minutes)

Place the calls inside `main`, then extend the program to print `long` size and
range using `LONG_MIN` and `LONG_MAX`. Use `%zu` for the result of `sizeof` and
`%ld` for the two `long` limits. Do not guess that `long` has the same size as
`int`; compile and let the current implementation answer.

<details>
<summary>Reveal solution</summary>

Add this inside `main` after including `<limits.h>` and `<stdio.h>`:

```c
printf("long: %zu bytes, range %ld through %ld\n", sizeof(long), LONG_MIN,
       LONG_MAX);
```

The numerical size and limits are implementation results. They must be read
from the program's output rather than assumed from another machine.

</details>

Unsigned arithmetic wraps modulo one more than the maximum value. Signed
overflow is undefined behavior. Mixing signed and unsigned values can convert a
negative number to a very large unsigned value:

```c
int index = -1;
size_t count = 10;
```

#### Try it now — expose the mixed-domain bug (3 minutes)

Turn the commented comparison into a printed result and compile with the course
warning flags. Predict the result first. Repair the comparison by choosing
types that represent the same intended domain; do not add a cast merely to
silence the warning.

<details>
<summary>Reveal solution</summary>

```c
printf("%d\n", index < count);
```

On common current implementations, the comparison prints zero because `index`
is converted to `size_t`; converting `-1` to that unsigned type produces its
maximum value, which is not less than 10. The warning is the portable evidence
that two different numeric domains are being mixed. If this small problem
genuinely uses `-1` as a sentinel and all counts fit in `int`, one coherent
repair is:

```c
int index = -1;
int count = 10;
printf("%d\n", index < count);
```

For a real container API, a separate success flag or another explicit absence
representation is often clearer than mixing a negative sentinel with an
unsigned size.

</details>

An integer-literal suffix participates in the expression's type. The suffix
`U` means “choose an unsigned integer type”; for the small literals `0U` and
`1U`, that type is `unsigned int`. Therefore both operands in `0U - 1U` are
unsigned, and the subtraction wraps to `UINT_MAX`. Related suffixes include
`L`, `LL`, and combinations such as `ULL`. Use a suffix when the required type
is part of the contract, not merely to silence a conversion warning.

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

#### Try it now — control the presentation (1 minute)

Change the precision from two digits after the decimal point to four, then add
a descriptive label before each value. Confirm that formatting changes the
output text but not the stored `ratio`.

<details>
<summary>Reveal solution</summary>

```c
printf("student score=%d success ratio=%.4f\n", score, ratio);
```

The output becomes `student score=95 success ratio=0.8750`. The additional
digits and labels affect presentation only; `ratio` remains the same `double`.

</details>

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

#### Try it now — test the input contract (3 minutes)

Place the fragment inside `main` in a program that includes `<stdio.h>`. Run it
with `10 20`, then with `10 x`, and finally with only one integer followed by
end-of-file. Record the `scanf` result for each case by temporarily storing it
in an `int conversions` variable. Restore the concise condition after you
understand the three outcomes.

<details>
<summary>Reveal solution</summary>

The diagnostic version begins as follows:

```c
int conversions = scanf("%d %d", &a, &b);
printf("conversions=%d\n", conversions);
if (conversions != 2) {
  fprintf(stderr, "expected two integers\n");
  return 1;
}
```

Input `10 20` produces two conversions and allows the sum to be printed.
`10 x` converts only the first integer, so the count is one. One integer
followed by end-of-file also produces one conversion. An immediate end-of-file
would produce `EOF`, not a successful conversion count.

</details>

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

#### Try it now — build a format checklist (2 minutes)

Declare one value of each numeric type in the table and write a single `printf`
call that prints them. Add one `scanf` call for a `double`. Exchange the code
with a partner and check every specifier against its corresponding argument
before compiling.

<details>
<summary>Reveal solution</summary>

One possible checklist program is:

```c
#include <stdio.h>

int main(void) {
  int signed_value = -3;
  unsigned int unsigned_value = 3U;
  long long_value = 3000L;
  double real_value = 3.5;
  char character = 'C';

  printf("%d %u %ld %.1f %c\n", signed_value, unsigned_value, long_value,
         real_value, character);

  double input_value;
  if (scanf("%lf", &input_value) != 1) {
    return 1;
  }
  printf("input=%.1f\n", input_value);
  return 0;
}
```

The important review is positional: each conversion specifier must match the
type of the corresponding argument.

</details>

### Try it now — Hour 2 checkpoint (5 minutes)

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

<details>
<summary>Reveal solution</summary>

`a` and `b` are `int`. Integer division therefore produces 3 before `x` stores
that value as `3.0`. The cast makes one operand of the second division `double`,
so `y` is `3.5`. Both operands of `0U - 1U` are `unsigned int`, so the result
wraps to `UINT_MAX`; `z` has type `unsigned int`.

A matching output statement is:

```c
printf("a=%d b=%d x=%.1f y=%.1f z=%u UINT_MAX=%u\n", a, b, x, y, z,
       UINT_MAX);
```

</details>

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

#### Try it now — change one rule (3 minutes)

Place the C fragment in a complete program with `limit = 10` and print the
result. Then change it to sum values divisible by three instead of values
divisible by two. Predict both totals before running the program.

<details>
<summary>Reveal solution</summary>

The original loop sums `2 + 4 + 6 + 8 + 10`, producing 30. The modified loop
can be written as:

```c
int limit = 10;
int total = 0;
for (int value = 1; value <= limit; ++value) {
  if (value % 3 == 0) {
    total += value;
  }
}
printf("%d\n", total);
```

It prints 18 because the included values are 3, 6, and 9.

</details>

C also provides `while`, `do ... while`, and `switch`. Prefer braces even for a
one-statement body; they prevent mistakes during later edits.

```c
char command = 'h';

switch (command) {
  case 'q':
    puts("quit");
    break;
  case 'h':
    puts("help");
    break;
  default:
    fprintf(stderr, "unknown command\n");
    break;
}
```

#### Try it now — make fallthrough visible (3 minutes)

Place the fragment inside `main` in a program that includes `<stdio.h>`. Add an
`r` command that prints `reset`. Temporarily omit its `break`, place it before
the `h` case, and predict both lines printed for `r`. Run once, then restore the
`break` and confirm that only the intended action remains.

<details>
<summary>Reveal solution</summary>

Without the first `break`, command `r` prints both `reset` and `help` because
execution continues into the next case. The corrected case is:

```c
case 'r':
  puts("reset");
  break;
case 'h':
  puts("help");
  break;
```

With the `break` restored, command `r` prints only `reset`.

</details>

Without `break`, execution continues into the next `case`. Use fallthrough only
when it is deliberate and documented.

### Input-driven loops and EOF

Judge data often contains an unknown number of records. In Python you might
iterate over standard-input lines. In C, the conversion count controls the loop:

```c
#include <limits.h>
#include <stddef.h>
#include <stdio.h>

int main(void) {
  int value;
  long long total = 0;
  size_t count = 0;

  while (scanf("%d", &value) == 1) {
    if ((value > 0 && total > LLONG_MAX - value) ||
        (value < 0 && total < LLONG_MIN - value)) {
      fprintf(stderr, "sum is outside the long long range\n");
      return 1;
    }
    total += value;
    ++count;
  }

  if (!feof(stdin)) {
    fprintf(stderr, "invalid token after %zu integers\n", count);
    return 1;
  }
  printf("count=%zu total=%lld\n", count, total);
  return 0;
}
```

#### Try it now — drive the loop from the shell (4 minutes)

Compile the program, then test it with a valid sequence, an empty input, and a
sequence containing `x` after two integers. For example, pipe text into the
program with `printf '10 -2 5\n' | ./program`. Explain why the invalid-token
case is different from ordinary end-of-file.

<details>
<summary>Reveal solution</summary>

- `10 -2 5` reaches end-of-file after three successful conversions and prints
  `count=3 total=13`.
- Empty input performs no conversions, reaches end-of-file normally, and prints
  `count=0 total=0`.
- `10 -2 x` performs two conversions, then stops at `x`. Because the failure is
  not end-of-file, the program reports `invalid token after 2 integers` and
  returns failure.

EOF is an ordinary end condition for this program. A noninteger token violates
the input contract and must not be silently treated as the same condition.

</details>

`scanf` can return `EOF` or a smaller conversion count. Never write
`while (!feof(stdin))`: EOF is observed only after a read attempt fails, so that
pattern commonly processes stale data once.

### Try it now — Hour 3 guided translation (8 minutes)

Translate the positive-square sum expressed by this Python program. For valid
input, preserve the behavior of printing one answer while making C's input and
range limits explicit:

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

<details>
<summary>Reveal solution</summary>

The following solution treats whitespace-separated `int` values up to
end-of-file as the input and accumulates into `long long`. It rejects a 101st
value, an invalid token, an unrepresentable square, or an unrepresentable sum:

```c
#include <limits.h>
#include <stddef.h>
#include <stdio.h>

int main(void) {
  int value;
  size_t count = 0;
  long long answer = 0;

  while (scanf("%d", &value) == 1) {
    if (count == 100) {
      fprintf(stderr, "too many values\n");
      return 1;
    }
    ++count;

    if (value > 0) {
      long long wide_value = value;
      if (wide_value > LLONG_MAX / wide_value) {
        fprintf(stderr, "square is outside the long long range\n");
        return 1;
      }
      long long square = wide_value * wide_value;
      if (answer > LLONG_MAX - square) {
        fprintf(stderr, "sum is outside the long long range\n");
        return 1;
      }
      answer += square;
    }
  }

  if (!feof(stdin)) {
    fprintf(stderr, "invalid integer input\n");
    return 1;
  }
  printf("%lld\n", answer);
  return 0;
}
```

No array is required because each value contributes once and is never needed
again. The wider accumulator reduces the common overflow risk, while the two
checks make its finite range explicit.

</details>

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

#### Try it now — repair before running (4 minutes)

Put the following fragment in `main`, but **do not run it yet**:

```c
int values[3] = {10, 20, 30};
int position = 3;
int denominator = 0;
printf("%d\n", values[position]);
printf("%d\n", 100 / denominator);
```

Identify the two violated preconditions. Modify the inputs or guard the
operations so that every evaluated array access and division is defined. Only
then compile and run the repaired version.

<details>
<summary>Reveal solution</summary>

Valid array indices are 0, 1, and 2, so `position == 3` is outside the array.
Integer division also requires a nonzero denominator. One guarded repair is:

```c
if (0 <= position && position < 3) {
  printf("%d\n", values[position]);
} else {
  fprintf(stderr, "position is outside the array\n");
}

if (denominator != 0) {
  printf("%d\n", 100 / denominator);
} else {
  fprintf(stderr, "denominator must not be zero\n");
}
```

The guards matter because they prevent the invalid operations from being
evaluated; merely printing an error afterward would be too late.

</details>

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

### Try it now — extend without duplicating (4 minutes)

Extend the program so it also reports whether the value is divisible by three.
Reuse the same `value`; do not add another input operation. Predict and test the
complete output for `-3`, `0`, `4`, and invalid input.

<details>
<summary>Reveal solution</summary>

Delay the newline after the parity result, then add one more independent test:

```c
if (value % 2 == 0) {
  printf(" and even");
} else {
  printf(" and odd");
}

if (value % 3 == 0) {
  printf(", divisible by three\n");
} else {
  printf(", not divisible by three\n");
}
```

The resulting descriptions are:

- `-3 is negative and odd, divisible by three`
- `0 is zero and even, divisible by three`
- `4 is positive and even, not divisible by three`

Invalid input still returns before either classification is printed.

</details>

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
