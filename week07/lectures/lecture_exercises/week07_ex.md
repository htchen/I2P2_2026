# Week 7 Lecture Exercises: Parsing and Syntax Trees

[Starter code](week07_starter.c)

## Hour 1 — Tokens and pipeline boundaries

Tokenize `12 + x * (3 - 1)`. Record each token kind and lexeme, and identify
which malformed inputs should be rejected by the lexer rather than the parser.

## Hour 2 — Precedence and AST ownership

Draw the AST for the token stream, then complete `parse_factor` in the starter.
State which function owns every allocated node on success and on a partial parse
failure.

## Hour 3 — Evaluation and code generation

Write postorder evaluation and a stack-machine instruction trace for the same
tree. Compare the trace with the infix source and explain how the tree makes
precedence explicit without parentheses in the emitted program.
