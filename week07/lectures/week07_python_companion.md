# Python Contrast Companion for Week 7

Read this companion beside the [Week 7 lecture
notes](week07_lecture.md). The compiler pipeline, grammar, recursive-descent
structure, tree evaluation, and code-generation order translate directly.
Manual AST allocation, cleanup on partial failure, fixed-width overflow, and C
output-pointer error contracts do not.

## Relationship labels

- **Direct:** the compiler stage or recursive algorithm transfers closely.
- **Same algorithm, different error/ownership model:** Python raises and garbage
  collects where C reports status and releases partial trees.
- **Closest Python model:** a dataclass/enum preserves structure without C
  layout.
- **Diagram/specification:** the language-independent artifact is reused.

## Coverage map

Every fenced example in the Week 7 note is accounted for below.

| Source example | Relationship | Companion treatment |
|---|---|---|
| Compiler-pipeline Mermaid diagram | Diagram/specification | Preserve characters → tokens → AST → checks → instructions |
| Token enum and structure | Closest Python model | Use `Enum` plus immutable dataclass |
| Lexer state, `peek`, and `take` | Direct state machine | Store source text and integer position |
| Checked integer accumulation | Same algorithm, different range | Python needs an explicit chosen maximum to model C `long` overflow |
| Expression grammar | Language-independent specification | Reuse unchanged as the parser contract |
| AST enum and structure | Closest Python model | Use an immutable dataclass with optional children |
| Parser state and `advance` | Direct state machine | Keep current token; raise `ParseError` rather than store a C string pointer |
| `parse_expression` loop | Direct grammar implementation | Preserve left associativity; exceptions replace cleanup/status branches |
| Validating `ast_create` | Direct invariant | Validate child shape in `__post_init__` |
| `parse_primary` | Direct grammar implementation | Consume integer/parenthesized forms and raise on mismatch |
| Evaluation-order Mermaid tree | Diagram/specification | Preserve postorder dependencies |
| `ast_evaluate` | Direct algorithm, different result/error | Return an integer; raise for division by zero or invalid shape |
| AST and stack-instruction text | Direct code-generation order | Emit the same postorder instruction sequence |

## Pipeline and tokens

The pipeline is language independent:

```text
source characters -> lexer -> tokens -> parser -> AST
                  -> semantic checks -> target instructions
```

Python makes a concise token representation:

```python
from dataclasses import dataclass
from enum import Enum, auto


class TokenKind(Enum):
    INTEGER = auto()
    PLUS = auto()
    MINUS = auto()
    STAR = auto()
    SLASH = auto()
    LEFT_PAREN = auto()
    RIGHT_PAREN = auto()
    END = auto()
    INVALID = auto()


@dataclass(frozen=True)
class Token:
    kind: TokenKind
    value: int = 0
    position: int = 0
```

The tag/value/position contract transfers. Python does not define the C enum's
integer representation or the structure's field layout.

## Lexer state and checked integers

```python
class Lexer:
    def __init__(self, source: str) -> None:
        self.source = source
        self.position = 0

    def peek(self) -> str:
        if self.position == len(self.source):
            return ""
        return self.source[self.position]

    def take(self) -> str:
        current = self.peek()
        if current:
            self.position += 1
        return current
```

An empty string is the end marker; there is no exposed C null terminator.

Python integers do not overflow at `LONG_MAX`. To test the same bounded language,
make the limit an explicit lexer policy:

```python
def take_integer(lexer: Lexer, maximum: int) -> int:
    value = 0
    while lexer.peek().isdigit():
        digit = int(lexer.take())
        if value > (maximum - digit) // 10:
            raise ValueError("integer literal is out of range")
        value = value * 10 + digit
    return value
```

The recurrence and pre-multiplication check are direct. The caller must choose
the maximum that matches the intended C implementation or simulated target.

## Grammar remains the specification

```text
expression  -> term (('+' | '-') term)*
term        -> unary (('*' | '/') unary)*
unary       -> ('+' | '-') unary | primary
primary     -> INTEGER | IDENTIFIER | '(' expression ')'
```

The bounded code examples below implement integers and parentheses; adding
identifiers requires a token kind, lexical rule, and semantic environment. The
grammar—not the implementation language—determines precedence and associativity.

## AST representation and invariants

```python
from __future__ import annotations
from dataclasses import dataclass
from enum import Enum, auto


class AstKind(Enum):
    INTEGER = auto()
    ADD = auto()
    SUBTRACT = auto()
    MULTIPLY = auto()
    DIVIDE = auto()
    NEGATE = auto()


@dataclass(frozen=True)
class Ast:
    kind: AstKind
    value: int = 0
    left: Ast | None = None
    right: Ast | None = None

    def __post_init__(self) -> None:
        is_number = self.kind is AstKind.INTEGER and self.left is None
        is_unary = (
            self.kind is AstKind.NEGATE
            and self.left is not None
            and self.right is None
        )
        is_binary = (
            self.kind not in (AstKind.INTEGER, AstKind.NEGATE)
            and self.left is not None
            and self.right is not None
        )
        if not (is_number or is_unary or is_binary):
            raise ValueError("invalid AST shape")
```

This is the Python counterpart of `ast_create`. Construction raises instead of
returning `NULL`, and partial subtrees remain managed during exception unwinding.
The invariant itself is unchanged.

## Parser state and recursive descent

```python
class ParseError(ValueError):
    pass


class Parser:
    def __init__(self, source: str) -> None:
        self.lexer = Lexer(source)
        self.current = self.lexer.next_token()

    def advance(self) -> None:
        self.current = self.lexer.next_token()
```

The runnable example supplies `next_token`. An exception carries the error
message and unwinds the call stack; there is no borrowed `const char* error`
field whose lifetime must be checked.

The expression loop preserves left associativity:

```python
def parse_expression(self) -> Ast:
    left = self.parse_term()
    while self.current.kind in (TokenKind.PLUS, TokenKind.MINUS):
        operation = self.current.kind
        self.advance()
        right = self.parse_term()
        kind = AstKind.ADD if operation is TokenKind.PLUS else AstKind.SUBTRACT
        left = Ast(kind=kind, left=left, right=right)
    return left
```

If parsing the right operand fails, Python keeps the already created `left`
tree safe until it becomes unreachable. The C parser must explicitly destroy it.

Primary parsing is equally direct:

```python
def parse_primary(self) -> Ast:
    if self.current.kind is TokenKind.INTEGER:
        value = self.current.value
        self.advance()
        return Ast(kind=AstKind.INTEGER, value=value)
    if self.current.kind is TokenKind.LEFT_PAREN:
        self.advance()
        inside = self.parse_expression()
        if self.current.kind is not TokenKind.RIGHT_PAREN:
            raise ParseError("expected ')'")
        self.advance()
        return inside
    raise ParseError("expected an integer or '('")
```

## Postorder evaluation

```python
def truncate_divide(left: int, right: int) -> int:
    if right == 0:
        raise ZeroDivisionError("division by zero")
    magnitude = abs(left) // abs(right)
    return -magnitude if (left < 0) != (right < 0) else magnitude


def evaluate(node: Ast) -> int:
    if node.kind is AstKind.INTEGER:
        return node.value
    if node.left is None:
        raise ValueError("missing left operand")
    left = evaluate(node.left)
    if node.kind is AstKind.NEGATE:
        return -left
    if node.right is None:
        raise ValueError("missing right operand")
    right = evaluate(node.right)
    operations = {
        AstKind.ADD: lambda: left + right,
        AstKind.SUBTRACT: lambda: left - right,
        AstKind.MULTIPLY: lambda: left * right,
        AstKind.DIVIDE: lambda: truncate_divide(left, right),
    }
    return operations[node.kind]()
```

The tree dependencies and postorder evaluation match C. `truncate_divide`
deliberately matches C integer truncation toward zero; Python `//` alone floors
negative results and is not equivalent. Python arithmetic still does not model
C `long` overflow.

## Instruction generation

```python
def generate(node: Ast) -> list[str]:
    if node.kind is AstKind.INTEGER:
        return [f"PUSH {node.value}"]
    if node.left is None:
        raise ValueError("missing operand")
    result = generate(node.left)
    if node.kind is AstKind.NEGATE:
        return result + ["NEG"]
    if node.right is None:
        raise ValueError("missing right operand")
    operation = {
        AstKind.ADD: "ADD",
        AstKind.SUBTRACT: "SUB",
        AstKind.MULTIPLY: "MUL",
        AstKind.DIVIDE: "DIV",
    }[node.kind]
    return result + generate(node.right) + [operation]
```

For `1 + 2 * 3`, the emitted order is exactly `PUSH 1`, `PUSH 2`, `PUSH 3`,
`MUL`, `ADD`. The target instruction contract is independent of the host
language.

## What the Python versions must not hide

- Python-managed ASTs do not prove that C partial trees are freed on failure.
- Exceptions replace status/output-pointer paths but do not remove the need to
  test every parser boundary.
- Arbitrary-precision literals and arithmetic do not model C `long` overflow.
- Python `//` is not C signed integer division.
- Grammar, associativity, postorder evaluation, and instruction order transfer
  directly and make a useful differential oracle.

The complete bounded lexer/parser/evaluator is in
[`python_examples.py`](python_examples.py).
