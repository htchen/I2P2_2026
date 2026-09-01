# Week 2 Lecture Notes — Functions, Arrays, and Strings in C

> September 15, 2026 · Source lineage: previous function, array, string, and
> input notes, reorganized around comparisons with Python sequences

> Python bridge: [Python Contrast Companion for Week 2](week02_python_companion.md)

## Student route

- **Core:** write typed functions, traverse an array with an explicit length,
  build/query a boundary-indexed prefix table, trace lower/upper bounds, and keep
  a C string inside its destination capacity.
- **Practice:** complete the [Week 2 exercise](lecture_exercises/week02_ex.md)
  and use its test driver before opening [the complete example](examples.c).
  The prefix implementation remains undisclosed; the example demonstrates the
  neighboring array, boundary-search, and bounded-string techniques.
- **Supporting ideas:** Big-O vocabulary and overflow contracts explain design
  choices; first make the ordinary loop or query correct on the stated inputs.
- **Python bridge:** use the companion for sequence comparisons rather than
  reading it as a second required lecture.

## Learning objectives

By the end of this lecture, you should be able to:

1. Declare, define, and call a C function through a prototype.
2. Explain pass-by-value and use return values for explicit results.
3. Read simple address-passing interfaces that use `&`, `*`, and pointer parameters.
4. Traverse arrays without reading outside their bounds.
5. Build a prefix table and answer half-open range queries from it.
6. Specify lower and upper boundaries in sorted data and trace their binary
   search invariants.
7. Explain the null-terminated representation of a C string.
8. Design interfaces that pass an array together with its length or capacity.

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | How do typed functions decompose a program? | Specify and implement a small function family |
| 2 | How can preprocessing replace repeated query work? | Trace prefix and sorted-boundary queries |
| 3 | How do null-terminated strings remain inside their buffers? | Build and test bounded string utilities |

## Hour 1 — Function contracts and decomposition

### 1. Functions are typed contracts

Python checks a function call while the program runs. A C compiler checks a
prototype before generating the call.

The first new qualifier is `const`, which creates a read-only view through a
particular name or parameter. Read `const int values[]` as “an array of `int`
elements that this function promises not to modify through `values`.” The
compiler will reject an assignment such as `values[0] = 7` inside the function.
It does not make the caller's array permanently immutable: the caller or another
non-`const` access path may still modify it. Week 4 develops the corresponding
pointer types; for now, use `const` on an input array whenever the function only
observes its elements.

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

double mean(const int values[], size_t count) {
  double total = 0.0;
  for (size_t i = 0; i < count; ++i) {
    total += values[i];
  }
  return count == 0 ? 0.0 : total / count;
}
```

Using `double` for the running total avoids signed-integer overflow, but floating-
point addition can round. If an application requires an exact integer sum, give
the input a checked range or use a checked wider integer representation.

Keep the declaration and definition identical. A prototype placed in a header
allows multiple source files to share the same contract.

### 2. C passes arguments by value

Each parameter starts as a copy of the corresponding argument.

```c
void ineffective_swap(int a, int b) {
  int temporary = a;
  a = b;
  b = temporary;
}
```

Calling `ineffective_swap(x, y)` does not modify `x` or `y`. Later we will pass
their addresses when mutation is required. For now, prefer returning the result:

```c
int absolute_value(int value) {
  return value < 0 ? -value : value;
}
```

Precondition: `value != INT_MIN`, because `-INT_MIN` may overflow. Interfaces
should make important preconditions visible in names, documentation, or checks.

### Address-passing bridge

Several ordinary C interfaces cannot wait until the full pointer lecture. Read
these three symbols operationally for now:

```c
int value = 10;
int* address = &value; /* address points to value */
*address = 20;         /* write through the address */
```

- In a declaration, `int* address` means “address of an `int`.”
- In an expression, `&value` obtains the address of `value`.
- In an expression, `*address` designates the pointed-to `int`.

That is enough to repair the swap contract:

```c
void swap(int* left, int* right) {
  int temporary = *left;
  *left = *right;
  *right = temporary;
}

void example(void) {
  int x = 1;
  int y = 2;
  swap(&x, &y);
}
```

Both pointers are borrowed and must designate valid `int` objects for the whole
call. The Week 4 lecture notes develop the complete model: pointer arithmetic, nullability,
array relationships, lifetime, dynamic allocation, and ownership. Until then,
do not infer that every address may be dereferenced or retained.

### Decompose before coding

The previous function notes built a program in stages. For a judge problem that
reads scores, removes one lowest score, and reports a rounded average, first
write contracts rather than a long `main`:

```c
int read_scores(int scores[], size_t capacity, size_t* count);
size_t index_of_minimum(const int scores[], size_t count);
void remove_at(int scores[], size_t* count, size_t index);
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
unsigned long next_sequence(void) {
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
The third design will be analyzed fully after the Week 4 lecture notes.

## Hour 2 — Array layout, prefix queries, and boundary algorithms

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
int maximum(const int values[], size_t count, int* result);
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

### 5. Prefix tables: preprocess repeated range queries

#### A small vocabulary for running time

Before comparing implementations, we need a way to describe how their work
grows with the input. Let `n` be the number of array elements and `q` the number
of queries. **Big-O notation** describes an upper bound on the growth rate; it
does not measure seconds and it normally omits fixed multipliers and smaller
terms.

- **O(1), constant time:** the number of relevant operations does not grow with
  `n`. Reading one array element and subtracting two prefix totals are examples.
- **O(n), linear time:** doubling the number of elements can roughly double the
  work. One complete array traversal is linear.
- **O(log n), logarithmic time:** each step discards a fixed fraction of the
  remaining candidates. Binary search has this shape.
- **O(n log n):** many comparison-based sorting algorithms have this growth
  rate.

Big-O is only one design constraint. Two O(n) loops can have different
constants, memory access patterns, and failure behavior. For this course, first
prove that the algorithm is correct; then use its growth rate to determine
whether it remains practical as the input limits increase.

Suppose a program receives an array once and then answers many questions about
contiguous ranges. Repeating a loop for every query costs time proportional to
the length of every range. A prefix table stores the accumulated total before
each boundary:

```text
values:  [ 3, -1,  4,  2 ]
boundary:  0   1   2   3   4
prefix:  [ 0,  3,  2,  6,  8 ]
```

The invariant is:

```text
prefix[i] = values[0] + values[1] + ... + values[i - 1]
```

The extra leading zero is deliberate. It makes `prefix` have `count + 1`
elements and represents the empty prefix without a special case. The total of
the half-open range `[left, right)` is therefore:

```text
prefix[right] - prefix[left]
```

For the table above, `[1, 4)` totals `8 - 3 = 5`. This matches C's usual loop
boundary: start at `left` and continue while `i < right`.

### Build/query contracts before implementation

Design two interfaces rather than hiding preprocessing inside `main`:

```c
#include <stddef.h>
#include <stdint.h>

int build_prefix(const int values[], size_t count, int64_t prefix[],
                 size_t prefix_capacity);

int query_total(const int64_t prefix[], size_t prefix_count, size_t left,
                size_t right, int64_t* result);
```

The first requires space for `count + 1` accumulated values. The second must
validate `left <= right` and `right < prefix_count`. Both should state how
arithmetic overflow is prevented or reported; using `int64_t` widens the common
case but is not a mathematical proof that every possible input fits.

With `n` values and `q` queries, preprocessing plus constant-time queries costs
O(n + q), compared with O(nq) in the worst case when each query scans its
range. The tradeoff is O(n) additional storage and the need to rebuild or
update the table if an input value changes.

### Prefixes of derived contributions

The accumulated value need not be the original element. A program can first
define a contribution—for example, `1` when a reading satisfies a condition and
`0` otherwise—and then prefix those contributions to count qualifying elements
in any range. Keep the transformation and range convention explicit; changing
either changes the meaning of every query.

### Prefix-table checkpoint

For `values = {5, -2, 0, 7, -3}`, build the six boundary totals by hand. Answer
`[0, 0)`, `[0, 3)`, `[2, 5)`, and `[4, 5)`. Then specify expected rejection for
three invalid boundary pairs. Only after the table and expectations are fixed,
write the two function bodies and compare their results with a direct loop.

### 6. Lower and upper boundaries in sorted data

When equal values form one contiguous block in an ascending sorted array, two
boundary queries describe that block precisely:

- **lower bound:** first position whose value is not less than the target;
- **upper bound:** first position whose value is greater than the target.

Both return the past-the-end position `count` when no element satisfies the
condition. If `lower` and `upper` are the two results, the sorted range is
partitioned as:

```text
[0, lower)       values < target
[lower, upper)   values equivalent to target
[upper, count)   values > target
```

Therefore `lower == upper` means the target is absent, and `upper - lower` is
the size of its equal block. The same boundaries also identify where a value
could be inserted while preserving order.

### A monotone-predicate view of binary search

Do not memorize two nearly identical loops. For lower bound, search for the
first index where `values[index] >= target` becomes true. For upper bound,
replace the predicate with `values[index] > target`. In both cases, maintain a
half-open candidate interval `[low, high)` containing the first true boundary.

A design trace must state:

- everything before `low` is known to make the predicate false;
- everything at or after `high` is known to make it true, with `count` acting
  as a valid sentinel boundary;
- each comparison removes `mid` from the candidate interval or makes it the new
  boundary, so the interval strictly shrinks;
- the midpoint is formed as `low + (high - low) / 2`, avoiding addition
  overflow from `(low + high) / 2`.

Write only the invariant and interval updates first. Test the trace on an empty
array, one element, all-equal values, a target below every value, a target above
every value, and duplicates at both ends. A conventional equality-returning
binary search is insufficient because it may find any duplicate rather than a
specified boundary.

### Sorting is a precondition, not part of the search

Boundary search requires an ascending sorted range. The search function should
state that precondition rather than silently sorting its input, because sorting
would modify the order and change the operation's running time. For now, use
data that is already sorted or the insertion-sort extension at the end of this
note. Week 4 introduces C's generic `qsort` interface after function pointers
and comparator contracts can be explained properly.

Sorting once and answering `q` boundary queries costs O(n log n + q log n).
Scanning the unsorted array for each query costs O(nq), but preserves original
order and needs no sorting. Choose from the complete workload and data contract,
not from the query operation alone.

### Boundary-search checkpoint

For `{-3, -1, -1, -1, 2, 5, 5}`, fill a table of lower and upper positions for
targets `-4`, `-1`, `0`, `5`, and `8`. For each comparison, record `[low, high)`
and the truth value of the relevant predicate. Then write function contracts
for the two searches without writing their bodies.

## Hour 3 — String representation, bounded input, and parsing

### 7. Strings are character arrays with a sentinel

```c
char language[] = "C17";
```

The array contains four characters: `'C'`, `'1'`, `'7'`, and the terminating
null character `'\0'`. Library functions find the end by scanning for this
sentinel. If the terminator is missing, a string function may continue beyond
the array.

```c
#include <string.h>

size_t length = strlen(language); /* 3, not 4 */
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

Capacity and logical length are different properties in every sequence
representation. Keeping them separate here prepares us to reason about dynamic
arrays and other containers later without depending on any one language API.

### 8. Reading a line safely

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
size_t string_length(const char text[]) {
  size_t length = 0;
  while (text[length] != '\0') ++length;
  return length;
}

int string_copy(char destination[], size_t capacity, const char source[]) {
  size_t length = string_length(source);
  if (length >= capacity) return 0;

  for (size_t i = 0; i <= length; ++i) {
    destination[i] = source[i]; /* includes '\0' */
  }
  return 1;
}
```

The copy loop uses `<= length` deliberately. A successful string copy must copy
the terminator as well as visible characters. This function follows an
**all-or-nothing** contract: if the complete source does not fit, it reports
failure and leaves the destination unchanged. The exercise later in this week
deliberately explores a different, truncating contract so that the two policies
can be compared. Discuss why calling either function on a nonterminated array
violates its precondition.

### Validate the line representation before processing it

A successful `fgets` call does not guarantee that the whole logical line fit in
the array. Search for `\n`. If it is present, replace it with `\0`; if it is
absent and the program has not reached end-of-file, the input line was longer
than the buffer and the rest must be rejected or discarded deliberately.

This validation order illustrates a reusable principle:

1. establish where the valid data ends;
2. establish that its representation is complete;
3. only then interpret its contents.

Converting a substring into a number requires an interface that reports both
the converted value and where conversion stopped. That interface is deferred
until Week 4, when pointer-valued positions and their lifetime rules have been
introduced fully.

### Hour 3 studio

Write `count_words` for a null-terminated character array. A word is one or more
non-whitespace characters, and any run of whitespace separates words. Trace a
Boolean state such as `inside_word` across an empty string, leading/trailing
spaces, and repeated separators. The important technique is recognizing a
transition from “outside” to “inside,” not memorizing a library function.

## Worked example: one-pass minimum

```c
#include <stddef.h>
int minimum(const int values[], size_t count) {
  int result = values[0];
  for (size_t i = 1; i < count; ++i) {
    if (values[i] < result) {
      result = values[i];
    }
  }
  return result;
}
```

The precondition is `count > 0`; the caller must establish it before the call.
At the start of each iteration, `result` is the minimum of the already-processed
half-open range `[0, i)`. The next comparison extends that claim to `[0, i + 1)`.
This is an example of a **loop invariant**: a statement that is true before and
after every iteration and explains why the final answer is correct.

A production interface may need to represent an empty result. Week 3 introduces
structures that can combine status and data, and Week 4 develops output-pointer
interfaces. This week's version keeps the focus on array bounds, function
preconditions, and the traversal proof.

## Check yourself

1. Why does `sizeof parameter / sizeof parameter[0]` fail in a function?
2. How many bytes are required to store the string `"tree"` as a `char` array?
3. Why does a prefix table for `count` values contain `count + 1` entries?
4. Express the inclusive mathematical range `left` through `right` as a C-style
   half-open range, checking for overflow in the boundary conversion.
5. State the three sorted regions defined by lower and upper bounds.
6. Why can ordinary binary search return the wrong position for duplicates?
7. Design a function to reverse an array. What must its contract include?
8. Find the off-by-one error in `for (i = 0; i <= count; ++i)`.
9. What should a string-building function know besides the current length?

## Summary

- Prototypes make function contracts available to the compiler.
- Arguments are passed by value; mutation requires explicit indirection.
- C arrays are contiguous and have no run-time length metadata.
- Prefix preprocessing turns repeated range totals into boundary subtraction.
- Lower and upper bounds locate the edges of an equal block in sorted data.
- A C string is an array convention: characters followed by `\0`.
- Pair every array with its length and every output buffer with its capacity.

## Optional enrichment and lab extensions

The following topics are useful applications of the same representation and
boundary rules, but they are not part of the three-hour lecture core.

### Two-dimensional arrays and row-major layout

```c
int matrix[3][4] = {0};
```

Elements are stored row by row. When passing this array, the compiler must know
the column stride:

```c
int sum_matrix(size_t rows, size_t columns, const int matrix[rows][columns]) {
  int total = 0;
  for (size_t r = 0; r < rows; ++r) {
    for (size_t c = 0; c < columns; ++c) {
      total += matrix[r][c];
    }
  }
  return total;
}
```

This VLA-parameter form requires `columns > 0`, a valid matrix whenever
`rows > 0`, and a mathematical sum representable as `int`. A production
interface that permits zero columns or unrestricted totals needs a different
representation or result contract.

The conceptual byte offset of `matrix[r][c]` is
`(r * columns + c) * sizeof(int)`. Draw a `2 x 3` matrix as six consecutive
cells and explain why the column count is part of the interface contract.

### Implementing a simple sort

```c
void insertion_sort(int values[], size_t count) {
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

Before iteration `i`, `[0, i)` is sorted and contains the original prefix's
values. Trace `{4, 2, 2, 1}` and identify what makes equal elements stable.

## References and source materials

- [Functions](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/function/function.md>)
- [Arrays](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/array/array.md>)
- [C strings](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/Printf%20and%20Scanf/String%20type.md>)
- [Input and output](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/Input%20and%20output/Input%20and%20output.md>)
