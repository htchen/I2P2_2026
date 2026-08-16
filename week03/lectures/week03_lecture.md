# Lecture 3 — Structures, Modules, Builds, and Debugging

> September 22, 2026 · Source lineage: the legacy structures, multi-file build,
> program-style, and debugging notes

## Learning objectives

By the end of this lecture, you should be able to:

1. Model a record with `struct`, `enum`, and `typedef` appropriately.
2. Separate a public interface from a private implementation.
3. Explain declarations, definitions, object files, and link errors.
4. Build and debug a multi-file C program.
5. State and check representation invariants.

## 1. Structures group related values

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

## 2. Tagged alternatives with `enum`

```c
enum TokenKind {
    TokenInteger,
    TokenPlus,
    TokenMinus,
    TokenEnd,
    TokenInvalid
};

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

## 3. Invariants turn records into abstractions

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

int rational_make(int numerator, int denominator, struct Rational *out);
```

The constructor-like function can reject zero and normalize the representation.
Do not make every caller rediscover these rules.

## 4. Interfaces live in headers

`rational.h`:

```c
#ifndef RATIONAL_H
#define RATIONAL_H

#include <stdio.h>

struct Rational {
    int numerator;
    int denominator;
};

int rational_make(int numerator, int denominator, struct Rational *out);
void rational_print(FILE *stream, const struct Rational *value);

#endif
```

`rational.c`:

```c
#include "rational.h"

#include <assert.h>

static int gcd(int a, int b)
{
    while (b != 0) {
        int remainder = a % b;
        a = b;
        b = remainder;
    }
    return a < 0 ? -a : a;
}

int rational_make(int numerator, int denominator, struct Rational *out)
{
    if (denominator == 0 || out == NULL) return 0;
    if (denominator < 0) {
        numerator = -numerator;
        denominator = -denominator;
    }
    int divisor = gcd(numerator, denominator);
    out->numerator = numerator / divisor;
    out->denominator = denominator / divisor;
    return 1;
}

void rational_print(FILE *stream, const struct Rational *value)
{
    assert(stream != NULL && value != NULL);
    fprintf(stream, "%d/%d", value->numerator, value->denominator);
}
```

`static` on `gcd` gives it internal linkage: other source files cannot name it.
The public header contains the contract; the source file contains private work.

## 5. Separate compilation and linking

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

## 6. Assertions, tests, and debugger evidence

Use assertions for internal conditions that indicate a programmer error:

```c
#include <assert.h>

int array_sum(const int values[], size_t count)
{
    assert(values != NULL || count == 0);
    /* ... */
}
```

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

## 7. Style as a correctness tool

- Give each function one clear responsibility.
- Use names that expose units and roles (`capacity`, `student_count`).
- Replace unexplained magic values with named constants.
- Keep declarations near first use.
- Use `const` for data a function must not modify.
- Document why a surprising choice is correct, not what obvious syntax does.

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
