# Week 2 Lecture Notes — Functions, Arrays, and Strings in C

> September 15, 2026 · Source lineage: previous function, array, string, and
> input notes, reorganized around comparisons with Python sequences

> Python bridge: [Python Contrast Companion for Week 2](week02_python_companion.md)

---

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

---

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

---

## Three-hour plan

| Hour | Main question | In-class production |
|------|---------------|---------------------|
| 1 | How do typed functions decompose a program? | Specify and implement a small function family |
| 2 | How can precomputation replace repeated query work? | Trace prefix and sorted-boundary queries |
| 3 | How do null-terminated strings remain inside their buffers? | Build and test bounded string utilities |

Each hour interleaves about 35–45 minutes of explanation and live coding with
roughly 15–20 minutes of core practice. The remaining time supports discussion,
transitions, and a short break; optional exercises can use that buffer when the
class is ready.

### Inline practice routine

Each **Try it now** stop asks you to retrieve and apply the idea that immediately
precedes it. Use a small scratch source file unless the question asks only for a
trace or contract:

1. predict the result, state change, or diagnostic before compiling;
2. make the requested change yourself;
3. compile C code with `-std=c17 -Wall -Wextra -Wpedantic`;
4. run the stated normal and boundary cases; and
5. explain which contract or invariant justifies the result.

Only the question is visible initially. Expand **Reveal solution** after making
and testing your own attempt. A solution panel gives the expected output, trace,
or reason that a declaration-only example has no run-time output. It does not
reveal the prefix-table implementation used in the separate exercise.

- **Core live:** part of the planned in-class path.
- **Extension:** additional practice for the lab, a break, or later study.

The core-live exercises total about 16 minutes in Hour 1, 18 minutes in Hour 2,
and 19 minutes in Hour 3.

---

## Hour 1 — Function contracts and decomposition

> **Hour 1 route:** [Functions are typed contracts](#1-functions-are-typed-contracts)
> → [C passes arguments by value](#2-c-passes-arguments-by-value)
> → [Address-passing bridge](#address-passing-bridge)
> → [Decompose before coding](#decompose-before-coding)
> → [Scope, storage duration, and `static` locals](#scope-storage-duration-and-static-locals)
> → [contract checkpoint](#try-it-now-core-live--hour-1-contract-checkpoint-4-minutes)

### 1. Functions are typed contracts

Python checks a function call while the program runs. A C compiler checks a
prototype before generating the call.

```c
int clamp_value(int value, int low, int high);
```

This declaration promises:

- the function is named `clamp_value`;
- it receives three `int` values;
- it returns one `int`; and
- callers may use the prototype before the full definition appears.

The definition provides the implementation:

```c
int clamp_value(int value, int low, int high) {
  if (value < low) {
    return low;
  }
  if (value > high) {
    return high;
  }
  return value;
}
```

The precondition is `low <= high`. The postcondition is that the result lies in
the closed interval from `low` through `high`: values below the interval become
`low`, values above it become `high`, and values already inside are unchanged.

Keep the declaration and definition identical. A prototype placed in a header
allows multiple source files to share the same contract.

#### Try it now [Core live] — call the contract (3 minutes)

Call `clamp_value` for `-3`, `7`, and `20` with the interval `[0, 10]`. Predict
the three results, then print them on one line.

<details>
<summary>Reveal solution</summary>

```c
#include <stdio.h>

int main(void) {
  printf("%d %d %d\n", clamp_value(-3, 0, 10),
         clamp_value(7, 0, 10), clamp_value(20, 0, 10));
  return 0;
}
```

The three calls exercise the below-range, inside-range, and above-range cases.

**Expected output:**

```text
0 7 10
```

The prototype by itself produces no run-time output; it only gives the compiler
the function's name, parameter types, and result type.

</details>

---

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

#### Try it now [Core live] — distinguish mutation from rebinding (2 minutes)

Start with `x = 3` and `y = 8`. Call `ineffective_swap(x, y)`, then print both
variables and `absolute_value(-7)`. Predict the output before compiling.

<details>
<summary>Reveal solution</summary>

```c
int main(void) {
  int x = 3;
  int y = 8;
  ineffective_swap(x, y);
  printf("x=%d y=%d absolute=%d\n", x, y, absolute_value(-7));
  return 0;
}
```

This fragment belongs in a file that includes `<stdio.h>` and contains the two
function definitions above. Assigning to `a` and `b` changes only their local
copies.

**Expected output:**

```text
x=3 y=8 absolute=7
```

</details>

---

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

#### Try it now [Core live] — trace an address-based update (3 minutes)

Add a `printf` call after `swap(&x, &y)` in `example`, or perform the same call
inside `main`. Predict the values of `x` and `y`, then identify exactly which two
assignments modify the caller's objects.

<details>
<summary>Reveal solution</summary>

```c
int main(void) {
  int x = 1;
  int y = 2;
  swap(&x, &y);
  printf("x=%d y=%d\n", x, y);
  return 0;
}
```

The assignments `*left = *right` and `*right = temporary` write through the
borrowed addresses. This fragment requires `<stdio.h>` and the `swap` definition
above.

**Expected output:**

```text
x=2 y=1
```

</details>

---

### Decompose before coding

The previous function notes built a program in stages. For a judge problem that
reads scores, removes one lowest score, and reports a rounded average, first
write contracts rather than a long `main`:

The declarations below preview one notation used in Hour 2: an array parameter
such as `int scores[]` designates a sequence whose element count must arrive in
a separate parameter. Read that role operationally here; the contiguous array
representation and its adjustment to a pointer are explained before any array
implementation later in this note. The `const` in `const int scores[]` records
that the function observes those elements without modifying them; Hour 2
develops that promise in context.

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

#### Try it now [Core live] — annotate four interfaces (4 minutes)

For each prototype, mark every parameter as input, output, or input/output.
State the empty-input policy and failure channel. Do not write the function
bodies yet.

<details>
<summary>Reveal solution</summary>

One coherent contract set is:

| Function | Parameter roles | Empty input and failure policy |
|----------|-----------------|--------------------------------|
| `read_scores` | `scores` is output storage; `capacity` is input; `count` is output | Empty input may succeed with `*count == 0`; return zero for malformed input or insufficient capacity |
| `index_of_minimum` | `scores` and `count` are input | Require `count > 0`; no separate failure result is available |
| `remove_at` | `scores` and `count` are input/output; `index` is input | Require `index < *count`; the prototype provides no failure result |
| `mean` | `scores` and `count` are input | The earlier implementation defines an empty mean as `0.0` |

These declarations produce no run-time output. The exercise is about making
their contracts explicit before implementation. A production design could
change a `void` or index result to `int` when invalid input must be reported.

</details>

---

### Scope, storage duration, and `static` locals

> **Supporting C feature:** local variables normally exist only during one
> function call. Read this section to recognize the less common case in which a
> local name refers to storage that lasts for the whole program; ordinary local
> variables remain the default in this course.

Ordinary local variables are created on entry and cease to exist on return.
A `static` local retains its value for the program's lifetime:

```c
unsigned long next_sequence(void) {
  static unsigned long value = 0;
  return ++value;
}
```

This hidden state can be useful, but every caller shares it and tests become
order-dependent. Prefer state passed explicitly through a parameter when it is
part of the function's contract; Week 3 introduces structures for grouping
several related state values.

#### Try it now [Extension] — expose persistent local state (2 minutes)

Call `next_sequence` three times in one `printf` statement. Then rewrite the
test using three separate calls. Why is the second form better for reasoning
about the output?

<details>
<summary>Reveal solution</summary>

Do not rely on the evaluation order of function-call arguments to associate the
three returned values with three textual positions. Store the results in
separate statements:

```c
unsigned long first = next_sequence();
unsigned long second = next_sequence();
unsigned long third = next_sequence();
printf("%lu %lu %lu\n", first, second, third);
```

**Expected output in a fresh program:**

```text
1 2 3
```

The explicit sequence also makes it clear that earlier calls in the same
process would change these numbers.

</details>

---

### Try it now [Core live] — Hour 1 contract checkpoint (4 minutes)

Write a prototype and five-line contract for a function that finds a target in
an integer array. Compare three result designs: return an index with a sentinel,
return success plus an output parameter, or return a pointer to the element.
The third design will be analyzed fully after the Week 4 lecture notes.

<details>
<summary>Reveal solution</summary>

Three possible interfaces are:

```c
size_t find_index_or_count(const int values[], size_t count, int target);
int find_index(const int values[], size_t count, int target, size_t* result);
const int* find_element(const int values[], size_t count, int target);
```

- The first can return `count` as a past-the-end “not found” sentinel.
- The second returns success separately and writes an index only on success.
- The third can return a pointer to the element or `NULL`, but its validity and
  lifetime require the Week 4 pointer model. `NULL` is C's conventional way to
  say that a pointer designates no object; Hour 3 introduces it through `fgets`.

All three require a valid readable range of `count` elements, preserve the
array, and return the first match if duplicates exist. These are declarations,
so they produce no run-time output.

</details>

---

## Hour 2 — Array layout, prefix queries, and boundary algorithms

> **Hour 2 route:** [Arrays are contiguous fixed-size storage](#3-arrays-are-contiguous-fixed-size-storage)
> → [Boundary reasoning](#4-boundary-reasoning)
> → [one-pass minimum](#worked-example-one-pass-minimum)
> → [Prefix tables: precompute repeated range queries](#5-prefix-tables-precompute-repeated-range-queries)
> → [Build/query contracts before implementation](#buildquery-contracts-before-implementation)
> → [Prefixes of derived contributions](#prefixes-of-derived-contributions)
> → [prefix-table checkpoint](#try-it-now-extension--prefix-table-checkpoint-4-minutes)
> → [Lower and upper boundaries in sorted data](#6-lower-and-upper-boundaries-in-sorted-data)
> → [A monotone-predicate view of binary search](#a-monotone-predicate-view-of-binary-search)
> → [Sorting is a precondition, not part of the search](#sorting-is-a-precondition-not-part-of-the-search)
> → [boundary-search checkpoint](#try-it-now-extension--boundary-search-checkpoint-4-minutes)

> **Algorithm applications:** prefix tables and boundary search develop array
> invariants and indexing discipline. They are problem-solving techniques, not
> additional C syntax; trace the contracts before memorizing either loop.

### 3. Arrays are contiguous fixed-size storage

```c
int scores[5] = {91, 82, 73, 94, 85};
```

The array contains five adjacent `int` objects indexed from `0` through `4`.
Unlike a Python list, it does not remember a run-time length and cannot grow.

Inside the same scope as the array declaration:

```c
size_t count = sizeof(scores) / sizeof(scores[0]);
```

As in Week 1, `size_t` is the standard unsigned type used for object sizes and
array indices; `<stddef.h>` provides its declaration. `sizeof(scores)` is the
total storage in bytes, and `sizeof(scores[0])` is the storage for one element,
so their quotient is the element count.

This expression does **not** work in a function parameter. In most expressions,
an array is converted to a pointer to its first element. Therefore every general
array function must receive a length explicitly.

The qualifier `const` creates a read-only access path. Read
`const int values[]` as “an array of `int` elements that this function promises
not to modify through `values`.” The compiler rejects `values[0] = 7` inside
such a function. It does not make the caller's array permanently immutable: the
caller or another non-`const` access path may still modify it. Week 4 develops
the corresponding pointer types; for now, use `const` whenever an array
parameter is input-only.

```c
double mean(const int values[], size_t count) {
  double total = 0.0;
  for (size_t i = 0; i < count; ++i) {
    total += values[i];
  }
  return count == 0 ? 0.0 : total / count;
}
```

Using `double` for the running total avoids signed-integer overflow, although
floating-point addition can round. If exact integer accumulation is required,
the contract must instead bound the input or check arithmetic in a suitable
integer type. This implementation deliberately defines the mean of an empty
range as `0.0`; another application could reject it instead.

```c
int maximum(const int values[], size_t count, int* result);
```

The return value can report whether a maximum exists; `result` can hold the
answer. We introduce this output-parameter style fully with pointers.

#### Try it now [Core live] — separate capacity from element count (2 minutes)

Print the element count of `scores` with `%zu` and its mean with one digit after
the decimal point. Then imagine passing `scores` to `maximum`: which value must
travel beside the array, and why can the function not recover it with `sizeof`?

<details>
<summary>Reveal solution</summary>

```c
int main(void) {
  int scores[5] = {91, 82, 73, 94, 85};
  size_t count = sizeof(scores) / sizeof(scores[0]);
  printf("count=%zu mean=%.1f\n", count, mean(scores, count));
  return 0;
}
```

This fragment requires `<stddef.h>` and `<stdio.h>`. The call to `maximum` must
receive `count` explicitly. In a function parameter, the array notation is
adjusted to a pointer type, so `sizeof(values)` would measure that pointer, not
the caller's array.

**Expected output:**

```text
count=5 mean=85.0
```

The `maximum` prototype itself is a declaration and produces no output.

</details>

---

### 4. Boundary reasoning

For `count` valid elements, the canonical traversal appears in this complete
function:

```c
int sum_array(const int values[], size_t count) {
  int total = 0;

  for (size_t i = 0; i < count; ++i) {
    total += values[i];
  }
  return total;
}
```

The function requires the mathematical sum to be representable as `int`.
Later integer-arithmetic exercises will develop checked alternatives for wider
input domains.

Ask three questions about every loop:

1. What is the first valid index?
2. What is the first invalid index?
3. Does the loop condition exclude the first invalid index?

Accessing `values[count]` is undefined behavior. C has no automatic bounds
check and no `IndexError`.

#### Try it now [Core live] — defend the first invalid boundary (3 minutes)

Call `sum_array` for `{4, -1, 3}`. Predict the result. Then explain why changing
`i < count` to `i <= count` is not a valid way to include the last element.
Repair the condition before running the program.

<details>
<summary>Reveal solution</summary>

```c
int main(void) {
  int values[] = {4, -1, 3};
  printf("sum=%d\n", sum_array(values, 3));
  return 0;
}
```

The valid indices are `0`, `1`, and `2`; the condition `i < 3` visits all three.
With `i <= 3`, the last iteration would evaluate `values[3]`, outside the array,
and the program would have undefined behavior. Do not run that defective form.

**Expected output of the correct program:**

```text
sum=6
```

</details>

---

### Worked example: one-pass minimum

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

#### Try it now [Core live] — state and use the loop invariant (3 minutes)

Trace `minimum` on `{8, -4, 6, -4}`. Before each iteration, record the processed
range and `result`. Then test the function in a small `main`. Do not call it with
an empty array because that would violate its stated precondition.

<details>
<summary>Reveal solution</summary>

| Before iteration | Processed range | `result` |
|------------------|-----------------|----------|
| `i == 1` | `[0, 1)` → `{8}` | `8` |
| `i == 2` | `[0, 2)` → `{8, -4}` | `-4` |
| `i == 3` | `[0, 3)` → `{8, -4, 6}` | `-4` |
| After the loop | `[0, 4)` → all elements | `-4` |

```c
int main(void) {
  int values[] = {8, -4, 6, -4};
  printf("minimum=%d\n", minimum(values, 4));
  return 0;
}
```

This fragment requires `<stdio.h>` and the `minimum` definition above.

**Expected output:**

```text
minimum=-4
```

</details>

---

### 5. Prefix tables: precompute repeated range queries

Here **precomputation** means performing an algorithmic preparation pass before
the queries arrive. It is unrelated to the C preprocessor that expands
`#include` and macros before compilation.

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

#### Try it now [Core live] — subtract boundaries (4 minutes)

Using the displayed prefix table, calculate `[0, 2)`, `[2, 4)`, and `[4, 4)`.
For each result, also list the original array elements included by the half-open
range.

<details>
<summary>Reveal solution</summary>

| Range | Included values | Boundary subtraction | Total |
|-------|-----------------|----------------------|-------|
| `[0, 2)` | `3, -1` | `prefix[2] - prefix[0] = 2 - 0` | `2` |
| `[2, 4)` | `4, 2` | `prefix[4] - prefix[2] = 8 - 2` | `6` |
| `[4, 4)` | none | `prefix[4] - prefix[4] = 8 - 8` | `0` |

This is a trace rather than an executable program, so it has no standard
output. Notice that the empty range needs no special-case formula.

</details>

---

### Build/query contracts before implementation

Design two interfaces rather than hiding precomputation inside `main`:

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

With `n` values and `q` queries, precomputation plus constant-time queries costs
O(n + q), compared with O(nq) in the worst case when each query scans its
range. The tradeoff is O(n) additional storage and the need to rebuild or
update the table if an input value changes.

#### Try it now [Extension] — audit the two interfaces (3 minutes)

For four input values, determine the minimum valid `prefix_capacity`. After a
successful build, determine the `prefix_count` passed to `query_total` and the
largest valid value of `right`. Explain what each function should do when its
capacity or range check fails.

<details>
<summary>Reveal solution</summary>

- Four input values require five prefix entries, so `prefix_capacity` must be
  at least `5`.
- The resulting `prefix_count` is `5`.
- Because the range contract checks `right < prefix_count`, the largest valid
  right boundary is `4`.
- A failed build or query returns zero. A failed query must not publish a value
  through `result`.

These declaration-only interfaces produce no run-time output. The prefix-table
function bodies remain an exercise; the contracts above are the oracle for
their implementation.

</details>

---

### Prefixes of derived contributions

The accumulated value need not be the original element. A program can first
define a contribution—for example, `1` when a reading satisfies a condition and
`0` otherwise—and then prefix those contributions to count qualifying elements
in any range. Keep the transformation and range convention explicit; changing
either changes the meaning of every query.

#### Try it now [Core live] — prefix a predicate (3 minutes)

For `{-2, 5, 0, 7}`, define each contribution as `1` when the value is positive
and `0` otherwise. Write the boundary-indexed contribution prefix and use it to
count positive values in `[1, 4)`.

<details>
<summary>Reveal solution</summary>

```text
values:        [ -2, 5, 0, 7 ]
contribution:  [  0, 1, 0, 1 ]
prefix:        [ 0, 0, 1, 1, 2 ]
```

The query is `prefix[4] - prefix[1] = 2 - 0 = 2`. This hand trace has no
standard output and deliberately does not reveal the exercise's C
implementation.

</details>

---

### Try it now [Extension] — prefix-table checkpoint (4 minutes)

For `values = {5, -2, 0, 7, -3}`, build the six boundary totals by hand. Answer
`[0, 0)`, `[0, 3)`, `[2, 5)`, and `[4, 5)`. Then specify expected rejection for
three invalid boundary pairs. Only after the table and expectations are fixed,
write the two function bodies and compare their results with a direct loop.

<details>
<summary>Reveal solution</summary>

The boundary totals for the original values—not the positive-only
contributions from the separate exercise—are:

```text
values:  [ 5, -2, 0, 7, -3 ]
prefix:  [ 0,  5, 3, 3, 10, 7 ]
```

| Range | Result |
|-------|--------|
| `[0, 0)` | `0` |
| `[0, 3)` | `3` |
| `[2, 5)` | `4` |
| `[4, 5)` | `-3` |

Examples of invalid boundaries are `(3, 2)` because `left > right`, `(0, 6)`
because `right >= prefix_count`, and `(6, 6)` because neither boundary belongs
to `[0, prefix_count)`. The panel supplies expected results, not the two C
function bodies. This is a hand trace and therefore has no standard output.

</details>

---

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

For the target `-1`, the sample array below makes those positions concrete:

```text
index:    0   1   2   3   4   5   6
value:   -3  -1  -1  -1   2   5   5
             ^ lower = 1
                         ^ upper = 4
```

Therefore `lower == upper` means the target is absent, and `upper - lower` is
the size of its equal block. The same boundaries also identify where a value
could be inserted while preserving order.

#### Try it now [Core live] — identify an equal block (3 minutes)

For the displayed array, identify the lower bound, upper bound, and duplicate
count for targets `-1` and `4`. Do not perform binary search yet; use only the
three-region definition.

<details>
<summary>Reveal solution</summary>

| Target | Lower bound | Upper bound | Count |
|--------|-------------|-------------|-------|
| `-1` | `1` | `4` | `3` |
| `4` | `5` | `5` | `0` |

For `4`, index `5` is both the first position whose value is at least `4` and
the first position whose value is greater than `4`. Equal boundaries therefore
describe an empty equal block. This hand classification has no run-time output.

</details>

---

### A monotone-predicate view of binary search

Do not memorize two nearly identical loops. For lower bound, search for the
first index where `values[index] >= target` becomes true. For upper bound,
replace the predicate with `values[index] > target`. In both cases, maintain a
half-open interval `[low, high)` of elements not yet classified. The boundary
is a position that remains somewhere from `low` through `high`, inclusive; it
may equal `count` when no array element makes the predicate true.

A design trace must state:

- everything before `low` is known to make the predicate false;
- every real index in `[high, count)` is known to make it true; if no real
  element is true, `count` is a virtual true boundary and is never accessed;
- each comparison removes `mid` from the candidate interval or makes it the new
  boundary, so the interval strictly shrinks;
- the midpoint is formed as `low + (high - low) / 2`, avoiding addition
  overflow from `(low + high) / 2`.

Write only the invariant and interval updates first. Test the trace on an empty
array, one element, all-equal values, a target below every value, a target above
every value, and duplicates at both ends. A conventional equality-returning
binary search is insufficient because it may find any duplicate rather than a
specified boundary.

#### Try it now [Extension] — trace first true (4 minutes)

Trace the lower-bound predicate `values[index] >= -1` on the displayed array.
Record `(low, high, middle)` before each update, then repeat with the upper-bound
predicate `values[index] > -1`.

<details>
<summary>Reveal solution</summary>

Lower bound:

| `low` | `high` | `middle` | Value | `value >= -1` | Update |
|-------|--------|----------|-------|---------------|--------|
| `0` | `7` | `3` | `-1` | true | `high = 3` |
| `0` | `3` | `1` | `-1` | true | `high = 1` |
| `0` | `1` | `0` | `-3` | false | `low = 1` |

The interval is empty at `[1, 1)`, so the lower boundary is `1`.

Upper bound:

| `low` | `high` | `middle` | Value | `value > -1` | Update |
|-------|--------|----------|-------|--------------|--------|
| `0` | `7` | `3` | `-1` | false | `low = 4` |
| `4` | `7` | `5` | `5` | true | `high = 5` |
| `4` | `5` | `4` | `2` | true | `high = 4` |

The interval is empty at `[4, 4)`, so the upper boundary is `4`. These are
algorithm traces, not program output.

</details>

---

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

---

### Try it now [Extension] — boundary-search checkpoint (4 minutes)

For `{-3, -1, -1, -1, 2, 5, 5}`, fill a table of lower and upper positions for
targets `-4`, `-1`, `0`, `5`, and `8`. For each comparison, record `[low, high)`
and the truth value of the relevant predicate. Then write function contracts
for the two searches without writing their bodies.

<details>
<summary>Reveal solution</summary>

| Target | Lower bound | Upper bound | Equal-block size |
|--------|-------------|-------------|------------------|
| `-4` | `0` | `0` | `0` |
| `-1` | `1` | `4` | `3` |
| `0` | `4` | `4` | `0` |
| `5` | `5` | `7` | `2` |
| `8` | `7` | `7` | `0` |

Both functions require an ascending sorted range of `count` readable elements,
preserve that range, and return a position in `[0, count]`. The lower-bound
result is the first value at least the target; the upper-bound result is the
first value greater than the target. The table is expected trace output; the C
function bodies remain for the exercise.

</details>

---

## Hour 3 — String representation, bounded input, and parsing

> **Hour 3 route:** [Strings are character arrays with a sentinel](#7-strings-are-character-arrays-with-a-sentinel)
> → [Capacity versus length](#capacity-versus-length)
> → [Reading a line safely](#8-reading-a-line-safely)
> → [Implement library ideas once](#implement-library-ideas-once)
> → [Validate the line representation before processing it](#validate-the-line-representation-before-processing-it)
> → [word-count studio](#try-it-now-core-live--hour-3-word-count-studio-5-minutes)

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

#### Try it now [Core live] — count storage and text separately (2 minutes)

Print both `sizeof(language)` and `strlen(language)`. Predict why the two
numbers differ by one.

<details>
<summary>Reveal solution</summary>

```c
printf("storage=%zu length=%zu\n", sizeof(language), strlen(language));
```

The array owns four `char` objects, but the string length counts only the three
characters before the first null character. This fragment belongs after the
declaration of `language` in a program that includes `<stdio.h>` and
`<string.h>`.

**Expected output:**

```text
storage=4 length=3
```

</details>

---

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

#### Try it now [Core live] — reserve the terminator (2 minutes)

Change the declaration to `char name[4] = "Ada";`. Determine its capacity,
length, and remaining space for additional visible characters. Then predict
whether appending one visible character would fit.

<details>
<summary>Reveal solution</summary>

The capacity is `4`, the current length is `3`, and the remaining visible-text
capacity is `0`. The fourth element already stores `\0`, so another visible
character would require at least a five-element destination.

```c
printf("capacity=%zu length=%zu available=%zu\n", sizeof(name), strlen(name),
       sizeof(name) - strlen(name) - 1);
```

**Expected output:**

```text
capacity=4 length=3 available=0
```

</details>

---

### 8. Reading a line safely

Week 1 introduced numeric format contracts. For a C string, `%s` has two
different but related roles:

- `printf("%s", text)` reads characters starting at `text` and prints until
  the first `\0`;
- `scanf("%31s", word)` skips leading whitespace, reads at most 31 non-whitespace
  characters, writes a terminating `\0`, and stops at whitespace.

The input field width must leave one array element for the terminator. It is a
literal maximum in the format string, so a 32-element destination pairs with
`%31s`:

```c
#include <stdio.h>

int main(void) {
  char word[32];
  if (scanf("%31s", word) != 1) {
    return 1;
  }
  printf("word=%s\n", word);
  return 0;
}
```

#### Try it now [Core live] — distinguish a word from a line (3 minutes)

Run the program with the input `Ada Lovelace`. Predict what it prints and what
input remains unread. Then explain why a width protects the destination but
does not turn `%s` into a whole-line parser.

<details>
<summary>Reveal solution</summary>

**Expected standard output:**

```text
word=Ada
```

The separating space ends the conversion, and `Lovelace` remains in `stdin` for
a later input operation. A token longer than 31 characters is also only partly
consumed. The width prevents this call from writing beyond `word`; the program
must still decide what to do with any remaining input.

</details>

For a whole line, prefer a bounded line read and then parse. This first version
assumes the input contract guarantees that the line fits in the array:

```c
#include <stdio.h>
#include <string.h>

int main(void) {
  char line[128];
  if (fgets(line, sizeof(line), stdin) == NULL) {
    return 1;
  }

  line[strcspn(line, "\n")] = '\0';
  printf("You entered %zu characters: \"%s\"\n", strlen(line), line);
  return 0;
}
```

`NULL` is C's conventional null-pointer constant: it means that a pointer does
not designate an object. Here, `fgets` returns `NULL` when it cannot read a
line. Week 4 develops null pointers together with pointer validity and dynamic
memory; for now, compare the returned pointer with `NULL` before using it.

On success, `fgets` stores a terminating `\0`, so `strcspn` can safely search
this array for a newline. If one is present, replacing it with `\0` removes the
line ending. If the input is longer than the buffer, however, `fgets` reads only
a prefix. The validation section below shows how to distinguish a complete line
from a truncated one.

#### Try it now [Core live] — observe bounded line input (3 minutes)

Run the program once with `Ada Lovelace` followed by Enter and once with an
empty line. Predict the character counts and output before running it.

<details>
<summary>Reveal solution</summary>

For `Ada Lovelace` followed by a newline:

```text
You entered 12 characters: "Ada Lovelace"
```

For an empty line:

```text
You entered 0 characters: ""
```

The newline is read into the array and then replaced, so it is not part of
either reported length. If immediate end-of-file occurs, the program produces
no standard output and returns a nonzero status.

</details>

---

### Implement library ideas once

Before relying on `<string.h>`, implement two small functions to expose the
sentinel and capacity contracts:

```c
#include <stddef.h>

size_t string_length(const char text[]) {
  size_t length = 0;
  while (text[length] != '\0') {
    ++length;
  }
  return length;
}

int string_copy(char destination[], size_t capacity, const char source[]) {
  size_t length = string_length(source);
  if (length >= capacity) {
    return 0;
  }

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

Both functions require valid arrays and a null-terminated `source`. The copy
contract does not support partially overlapping source and destination ranges,
because a write could change a source character that has not yet been read.

#### Try it now [Core live] — test exact fit and rejection (4 minutes)

Use a four-element destination. First copy `"C17"`; then try to copy `"C17!"`
into the same destination. Predict each status and the destination text after
each call.

<details>
<summary>Reveal solution</summary>

```c
int main(void) {
  char destination[4] = "old";
  int first_status = string_copy(destination, sizeof(destination), "C17");
  printf("status=%d text=%s\n", first_status, destination);

  int second_status = string_copy(destination, sizeof(destination), "C17!");
  printf("status=%d text=%s\n", second_status, destination);
  return 0;
}
```

This fragment requires `<stdio.h>` and the two definitions above. `"C17"`
needs exactly four array elements including `\0`. The second source needs five,
so the all-or-nothing check fails before changing the destination.

**Expected output:**

```text
status=1 text=C17
status=0 text=C17
```

</details>

---

### Validate the line representation before processing it

A successful `fgets` call does not guarantee that the whole logical line fit in
the array. Search for `\n`. If it is present, replace it with `\0`; if it is
absent and the program has not reached end-of-file, the input line was longer
than the buffer and the rest must be rejected or discarded deliberately.

This validation order illustrates a reusable principle:

1. establish where the valid data ends;
2. establish that its representation is complete;
3. only then interpret its contents.

The following helper handles the boundary case in which the array fills just
before a newline. It assumes that `fgets` has already succeeded. If no newline
is stored, it reads one more character: a newline or true end-of-file means the
line fit exactly, while another character proves that the line was truncated.
In the truncated case it discards the rest of that logical line so the next
read begins at a clean boundary.

```c
#include <stdio.h>
#include <string.h>

int finish_bounded_line(char line[]) {
  size_t end = strcspn(line, "\n");
  if (line[end] == '\n') {
    line[end] = '\0';
    return 1;
  }

  int next = fgetc(stdin);
  if (next == '\n') {
    return 1;
  }
  if (next == EOF) {
    return feof(stdin) != 0;
  }

  do {
    next = fgetc(stdin);
  } while (next != '\n' && next != EOF);
  return 0;
}
```

#### Try it now [Extension] — distinguish exact fit from truncation (3 minutes)

With `char line[8]`, trace `fgets` followed by `finish_bounded_line` for the
inputs `Ada\n`, `1234567\n`, and `12345678\n`. Record the returned status and
the text stored in `line`.

<details>
<summary>Reveal solution</summary>

| Input | Text initially stored by `fgets` | Helper status | Meaning |
|-------|-----------------------------------|---------------|---------|
| `Ada\n` | `"Ada\n"` | `1` | Newline was stored and replaced with `\0` |
| `1234567\n` | `"1234567"` | `1` | The helper consumes the following newline; seven visible characters fit exactly |
| `12345678\n` | `"1234567"` | `0` | The following `8` proves truncation; the helper discards through the newline |

The helper itself produces no standard output. A caller must use the stored line
only when the returned status is `1`.

</details>

Converting a substring into a number requires deciding both what the digits
mean and where the conversion stopped. Week 7 develops that conversion directly
in the lexer: it accumulates digits one at a time, checks each step against the
representable range before multiplying, and leaves the scan position on the
first character that is not part of the number. Until then, read numeric input
with the Week 1 `scanf` contracts under their stated input bounds.

---

### Try it now [Core live] — Hour 3 word-count studio (5 minutes)

Write `count_words` for a null-terminated character array. A word is one or more
non-whitespace characters, and any run of whitespace separates words. Trace a
Boolean state such as `inside_word` across an empty string, leading/trailing
spaces, and repeated separators. The important technique is recognizing a
transition from “outside” to “inside,” not memorizing a library function.

<details>
<summary>Reveal solution</summary>

```c
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>

size_t count_words(const char text[]) {
  size_t count = 0;
  int inside_word = 0;

  for (size_t i = 0; text[i] != '\0'; ++i) {
    int whitespace = isspace((unsigned char)text[i]) != 0;
    if (whitespace) {
      inside_word = 0;
    } else if (!inside_word) {
      ++count;
      inside_word = 1;
    }
  }
  return count;
}

int main(void) {
  printf("empty=%zu\n", count_words(""));
  printf("words=%zu\n", count_words("  C  arrays\tand strings "));
  return 0;
}
```

`count` changes only on an outside-to-inside transition. The cast gives
`isspace` a value in its required `unsigned char` domain, avoiding undefined
behavior for a negative plain `char` value.

**Expected output:**

```text
empty=0
words=4
```

</details>

---

## Check yourself

1. Why does `sizeof(parameter) / sizeof(parameter[0])` fail in a function?
2. How many bytes are required to store the string `"tree"` as a `char` array?
3. Why does a prefix table for `count` values contain `count + 1` entries?
4. Express the inclusive mathematical range `left` through `right` as a C-style
   half-open range, checking for overflow in the boundary conversion.
5. State the three sorted regions defined by lower and upper bounds.
6. Why can ordinary binary search return the wrong position for duplicates?
7. Design a function to reverse an array. What must its contract include?
8. Find the off-by-one error in `for (i = 0; i <= count; ++i)`.
9. What should a string-building function know besides the current length?

---

## Summary

- Prototypes make function contracts available to the compiler.
- Arguments are passed by value; mutation requires explicit indirection.
- C arrays are contiguous and have no run-time length metadata.
- Prefix precomputation turns repeated range totals into boundary subtraction.
- Lower and upper bounds locate the edges of an equal block in sorted data.
- A C string is an array convention: characters followed by `\0`.
- Pair every array with its length and every output buffer with its capacity.

---

## Optional enrichment and lab extensions

The following topics are useful applications of the same representation and
boundary rules, but they are not part of the three-hour lecture core.

### Two-dimensional arrays and row-major layout

```c
#define COLUMN_COUNT 4

int matrix[3][COLUMN_COUNT] = {0};
```

Elements are stored row by row. When passing this array, the compiler must know
the column stride:

```c
int sum_matrix(size_t rows, const int matrix[][COLUMN_COUNT]) {
  int total = 0;
  for (size_t r = 0; r < rows; ++r) {
    for (size_t c = 0; c < COLUMN_COUNT; ++c) {
      total += matrix[r][c];
    }
  }
  return total;
}
```

This portable fixed-column form makes the row stride part of the function type.
It requires a valid matrix whenever `rows > 0` and a mathematical sum
representable as `int`. A function that accepts different column counts needs a
different representation or, on implementations that support them, an
explicitly labeled variable-length-array interface.

The conceptual byte offset of `matrix[r][c]` is
`(r * COLUMN_COUNT + c) * sizeof(int)`. Draw a `2 x 4` matrix as eight
consecutive cells and explain why the column count is part of the interface
contract.

#### Try it now [Extension] — flatten a matrix index (3 minutes)

Initialize a `2 x 4` matrix with the values `1` through `8`. Calculate the
linear element offset of `matrix[1][2]`, then call `sum_matrix` for both rows.

<details>
<summary>Reveal solution</summary>

The element offset is `1 * 4 + 2 = 6`, so `matrix[1][2]` is the seventh stored
element and contains `7`.

```c
int main(void) {
  int values[2][COLUMN_COUNT] = {{1, 2, 3, 4}, {5, 6, 7, 8}};
  printf("value=%d sum=%d\n", values[1][2], sum_matrix(2, values));
  return 0;
}
```

This fragment requires `<stdio.h>` and the definition above.

**Expected output:**

```text
value=7 sum=36
```

</details>

---

### Implementing a simple sort

```c
#include <stddef.h>

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

#### Try it now [Extension] — trace stable insertion (4 minutes)

Run the function on `{4, 2, 2, 1}` and print the result. During the trace,
distinguish the first `2` from the second even though their stored integer
values are equal. Which comparison preserves their relative order?

<details>
<summary>Reveal solution</summary>

```c
int main(void) {
  int values[] = {4, 2, 2, 1};
  insertion_sort(values, 4);
  for (size_t i = 0; i < 4; ++i) {
    printf("%s%d", i == 0 ? "" : " ", values[i]);
  }
  printf("\n");
  return 0;
}
```

This fragment requires `<stdio.h>` and the function above.

**Expected output:**

```text
1 2 2 4
```

The loop shifts only while `values[position - 1] > current`. Equal values are
not shifted past one another, so their original relative order is preserved.

</details>

---

## References and source materials

- [Functions](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/function/function.md>)
- [Arrays](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/array/array.md>)
- [C strings](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/Printf%20and%20Scanf/String%20type.md>)
- [Input and output](<https://github.com/htchen/i2p-nthu/blob/master/程式設計一/Input%20and%20output/Input%20and%20output.md>)
