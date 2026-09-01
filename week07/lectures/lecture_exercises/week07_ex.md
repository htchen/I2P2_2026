# Week 7 Lecture Exercises: Parsing and Syntax Trees

[Starter code](week07_starter.c)

The starter is a complete testable pipeline. Use `parse_unary` as a worked
example of how a grammar production becomes control flow and how ownership is
handled when recursive construction fails. The activities below emphasize
tracing, contract explanation, and testing rather than copying a missing
implementation from another repository file.

## Hour 1 — Tokens and pipeline boundaries

Tokenize `12 + 4 * (3 - 1)`. Before running any code, complete this expected
token table:

| Lexeme | Kind | Integer value when applicable |
|--------|------|-------------------------------|
| `12` | `TOKEN_INTEGER` | 12 |
| `+` | `TOKEN_PLUS` | — |
| `4` | `TOKEN_INTEGER` | 4 |
| `*` | `TOKEN_STAR` | — |
| `(` | `TOKEN_LEFT_PAREN` | — |
| `3` | `TOKEN_INTEGER` | 3 |
| `-` | `TOKEN_MINUS` | — |
| `1` | `TOKEN_INTEGER` | 1 |
| `)` | `TOKEN_RIGHT_PAREN` | — |
| end of input | `TOKEN_END` | — |

The lecture grammar deliberately contains integers but not identifiers. Thus
`12 + x` must produce `TOKEN_INVALID` at `x`; adding identifier support would
also require a new token kind, AST kind, lexical rule, and evaluation environment.

## Hour 2 — Precedence and AST ownership

The expected tree is:

```text
        (+)
       /   \
     12     (*)
           /   \
          4     (-)
               / \
              3   1
```

The starter prints the same shape in prefix form:

```text
(+ 12 (* 4 (- 3 1)))
```

Trace `parse_unary` against this production:

```text
unary -> ('+' | '-') unary | primary
```

Unary plus returns the owned subtree produced by the recursive call. Unary minus
passes that subtree to `ast_create(AST_NEGATE, ...)`. `ast_create` transfers
ownership only on success, so the caller must destroy the operand if allocation
fails. The supplied `ast_create`, `parse_primary`, `parse_term`,
`parse_expression`, and `ast_destroy` provide concrete examples of the same
contract.

Test `+5`, `-5`, `--5`, `-(2 + 3)`, and an incomplete `-`. For every case, mark
the current token before and after `parse_unary` and the owner of each allocated
node. Then temporarily inject an `ast_create` failure and verify that the
operand subtree is released.

## Hour 3 — Evaluation and code generation

### Compile and verify

Compile and run the starter:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  week07/lectures/lecture_exercises/week07_starter.c -o /tmp/week07_starter
/tmp/week07_starter
```

The command above is written for the repository root. If your shell is already
inside `week07/lectures/lecture_exercises`, use `week07_starter.c` as the source
path instead.

The final line must be `7/7 tests passed`. The first three successful AST lines
must be:

```text
AST:  12 + 4 * (3 - 1) => (+ 12 (* 4 (- 3 1)))
AST:  8 - 3 - 2 => (- (- 8 3) 2)
AST:  --5 + 1 => (+ (neg (neg 5)) 1)
```

Run the completed program with AddressSanitizer or another available memory
checker. Both accepted and rejected inputs must finish without leaks or invalid
accesses.

Finally, write this tree's stack-machine instruction trace. Compare it with the
infix source and explain how the AST makes precedence explicit without retaining
parenthesis nodes. The trace should evaluate to 20:

```text
PUSH 12
PUSH 4
PUSH 3
PUSH 1
SUB
MUL
ADD
```
