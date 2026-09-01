# Week 3 Lecture Notes — Structures, Modules, Builds, and Debugging

> September 22, 2026 · Source lineage: previous structures, multi-file build,
> program-style, and debugging notes

> Python bridge: [Python Contrast Companion for Week 3](week03_python_companion.md)

## Student route

- **Core:** state a `struct` invariant, separate declarations from definitions,
  compile multiple translation units, and diagnose one failure from evidence.
- **Practice:** complete the [Week 3 exercise](lecture_exercises/week03_ex.md)
  before comparing with [the complete example](examples.c).
- **Supporting ideas:** build-system conveniences are useful, but the required
  model is source files to object files to one linked program.
- **Python bridge:** use the companion when fixed-layout records or separate
  compilation have no direct Python analogue.

## Learning objectives

By the end of this lecture, you should be able to:

1. Model a record with `struct`, `enum`, and `typedef` appropriately.
2. Separate a public interface from a private implementation.
3. Explain declarations, definitions, object files, and link errors.
4. Build and debug a multi-file C program.
5. State and check representation invariants.

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | How do records become reliable abstractions? | Design tokens and rational values with invariants |
| 2 | How do source files become one program? | Build a three-file module and diagnose link failures |
| 3 | How do tests and tools turn failure into evidence? | Debug a seeded multi-file defect and add a regression test |

## Hour 1 — Records, tagged data, and invariants

### 1. Structures group related values

A Python dictionary or simple class can group heterogeneous fields. The C
equivalent has a fixed compile-time layout:

```c
struct Student {
  int id;
  char name[32];
  double grade;
};

struct Student student = {1001, "Ada", 92.5};
printf("%s: %.1f\n", student.name, student.grade);
```

Assignment copies all members:

```c
struct Student copy = student;
```

An array member is copied as part of the structure even though a standalone
array cannot be assigned. Passing a structure by value also copies it; pass a
pointer to avoid copying a large record or to allow modification.

### 2. Tagged alternatives with `enum`

```c
enum TokenKind { TokenInteger, TokenPlus, TokenMinus, TokenEnd, TokenInvalid };

struct Token {
  enum TokenKind kind;
  int value;
};
```

The tag tells us how to interpret the rest of the record. This pattern will
become the token and AST representation in the compiler project.

Use `typedef` to name a genuinely useful abstraction, not to hide every type:

```c
typedef struct Token Token;
```

Both `struct Token` and `Token` are reasonable course styles; be consistent.

### 3. Invariants turn records into abstractions

A representation invariant is a property that must hold whenever clients can
observe an object. For a rational number:

- denominator is nonzero;
- denominator is positive;
- numerator and denominator share no common factor except one.

```c
struct Rational {
  int numerator;
  int denominator;
};

int rational_make(int numerator, int denominator, struct Rational* out);
```

The constructor-like function can reject zero and normalize the representation.
Do not make every caller rediscover these rules.

### Designated initializers and partial initialization

C designated initializers make field meaning explicit and tolerate field order
changes better than positional initialization:

```c
struct Student student = {.id = 1001, .name = "Ada", .grade = 92.5};
```

Unspecified members are initialized to zero. This differs from an uninitialized
automatic structure, whose members have indeterminate values.

### Tagged unions

The earlier C course introduced unions. They are useful only when paired with a
tag that records the active representation:

```c
enum ValueKind { ValueInteger, ValueReal, ValueError };

struct Value {
  enum ValueKind kind;
  union {
    long integer;
    double real;
    const char* error;
  } as;
};
```

Reading a union member inconsistent with `kind` violates the abstraction. This
combination of a tag and several alternative payloads is a general technique
for representing “exactly one of these cases.” Every function that reads the
payload must first inspect the tag, and every function that changes the case
must update the tag and payload together.

### Hour 1 design exercise

Design a `struct Date` and functions `date_make`, `date_next`, and `date_print`.
Decide which representation and operations belong in the public header. State
leap-year and valid-day invariants and give boundary tests for February, month
transitions, and invalid construction. After Week 4, revisit whether hiding the
representation behind an opaque pointer would improve the interface enough to
justify its ownership costs.

## Hour 2 — Headers, the preprocessor, and the build graph

### 4. Interfaces live in headers

`rational.h`:

```c
#ifndef RATIONAL_H
#define RATIONAL_H

#include <stdio.h>

struct Rational {
  int numerator;
  int denominator;
};

int rational_make(int numerator, int denominator, struct Rational* out);
void rational_print(FILE* stream, const struct Rational* value);

#endif
```

`rational.c`:

```c
#include "rational.h"

#include <assert.h>
#include <limits.h>

static int gcd(int a, int b) {
  while (b != 0) {
    int remainder = a % b;
    a = b;
    b = remainder;
  }
  return a < 0 ? -a : a;
}

int rational_make(int numerator, int denominator, struct Rational* out) {
  if (denominator == 0 || numerator == INT_MIN || denominator == INT_MIN ||
      out == NULL)
    return 0;
  if (denominator < 0) {
    numerator = -numerator;
    denominator = -denominator;
  }
  int divisor = gcd(numerator, denominator);
  out->numerator = numerator / divisor;
  out->denominator = denominator / divisor;
  return 1;
}

void rational_print(FILE* stream, const struct Rational* value) {
  assert(stream != NULL && value != NULL);
  fprintf(stream, "%d/%d", value->numerator, value->denominator);
}
```

`static` on `gcd` gives it internal linkage: other source files cannot name it.
The public header contains the contract; the source file contains private work.
This teaching representation rejects `INT_MIN` because negating it is not
representable as `int`; a production numeric type should document or redesign
that range limitation explicitly.

### 5. Separate compilation and linking

```sh
cc -std=c17 -Wall -Wextra -Wpedantic -g -c rational.c
cc -std=c17 -Wall -Wextra -Wpedantic -g -c main.c
cc rational.o main.o -o rational_demo
```

- Each `-c` command creates an object file without linking.
- The final command resolves cross-file references.
- “Implicit declaration” usually means the compiler did not see a prototype.
- “Multiple definition” means more than one object defines the same symbol.
- “Undefined reference” means no linked object supplied a required definition.

Include your own header first in its implementation file. If the header is not
self-contained, the mistake is found close to its source.

### Encapsulation before opaque ownership

A module can begin with a visible structure definition while still requiring
clients to use its functions:

```c
/* counter.h */
struct Counter {
  long value;
};

void counter_initialize(struct Counter* counter);
void counter_increment(struct Counter* counter);
long counter_value(const struct Counter* counter);
```

The visible layout means the compiler knows how much storage a `Counter` needs,
so a client can declare one directly. The function contracts still centralize
valid initialization and state changes. This is convention-based
encapsulation: the compiler does not prevent a client from writing `value`.

A stronger design can hide the members behind an incomplete, or **opaque**,
structure type. Doing so normally requires clients to manipulate pointers and
raises allocation and destruction questions. Week 4 introduces the necessary
pointer, lifetime, and ownership model before presenting that interface. The
ordering matters: hiding representation is useful only when we can also state
who creates, owns, and destroys the hidden object.

### Preprocessor discipline

Object-like macros perform token substitution and have no type:

```c
#define BUFFER_CAPACITY 256
```

Function-like macros can evaluate arguments more than once:

```c
#define BAD_SQUARE(x) ((x) * (x))
/* BAD_SQUARE(i++) modifies i twice without sequencing: undefined behavior. */
```

Prefer `enum` constants, `const` objects, and functions when they express the
same intent. Use conditional compilation for genuine platform or build choices,
not to hide multiple unrelated implementations in one file.

### A minimal Makefile

```make
CC = cc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -g

rational_demo: main.o rational.o
	$(CC) main.o rational.o -o rational_demo

main.o: main.c rational.h
	$(CC) $(CFLAGS) -c main.c

rational.o: rational.c rational.h
	$(CC) $(CFLAGS) -c rational.c
```

The dependency edges explain what must be rebuilt after a header changes. Make
is not the compiler; it decides which compiler/linker commands are out of date.

### Hour 2 failure lab

Seed and classify these defects in a three-file program:

1. omit a header dependency from the Makefile;
2. declare `double mean(...)` but define `int mean(...)`;
3. define a non-`static` helper with the same name in two source files;
4. place a function definition in a header included by both source files;
5. change a function body without relinking.

For each, identify the first stage capable of detecting the defect.

## Hour 3 — Assertions, file boundaries, tests, and debugging

### 6. Assertions, tests, and debugger evidence

Use assertions for internal conditions that indicate a programmer error:

```c
#include <assert.h>
#include <stddef.h>

int array_sum(const int values[], size_t count) {
  assert(values != NULL || count == 0);
  int total = 0;
  for (size_t i = 0; i < count; ++i) {
    total += values[i];
  }
  return total;
}
```

This teaching version requires the mathematical sum to be representable as an
`int`. An interface for unrestricted inputs must use checked arithmetic or
report overflow explicitly.

Do not use `assert` as the only validation of ordinary user input; assertions
may be disabled. Return an error or report a diagnostic for expected failures.

A practical debugging loop is:

1. Reproduce the smallest failing input.
2. State the expected and observed behavior.
3. Compile with warnings and sanitizers.
4. Stop at a relevant line in the debugger.
5. Inspect control flow and data; do not guess blindly.
6. Add a regression test before or with the fix.

Typical debugger commands are `break`, `run`, `next`, `step`, `print`, and
`backtrace`. Learn the concepts; the exact command spelling varies by debugger.

### File I/O is another contract boundary

The previous notes used redirection and `FILE *`. A module can accept a stream
instead of opening a hard-coded path:

```c
int students_read(FILE* input, struct Student students[], size_t capacity,
                  size_t* count) {
  if (input == NULL || count == NULL || (students == NULL && capacity != 0)) {
    return 0;
  }
  *count = 0;
  while (*count < capacity) {
    struct Student next;
    int converted =
        fscanf(input, "%d %31s %lf", &next.id, next.name, &next.grade);
    if (converted == EOF) return feof(input) != 0;
    if (converted != 3) return 0;
    students[(*count)++] = next;
  }
  int extra = fscanf(input, "%*s");
  return extra == EOF && feof(input) != 0; /* reject extras/read errors */
}
```

Receiving `FILE *` makes the parser testable with redirected files or temporary
streams. It also separates “where bytes come from” from “how records are parsed.”

### Debugging studio: invariant first

Given a rational module that occasionally prints `2/-4`, work in this order:

1. add `assert(value->denominator > 0)` at public observation points;
2. construct the smallest input that triggers the assertion;
3. break in `normalize` and inspect both members before and after each branch;
4. determine which operation bypassed normalization;
5. repair the public mutation path;
6. add a regression test that checks both value and invariant;
7. run the complete test set with sanitizers.

The assertion is not the repair. It converts a distant wrong output into a
failure at the boundary where the invariant first becomes observable.

### 7. Style as a correctness tool

- Give each function one clear responsibility.
- Use names that expose units and roles (`capacity`, `student_count`).
- Replace unexplained magic values with named constants.
- Keep declarations near first use.
- Use `const` for data a function must not modify.
- Document why a surprising choice is correct, not what obvious syntax does.

## Midterm project connection — Map before modifying

The expression-compiler scaffold and its companion tools are released this
week. Treat them as an unfamiliar system, not as a collection of blanks to send
to an LLM.
Before changing code, identify:

- the entry point and input contract;
- token representation and the lexer boundary;
- the parser's input and AST output;
- semantic and instruction-generation stages;
- allocation, cleanup, and error-reporting responsibilities;
- each TODO's precondition and postcondition.

Trace one public expression through the existing stages and record where the
scaffold is complete, incomplete, or deliberately simplified. An AI tool may
help explain a function, but students must verify every claim against the
actual declarations and one executed trace. Thursday's deliverable is a build
record and pipeline map, not project implementation.

## Check yourself

1. Which declarations belong in a public header, and which should remain private?
2. Why does defining an ordinary function in a header often cause link errors?
3. What invariant would you require for a date structure?
4. Classify a missing prototype versus a missing function body.
5. Design three tests for `rational_make`, including one invalid input.

## Summary

- Structures give a fixed layout to related fields.
- Enums make states and tagged alternatives explicit.
- Headers declare contracts; source files define behavior.
- Compilation checks each translation unit; linking connects them.
- Invariants, assertions, focused tests, and debuggers turn failures into evidence.

## References and source materials

- [Structures, enumerations, and related C topics](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/Supplementary%20Material%202/README.md>)
- [Compiling multiple source files](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/如何compile多個檔案/如何%20compile%20多個檔案.md>)
- [Debugging](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/Programming%20related%20Topic/Debug.md>)
- [Programming style](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/Programming%20related%20Topic/程式撰寫風格.md>)
