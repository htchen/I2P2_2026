# Python Contrast Companion for Week 1

This companion should be read beside the [Week 1 lecture
notes](week01_lecture.md). It connects familiar Python behavior to the C17
examples without implying that the two languages have the same execution or
type model. The C note remains authoritative for C syntax, compilation, and
exam expectations.

## Relationship labels

- **Direct:** the Python program expresses essentially the same algorithm and
  observable behavior.
- **Same goal, different mechanism:** the result is comparable, but Python uses
  a different operation or representation.
- **No faithful equivalent:** the C example exposes a property that ordinary
  Python does not expose. The Python code is only a contrast.
- **Already Python:** the source block is itself the reference behavior.

## Coverage map

Every fenced example in the Week 1 note is accounted for below.

| Source example | Relationship | Companion treatment |
|---|---|---|
| `cc ... hello.c` and `./hello` | Same goal, different mechanism | Run a source file with `python3`; there is normally no student-visible link step |
| `cc -E`, `-S`, `-c`, and link commands | No faithful equivalent | Contrast bytecode compilation and `dis` with the C translation pipeline |
| Declaration/definition of `twice` | Direct algorithm, different program model | Define `twice` before it is called; Python has no C-style declaration or link error |
| Static objects and `add_one` | No faithful storage-section equivalent | Use module-level names and a local result, then explain what Python hides |
| First `printf` program | Direct | Use `print` and an f-string |
| Scalar C declarations | Same goal, different type model | Use values and type hints; explain arbitrary-precision integers and dynamic checking |
| Basic operators and precedence | Mostly direct, different spelling and numeric rules | Translate arithmetic and assignment; contrast division, remainder, Boolean operators, and increment |
| Integer division and cast | Same goal, different operators | Use `//` for floor division and `/` for real division |
| Boolean eligibility expression | Direct | Use `and` with a Boolean expression |
| `<limits.h>` report | No fixed-width `int` equivalent | Contrast arbitrary-precision Python integers with `sys.maxsize` |
| Signed `index` versus unsigned `count` | No unsigned-conversion equivalent | The ordinary Python comparison remains signed |
| Formatted score/ratio output | Direct | Use an f-string format specification |
| Checked two-integer input | Same goal, different mechanism | Split, convert, validate arity, and catch `ValueError` |
| Division/unsigned checkpoint | Partial | Division translates; unsigned wraparound has no ordinary Python counterpart |
| Python even-sum loop | Already Python | Retained as the reference behavior |
| C even-sum loop | Direct | It maps back to the existing Python loop |
| C `switch` | Same goal, different mechanism | Use `if`/`elif` or `match`; no fallthrough is needed here |
| `scanf` loop through EOF | Same goal, different mechanism | Iterate over standard-input tokens and handle conversion failure |
| Positive-square Python comprehension | Already Python | Also show a streaming version matching the C representation decision |
| Integer-classification program | Direct algorithm | Return the classification text and print it |

## Running and inspecting a program

The closest ordinary Python command to “build and run” is simply:

```sh
python3 hello.py
```

An implementation such as CPython may compile source to bytecode and cache it,
but this is not the C preprocessing/assembly/linking pipeline. Two useful
inspection commands are:

```sh
python3 -m py_compile hello.py
python3 -m dis hello.py
```

`py_compile` checks and produces implementation-specific bytecode. `dis` shows
Python virtual-machine instructions, not native instructions or a portable
machine-level definition of Python. There is no direct counterpart to C's
header expansion, object files, unresolved external symbols, or explicit link
command in this small single-file workflow.

## Functions and diagnostic timing

The arithmetic performed by `twice` translates directly:

```python
def twice(value: int) -> int:
    return value * 2


def main() -> None:
    print(twice(21))


if __name__ == "__main__":
    main()
```

Python does not need a separate declaration before `main` is compiled. The name
`twice` must instead be bound when execution reaches the call. Removing the
definition therefore produces a run-time `NameError` if that path executes,
not a C link-time “undefined reference.” Passing a string may also survive until
the multiplication is executed because Python checks operations dynamically.

## Module names are not C storage sections

The closest surface rewrite of the static/local example is:

```python
zero_count = 0
initial_count = 7


def add_one(value: int) -> int:
    result = value + 1
    return result
```

The two module-level names remain reachable while the module is loaded, and
`result` is local to one call. That similarity is useful, but Python does not
promise C-style `.bss`, `.data`, or automatic-object layout. Object placement,
reference representation, and optimization belong to the Python
implementation. Use the C example when reasoning about object-file sections.

## First output program

```python
def main() -> None:
    courses_completed = 1
    print(f"Programming courses completed: {courses_completed}")
```

The algorithm is direct. Differences such as `main`'s integer exit status,
`printf`'s format contract, and an explicitly declared `int` are C concepts,
not details that this Python version exercises.

## Values, type hints, and integer representation

```python
grade: str = "A"
count: int = 42
average: float = 87.5
passed: bool = True
length: int = 10
exact_width_value: int = 1000
```

These annotations document intent, but ordinary Python execution does not make
them fixed storage declarations. `grade` is a one-character string, not a C
`char`; `float` normally represents a floating-point value, but its language
semantics are not introduced through C's `double` declaration; and Python
integers grow beyond machine-word range while memory is available.

If a file or network format truly requires a signed 32-bit field, a library
such as `struct` can validate that boundary while encoding:

```python
import struct

encoded = struct.pack("<i", 1000)
decoded, = struct.unpack("<i", encoded)
```

That is a data-format operation, not a declaration that every Python integer is
32 bits.

## Basic operators and precedence

Most arithmetic grouping transfers directly, but division and increment do
not have identical spelling:

```python
quotient = 7 // 3       # 2 for these nonnegative operands
remainder = 7 % 3       # 1
precedence = 2 + 3 * 4  # 14
grouped = (2 + 3) * 4   # 20

score = 10
score += 5              # 15
score += 1              # Python has no ++ operator
```

Python spells logical conjunction, disjunction, and negation as `and`, `or`,
and `not`. They short-circuit like C's `&&` and `||`, but Python returns one of
its operand objects while C's logical operators produce the integer `0` or `1`.
For negative operands, Python `//` and `%` follow floor-division rules, while C
integer division truncates toward zero and defines the remainder consistently
with that quotient. These examples therefore transfer directly only for the
shown nonnegative values.

## Division and conversion

Using the same spelling does not give the same result:

```python
c_integer_result = 5 // 2  # 2: closest match to positive C integer division
real_result = 5 / 2        # 2.5
```

In Python, `/` always requests real division, while `//` is floor division. For
negative operands, Python floors toward negative infinity, whereas C integer
division truncates toward zero. Therefore `//` is only a direct behavioral
translation for nonnegative operands unless the rounding rule is handled
explicitly.

## Boolean expressions

```python
eligible = age >= 18 and has_id
```

The high-level condition is direct. Python uses `and` rather than `&&` and
normally uses `True` and `False`, although other objects also have truth values.

## Integer limits and signed/unsigned behavior

Python has no ordinary equivalent of printing `INT_MIN`, `INT_MAX`, and
`UINT_MAX`, because its `int` is not a fixed-width signed or unsigned C type.
`sys.maxsize` describes a practical platform index limit, not the largest
Python integer:

```python
import sys

print(f"practical container index limit: {sys.maxsize}")
print(2**200)  # still an ordinary Python integer
```

The mixed-sign comparison also behaves differently:

```python
index = -1
count = 10
print(index < count)  # True; no conversion to an unsigned size type
```

To model a C unsigned field, validate a range or explicitly mask to a chosen
width. Do not use that model to predict C implicit-conversion rules; reason from
the C operand types instead.

## Formatted output and checked input

The score and ratio output has a direct f-string form:

```python
score = 95
ratio = 0.875
print(f"score={score} ratio={ratio:.2f}")
```

For exactly two integers, Python validates different boundaries:

```python
def read_two_integers(line: str) -> tuple[int, int]:
    fields = line.split()
    if len(fields) != 2:
        raise ValueError("expected two integers")
    return int(fields[0]), int(fields[1])


try:
    a, b = read_two_integers(input())
except ValueError as error:
    print(error)
else:
    print(a + b)
```

There are no destination addresses or format specifiers. `int` conversion may
raise `ValueError`, and the resulting integer does not overflow at C `int`
boundaries. A faithful test of the C program must additionally check the C
range and format contract.

The checkpoint's `a / b` distinction follows the division discussion. The `U`
suffix in `0U - 1U` makes the C literals unsigned; Python has no corresponding
literal suffix or automatic unsigned wraparound. The expression
`(0 - 1) % (2**32)` can model one chosen 32-bit result, but the width is an
explicit assumption rather than a property of Python's `int`.

## Selection and iteration

The Python loop already present in the lecture note is the direct counterpart
of the C `for` loop:

```python
total = 0
for value in range(1, limit + 1):
    if value % 2 == 0:
        total += value
```

The command selection can be expressed without C's fallthrough behavior:

```python
if command == "q":
    running = False
elif command == "h":
    print_help()
else:
    print("unknown command")
```

Python's `match` statement is another option, but `if`/`elif` keeps the example
focused on selection rather than pattern matching. There is no omitted `break`
hazard in this rewrite.

## Reading tokens through end-of-file

This version keeps the C example's “consume all integer tokens” behavior and
reports the first invalid token:

```python
import sys

total = 0
count = 0
try:
    for line in sys.stdin:
        for token in line.split():
            total += int(token)
            count += 1
except ValueError:
    print(f"invalid token after {count} integers", file=sys.stderr)
    raise SystemExit(1)

print(f"count={count} total={total}")
```

Iteration naturally ends at EOF. Python still needs explicit handling for a
bad conversion, but it has no stale destination object corresponding to the
incorrect C pattern `while (!feof(stdin))`.

## Streaming the positive-square calculation

The comprehension in the C note is already the concise Python reference. A
closer representation match to the requested streaming C solution is:

```python
def sum_positive_squares(lines: list[str], maximum: int = 100) -> int:
    answer = 0
    count = 0
    for line in lines:
        for token in line.split():
            if count == maximum:
                raise ValueError("too many integers")
            value = int(token)
            if value > 0:
                answer += value * value
            count += 1
    return answer
```

This avoids retaining the converted integers but still lets `split` create a
small list of strings for each input line. Python integers do not reproduce C
overflow, so the C version still needs a stated numeric range and checked
arithmetic.

## Integer classification

```python
def classify_integer(value: int) -> str:
    if value < 0:
        sign = "negative"
    elif value > 0:
        sign = "positive"
    else:
        sign = "zero"

    parity = "even" if value % 2 == 0 else "odd"
    return f"{value} is {sign} and {parity}"


print(classify_integer(int(input())))
```

The branch structure and mathematical classification are direct. Python's
input conversion raises an exception instead of returning a conversion count,
and Python integers do not overflow at the C `int` boundary.

## What the Python versions must not hide

- A Python type hint is not a C storage declaration.
- Python's arbitrary-precision integers do not predict C overflow.
- Python exceptions do not model C undefined behavior.
- Bytecode inspection does not replace the C compile/assemble/link model.
- Similar control flow does not remove the need to validate C formats, ranges,
  and exit status.

Runnable versions of the main direct translations are collected in
[`python_examples.py`](python_examples.py).
