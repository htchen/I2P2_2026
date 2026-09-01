#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
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
  const char* input;
  size_t position;
} Lexer;

static Token lexer_next(Lexer* lexer) {
  while (isspace((unsigned char)lexer->input[lexer->position])) {
    ++lexer->position;
  }

  const size_t start = lexer->position;
  const unsigned char current = (unsigned char)lexer->input[start];
  if (current == '\0') {
    return (Token){TOKEN_END, 0, start};
  }

  if (isdigit(current)) {
    long value = 0;
    while (isdigit((unsigned char)lexer->input[lexer->position])) {
      const int digit = lexer->input[lexer->position] - '0';
      if (value > (LONG_MAX - digit) / 10) {
        while (isdigit((unsigned char)lexer->input[lexer->position])) {
          ++lexer->position;
        }
        return (Token){TOKEN_INVALID, 0, start};
      }
      value = value * 10 + digit;
      ++lexer->position;
    }
    return (Token){TOKEN_INTEGER, value, start};
  }

  ++lexer->position;
  switch (current) {
    case '+':
      return (Token){TOKEN_PLUS, 0, start};
    case '-':
      return (Token){TOKEN_MINUS, 0, start};
    case '*':
      return (Token){TOKEN_STAR, 0, start};
    case '/':
      return (Token){TOKEN_SLASH, 0, start};
    case '(':
      return (Token){TOKEN_LEFT_PAREN, 0, start};
    case ')':
      return (Token){TOKEN_RIGHT_PAREN, 0, start};
    default:
      return (Token){TOKEN_INVALID, 0, start};
  }
}

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
  struct Ast* left;
  struct Ast* right;
} Ast;

static void ast_destroy(Ast* node) {
  if (node == NULL) {
    return;
  }
  ast_destroy(node->left);
  ast_destroy(node->right);
  free(node);
}

static Ast* ast_create(AstKind kind, long value, Ast* left, Ast* right) {
  const bool is_integer =
    kind == AST_INTEGER && left == NULL && right == NULL;
  const bool is_unary = kind == AST_NEGATE && left != NULL && right == NULL;
  const bool is_binary =
    (kind == AST_ADD || kind == AST_SUBTRACT || kind == AST_MULTIPLY ||
     kind == AST_DIVIDE) &&
    left != NULL && right != NULL;
  if (!is_integer && !is_unary && !is_binary) {
    return NULL;
  }

  Ast* node = malloc(sizeof(*node));
  if (node == NULL) {
    return NULL;
  }
  node->kind = kind;
  node->value = value;
  node->left = left;
  node->right = right;
  return node;
}

typedef struct Parser {
  Lexer lexer;
  Token current;
  const char* error;
} Parser;

static Ast* parse_expression(Parser* parser);

static void parser_advance(Parser* parser) {
  parser->current = lexer_next(&parser->lexer);
  if (parser->current.kind == TOKEN_INVALID && parser->error == NULL) {
    parser->error = "invalid token or integer overflow";
  }
}

static void parser_init(Parser* parser, const char* input) {
  parser->lexer = (Lexer){input, 0};
  parser->error = NULL;
  parser_advance(parser);
}

static Ast* parse_primary(Parser* parser) {
  if (parser->current.kind == TOKEN_INTEGER) {
    const long value = parser->current.value;
    parser_advance(parser);
    return ast_create(AST_INTEGER, value, NULL, NULL);
  }

  if (parser->current.kind == TOKEN_LEFT_PAREN) {
    parser_advance(parser);
    Ast* inside = parse_expression(parser);
    if (inside == NULL) {
      return NULL;
    }
    if (parser->current.kind != TOKEN_RIGHT_PAREN) {
      if (parser->error == NULL) {
        parser->error = "expected ')'";
      }
      ast_destroy(inside);
      return NULL;
    }
    parser_advance(parser);
    return inside;
  }

  if (parser->error == NULL) {
    parser->error = "expected an integer or '('";
  }
  return NULL;
}

static Ast* parse_unary(Parser* parser) {
  if (parser->current.kind == TOKEN_PLUS) {
    parser_advance(parser);
    return parse_unary(parser);
  }
  if (parser->current.kind == TOKEN_MINUS) {
    parser_advance(parser);
    Ast* operand = parse_unary(parser);
    if (operand == NULL) {
      return NULL;
    }
    Ast* node = ast_create(AST_NEGATE, 0, operand, NULL);
    if (node == NULL) {
      ast_destroy(operand);
    }
    return node;
  }
  return parse_primary(parser);
}

static Ast* parse_term(Parser* parser) {
  Ast* left = parse_unary(parser);
  if (left == NULL) {
    return NULL;
  }

  while (parser->current.kind == TOKEN_STAR ||
       parser->current.kind == TOKEN_SLASH) {
    const TokenKind operation = parser->current.kind;
    parser_advance(parser);
    Ast* right = parse_unary(parser);
    if (right == NULL) {
      ast_destroy(left);
      return NULL;
    }

    const AstKind kind =
      operation == TOKEN_STAR ? AST_MULTIPLY : AST_DIVIDE;
    Ast* combined = ast_create(kind, 0, left, right);
    if (combined == NULL) {
      ast_destroy(left);
      ast_destroy(right);
      return NULL;
    }
    left = combined;
  }
  return left;
}

static Ast* parse_expression(Parser* parser) {
  Ast* left = parse_term(parser);
  if (left == NULL) {
    return NULL;
  }

  while (parser->current.kind == TOKEN_PLUS ||
       parser->current.kind == TOKEN_MINUS) {
    const TokenKind operation = parser->current.kind;
    parser_advance(parser);
    Ast* right = parse_term(parser);
    if (right == NULL) {
      ast_destroy(left);
      return NULL;
    }

    const AstKind kind = operation == TOKEN_PLUS ? AST_ADD : AST_SUBTRACT;
    Ast* combined = ast_create(kind, 0, left, right);
    if (combined == NULL) {
      ast_destroy(left);
      ast_destroy(right);
      return NULL;
    }
    left = combined;
  }
  return left;
}

static Ast* parse(const char* input, const char** error,
          size_t* error_position) {
  Parser parser;
  parser_init(&parser, input);
  Ast* tree = parse_expression(&parser);

  if (tree != NULL && parser.current.kind != TOKEN_END) {
    if (parser.error == NULL) {
      parser.error = "unexpected token after expression";
    }
    ast_destroy(tree);
    tree = NULL;
  }
  if (tree == NULL && parser.error == NULL) {
    parser.error = "allocation failed";
  }
  *error = parser.error;
  *error_position = parser.current.position;
  return tree;
}

static bool add_checked(long left, long right, long* result) {
  if ((right > 0 && left > LONG_MAX - right) ||
    (right < 0 && left < LONG_MIN - right)) {
    return false;
  }
  *result = left + right;
  return true;
}

static bool subtract_checked(long left, long right, long* result) {
  if ((right > 0 && left < LONG_MIN + right) ||
    (right < 0 && left > LONG_MAX + right)) {
    return false;
  }
  *result = left - right;
  return true;
}

static bool multiply_checked(long left, long right, long* result) {
  if (left == 0 || right == 0) {
    *result = 0;
    return true;
  }
  if ((left == -1 && right == LONG_MIN) ||
    (right == -1 && left == LONG_MIN)) {
    return false;
  }
  if ((left > 0 && right > 0 && left > LONG_MAX / right) ||
    (left > 0 && right < 0 && right < LONG_MIN / left) ||
    (left < 0 && right > 0 && left < LONG_MIN / right) ||
    (left < 0 && right < 0 && right < LONG_MAX / left)) {
    return false;
  }
  *result = left * right;
  return true;
}

static bool ast_evaluate(const Ast* node, long* result) {
  if (node == NULL || result == NULL) {
    return false;
  }
  if (node->kind == AST_INTEGER) {
    *result = node->value;
    return true;
  }

  long left = 0;
  if (!ast_evaluate(node->left, &left)) {
    return false;
  }
  if (node->kind == AST_NEGATE) {
    if (left == LONG_MIN) {
      return false;
    }
    *result = -left;
    return true;
  }

  long right = 0;
  if (!ast_evaluate(node->right, &right)) {
    return false;
  }
  switch (node->kind) {
    case AST_ADD:
      return add_checked(left, right, result);
    case AST_SUBTRACT:
      return subtract_checked(left, right, result);
    case AST_MULTIPLY:
      return multiply_checked(left, right, result);
    case AST_DIVIDE:
      if (right == 0 || (left == LONG_MIN && right == -1)) {
        return false;
      }
      *result = left / right;
      return true;
    case AST_INTEGER:
    case AST_NEGATE:
      return false;
  }
  return false;
}

static const char* ast_operator(AstKind kind) {
  switch (kind) {
    case AST_ADD:
      return "+";
    case AST_SUBTRACT:
      return "-";
    case AST_MULTIPLY:
      return "*";
    case AST_DIVIDE:
      return "/";
    case AST_INTEGER:
    case AST_NEGATE:
      return "?";
  }
  return "?";
}

static void ast_print(const Ast* node) {
  if (node->kind == AST_INTEGER) {
    printf("%ld", node->value);
    return;
  }
  if (node->kind == AST_NEGATE) {
    fputs("(neg ", stdout);
    ast_print(node->left);
    putchar(')');
    return;
  }

  printf("(%s ", ast_operator(node->kind));
  ast_print(node->left);
  putchar(' ');
  ast_print(node->right);
  putchar(')');
}

typedef struct TestCase {
  const char* input;
  bool should_parse;
  bool should_evaluate;
  long expected_value;
} TestCase;

static bool run_test(const TestCase* test) {
  const char* error = NULL;
  size_t error_position = 0;
  Ast* tree = parse(test->input, &error, &error_position);

  if (!test->should_parse) {
    if (tree != NULL) {
      printf("FAIL: %s parsed but should have been rejected\n", test->input);
      ast_destroy(tree);
      return false;
    }
    printf("PASS: %s rejected at %zu: %s\n", test->input,
         error_position, error);
    return true;
  }

  if (tree == NULL) {
    printf("FAIL: %s rejected at %zu: %s\n", test->input,
         error_position, error);
    return false;
  }

  printf("AST:  %s => ", test->input);
  ast_print(tree);
  putchar('\n');

  long result = 0;
  const bool evaluated = ast_evaluate(tree, &result);
  ast_destroy(tree);
  if (!test->should_evaluate) {
    if (evaluated) {
      printf("FAIL: expected an evaluation error, got %ld\n", result);
      return false;
    }
    puts("PASS: evaluation rejected as expected");
    return true;
  }
  if (!evaluated || result != test->expected_value) {
    printf("FAIL: expected %ld\n", test->expected_value);
    return false;
  }
  printf("PASS: value = %ld\n", result);
  return true;
}

int main(void) {
  const TestCase kTests[] = {
    {"12 + 4 * (3 - 1)", true, true, 20},
    {"8 - 3 - 2", true, true, 3},
    {"--5 + 1", true, true, 6},
    {"1 +", false, false, 0},
    {"(3 + 4", false, false, 0},
    {"12 + x", false, false, 0},
    {"1 / 0", true, false, 0},
  };
  const size_t test_count = sizeof(kTests) / sizeof(kTests[0]);
  size_t passed = 0;

  for (size_t i = 0; i < test_count; ++i) {
    if (run_test(&kTests[i])) {
      ++passed;
    }
  }
  printf("%zu/%zu tests passed\n", passed, test_count);
  return passed == test_count ? EXIT_SUCCESS : EXIT_FAILURE;
}
