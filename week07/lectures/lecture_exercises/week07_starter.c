#include <stdbool.h>
#include <stddef.h>

typedef enum { TOKEN_NUMBER, TOKEN_PLUS, TOKEN_STAR, TOKEN_LPAREN,
               TOKEN_RPAREN, TOKEN_END } TokenKind;

typedef struct {
    TokenKind kind;
    int value;
} Token;

typedef struct Ast {
    TokenKind operation;
    int value;
    struct Ast *left;
    struct Ast *right;
} Ast;

typedef struct {
    const Token *tokens;
    size_t position;
} Parser;

Ast *parse_factor(Parser *parser, bool *ok) {
    /* TODO: parse a number or a parenthesized expression. */
    (void)parser;
    *ok = false;
    return NULL;
}
