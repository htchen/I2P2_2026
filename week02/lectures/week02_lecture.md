# Lecture 2 — Functions, Arrays, and Strings in C

> September 15, 2026 · Source lineage: the legacy function, array, string, and
> input notes, reorganized around comparisons with Python sequences

## Learning objectives

By the end of this lecture, you should be able to:

1. Declare, define, and call a C function through a prototype.
2. Explain pass-by-value and use return values for explicit results.
3. Traverse arrays without reading outside their bounds.
4. Explain the null-terminated representation of a C string.
5. Design interfaces that pass an array together with its length or capacity.

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | How do typed functions decompose a program? | Specify and implement a small function family |
| 2 | How do array layout and bounds shape algorithms? | Trace one- and two-dimensional array operations |
| 3 | How do null-terminated strings remain inside their buffers? | Build and test bounded string utilities |

## Hour 1 — Function contracts and decomposition

### 1. Functions are typed contracts

Python checks a function call while the program runs. A C compiler checks a
prototype before generating the call.

```c
double mean(const int values[], size_t count);
```

This declaration promises:

- the function is named `mean`;
- it returns a `double`;
- it receives a sequence of integers that it will not modify;
- it receives the number of valid elements.

The definition provides the implementation:

```c
#include <stddef.h>

double mean(const int values[], size_t count)
{
    long long total = 0;
    for (size_t i = 0; i < count; ++i) {
        total += values[i];
    }
    return count == 0 ? 0.0 : (double)total / count;
}
```

Keep the declaration and definition identical. A prototype placed in a header
allows multiple source files to share the same contract.

### 2. C passes arguments by value

Each parameter starts as a copy of the corresponding argument.

```c
void ineffective_swap(int a, int b)
{
    int temporary = a;
    a = b;
    b = temporary;
}
```

Calling `ineffective_swap(x, y)` does not modify `x` or `y`. Later we will pass
their addresses when mutation is required. For now, prefer returning the result:

```c
int absolute_value(int value)
{
    return value < 0 ? -value : value;
}
```

Precondition: `value != INT_MIN`, because `-INT_MIN` may overflow. Interfaces
should make important preconditions visible in names, documentation, or checks.

### Decompose before coding

The legacy function notes built a program in stages. For a judge problem that
reads scores, removes one lowest score, and reports a rounded average, first
write contracts rather than a long `main`:

```c
int read_scores(int scores[], size_t capacity, size_t *count);
size_t index_of_minimum(const int scores[], size_t count);
void remove_at(int scores[], size_t *count, size_t index);
double mean(const int scores[], size_t count);
```

For each function, state:

- valid inputs and array bounds;
- which objects may change;
- how failure is reported;
- the valid range of the result;
- whether empty input is permitted.

This is C's explicit replacement for many run-time assumptions hidden inside a
short Python expression.

### Scope, storage duration, and `static` locals

Ordinary local variables are created on entry and cease to exist on return.
A `static` local retains its value for the program's lifetime:

```c
unsigned long next_sequence(void)
{
    static unsigned long value = 0;
    return ++value;
}
```

This hidden state can be useful but makes tests order-dependent and is not
automatically safe for concurrency. Prefer explicit state passed through a
structure when the state is part of the abstraction.

### Hour 1 checkpoint

Write a prototype and five-line contract for a function that finds a target in
an integer array. Compare three result designs: return an index with a sentinel,
return success plus an output parameter, or return a pointer to the element.
The third design will become meaningful after Lecture 4.

## Hour 2 — Array layout, multidimensional storage, and algorithms

### 3. Arrays are contiguous fixed-size storage

```c
int scores[5] = {91, 82, 73, 94, 85};
```

The array contains five adjacent `int` objects indexed from `0` through `4`.
Unlike a Python list, it does not remember a run-time length and cannot grow.

Inside the same scope as the array declaration:

```c
size_t count = sizeof scores / sizeof scores[0];
```

This expression does **not** work in a function parameter. In most expressions,
an array is converted to a pointer to its first element. Therefore every general
array function must receive a length explicitly.

```c
int maximum(const int values[], size_t count, int *result);
```

The return value can report whether a maximum exists; `result` can hold the
answer. We introduce this output-parameter style fully with pointers.

### 4. Boundary reasoning

For `count` valid elements, the canonical traversal is:

```c
for (size_t i = 0; i < count; ++i) {
    use(values[i]);
}
```

Ask three questions about every loop:

1. What is the first valid index?
2. What is the first invalid index?
3. Does the loop condition exclude the first invalid index?

Accessing `values[count]` is undefined behavior. C has no automatic bounds
check and no `IndexError`.

### 5. Two-dimensional arrays

```c
enum { Rows = 3, Columns = 4 };
int matrix[Rows][Columns] = {0};
```

Elements are stored row by row. When passing this array, the compiler must know
the column stride:

```c
int sum_matrix(size_t rows, size_t columns, const int matrix[rows][columns])
{
    int total = 0;
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < columns; ++c) {
            total += matrix[r][c];
        }
    }
    return total;
}
```

### Row-major address calculation

For `int matrix[Rows][Columns]`, the conceptual offset of `matrix[r][c]` is:

```text
(r * Columns + c) * sizeof(int)
```

The column count is therefore part of the function's representation contract.
Draw a `2 x 3` matrix as six consecutive cells, label the offsets, and explain
why a function cannot traverse rows correctly if it knows only the row count.

### In-place insertion and removal

Arrays make shifts explicit:

```c
int insert_at(int values[], size_t *count, size_t capacity,
              size_t index, int value)
{
    if (count == NULL || *count >= capacity || index > *count) return 0;

    for (size_t i = *count; i > index; --i) {
        values[i] = values[i - 1];
    }
    values[index] = value;
    ++*count;
    return 1;
}
```

Trace the loop backward. A forward shift would overwrite values before they are
copied. Test insertion at the front, middle, end, into an empty array, and into
a full array.

### Sorting and comparator previews

The legacy notes used insertion sort before introducing `qsort`. Students
should be able to implement and reason about the simple algorithm:

```c
void insertion_sort(int values[], size_t count)
{
    for (size_t i = 1; i < count; ++i) {
        int current = values[i];
        size_t position = i;
        while (position > 0 && values[position - 1] > current) {
            values[position] = values[position - 1];
            --position;
        }
        values[position] = current;
    }
}
```

Loop invariant: before iteration `i`, the prefix `[0, i)` is sorted and contains
the original prefix's values. The algorithm is O(n²) in the worst case but is a
useful exercise in bounds and mutation.

### Hour 2 checkpoint

Trace `insertion_sort` on `{4, 2, 2, 1}`. After each outer iteration, record the
array, `current`, and `position`. Then identify exactly which comparisons make
the algorithm stable for equal elements.

## Hour 3 — String representation, bounded input, and parsing

### 6. Strings are character arrays with a sentinel

```c
char language[] = "C17";
```

The array contains four characters: `'C'`, `'1'`, `'7'`, and the terminating
null character `'\0'`. Library functions find the end by scanning for this
sentinel. If the terminator is missing, a string function may continue beyond
the array.

```c
#include <string.h>

size_t length = strlen(language);  /* 3, not 4 */
```

`strlen` is linear time; it does not know the array capacity.

### Capacity versus length

```c
char name[32] = "Ada";
```

- Capacity: 32 characters of storage.
- Current string length: 3 characters.
- Available space for additional text: 28 characters, because one position is
  reserved for `\0`.

This distinction returns later as C++ `vector::capacity()` versus `size()`.

### 7. Reading a line safely

For text, prefer a bounded line read and then parse:

```c
#include <stdio.h>
#include <string.h>

char line[128];
if (fgets(line, sizeof line, stdin) == NULL) {
    return 1;
}

line[strcspn(line, "\n")] = '\0';
printf("You entered %zu characters: %s\n", strlen(line), line);
```

If the input is longer than the buffer, `fgets` reads only a prefix. Production
code must detect whether the newline was read and decide whether to reject,
discard, or continue the line.

Avoid unbounded `%s` input. It cannot know the destination capacity.

### Implement library ideas once

Before relying on `<string.h>`, implement two small functions to expose the
sentinel and capacity contracts:

```c
size_t string_length(const char text[])
{
    size_t length = 0;
    while (text[length] != '\0') ++length;
    return length;
}

int string_copy(char destination[], size_t capacity, const char source[])
{
    size_t length = string_length(source);
    if (length >= capacity) return 0;

    for (size_t i = 0; i <= length; ++i) {
        destination[i] = source[i]; /* includes '\0' */
    }
    return 1;
}
```

The copy loop uses `<= length` deliberately. A successful string copy must copy
the terminator as well as visible characters. Discuss why calling either
function on a nonterminated array violates its precondition.

### Parse numbers from a line

`scanf` is convenient for fixed judge formats; `fgets` plus `strtol` gives
better control for interactive or diagnostic input:

```c
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

char *end;
errno = 0;
long parsed = strtol(line, &end, 10);
if (end == line || errno == ERANGE || parsed < INT_MIN || parsed > INT_MAX) {
    fprintf(stderr, "not an int\n");
} else {
    while (*end == ' ' || *end == '\t' || *end == '\n') ++end;
    if (*end != '\0') fprintf(stderr, "unexpected suffix: %s\n", end);
}
```

This preview combines strings with pointers. The important idea is staged
validation: conversion must consume digits, fit the destination type, and leave
only permitted trailing characters.

### Hour 3 studio

Write `split_words` for a mutable line buffer. It should replace whitespace
separators with `\0`, store pointers to the first character of each word in a
caller-provided array, and return the number of words. State the maximum words,
buffer lifetime, and whether consecutive spaces produce empty words. Use the
result to explain why the returned word pointers cannot outlive the line array.

## Worked example: one-pass statistics

```c
#include <stddef.h>
#include <stdbool.h>

struct Statistics {
    int minimum;
    int maximum;
    double mean;
};

bool statistics(const int values[], size_t count, struct Statistics *out)
{
    if (count == 0 || out == NULL) {
        return false;
    }

    long long total = values[0];
    out->minimum = values[0];
    out->maximum = values[0];
    for (size_t i = 1; i < count; ++i) {
        if (values[i] < out->minimum) out->minimum = values[i];
        if (values[i] > out->maximum) out->maximum = values[i];
        total += values[i];
    }
    out->mean = (double)total / count;
    return true;
}
```

This preview uses a structure and a pointer, topics developed in Lectures 3 and
4. Focus now on the boundary: the function reads `values[0]` only after proving
that `count > 0`.

## Check yourself

1. Why does `sizeof parameter / sizeof parameter[0]` fail in a function?
2. How many bytes are required to store the string `"tree"` as a `char` array?
3. Design a function to reverse an array. What must its contract include?
4. Find the off-by-one error in `for (i = 0; i <= count; ++i)`.
5. What should a string-building function know besides the current length?

## Summary

- Prototypes make function contracts available to the compiler.
- Arguments are passed by value; mutation requires explicit indirection.
- C arrays are contiguous and have no run-time length metadata.
- A C string is an array convention: characters followed by `\0`.
- Pair every array with its length and every output buffer with its capacity.
