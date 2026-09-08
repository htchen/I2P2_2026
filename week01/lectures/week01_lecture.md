# Week 1 Lecture Notes — From Python to C

> September 8, 2026 · C17 · Source lineage: previous C introduction,
> formatted-I/O, operators, and looping notes plus the instructor-provided
> *From C to Assembly* handout

> Python bridge: [Python Contrast Companion for Week 1](week01_python_companion.md)

---

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

---

## Learning objectives

By the end of this lecture, you should be able to:

1. Describe preprocessing, compilation, assembly, linking, and execution, and
   inspect the generated assembly as evidence of translation.
2. Translate a small Python program into typed C.
3. Use formatted input/output and C control flow safely.
4. Distinguish a compile-time error, a link-time error, and a run-time fault.
5. Compile with warnings and treat diagnostics as useful evidence.

---

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | How does typed C become an executable? | Compile, inspect, deliberately break, and repair a small program |
| 2 | How are Python-familiar values represented and formatted? | Type/conversion worksheet and robust input fragment |
| 3 | How do we translate control flow without inheriting C-specific bugs? | Complete and test a judge-style classification program |

Each hour interleaves about 35–45 minutes of explanation and live coding with
15–18 minutes of core practice. The remaining time is a buffer for discussion,
transitions, and a short break; optional extensions can use that buffer when
the class is ready.

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
attempt. Each solution panel identifies the expected standard output, a
representative diagnostic, or the reason that the example has no runtime
output.

- **Core live:** part of the planned in-class path.
- **Extension:** remains beside the example for additional practice, but may be
  completed during a break, in the lab, or after class if time is short.

The core-live exercises total about 15 minutes in Hour 1, 18 minutes in Hour 2,
and 16 minutes in Hour 3. This leaves time for transitions, questions, and a
short break without removing the immediate practice opportunities.

---

## Hour 1 — Program translation and the C execution model

> **Hour 1 route:** [machine model](#1-same-algorithms-different-machine-model)
> → [translation pipeline](#2-the-translation-pipeline)
> → [live diagnostic build](#hour-1-live-build-classify-the-diagnostic)
> → [assembly as evidence](#assembly-is-an-observation-window)
> → [first complete program](#3-first-program)

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

---

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

The command-line pieces used this week mean:

| Command or option | Purpose |
|-------------------|---------|
| `cc` | run the system's C compiler driver |
| `-std=c17` | select the C17 language version |
| `-Wall -Wextra -Wpedantic` | request useful warning groups |
| `-g` | retain information used by a debugger |
| `-o filename` | name the output file |
| `-E` | stop after preprocessing |
| `-S` | stop after producing assembly text |
| `-c` | stop after producing an object file |
| `-O0` | minimize optimization so source structure is easier to observe |
| `-O2` | enable a substantial, commonly used optimization level |
| `./hello` | run the file named `hello` from the current directory |

<details>
<summary>Side note — what the four build artifacts look like</summary>

Use the complete `hello.c` program in the live-build section below. The first
two outputs are text files that can be read in an editor. The last two are
binary files, so inspect them with development tools rather than printing their
raw bytes in the terminal. On success, these four `cc` commands normally print
nothing: the result is the file named after `-o`. The commands below make each
result observable.

#### 1. Preprocessed C: `hello.i`

```sh
cc -std=c17 -E hello.c -o hello.i
```

The preprocessor expands directives before ordinary C compilation. In
particular, `#include <stdio.h>` is replaced by declarations provided by the
implementation. The resulting file is usually much longer than `hello.c`.
Search for the program's own function instead of reading from the beginning:

```sh
grep -n "int twice" hello.i
```

Here, `grep -n` prints matching text together with its line number. The start
of `hello.i` commonly contains lines similar to these:

```text
# 1 "hello.c"
# 1 "<built-in>" 1
# 1 "/.../include/stdio.h" 1 3 4
```

Lines beginning with `#` are **line markers**. They let later diagnostics refer
back to the appropriate source or header even though preprocessing combined
many files. Paths and trailing marker numbers are implementation-specific.
Farther down, a small excerpt still resembles the original C:

```c
int twice(int value);

int main(void) {
  printf("%d\n", twice(21));
  return 0;
}
```

The original `#include <stdio.h>` line is no longer an instruction to include a
file: declarations from that header now appear in the translation unit. Macro
uses have also been replaced by their expansions, and comments may have been
removed. Function bodies, declarations, and expressions are still C—not
assembly or machine code. The exact header declarations and line-marker
spellings are not course material; the portable observation is that
preprocessing produces another C translation unit.

#### 2. Assembly text: `hello.s`

```sh
cc -std=c17 -O0 -S hello.c -o hello.s
```

The compiler translates the preprocessed C into assembly for the current
machine. Locate the function labels with:

```sh
grep -n "twice" hello.s
```

An illustrative ARM/macOS excerpt may contain:

```text
        .globl  _main
_main:
        ... prepare the argument 21 ...
        bl      _twice
        ... prepare the format string and result ...
        bl      _printf
        ret

        .globl  _twice
_twice:
        ... load value ...
        lsl     w0, w8, #1
        ret

        .asciz  "%d\n"
```

This output mixes instructions with assembler directives:

- `.globl` makes a symbol visible to the linker;
- `_main:` and `_twice:` are labels naming instruction locations;
- `bl` calls another function and `ret` returns on this ARM target;
- `lsl` shifts bits left and can implement multiplication by two; and
- `.asciz` stores the format string followed by its terminating zero byte.

An x86 compiler may instead use labels without leading underscores, `call` for
a function call, and different registers or arithmetic instructions. Even at
`-O0`, the compiler need not translate each C operator into an instruction with
the same name: selecting a shift for multiplication by two preserves the C
result. At this stage, identify function boundaries, calls, and constants; do
not memorize one target's instruction spelling.

#### 3. Relocatable object file: `hello.o`

```sh
cc -std=c17 -c hello.c -o hello.o
```

`hello.o` contains encoded machine instructions, data, a symbol table, and
information that the linker still needs. The `file` command describes the
binary without dumping it:

```sh
file hello.o
nm hello.o
```

Representative `file` descriptions include:

```text
hello.o: Mach-O 64-bit object arm64
hello.o: ELF 64-bit LSB relocatable, x86-64, ...
```

The important word is `relocatable`: code and data exist, but their final
addresses are not yet fixed. The object is not a complete executable and still
contains references for the linker to resolve.

The `nm` command lists symbols known to the object file. A representative
macOS result is:

```text
0000000000000000 T _main
                 U _printf
0000000000000048 T _twice
0000000000000060 s l_.str
```

The left column contains offsets written in hexadecimal. In the middle column,
`T` identifies a globally visible symbol defined in the code section, `U`
means undefined in this object, and lowercase `s` commonly identifies a local
section symbol. Thus `main` and `twice` have code here, whereas `printf` must be
connected to the C library during linking. Linux commonly omits the leading
underscores and may use somewhat different symbol letters. Symbol spelling and
offsets are evidence from one toolchain, not source-level C rules.

The link step combines and relocates the relevant pieces and connects external
references to libraries. With dynamic linking, part of that connection is
recorded for the loader to finish when the program starts.

#### 4. Linked executable: `hello`

```sh
cc hello.o -o hello
```

The linker resolves the remaining references and produces a file that the
operating system can load. Inspect and then run it:

```sh
file hello
./hello
```

Representative descriptions include:

```text
hello: Mach-O 64-bit executable arm64
hello: ELF 64-bit LSB pie executable, x86-64, ...
```

Unlike the relocatable object, this file contains the metadata required to
start a process. It is usually larger than `hello.o` because it also contains
headers, loader information, and other link-time metadata. File sizes vary and
are not a measure of how many C statements were written.

Running the program and then inspecting the shell's saved exit status gives:

```text
$ ./hello
42
$ echo $?
0
```

The line `42` is ordinary program output written by `printf`; the newline in
`"%d\n"` moves the terminal to the next line. The program does not print the
final zero. The shell stores that status because `main` returned `0`, and
`echo $?` displays it. A nonzero status conventionally reports failure.

The complete progression is:

| Artifact | Representation | Useful inspection | What is still missing? |
|----------|----------------|-------------------|------------------------|
| `hello.i` | preprocessed C text | editor, `grep` | C compilation |
| `hello.s` | target assembly text | editor, `grep` | assembly into binary instructions |
| `hello.o` | relocatable binary object | `file`, `nm` | final addresses and external definitions |
| `hello` | linked executable binary | `file`, `./hello` | nothing before normal loading and execution |

</details>

> **Try it now [Core live] — name the artifact (2 minutes):** without running
> the commands, write the expected output filename after each line. Then run
> them later with the complete `hello.c` program below and correct your
> predictions. Which command produces something that can be executed directly?

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

**Expected terminal output:** the four successful `cc` commands normally print
nothing; each writes the file named after `-o`. After linking, running the
program produces:

```text
42
```

</details>

A program that compiles with a warning is not necessarily safe. Read the first
diagnostic, locate the referenced source, and decide whether the code or the
stated contract is wrong.

> **Minimum takeaway for the first reading:** source code is checked and
> translated before it runs; the linker combines separately translated pieces;
> and failures at compilation, linking, and execution are different evidence.
> You do not need to memorize file suffixes, loader details, or assembly
> instructions yet. Use the commands above to observe the stages and return to
> their lower-level details after writing the first C program.

---

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

#### Try it now [Core live] — locate the failure stage (10 minutes)

1. Remove the semicolon after `return value * 2`. Which stage rejects the
   program first?
2. Keep the prototype but remove the definition. Which stage now fails?
3. Run `cc -E` and locate the original source among preprocessed declarations.
4. Run `cc -S`, find the code for `twice`, and then compare it with an `-O2`
   build without expecting a line-for-line correspondence.
5. Run `cc -c`, inspect the object filename, and link it in a separate command.

<details>
<summary>Reveal solution</summary>

1. The missing semicolon makes the C translation unit syntactically invalid, so
   compilation fails before an object file is produced.
2. The call agrees with the visible declaration, so compilation can succeed.
   Linking fails because no linked object supplies the definition of `twice`.
3. `hello.i` contains the included declarations plus recognizable lines from
   the original file.
4. An unoptimized build normally contains code corresponding to `twice`; an
   optimized build may simplify or inline the call while preserving the result.
5. `cc -c hello.c -o hello.o` produces `hello.o`, and
   `cc hello.o -o hello` produces the executable.

**Representative diagnostics and output:** diagnostic wording depends on the
compiler and linker, but the observations should have this form:

| Experiment | Representative terminal evidence | Runtime output |
|------------|----------------------------------|----------------|
| Missing semicolon | `error: expected ';' after return statement` | None; compilation stops |
| Missing definition | `undefined reference to 'twice'` or `Undefined symbols ... _twice` | None; linking stops |
| Valid restored program | Successful build commands are silent | `42` |

The `-E`, `-S`, and `-c` commands also normally print nothing when successful;
their observable outputs are `hello.i`, `hello.s`, and `hello.o`.

</details>

Students should record the stage, diagnostic evidence, and smallest repair. The
goal is not to memorize messages but to locate responsibility in the pipeline.

---

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

An object declared outside every function has **static storage duration**: it
exists for the entire execution of the program and is initialized to zero when
no initializer is written. An ordinary block-local object has **automatic
storage duration**: it exists while execution is in that block and has no
automatic initial value. At file scope, the keyword `static` also keeps the
name private to this source file. These lifetime rules are the C concepts; the
section names below are only common implementation evidence.

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

int counts_total(void) {
  return zero_count + initial_count;
}

int add_one(int value) {
  int result = value + 1;
  return result;
}
```

#### Try it now [Extension] — observe without memorizing (4 minutes)

At `-O0`, locate evidence for the two static objects and both calculations. At
`-O2`, determine which names or storage locations remain and which may have
been replaced by constants or simpler instructions. Which semantic observations
remain true even when instruction sequences, registers, and labels differ?

<details>
<summary>Reveal solution</summary>

Because both objects are read by `counts_total`, a typical `-O0` assembly file
retains `zero_count` in a zero-initialized region such as `.bss` and
`initial_count` in `.data`. At `-O2`, the compiler can prove that their sum is
always seven in this translation unit; it may make `counts_total` return that
constant and omit both private objects. Similarly, the named local `result` may
have no memory location in optimized `add_one`.

The portable observations are that `counts_total()` returns seven and, whenever
the mathematical result is representable as an `int`, `add_one(value)` returns
one more than `value`. Exact sections, symbols, registers, and instruction
sequences are implementation evidence rather than C language guarantees.

**Runtime output:** none. This source intentionally has no `main` function and
is translated only with `-S`. Its output is the assembly file. A typical `-O0`
file contains storage or symbol evidence for both objects and instructions for
both functions; an `-O2` file may contain only simplified function bodies.

</details>

---

### 3. First program

```c
#include <stdio.h>

int main(void) {
  int courses_completed = 1;
  printf("Programming courses completed: %d\n", courses_completed);
  return 0;
}
```

#### Try it now [Core live] — edit, compile, run (3 minutes)

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

**Expected output for the shown edit:**

```text
Previous programming courses: 2
```

Before that edit, the original program prints:

```text
Programming courses completed: 1
```

With the semicolon removed, there is no runtime output because compilation
stops with a syntax diagnostic.

</details>

- `#include <stdio.h>` makes the declarations of standard I/O functions known.
- `main` is the program entry point.
- Braces delimit a block; semicolons terminate statements.
- `int courses_completed` declares storage and its interpretation.
- Returning zero conventionally reports success to the operating system.

---

## Hour 2 — Types, representation, conversion, and formatted I/O

> **Hour 2 route:** [types and expressions](#4-types-and-expressions)
> → [operators](#basic-operators-and-precedence)
> → [division and conversion](#integer-division-and-conversion)
> → [truth values](#truth-values)
> → [integer ranges](#supporting-reference--integer-ranges-and-signedunsigned-interactions)
> → [formatted I/O](#5-formatted-io)
> → [format contracts](#format-contract-reference)
> → [checkpoint](#try-it-now-core-live--hour-2-checkpoint-5-minutes)

### 4. Types and expressions

The core scalar types for the first week are:

- `char` stores one character-sized integer value;
- `int` is the ordinary whole-number type;
- `double` stores a floating-point approximation; and
- `_Bool` stores zero or one. In C17, `<stdbool.h>` supplies the more readable
  spellings `bool`, `false`, and `true` for `_Bool`, zero, and one.

```c
#include <stdbool.h>

char grade = 'A';
int count = 42;
double average = 87.5;
bool passed = true;
```

To observe the `char`, `int`, and `double` values, use the `printf` function
introduced in the first program. Its first argument is a format string; each
conversion beginning with `%` describes the corresponding value that follows
it:

| C value type | First output conversion | Meaning |
|--------------|-------------------------|---------|
| `int` | `%d` | print a decimal integer |
| `double` | `%.1f` | print one digit after the decimal point |
| `char` | `%c` | print the character |

The complete format-contract reference later in this hour covers input and
additional types.

#### Try it now [Core live] — choose a representation (2 minutes)

Use the earlier first program as a scaffold. Add variables for a whole-number
student count, a fractional temperature, and a letter grade. Choose the type
before the initial value, then print them with `%d`, `%.1f`, and `%c`,
respectively, following the table above. Compile and run; do not copy
demonstration variables that your program does not use.

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

**Expected output for the shown complete program:**

```text
students=40 temperature=26.5 grade=A
```

The earlier declaration-only fragment does not print by itself; the output
appears only after the solution passes those values to `printf`.

</details>

Use `sizeof value` to ask how many bytes an object occupies. Except for `char`,
the exact size of basic types can depend on the implementation.

> **Supporting type names:** `size_t` is the unsigned type used for object sizes
> and becomes important for arrays in Week 2. Exact-width types such as
> `int32_t` belong in code whose external data contract requires exactly that
> width; they are reference material rather than default replacements for
> `int`.

```c
#include <stddef.h>
#include <stdint.h>

size_t length = 10;
int32_t exact_width = 1000;
```

<details>
<summary>Output note — declarations alone do not print values</summary>

**Runtime output:** none. These lines declare and initialize two objects, but
they do not call an output function. If they are placed in a complete program,
the program produces output only when later statements pass their values to an
operation such as `printf`.

</details>

---

### Basic operators and precedence

The underlying operations are familiar from Python, but several spellings and
type rules differ. Start with these groups:

| Purpose | C operators | Important rule |
|---------|-------------|----------------|
| Arithmetic | `+`, `-`, `*`, `/`, `%` | `/` uses the operand types; `%` requires integer operands |
| Comparison | `<`, `<=`, `>`, `>=`, `==`, `!=` | the result is `0` or `1` |
| Logic | `&&`, `\|\|`, `!` | `&&` and `\|\|` short-circuit from left to right |
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

#### Try it now [Core live] — predict before printing (3 minutes)

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

**One possible output line:** if the five values are printed in the order used
above with spaces between them, the output is:

```text
2 3 17 42 12
```

</details>

Prefix and postfix `++`/`--` differ when their value is used inside a larger
expression. That distinction is rarely worth the reduced readability in an
introductory program: prefer a separate `++index;` or `--count;` statement and
do not modify the same object multiple times in one expression.

---

### Integer division and conversion

```c
double wrong = 5 / 2;         /* 2.0: division happened as int */
double right = (double)5 / 2; /* 2.5 */
```

#### Try it now [Core live] — move the conversion (2 minutes)

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

**Expected output:** with `printf("%.1f %.1f\n", wrong, right)`, the three
operand pairs produce:

| Operands | Output |
|----------|--------|
| `5` and `2` | `2.0 2.5` |
| `-5` and `2` | `-2.0 -2.5` |
| `5` and `-2` | `-2.0 -2.5` |

</details>

Conversions in C can discard information. Compile with warnings and make a
conversion explicit when it is intentional.

---

### Truth values

In a condition, zero is false and any nonzero scalar value is true. Relational
and logical operators produce `0` or `1`. An `if` statement evaluates the
parenthesized condition and executes its first braced block when that condition
is true. An optional `else` supplies the alternative block.

```c
int age = 18;
bool has_id = true;
bool eligible = age >= 18 && has_id;
```

#### Try it now [Core live] — test the boundary (2 minutes)

Add an `if`/`else` that prints `eligible` or `not eligible`. Run the four
combinations formed by ages 17 and 18 and ID values `false` and `true`. Identify
which part of the expression rejects each unsuccessful case.

<details>
<summary>Reveal solution</summary>

```c
if (eligible) {
  printf("eligible\n");
} else {
  printf("not eligible\n");
}
```

Only age 18 with `has_id == true` is eligible. At age 17, the left operand of
`&&` is false, so short-circuit evaluation does not need the right operand. At
age 18 without an ID, the right operand is false.

**Expected output:**

| `age` | `has_id` | Output |
|-------|----------|--------|
| `17` | `false` | `not eligible` |
| `17` | `true` | `not eligible` |
| `18` | `false` | `not eligible` |
| `18` | `true` | `eligible` |

</details>

Do not confuse assignment (`=`) with comparison (`==`).

---

### Supporting reference — integer ranges and signed/unsigned interactions

For the first reading, remember that C integer types have finite ranges and
that the types of both operands affect a calculation. The exact limit macros
and mixed signed/unsigned conversion rules below are useful diagnostic
references, but the extension exercises need not be completed during the core
lecture path.

Connect `sizeof` to the limits headers rather than assuming a fixed machine:

```c
#include <limits.h>
#include <stdio.h>

printf("int: %zu bytes, range %d through %d\n", sizeof(int), INT_MIN, INT_MAX);
printf("unsigned int maximum: %u\n", UINT_MAX);
```

#### Try it now [Extension] — ask the implementation (2 minutes)

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

**Illustrative output on a common 64-bit Unix-like system:** including the two
original calls and the added `long` call gives:

```text
int: 4 bytes, range -2147483648 through 2147483647
unsigned int maximum: 4294967295
long: 8 bytes, range -9223372036854775808 through 9223372036854775807
```

This exact line is not portable. A conforming implementation may give `long`
a different size and range; the program's own output is the answer for the
current environment.

</details>

Unsigned arithmetic wraps modulo one more than the maximum value. Signed
overflow is undefined behavior. Mixing signed and unsigned values can convert a
negative number to a very large unsigned value:

```c
#include <stddef.h>
#include <stdio.h>

int main(void) {
  int index = -1;
  size_t count = 10;

  printf("index=%d count=%zu\n", index, count);
  /* Uncomment only after predicting the result. */
  /* printf("%d\n", index < count); */
  return 0;
}
```

#### Try it now [Extension] — expose the mixed-domain bug (3 minutes)

Uncomment the comparison and compile with the course warning flags. Predict the
result first. Repair the comparison by choosing types that represent the same
intended domain; do not add a cast merely to silence the warning.

<details>
<summary>Reveal solution</summary>

```c
printf("%d\n", index < count);
```

On common current implementations, the comparison prints zero because `index`
is converted to `size_t`; converting `-1` to that unsigned type produces its
maximum value, which is not less than 10. With the course warning flags, common
compilers diagnose that two different numeric domains are being mixed. If this
small problem genuinely uses `-1` as a sentinel and all counts fit in `int`,
one coherent repair is:

```c
int index = -1;
int count = 10;
printf("%d\n", index < count);
```

For a real container API, a separate success flag or another explicit absence
representation is often clearer than mixing a negative sentinel with an
unsigned size.

**Representative output on the common implementations described above:** the
original mixed-type program prints its values first and, after the comparison
is uncommented, prints zero:

```text
index=-1 count=10
0
```

After changing `count` to `int`, the repaired comparison is between two signed
values and prints:

```text
1
```

</details>

#### Supporting reference — integer literal suffixes

An integer-literal suffix participates in the expression's type. The suffix
`U` means “choose an unsigned integer type”; for the small literals `0U` and
`1U`, that type is `unsigned int`. Therefore both operands in `0U - 1U` are
unsigned, and the subtraction wraps to `UINT_MAX`. Related suffixes include
`L`, `LL`, and combinations such as `ULL`. Use a suffix when the required type
is part of the contract, not merely to silence a conversion warning.

Do not “fix” every warning with a cast. First decide which domain the
program means. Loop indices for array sizes commonly use `size_t`; values that
must represent `-1` need a signed type or a different absence representation.

---

### 5. Formatted I/O

Every C program starts with three standard text streams:

- `stdin` supplies ordinary input;
- `stdout` receives ordinary output; and
- `stderr` receives diagnostics separately from ordinary output.

`scanf` reads from `stdin`, and `printf` writes to `stdout`. The related call
`fprintf(stderr, ...)` uses the same style of format string as `printf` but
sends the message to the diagnostic stream. This separation matters to an
online judge because diagnostics must not become part of the required answer.
In every formatted call, the conversion specifiers must agree with the
corresponding argument types. Returning zero from `main` reports success;
returning a nonzero value reports that the program could not complete its
contract.

```c
int score = 95;
double ratio = 0.875;
printf("score=%d ratio=%.2f\n", score, ratio);
```

#### Try it now [Core live] — control the presentation (1 minute)

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

**Expected output:**

```text
student score=95 success ratio=0.8750
```

Before the requested formatting change, the original call prints
`score=95 ratio=0.88` under the ordinary round-to-nearest environment.

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

#### Try it now [Core live] — test the input contract (3 minutes)

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

**Expected output by stream:** assume the original sum statement remains after
the diagnostic fragment. The numeric value used for `EOF` is implementation-
defined and is commonly `-1`.

| Input | Standard output | Standard error | Exit status |
|-------|-----------------|----------------|-------------|
| `10 20` | `conversions=2` followed by `30` | None | `0` |
| `10 x` | `conversions=1` | `expected two integers` | nonzero |
| `10` then EOF | `conversions=1` | `expected two integers` | nonzero |
| Immediate EOF | `conversions=<EOF value>` | `expected two integers` | nonzero |

</details>

`scanf` needs the **addresses** of `a` and `b` so it can modify them. We will
explain addresses in the Week 4 lecture notes. Until then, treat the format
string and each corresponding argument as a checked pair.

---

### Format-contract reference

| Value type | `printf` | `scanf` |
|------------|----------|---------|
| `int` | `%d` | `%d` with `&integer_variable` |
| `unsigned int` | `%u` | `%u` with `&unsigned_variable` |
| `long` | `%ld` | `%ld` with `&long_variable` |
| `long long` | `%lld` | `%lld` with `&long_long_variable` |
| `size_t` | `%zu` | `%zu` with `&size_variable` |
| `double` | `%f` | `%lf` with `&double_variable` |
| character | `%c` | `%c` with `&character_variable` |
| `bool` | `%d` after integer promotion | No direct conversion; read and validate an `int` |

For `printf`, a `float` argument is promoted to `double`, so `%f` is used. For
`scanf`, `%f` requires the address of a `float`, while `%lf` requires the
address of a `double`. This asymmetry is a common source of memory corruption.
String and pointer formatting are introduced only after Week 2 establishes
array representation and Week 4 establishes the pointer model.

When a `bool` is passed to `printf`, it is promoted to `int`, so `%d` prints
zero or one. Do not pass a `bool*` to `scanf` with `%d`: `%d` requires an
`int*`. Read into an `int`, validate the accepted values, and then assign the
result to a `bool`.

#### Try it now [Extension] — interpret a format warning (2 minutes)

Return to the Hour 1 `twice` program. Change only the output conversion from
`%d` to `%f`, then compile with the course warning flags. What type does `%f`
require, what type does `twice(21)` produce, and why should you restore the
correct conversion before running the program?

<details>
<summary>Reveal solution</summary>

For `printf`, `%f` requires a corresponding `double`, while `twice` is declared
to return `int`. A warning-enabled compiler can therefore diagnose the mismatch.
`printf` relies on the format string to decide how to interpret every following
argument. Supplying the wrong type gives the call **undefined behavior**, which
means C specifies no required result; Section 7 develops that concept. An
apparently plausible output would not make the call correct. Restore `%d`
because the program intends to print the integer result of `twice(21)`.

**Runtime output of the defective program:** none should be requested; do not
run it. A representative compilation diagnostic is:

```text
warning: format specifies type 'double' but the argument has type 'int'
```

After `%d` is restored, the warning disappears and the program prints:

```text
42
```

</details>

#### Try it now [Extension] — build a format checklist (2 minutes)

Choose five rows from the table, including `size_t` and `bool`, and write a
single `printf` call that prints them. Add one `scanf` call for a `double`.
Exchange the code with a partner and check every specifier against its
corresponding argument before compiling.

<details>
<summary>Reveal solution</summary>

One possible checklist program is:

```c
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

int main(void) {
  int signed_value = -3;
  unsigned int unsigned_value = 3U;
  size_t item_count = 5;
  double real_value = 3.5;
  bool ready = true;

  printf("%d %u %zu %.1f %d\n", signed_value, unsigned_value, item_count,
         real_value, ready);

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

**Expected output for input `2.5`:**

```text
-3 3 5 3.5 1
input=2.5
```

</details>

---

### Try it now [Core live] — Hour 2 checkpoint (5 minutes)

Predict the type and value before compiling:

```c
int a = 7;
int b = 2;
double x = a / b;
double y = (double)a / b;
```

Then compile a program that prints the four values. Explain why the two
floating-point results differ instead of stopping after the numerical answer.

<details>
<summary>Reveal solution</summary>

`a` and `b` are `int`. Integer division therefore produces 3 before `x` stores
that value as `3.0`. The cast makes one operand of the second division `double`,
so `y` is `3.5`.

A matching output statement is:

```c
printf("a=%d b=%d x=%.1f y=%.1f\n", a, b, x, y);
```

**Expected output:**

```text
a=7 b=2 x=3.0 y=3.5
```

</details>

---

## Hour 3 — Selection, iteration, EOF, and judge-style translation

> **Hour 3 route:** [selection and iteration](#6-selection-and-iteration)
> → [input-driven loops](#input-driven-loops-and-eof)
> → [guided translation](#try-it-now-core-live--hour-3-guided-translation-8-minutes)
> → [undefined behavior](#7-undefined-behavior-is-not-an-exception)

### 6. Selection and iteration

Python indentation becomes explicit braces:

```python
limit = 10
total = 0
for value in range(1, limit + 1):
    if value % 2 == 0:
        total += value
```

```c
int limit = 10;
int total = 0;
for (int value = 1; value <= limit; ++value) {
  if (value % 2 == 0) {
    total += value;
  }
}
```

A C `for` loop has three control clauses separated by semicolons. Here,
`int value = 1` runs once before the loop, `value <= limit` is checked before
each iteration, and `++value` runs after each completed iteration. The `if`
statement decides whether that iteration updates `total`. Because `value` is
declared in the `for` statement, its name is available only in that loop.

#### Try it now [Core live] — change one rule (3 minutes)

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

**Expected output:**

| Version | Output |
|---------|--------|
| Original even-number rule | `30` |
| Modified divisible-by-three rule | `18` |

</details>

C also provides `while` and `switch`; the next two examples give each construct
a concrete purpose. A `switch` evaluates its controlling expression once and
jumps to the matching `case`. The `default` label handles every unmatched
value, and `break` exits the `switch`. Prefer braces even for a one-statement
body because they prevent mistakes during later edits.

```c
char command = 'h';

switch (command) {
  case 'q':
    printf("quit\n");
    break;
  case 'h':
    printf("help\n");
    break;
  default:
    fprintf(stderr, "unknown command\n");
    break;
}
```

#### Try it now [Extension] — make fallthrough visible (3 minutes)

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
  printf("reset\n");
  break;
case 'h':
  printf("help\n");
  break;
```

With the `break` restored, command `r` prints only `reset`.

**Expected output for the original example with `command == 'h'`:**

```text
help
```

**Expected output without the first `break`:**

```text
reset
help
```

**Expected output after restoring the `break`:**

```text
reset
```

</details>

Without `break`, execution continues into the next `case`. Use fallthrough only
when it is deliberate and documented.

---

### Input-driven loops and EOF

Judge data sometimes contains an unknown number of records. A `while`
statement checks its parenthesized condition before every iteration and
continues only while that condition is true. In Python, iteration over an input
stream ends naturally. In C, `scanf` reports how many requested conversions
succeeded, so `scanf("%d", &value) == 1` means “one integer was read; process
it.”

For this first example, the input contract allows at most 100 numeric tokens,
each representable as `int` and within `[-30000, 30000]`. The magnitude of the
sum can therefore be at most `100 * 30000`, or 3,000,000, which fits in the
minimum range guaranteed for `long long`. This proof keeps the example focused
on input-loop behavior. Assume the course judge supplies a readable input
stream; detecting a device-level I/O error is outside this exercise. Safe
conversion of arbitrary-length numeric text is introduced after character
arrays and pointers.

After the loop, `feof(stdin)` is nonzero only if the failed read encountered
end-of-file. If the next token was not an integer, the conversion count is zero
and `feof(stdin)` remains zero. The program can therefore distinguish an
ordinary end of input from an invalid token:

```c
#include <stddef.h>
#include <stdio.h>

int main(void) {
  int value;
  long long total = 0;
  size_t count = 0;

  while (scanf("%d", &value) == 1) {
    if (count == 100) {
      fprintf(stderr, "too many integers\n");
      return 1;
    }
    if (value < -30000 || value > 30000) {
      fprintf(stderr, "integer is outside the supported range\n");
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

#### Try it now [Core live] — drive the loop from the shell (5 minutes)

Compile the program, then test it with a valid sequence, an empty input, and a
sequence containing `x` after two integers. For example, pipe text into the
program with `printf '10 -2 5\n' | ./program`. Also test a value at each numeric
boundary, a value just outside a boundary, and a 101st integer. Explain which
part of the input contract each rejected case violates.

<details>
<summary>Reveal solution</summary>

- `10 -2 5` reaches end-of-file after three successful conversions and prints
  `count=3 total=13`.
- Empty input performs no conversions, reaches end-of-file normally, and prints
  `count=0 total=0`.
- `10 -2 x` performs two conversions, then stops at `x`. Because the failure is
  not end-of-file, the program reports `invalid token after 2 integers` and
  returns failure.
- Values `-30000` and `30000` satisfy the inclusive numeric boundary, while
  either neighboring outside value is rejected.
- The first 100 integers are processed; a successfully read 101st integer is
  rejected before it is added to the total.

EOF is an ordinary end condition for this program. A noninteger token violates
the input contract and must not be silently treated as the same condition.

**Expected output by stream:**

| Input | Standard output | Standard error | Exit status |
|-------|-----------------|----------------|-------------|
| `10 -2 5` | `count=3 total=13` | None | `0` |
| Empty input | `count=0 total=0` | None | `0` |
| `10 -2 x` | None | `invalid token after 2 integers` | nonzero |
| `-30000 30000` | `count=2 total=0` | None | `0` |
| `30001` | None | `integer is outside the supported range` | nonzero |
| 101 copies of `1` | None | `too many integers` | nonzero |

</details>

With one requested conversion, `scanf` returns `1` after converting an integer,
`0` when the next token does not match, or `EOF` when input ends before a
conversion. Never write `while (!feof(stdin))`: EOF is observed only after a
read attempt fails, so that pattern commonly processes stale data once.

---

### Try it now [Core live] — Hour 3 guided translation (8 minutes)

Translate the positive-square sum expressed by this Python program. The Python
version reads one line; the C exercise intentionally generalizes that input to
whitespace-separated integers continuing until end-of-file. Both versions
print one answer for valid input:

```python
values = [int(token) for token in input().split()]
answer = sum(value * value for value in values if value > 0)
print(answer)
```

Process each integer as it is read, without storing an array. Accept at most 100
inputs, require every value to be in `[-30000, 30000]`, accumulate into a
`long long`, and distinguish end-of-file from an invalid token. As with typical
judge input using `%d`, assume that every numeric token is representable as an
`int`; safe conversion of arbitrary-length text is introduced after character
arrays and pointers. Test:

- an empty line/end-of-file;
- all negative values;
- zero mixed with positives;
- exactly 100 values;
- a 101st value;
- a noninteger token;
- values at both ends of the stated range.

The final discussion should distinguish translation of the algorithm from the
new representation and range decisions demanded by C.

<details>
<summary>Reveal solution</summary>

The following solution implements the explicitly revised stream contract:

```c
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
    if (value < -30000 || value > 30000) {
      fprintf(stderr, "value is outside the supported range\n");
      return 1;
    }
    ++count;

    if (value > 0) {
      long long wide_value = value;
      answer += wide_value * wide_value;
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
again. At most 100 squares of 30000 sum to 90,000,000,000, which fits in the
minimum range guaranteed for `long long`. The input bounds therefore establish
arithmetic safety without interrupting the central loop with advanced overflow
formulas.

**Expected output by stream:**

| Input | Standard output | Standard error | Exit status |
|-------|-----------------|----------------|-------------|
| Empty input | `0` | None | `0` |
| `-3 -1 0` | `0` | None | `0` |
| `0 3 4` | `25` | None | `0` |
| `-30000 30000` | `900000000` | None | `0` |
| `30001` | None | `value is outside the supported range` | nonzero |
| `1 x` | None | `invalid integer input` | nonzero |
| 101 copies of `1` | None | `too many values` | nonzero |

</details>

---

### 7. Undefined behavior is not an exception

Python normally stops and reports errors such as an out-of-range list access.
The C standard instead leaves some invalid operations without defined meaning.
Examples include:

- reading an uninitialized automatic variable;
- signed integer overflow;
- dividing an integer by zero;
- accessing storage outside an object's valid bounds, developed with arrays in
  Week 2;
- using a mismatched `printf` format.

The compiler is allowed to assume undefined behavior never occurs. “It worked
once” is therefore not evidence that the program is correct.

#### Try it now [Extension] — repair before running (4 minutes)

Put the following fragment in `main`, but **do not run it yet**:

```c
int denominator = 0;
int uninitialized_value;
printf("%d\n", 100 / denominator);
printf("%d\n", uninitialized_value);
```

Identify the two violated preconditions. Modify the inputs or guard the
operations so that every evaluated division and scalar read is defined. Only
then compile and run the repaired version.

<details>
<summary>Reveal solution</summary>

Integer division requires a nonzero denominator, and an automatic scalar must
receive a value before it is read. One guarded repair is:

```c
int initialized_value = 25;

if (denominator != 0) {
  printf("%d\n", 100 / denominator);
} else {
  fprintf(stderr, "denominator must not be zero\n");
}
printf("%d\n", initialized_value);
```

The initialization and guard matter because they prevent invalid operations
from being evaluated; merely printing an error afterward would be too late.

**Expected output with the original `denominator == 0`:** the diagnostic and
ordinary result are written to different streams.

```text
standard error: denominator must not be zero
standard output: 25
```

</details>

---

## Worked example: classify an integer

The `if`/`else` form from Hour 2 can be extended into an `else if` chain. Each
condition is checked from top to bottom, and only the first true branch runs.
This program uses the chain to distinguish negative, positive, and zero values,
then uses a separate `if`/`else` to classify parity:

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

### Try it now [Extension] — extend without duplicating (4 minutes)

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

**Expected output before the extension:** the original classifier produces:

```text
-3 is negative and odd
0 is zero and even
4 is positive and even
```

The resulting descriptions are:

- `-3 is negative and odd, divisible by three`
- `0 is zero and even, divisible by three`
- `4 is positive and even, not divisible by three`

Invalid input still returns before either classification is printed.

**Expected standard output from three separate valid runs:**

```text
-3 is negative and odd, divisible by three
0 is zero and even, divisible by three
4 is positive and even, not divisible by three
```

For invalid input, standard output is empty and the program returns a nonzero
status.

</details>

---

## Check yourself

1. Where does an “undefined reference” diagnostic occur in the pipeline?
2. What distinct artifacts do `-E`, `-S`, and `-c` produce?
3. What are the values of `7 / 3` and `(double)7 / 3`?
4. Why must the argument for `%d` have the expected integer type?
5. Translate a Python `while` loop that repeatedly reads until `0` into C.
6. **Extension:** Why can the assembly produced at `-O2` omit a named local
   variable?

---

## Summary

- Your programming knowledge transfers; C exposes types, storage, and failures.
- A C program is preprocessed, compiled, assembled, linked, and then executed.
- Generated assembly is target- and option-dependent evidence, not the C
  language definition.
- Declarations, format strings, and conversions are contracts.
- Warnings, exit status, and tests are part of normal development.
- Avoiding undefined behavior is a correctness requirement.

---

## References and source materials

- [Instructor handout: *From C to Assembly*](../../assets/references/from_c_to_assembly.pdf)
- [Introduction to programming](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/Introduction%20to%20programming/README.md>)
- [Operators, expressions, and statements](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/Operators%2C%20Expressions%2C%20and%20Statements/README.md>)
- [Looping](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/Looping/README.md>)
- [`printf` and `scanf` summary](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/Printf%20and%20Scanf/總整理.md>)
