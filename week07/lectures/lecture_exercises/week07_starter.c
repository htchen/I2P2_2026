#include <stdbool.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

typedef enum TokenKind {
    TOKEN_INTEGER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_END,
    TOKEN_INVALID
} TokenKind;

typedef struct Token {
    TokenKind kind;
    long value;
    size_t position;
} Token;

typedef struct Lexer {
    const char *input;
    size_t position;
} Lexer;

typedef enum AstKind {
    AST_INTEGER,
    AST_ADD,
    AST_SUBTRACT,
    AST_MULTIPLY,
    AST_DIVIDE,
    AST_NEGATE
} AstKind;

typedef struct Ast {
    AstKind kind;
    long value;
    struct Ast *left;
    struct Ast *right;
} Ast;

typedef struct Parser {
    Lexer lexer;
    Token current;
    const char *error;
} Parser;

Ast *parse_expression(Parser *parser);
void ast_destroy(Ast *node);

Token lexer_next(Lexer *lexer) {
    /* TODO: skip spaces, scan one token, and always make progress. */
    const size_t position = lexer->position;
    return (Token){TOKEN_INVALID, 0, position};
}

void parser_advance(Parser *parser) {
    parser->current = lexer_next(&parser->lexer);
    if (parser->current.kind == TOKEN_INVALID && parser->error == NULL) {
        parser->error = "invalid token or integer overflow";
    }
}

void parser_init(Parser *parser, const char *input) {
    parser->lexer = (Lexer){input, 0};
    parser->error = NULL;
    parser_advance(parser);
}

Ast *ast_create(AstKind kind, long value, Ast *left, Ast *right) {
    /* TODO: validate child shape, allocate, initialize, and return ownership. */
    (void)kind;
    (void)value;
    (void)left;
    (void)right;
    return NULL;
}

Ast *parse_primary(Parser *parser) {
    /* TODO: parse an integer or one parenthesized expression. */
    (void)parser;
    return NULL;
}

Ast *parse_unary(Parser *parser) {
    /* TODO: accept leading '+' and build AST_NEGATE for leading '-'. */
    (void)parser;
    return NULL;
}

Ast *parse_term(Parser *parser) {
    /* TODO: combine unary/primary nodes joined by '*' or '/'. */
    (void)parser;
    return NULL;
}

Ast *parse_expression(Parser *parser) {
    /* TODO: combine term nodes joined by '+' or '-'. */
    (void)parser;
    return NULL;
}

bool ast_evaluate(const Ast *node, long *result) {
    /* TODO: evaluate children first; reject division by zero and overflow. */
    (void)node;
    (void)result;
    return false;
}

void ast_destroy(Ast *node) {
    /* TODO: destroy both owned children before the node. */
    free(node);
}
