# Week 7 Lecture Exercises: Parsing and Syntax Trees

[Starter code](week07_starter.c) · [Complete example](../examples.c)

Both files use the same token, lexer, parser, and AST types. The complete example
is the reference for helper names and ownership conventions; the starter leaves
the selected learning steps as TODOs.

## Hour 1 — Tokens and pipeline boundaries

Tokenize `12 + x * (3 - 1)`. Record each token kind and lexeme, and identify
which malformed inputs should be rejected by the lexer rather than the parser.

## Hour 2 — Precedence and AST ownership

Draw the AST for the token stream, then complete `ast_create`, `parse_primary`,
`parse_unary`, `parse_term`, and `parse_expression` in the starter. State which
function owns every allocated node on success and on a partial parse failure.

## Hour 3 — Evaluation and code generation

Complete `ast_evaluate` and `ast_destroy`, then write a stack-machine instruction
trace for the same tree. Compare the trace with the infix source and explain how
the tree makes precedence explicit without parentheses in the emitted program.
